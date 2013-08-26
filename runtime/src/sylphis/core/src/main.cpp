
#include <Python.h>

#ifdef _WIN32
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <direct.h>
#endif

#include "globaldefs.h"

#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <engine.h>
#include <exception.h>
#include <console.h>
#include <python/pyhelper.h>
#include <stackless.h>
#include <stackless_api.h>

#ifdef USE_HAP
#include <HapDbg/hapdbg.h>
#endif

using namespace std;

#ifdef _WIN32
static char *cmdPtr; 
vector<string> params;
char *_cmdNext() {
        char *ptr = cmdPtr; 
 
        if (cmdPtr!=0) { 
                while(cmdPtr[0]!='\0'&&cmdPtr[0]!=' ') 
                        cmdPtr++; 
                if (cmdPtr[0]=='\0') { 
                        if (ptr==cmdPtr) ptr=0; 
                        cmdPtr=0; 
                } 
                else {cmdPtr[0]='\0';cmdPtr++;} 
        } 
 
        return(ptr); 
}
void cmdInit(char *parms){ 
        cmdPtr=parms; 
		char *param;
		while(param = _cmdNext()){
			params.push_back(param);
		}
} 

S32 paramidx;
const char *cmdNext() {
	if(paramidx>=params.size())
		return 0;
	return params[paramidx++].c_str();
}

#else

S32 gArgc;
S32 paramidx;
char **gArgv;

void initCmd(S32 argc, char *argv[]){ 
    gArgc = argc;
    gArgv = argv;
    paramidx = 0;
} 
 
char *cmdNext() {
    if(paramidx + 1 == gArgc)
        return 0;

    paramidx++;
    printf("%s\n", gArgv[paramidx]);
    return gArgv[paramidx];
}

#endif
#include "python/initmodules.h"

S32 getArgc(){
	PyObject *m, *d, *o, *s;

	m = PyImport_ImportModule("sys");
	d = PyModule_GetDict(m);
	s = PyString_FromString("argv");
	o = PyDict_GetItem(d, s);
	Py_DECREF(s);
	Py_DECREF(m);

	return PyList_Size(o);
}

string getArgv(S32 i){
	if(getArgc() <= i)
		return "";

	PyObject *m, *d, *o, *s;

	m = PyImport_ImportModule("sys");
	d = PyModule_GetDict(m);
	s = PyString_FromString("argv");
	o = PyDict_GetItem(d, s);
	Py_DECREF(s);
	s = PyList_GetItem(o, i);
	Py_DECREF(m);

	return PyString_AsString(s);
}

string getParamValue(const string &param){
	for(S32 i = 0 ; i < getArgc() ; ++i){
		if(getArgv(i) == string("-") + param){
			return getArgv(i + 1);
		}
	}
	return "";
}

bool checkParam(const string &param){
	for(S32 i = 0 ; i < getArgc() ; ++i){
		if(getArgv(i) == string("-") + param){
			return true;
		}
	}
	return false;
}

PyObject *pyEngine;
bool verboseBoot;

void initPython(){
	if(verboseBoot)gcon.printf("Init python\n");
    PyObject *m, *d, *o, *s;
	const char *param;

    if(verboseBoot)Py_VerboseFlag++;

	Py_Initialize();

#ifdef USE_HAP
    hapdbg::Initialize(HE_DEBUGGER_PORTNUM);
    hapdbg::WaitForConnection();
#endif

	// Build the argv for python
	if(verboseBoot)gcon.printf("... importing 'sys'\n");
	m = PyImport_ImportModule("sys");
	d = PyModule_GetDict(m);
	o = PyList_New(0);
	s = PyString_FromString("argv");
	PyDict_SetItem(d, s, o);
	Py_DECREF(s);

	if(verboseBoot)gcon.printf("... setting up command line parameters\n");
	while(param = cmdNext()){
		s = PyString_FromString(param);
		PyList_Append(o, s);
		Py_DECREF(s);
	}

	Py_DECREF(o);

	if(verboseBoot)gcon.printf("... setting up system path\n");
	s = PyString_FromString("path");
	PyObject *path = PyDict_GetItem(d, s);
	Py_DECREF(s);
	S32 count = PyList_Size(path);
	for(S32 i = 0 ; i < count ; i++)
		PySequence_DelItem(path, 0);	

	s = PyString_FromString("");
	PyList_Append(path, s);
	Py_DECREF(s);

#ifdef _WIN32
	s = PyString_FromString(".\\");
#else
    s = PyString_FromString("./");
#endif
	PyList_Append(path, s);
	Py_DECREF(s);

#ifdef _WIN32
	s = PyString_FromString(".\\base");
#else
    s = PyString_FromString("./base");
#endif
    PyList_Append(path, s);
	Py_DECREF(s);

#ifdef _WIN32
	s = PyString_FromString(".\\base\\core");
#else
    s = PyString_FromString("./base/core");
#endif
    PyList_Append(path, s);
	Py_DECREF(s);

#ifdef _WIN32
	s = PyString_FromString(".\\base\\game\\actors");
#else
    s = PyString_FromString("./base/game/actors");
#endif
    PyList_Append(path, s);
	Py_DECREF(s);

#ifdef _WIN32
	s = PyString_FromString(".\\stdlib\\");
#else
	s = PyString_FromString("./stdlib_linux/");
#endif
    PyList_Append(path, s);
	Py_DECREF(s);

#ifdef _WIN32
	s = PyString_FromString(".\\system\\");
#else
	s = PyString_FromString("./system/");
#endif
    PyList_Append(path, s);
	Py_DECREF(s);

	count = PyList_Size(path);
	
	Py_DECREF(m);

	if(verboseBoot)gcon.printf("... init sylphis extentions\n");
	init_sylphis();
}

void import_importer(){
    PyObject *modname, *mod;

	if(verboseBoot)gcon.printf("Init python importer\n");
    modname = PyString_FromString("importer");
	mod = PyImport_Import(modname);
	Py_DECREF(modname);
	if(!mod){
        PyErr_Print();
		if(verboseBoot)gcon.printf("... FATAL : failed to import the importer!!\n");
		throw CException("Failed to import python importer");
	}
	Py_DECREF(mod);
}

#include <configfile.h>

void import_entry(){
    PyObject *modname, *mod;

	if(verboseBoot)gcon.printf("Importing python entry code\n");
	modname = PyString_FromString("entry");
	mod = PyImport_Import(modname);
	
	Py_DECREF(modname);
	if(!mod){
		PyErr_Print();
		if(verboseBoot)gcon.printf("... FATAL : failed to import entry code!!\n");
		throw CException("Failed to import entry code");
	}
    pyEngine = mod;

	if(verboseBoot)gcon.printf("Connecting to configuration file\n");
	gCfgFile.connect("configfile", "gCfgFile");
}

void pythonEntry(){
	if(verboseBoot)gcon.printf("Starting python entry code\n");
	PyObject *mdict = PyModule_GetDict(pyEngine);
	PyObject *func = PyDict_GetItemString(mdict, "entry");

    if (func && PyCallable_Check(func)) {
		PyObject *rslt = PyObject_CallFunction(func, 0);
		if(rslt){
			Py_DECREF(rslt);
		} else {
			PyErr_Print();
			throw CException("");
		}
	} else {
		PyObject *a, *b, *c;
		PyErr_Fetch(&a, &b, &c);
		string s = PyString_AsString(b);	
		PyErr_Print();
		throw CException(s);
		throw CException("Failed to call mainLoop()");
	}
}

void pythonExit(){
	if(verboseBoot)gcon.printf("Terminating python code\n");
	if(!pyEngine){
		gcon.printf("Terminating python code : no available engine running\n");
		return;
	}
	PyObject *mdict = PyModule_GetDict(pyEngine);
	PyObject *func = PyDict_GetItemString(mdict, "exit");

    if (func && PyCallable_Check(func)) {
		PyObject *rslt = PyObject_CallFunction(func, 0);
		if(rslt){
			Py_DECREF(rslt);
		} else {
			PyErr_Print();
		}
	} else {
		PyObject *a, *b, *c;
		PyErr_Fetch(&a, &b, &c);
		string s = PyString_AsString(b);	
		PyErr_Print();
	}
}

CEngine *gameEngine;

void setGameEngine(CEngine *e){
	gameEngine = e;
	e->setPyModule(pyEngine);
}

void startVFS(){
	if(verboseBoot)gcon.printf("Init Virtual File System\n");
    gfs.startup();
	gfs.setDebug(checkParam("debugvfs"));
	if(verboseBoot)gcon.printf("... mounting file systems\n");
	gfs.mount("/", "base/");

	string gamedir = getParamValue("gamedir");
	if(gamedir == ""){
		gfs.setBaseDir("base");
	} else {
		gfs.setBaseDir(gamedir);
		gfs.mount("/", gamedir);
	}
    //gfs.addCollection("base.col");

	CVirtualFS::FindResult fr;
	CVirtualFS::FindResult::iterator i;
	gfs.findFiles("", "col", fr);

	for(i = fr.begin() ; i != fr.end() ; i++){
		if((*i).getName() == "base.col")
			continue;
		gfs.addCollection("base" + (*i).getName());
	}
}

string dir, file; 
//#undef _WIN32
#ifdef _WIN32
extern HINSTANCE gInstance;
S32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, S32 iCmdShow){
	// Start processing command-line params 
	pyEngine = 0;
	cmdInit(szCmdLine);
	verboseBoot = false;
	paramidx = 0;

	const char *param;
	while(param = cmdNext()){
		if(string(param) == "-verboseboot"){
			verboseBoot = true;
			break;
		}
	}
	paramidx = 0;

	gInstance = hInstance;

	gcon.addConsole(new CConsoleWin32(cin, cout, cerr));
	gcon.addConsole(new CConsoleLog("system.log"));

    try {
        initPython();

		startVFS();
 
        import_importer();
        import_entry();

        PyObject *time = CPython::getSingleton()->getObject("mthread.time");
        PyObject *v = PyObject_CallFunction(time, 0);
        F32 f = PyFloat_AsDouble(v);

		pythonEntry();

        //while(1){
        //    PyStackless_RunWatchdog(20000000);
        //}

	} catch (CException e){
		pythonExit();
		MessageBox(0, e.getExplanation().c_str(), "Exception", 0);
	}

	return(0);
}
#endif


#ifndef _WIN32
S32 main(S32 argc, char *argv[]){
    initCmd(argc, argv);
    pyEngine = 0;
    verboseBoot = false;
    paramidx = 0;

    const char *param;
    while(param = cmdNext()){
        if(string(param) == "-verboseboot"){
            verboseBoot = true;
            break;
        }
    }
    paramidx = 0;

    gcon.addConsole(new CConsoleTTY(0,1,2));
    gcon.addConsole(new CConsoleLog("system.log"));

    try {
        initPython();

        startVFS();

        import_importer();
        import_entry();

        PyObject *time = CPython::getSingleton()->getObject("mthread.time");
        PyObject *v = PyObject_CallFunction(time, 0);
        F32 f = PyFloat_AsDouble(v);

        pythonEntry();
    } catch (CException e){
        pythonExit();
        printf("Exception : %s\n", e.getExplanation().c_str());
    }

    return(0);
}
#endif
