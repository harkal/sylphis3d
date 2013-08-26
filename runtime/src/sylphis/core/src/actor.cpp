/***************************************************************************
                          actor.cpp  -  description
                             -------------------
    begin                : Thu Mar 28 2002
    copyright            : (C) 2002 by Harry Kalogirou
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

