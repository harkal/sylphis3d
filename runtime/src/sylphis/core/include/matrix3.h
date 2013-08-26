/***************************************************************************
                          matrix3.h  -  description
                             -------------------
    begin                : Apr 22 2002
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

#ifndef _MATRIX3_H_
#define _MATRIX3_H_

#include "vector.h"
#include "plane.h"

typedef F32 FMatrix3[3][3];

class CMatrix3 {
public:
	CMatrix3();
	
	void zero();
	void identity();

	CMatrix3 &rotateZ(F32 rad);
	CMatrix3 &rotateY(F32 rad);
	CMatrix3 &rotateX(F32 rad);
	CMatrix3 &rotate(CVector3 &a);

	CMatrix3	getInverted();
	void	invert();
	CMatrix3 &transpose();
	
	CMatrix3& operator=  (const CMatrix3& b);
	CMatrix3& operator*= (CMatrix3& b);
	friend CMatrix3 operator* (const CMatrix3& a, const CMatrix3& b);
	friend CMatrix3 operator* (const CMatrix3& a, const F32 b);
	friend CVector3 operator* (const CMatrix3& a, const CVector3& b);
	friend CVector4 operator* (const CMatrix3& a, const CVector4& b);

	F32 m[3][3];
};

inline CVector3 operator* (const CMatrix3& a, const CVector3& b) {
	CVector3 v;
	
    v.x = (a.m[0][0] * b.x) + (a.m[1][0] * b.y) + (a.m[2][0] * b.z);
	v.y = (a.m[0][1] * b.x) + (a.m[1][1] * b.y) + (a.m[2][1] * b.z);
	v.z = (a.m[0][2] * b.x) + (a.m[1][2] * b.y) + (a.m[2][2] * b.z);

    return v;
}

#endif


