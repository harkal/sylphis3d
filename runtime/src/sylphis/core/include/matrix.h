/***************************************************************************
                          matrix.h  -  description
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

#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include "vector.h"
#include "plane.h"

typedef F32 FMatrix4[4][4];

class CMatrix4 {
public:
	CMatrix4();
	
	void zero();
	void identity();

	CMatrix4 &rotateZ(F32 rad);
	CMatrix4 &rotateY(F32 rad);
	CMatrix4 &rotateX(F32 rad);
	CMatrix4 &rotate(CVector3 &a);

	CMatrix4	getInverted();
	void	invert();
	CMatrix4 &transpose();
	void setTransform(const CVector3& v);
	
	CMatrix4& operator=  (const CMatrix4& b);
	CMatrix4& operator*= (CMatrix4& b);
	friend CMatrix4 operator* (const CMatrix4& a, const CMatrix4& b);
	friend CMatrix4 operator* (const CMatrix4& a, const F32 b);
	friend CVector3 operator* (const CMatrix4& a, const CVector3& b);
	friend CVector4 operator* (const CMatrix4& a, const CVector4& b);

	F32 m[4][4];
};

inline CVector3 operator* (const CMatrix4& a, const CVector3& b) {
	CVector3 v;
	
    v.x = (a.m[0][0] * b.x) + (a.m[1][0] * b.y) + (a.m[2][0] * b.z) + a.m[3][0];
	v.y = (a.m[0][1] * b.x) + (a.m[1][1] * b.y) + (a.m[2][1] * b.z) + a.m[3][1];
	v.z = (a.m[0][2] * b.x) + (a.m[1][2] * b.y) + (a.m[2][2] * b.z) + a.m[3][2];

    return v;
}

#endif


