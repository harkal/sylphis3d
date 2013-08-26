/***************************************************************************
                          mthread.cpp  -  description
                             -------------------
    begin                : Sun Oct 19 2003
    copyright            : (C) 2003 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#include <python/mthread.h>
#include <exception.h>

CMThread::CMThread(){
	mModule = PyImport_ImportModule("mthread");
	mDict = PyModule_GetDict(mModule);

	mSL = PyImport_ImportModule("stackless");
    if(!mSL){
        throw CException("Non Stackless version of Python found. Please use Stackless Python");
    }
	mSLDict = PyModule_GetDict(mSL);
}

CMThread::~CMThread(){
	Py_DECREF(mModule);
	Py_DECREF(mSL);
}

PyObject *CMThread::getCurrentThread(){
	PyObject *s = PyString_FromString("getcurrent");
	PyObject *func = PyDict_GetItem(mSLDict, s);
	Py_DECREF(s);

	PyObject *rslt = PyObject_CallFunction(func, 0);
	if(!rslt){
		PyErr_Print();
	}

	// New reference
	return rslt;
}

std::string CMThread::getThreadName(PyObject *thread){
	PyObject *name = PyObject_GetAttrString(thread, "name");
	std::string s = PyString_AsString(name);
	Py_DECREF(name);
	return s;
}

void CMThread::wakeUp(PyObject *thread){

	PyObject *s = PyString_FromString("wakethreads");
	PyObject *list = PyDict_GetItem(mDict, s);
	Py_DECREF(s);

	PyList_Append(list, thread);

//	PyChannel_Send(channel, Py_None);
	/*
	PyObject *rslt = PyObject_CallFunction(func, "O", thread);
	if(!rslt){
		PyErr_Print();
	}
	Py_XDECREF(rslt);
	*/
}

void CMThread::sleep(F64 sec){
	PyObject *s = PyString_FromString("sleep");
	PyObject *func = PyDict_GetItem(mDict, s);
	Py_DECREF(s);

	PyObject *rslt = PyObject_CallFunction(func, "d", sec);
	if(!rslt){
		PyErr_Print();
	}
	Py_XDECREF(rslt);
}

void CMThread::update(){
    PyObject *s = PyString_FromString("update");
	PyObject *func = PyDict_GetItem(mDict, s);
	Py_DECREF(s);

	PyObject *rslt = PyObject_CallFunction(func, "");
	if(!rslt){
		PyErr_Print();
	}
	Py_XDECREF(rslt);
}

