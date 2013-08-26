
#include <python/pyconsole.h>

void CPyConsole::printString(const string &s){
	if(self){
		PyObject *rslt = PyObject_CallMethod(self, "printString", "s", s.c_str());
		if(rslt){
			Py_DECREF(rslt);
		} else {
			PyErr_Print();
		}
	}
}

