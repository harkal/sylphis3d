
/***************************************************************************
                        pyhelper.cpp  -  description
                             -------------------
    begin                : Sun Mar 20 2005
    copyright            : (C) 2005 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#include <python/pyhelper.h>

CPython::CPython(){
	mImporterMod = PyImport_ImportModule("importer");
	mImporterDict = PyModule_GetDict(mImporterMod);
}

CPython::~CPython(){
	Py_DECREF(mImporterMod);
}


PyObject *CPython::getObject(const std::string &name){
    PyObject *s = PyString_FromString("getObject");
	PyObject *func = PyDict_GetItem(mImporterDict, s);
	Py_DECREF(s);

    s = PyString_FromString(name.c_str());

	PyObject *rslt = PyObject_CallFunction(func, "O", s);
	if(!rslt){
		PyErr_Print();
	}

    Py_DECREF(s);

	// New reference
	return rslt;
}
