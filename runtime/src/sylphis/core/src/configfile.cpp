/***************************************************************************
                        configfile.cpp  -  description
                             -------------------
    begin                : Mon Oct 7 2002
    copyright            : (C) 2002 by Harry Kalogirou
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

#include <platform/types.h>
#include <configfile.h>

CConfigFile gCfgFile;

CConfigFile::CConfigFile(){
	mPyCfg = 0;
}

CConfigFile::~CConfigFile(){
	Py_XDECREF(mPyCfg);
}

void CConfigFile::connect(const std::string &module, const std::string &object){
	PyObject *m, *d, *s;
	char temp[1024];
	strcpy(temp, module.c_str());
	m = PyImport_ImportModule(temp);
	d = PyModule_GetDict(m);
	strcpy(temp, object.c_str());
	s = PyString_FromString(temp);
	mPyCfg = PyDict_GetItem(d, s);
	Py_INCREF(mPyCfg);
	Py_DECREF(s);
	Py_DECREF(m);
}

S32 CConfigFile::getAsInt(const std::string &var){
	PyObject *v = getAttr(var);
	if(v){
		S32 ret = (S32)PyInt_AsLong(v);
		Py_DECREF(v);
		return ret;
	}
	return 0;
}

F32 CConfigFile::getAsFloat(const std::string &var){
	PyObject *v = getAttr(var);
	if(v){
		F32 ret = (F32)PyFloat_AsDouble(v);
		Py_DECREF(v);
		return ret;
	}
	return 0;
}

PyObject *CConfigFile::getAttr(const std::string &attr){
	return PyObject_GetAttrString(mPyCfg, (char * const)attr.c_str());
}
