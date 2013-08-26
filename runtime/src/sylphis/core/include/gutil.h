/***************************************************************************
                          gutil.h  -  description
                             -------------------
    begin                : Wed Mar 20 2002
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

#ifndef _GUTIL_H_
#define _GUTIL_H_

#include <vector.h>

void calcTangentVector(const CVector3 &v0,
					   const CVector3 &v1,
					   const CVector3 &v2,
					   const CTexCoord &t0,
					   const CTexCoord &t1,
					   const CTexCoord &t2,
					   const CVector3 &n0,
					   CVector3 &tang);


void calcTangentVector(const CVector4 &v0,
					   const CVector4 &v1,
					   const CVector4 &v2,
					   const CTexCoord &t0,
					   const CTexCoord &t1,
					   const CTexCoord &t2,
					   const CVector3 &n0,
					   CVector3 &tang);

#ifdef _DEBUG
void checkGLError();
#else
#define checkGLError() 
#endif

void myglPopMatrix();
void myglPushMatrix();

F32 WrapToOne(F32 x);

#ifndef __GNUC__ //Already present in GCC
F64 random();
#endif

F64 randomInRange(F64 start, F64 stop);

bool isPowerOfTwo(U32 i);

#endif


