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

#include <matrix3.h>
#include <math.h>
#include <memory.h>

#ifdef __QNX__
#define	sMemset(x,y,z)	std::memset(x,y,z)
#define	sMemcpy(x,y,z)	std::memcpy(x,y,z)
#else
#define	sMemset(x,y,z)	memset(x,y,z)
#define	sMemcpy(x,y,z)	memcpy(x,y,z)
#endif

#define ZERO_MATRIX(m)	sMemset((m), 0, sizeof((m)))
#define COPY_MATRIX(d,s)	sMemcpy((d), (s), sizeof((d)))



CMatrix3::CMatrix3() {
}

void CMatrix3::zero() {
	ZERO_MATRIX(this->m);
}

void CMatrix3::identity() {
	zero();
	
	this->m[0][0] = 1.0f;
	this->m[1][1] = 1.0f;
	this->m[2][2] = 1.0f;
}

CMatrix3 &CMatrix3::rotateZ(F32 rad){
	F32 c,s;
	F32 Temp00, Temp01, Temp10, Temp11, Temp20, Temp21;

	s=(F32)sin(rad);
	c=(F32)cos(rad);

	Temp00=c*m[0][0]+s*m[0][1];
	Temp10=c*m[1][0]+s*m[1][1];
	Temp20=c*m[2][0]+s*m[2][1];
	Temp01=c*m[0][1]-s*m[0][0];
	Temp11=c*m[1][1]-s*m[1][0];
	Temp21=c*m[2][1]-s*m[2][0];

	m[0][0]=Temp00; m[0][1]=Temp01;
	m[1][0]=Temp10; m[1][1]=Temp11;
	m[2][0]=Temp20; m[2][1]=Temp21;
	
	return *this;
}

CMatrix3 &CMatrix3::rotateY(F32 rad){
	F32 c,s;
	F32 Temp00, Temp10, Temp20, Temp02, Temp12, Temp22;

	s=(F32)sin(rad);
	c=(F32)cos(rad);

	Temp00=c*m[0][0]+s*m[0][2];
	Temp10=c*m[1][0]+s*m[1][2];
	Temp20=c*m[2][0]+s*m[2][2];
	Temp02=c*m[0][2]-s*m[0][0];
	Temp12=c*m[1][2]-s*m[1][0];
	Temp22=c*m[2][2]-s*m[2][0];

	m[0][0]=Temp00; m[0][2]=Temp02;
	m[1][0]=Temp10; m[1][2]=Temp12;
	m[2][0]=Temp20; m[2][2]=Temp22;
	
	return *this;
}

CMatrix3 &CMatrix3::rotateX(F32 rad){
	F32 c,s;
	F32 Temp01, Temp11, Temp21, Temp02, Temp12, Temp22;

	s=(F32)sin(rad);
	c=(F32)cos(rad);

	Temp01=c*m[0][1]+s*m[0][2];
	Temp11=c*m[1][1]+s*m[1][2];
	Temp21=c*m[2][1]+s*m[2][2];
	Temp02=c*m[0][2]-s*m[0][1];
	Temp12=c*m[1][2]-s*m[1][1];
	Temp22=c*m[2][2]-s*m[2][1];

	m[0][1]=Temp01; m[0][2]=Temp02;
	m[1][1]=Temp11; m[1][2]=Temp12;
	m[2][1]=Temp21; m[2][2]=Temp22;
	
	return *this;
}

CMatrix3 &CMatrix3::rotate(CVector3 &a){
	F32 cos_b=(F32)cos(a.x);
	F32 sin_b=(F32)sin(a.x);
	F32 cos_c=(F32)cos(a.y);
	F32 sin_c=(F32)sin(a.y);
	F32 cos_a=(F32)cos(a.z);
	F32 sin_a=(F32)sin(a.z);

	m[0][0]=cos_a*cos_c-sin_a*sin_b*sin_c;
	m[0][1]=sin_a*cos_c+cos_a*sin_b*sin_c;
	m[0][2]=cos_b*sin_c;

	m[1][0]=-sin_a*cos_b;
	m[1][1]=cos_a*cos_b;
	m[1][2]=-sin_b;

	m[2][0]=-cos_a*sin_c-sin_a*sin_b*cos_c;
	m[2][1]=-sin_a*sin_c+cos_a*sin_b*cos_c;
	m[2][2]=cos_b*cos_c;
	
	return *this;
}

CMatrix3& CMatrix3::transpose() {
	#define matrix_swap(a,b) {F32 tmp=m[a][b]; m[a][b]=m[b][a]; m[b][a]=tmp;}
	matrix_swap(0,1);
	matrix_swap(0,2);
	matrix_swap(1,2);
	#undef matrix_swap
	
	return *this;
}

CMatrix3& CMatrix3::operator= (const CMatrix3& b) {
	sMemcpy(m, (b.m), sizeof(b.m));
	return *this;
}

CMatrix3& CMatrix3::operator*= (CMatrix3& b) {
	CMatrix3 temp = (*this) * b;
	*this = temp;
	return *this;
}

CMatrix3 operator*  (const CMatrix3& a_, const CMatrix3& b_) {
    CMatrix3 r;
    r.m[0][0] = a_.m[0][0] * b_.m[0][0] + a_.m[0][1] * b_.m[1][0] + a_.m[0][2] * b_.m[2][0];
    r.m[0][1] = a_.m[0][0] * b_.m[0][1] + a_.m[0][1] * b_.m[1][1] + a_.m[0][2] * b_.m[2][1];
    r.m[0][2] = a_.m[0][0] * b_.m[0][2] + a_.m[0][1] * b_.m[1][2] + a_.m[0][2] * b_.m[2][2];

    r.m[1][0] = a_.m[1][0] * b_.m[0][0] + a_.m[1][1] * b_.m[1][0] + a_.m[1][2] * b_.m[2][0];
    r.m[1][1] = a_.m[1][0] * b_.m[0][1] + a_.m[1][1] * b_.m[1][1] + a_.m[1][2] * b_.m[2][1];
    r.m[1][2] = a_.m[1][0] * b_.m[0][2] + a_.m[1][1] * b_.m[1][2] + a_.m[1][2] * b_.m[2][2];

    r.m[2][0] = a_.m[2][0] * b_.m[0][0] + a_.m[2][1] * b_.m[1][0] + a_.m[2][2] * b_.m[2][0];
    r.m[2][1] = a_.m[2][0] * b_.m[0][1] + a_.m[2][1] * b_.m[1][1] + a_.m[2][2] * b_.m[2][1];
    r.m[2][2] = a_.m[2][0] * b_.m[0][2] + a_.m[2][1] * b_.m[1][2] + a_.m[2][2] * b_.m[2][2];

    return r;
}

CMatrix3 operator*  (const CMatrix3& a, const F32 b) {
	CMatrix3 c;
	c.zero();

	for (S32 i=0; i<3; i++)
		for (S32 j=0; j<3; j++)
			for (S32 k=0; k<3; k++)
				c.m[i][j] += a.m[k][j] * b;
	return c;
}

CVector4 operator* (const CMatrix3& a, const CVector4& b) {
	CVector4 v;
	
    F32 m00 = a.m[0][0];
    F32 m01 = a.m[0][1];
    F32 m02 = a.m[0][2];

    F32 m10 = a.m[1][0];
    F32 m11 = a.m[1][1];
    F32 m12 = a.m[1][2];

    F32 m20 = a.m[2][0];
    F32 m21 = a.m[2][1];
    F32 m22 = a.m[2][2];

	v.x = (m00 * b.x) + (m10 * b.y) + (m20 * b.z);
	v.y = (m01 * b.x) + (m11 * b.y) + (m21 * b.z);
	v.z = (m02 * b.x) + (m12 * b.y) + (m22 * b.z);
//	v.w = (a.m[0][3] * b.x) + (a.m[1][3] * b.y) + (a.m[2][3] * b.z);

    return v;
}

CMatrix3 CMatrix3::getInverted(){
    CMatrix3 outMatrix;

    outMatrix = *this;
    outMatrix.transpose();
	
  	return outMatrix;
}

void CMatrix3::invert(){
	CMatrix3 m = getInverted();
	*this = m;
}
