
#ifndef _PYCONSOLE_H_
#define _PYCONSOLE_H_

#include <Python.h>
#include <console.h>

class CPyConsole : public CConsole {
public:
	virtual void printString(const string &s);
	PyObject *self;
};

#endif


