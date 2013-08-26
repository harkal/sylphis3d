
#include <stdio.h>
#include <iostream>
#include <stdarg.h> 
#include <console.h>

#ifdef _WIN32
extern "C" {
S32 write(S32 fd, const S8 *str, S32 size);
}
#endif

void CConsole::printf(S8 *fmt, ...){ 
	va_list	argptr;
	S8	str[1024];

	va_start (argptr,fmt); 
	vsprintf (str, fmt, argptr);
	va_end (argptr);

	printString(str);
}

void CConsole::errorf(S8 *fmt, ...){ 
	va_list	argptr;
	S8	str[1024];

	va_start (argptr,fmt); 
	vsprintf (str, fmt, argptr);
	va_end (argptr);

	printError(str);
}

void CConsole::printError(const string &s) {
	printString(s);
}


//
// CConsoleTTY
//

CConsoleTTY::CConsoleTTY(S32 in, S32 out, S32 err){
	fdIn = in;
	fdOut = out;
	fdErr = err;
}

void CConsoleTTY::printString(const string &s) {
	write(fdOut, s.c_str(), s.length());
}

void CConsoleTTY::printError(const string &s) {
	write(fdErr, s.c_str(), s.length());
}

//
// CConsoleLog
//

CConsoleLog::CConsoleLog(const string &filename){
	file = fopen(filename.c_str(), "a");
	printString("==== Logging service started ====\n");
}

CConsoleLog::~CConsoleLog(){
	fclose(file);
}

void CConsoleLog::printString(const string &s) {
	fprintf(file, "%s", s.c_str());
	fflush(file);
}

//
// CConsoleWin32 
//
#ifdef _WIN32

CConsoleWin32::CConsoleWin32(HANDLE in, HANDLE out, HANDLE err){
	fdIn = in;
	fdOut = out;
	fdErr = err;
}

void CConsoleWin32::printString(const string &s) {
	cout << s;
	cout.flush();
}

void CConsoleWin32::printError(const string &s) {	
    cerr << s;
	cerr.flush();
}

#endif

//
// CConsoleMulti
//

void CConsoleMulti::printString(const string &s) {
	vector<CConsole*>::iterator iter;

	for(iter = consoles.begin() ; iter != consoles.end() ; iter++){
		(*iter)->printString(s);
	}
}

void CConsoleMulti::printError(const string &s) {
	vector<CConsole*>::iterator iter;

	for(iter = consoles.begin() ; iter != consoles.end() ; iter++){
		(*iter)->printError(s);
	}
}

void CConsoleMulti::addConsole(CConsole *c){
	consoles.push_back(c);
}

void CConsoleMulti::freeConsoles(){		
	vector<CConsole*>::iterator iter;

	for(iter = consoles.begin() ; iter != consoles.end() ; iter++){
		delete (*iter);
	}

	consoles.clear();
}

CConsoleMulti gcon;
