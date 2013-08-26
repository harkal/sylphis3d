/***************************************************************************
                         console.h  -  description
                             -------------------
    begin                : Wed Jun 15 2005
    copyright            : (C) 2005 by Harry Kalogirou
    email                : harkal@sylphis3d.com
   ***************************************************************************
    This file is part of "Sylphis3D Game Engine".

    "Sylphis3D Game Engine" is free software; you can redistribute it and/or 
    modify it under the terms of the GNU General Public License as 
    published by the Free Software Foundation; either version 2 of the 
    License, or (at your option) any later version, 
    plus a special exception:
      Linking "Sylphis3D Game Engine" statically or dynamically with other 
    modules is making a combined work based on this library. Thus, the 
    terms and conditions of the GNU General Public License cover the whole 
    combination. As a special exception, the copyright holders of this 
    library give you permission to link this library with independent 
    modules to produce an executable, regardless of the license terms of 
    these independent modules, and to copy and distribute the resulting 
    executable under terms of your choice, provided that you also meet, 
    for each linked independent module, the terms and conditions of the 
    license of that module. An independent module is a module which is 
    not derived from or based on this program. If you modify this program,
    you may extend this exception to your version of the library, but you 
    are not obligated to do so. If you do not wish to do so, delete this 
    exception statement from your version.

    "Sylphis3D Game Engine" is distributed in the hope that it will be 
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with "Sylphis3D Game Engine"; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
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
