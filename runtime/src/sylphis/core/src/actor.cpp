/***************************************************************************
                          actor.cpp  -  description
                             -------------------
    begin                : Thu Mar 28 2002
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

#include <actor.h>
#include <stdarg.h> 
#include <console.h>

#include <rigidbody.h>

CActor::CActor() {
}

CActor::~CActor(){
//	Py_XDECREF(self);
}
/*
void CActor::update(F32 timeDelta){
	if(self)
		PyEval_CallMethod(self, "update", "(f)", timeDelta);
}
*/
bool CActor::handleEvent(char *fmt, ...) {
	va_list argptr;

	if(self){
		if(!strcmp(fmt,"ip")){
			va_start(argptr, fmt); 
			S32 i = va_arg(argptr, S32);
			CActor *f1 = (CActor *)va_arg(argptr, CActor *); //FIXME
			va_end(argptr);
			PyEval_CallMethod(self, "handleEvent", fmt, i, f1);
		}
		if(!strcmp(fmt, "ifff")){
			va_start(argptr, fmt); 
			S32 i = va_arg(argptr, S32);
			F32 f1 = (F32)va_arg(argptr, F64);
			F32 f2 = (F32)va_arg(argptr, F64);
			F32 f3 = (F32)va_arg(argptr, F64);
			va_end(argptr);
			PyEval_CallMethod(self, "handleEvent", fmt, i, f1, f2, f3);
		} else if(!strcmp(fmt, "ifffffff")){
			va_start(argptr, fmt); 
			S32 i = va_arg(argptr, S32);
			F32 f1 = (F32)va_arg(argptr, F64);
			F32 f2 = (F32)va_arg(argptr, F64);
			F32 f3 = (F32)va_arg(argptr, F64);
			F32 f4 = (F32)va_arg(argptr, F64);
			F32 f5 = (F32)va_arg(argptr, F64);
			F32 f6 = (F32)va_arg(argptr, F64);
			F32 f7 = (F32)va_arg(argptr, F64);
			va_end(argptr);
			PyEval_CallMethod(self, "handleEvent", fmt, i, f1, f2, f3, f4, f5, f6, f7);
		} else {
			assert(false);
		}
	}

	return true;
}

bool CActor::handleCollition(CRigidBody *other){
    PyObject *body;
	if(other && other->self){
        body = other->self;
	} else {
		body = Py_None;
	}
	//gcon.printf("handleCollition(%p, %p)\n",self, otherSelf);
	PyObject *o = PyEval_CallMethod(self, "handleEvent", "iO", 11, body);

	return true;
}

