/***************************************************************************
                         console.h  -  description
                             -------------------
    begin                : Wed Jun 15 2005
    copyright            : (C) 2005 by Harry Kalogirou
    email                : harkal@sylphis3d.com
   ***************************************************************************
    This file is part of "Sylphis3D Game Engine".

	Copyright (c) 2013 Charilaos Kalogirou.
	All rights reserved.

	Redistribution and use in source and binary forms are permitted
	provided that the above copyright notice and this paragraph are
	duplicated in all such forms and that any documentation,
	advertising materials, and other materials related to such
	distribution and use acknowledge that the software was developed
	by Charilaos Kalogirou. The name of the
	Charilaos Kalogirou may not be used to endorse or promote products derived
	from this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 ***************************************************************************/
#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

#include <platform/types.h>

///
/// A console that the system can output text to.
///
class CConsole {
public:
	void printf(S8 *fmt, ...);
	void errorf(S8 *fmt, ...);
	virtual void printString(const string &s) = 0;
	virtual void printError(const string &s);
};

///
/// A console for UNIX type of systems
///
class CConsoleTTY : public CConsole {
public:
	CConsoleTTY(S32 in, S32 out, S32 err);
	virtual void printString(const string &s);
	virtual void printError(const string &s);
protected:
	S32 fdIn;
	S32 fdOut;
	S32 fdErr;
};

///
/// A console that simply logs all output to a file
///
class CConsoleLog : public CConsole {
public:
	CConsoleLog(const string &filename);
	virtual ~CConsoleLog();
	virtual void printString(const string &s);
protected:
	FILE *file;
};

#ifdef _WIN32
#include <windows.h>

///
/// A console for the WIN32 platform
///
class CConsoleWin32 : public CConsole {
public:
	CConsoleWin32(HANDLE in, HANDLE out, HANDLE err);
	virtual void printString(const string &s);
	virtual void printError(const string &s);
protected:
	HANDLE fdIn;
	HANDLE fdOut;
	HANDLE fdErr;
};

#endif

///
/// A console that forwards output to number of other consoles.
///
class CConsoleMulti : public CConsole {
public:
    virtual ~CConsoleMulti(){};
	virtual void printString(const string &s);
	virtual void printError(const string &s);
	void addConsole(CConsole *c);
	void freeConsoles();
protected:
	vector<CConsole*> consoles;
};

/// The global system console
extern CConsoleMulti gcon;

#endif
