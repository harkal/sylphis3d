/***************************************************************************
quaternion.cpp  -  description
-------------------
begin                : Mon Jun 3 2002
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

#include "quaternion.h"
#include "vector.h"
#include <matrix3.h>
#include <matrix.h>
#include <math.h>
#include <serializer.h>

CQuaternion CQuaternion::ZERO(0.0,0.0,0.0,0.0);
CQuaternion CQuaternion::IDENTITY(0.0,0.0,0.0,1.0);

CQuaternion::CQuaternion(F32 _x, F32 _y, F32 _z, F32 _w){
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

CQuaternion::CQuaternion(const CQuaternion &q){
    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
}

CQuaternion::~CQuaternion(){
}

CQuaternion CQuaternion::operator+ (const CQuaternion& q){
    return CQuaternion(x + q.x, y + q.y, z + q.z, w + q.w);
}

CQuaternion CQuaternion::operator- (const CQuaternion& q){
    return CQuaternion(x - q.x, y - q.y, z - q.z, w - q.w);
}

CQuaternion CQuaternion::operator* (const CQuaternion& q) const {
#if 0
    return CQuaternion
        (
        w * q.x + x * q.w + y * q.z - z * q.y,
        w * q.y + y * q.w + z * q.x - x * q.z,
        w * q.z + z * q.w + x * q.y - y * q.x,
        w * q.w - x * q.x - y * q.y - z * q.z
        );
#else
    return CQuaternion
        (
        w * q.x + x * q.w + y * q.z - z * q.y,
        w * q.y - x * q.z + y * q.w + z * q.x,
        w * q.z + x * q.y - y * q.x + z * q.w,
        w * q.w - x * q.x - y * q.y - z * q.z
        );
#endif
}

CQuaternion CQuaternion::operator* (F32 f) {
    return CQuaternion(f*x, f*y, f*z, f*w);
}
/*
CQuaternion operator* (F32 f, const CQuaternion& q){
return CQuaternion(f*q.x,f*q.y,f*q.z, f*q.w);
}
*/
CQuaternion CQuaternion::operator- () {
    return CQuaternion(-w,-x,-y,-z);
}

CVector3 CQuaternion::operator* (const CVector3& v) const {
#if 0
    CMatrix4 m;

    toRotationMatrix(m);

    return m * v;
#else
    // nVidia SDK implementation
	CVector3 uv, uuv;
	CVector3 qvec(x, y, z);
	uv = qvec ^ v;
	uuv = qvec ^ uv;
	uv *= (2.0f * w);
	uuv *= 2.0f;

	return v + uv + uuv;
#endif
#if 0
    CQuaternion thiss = *this;
    thiss.invert();
    thiss = thiss * CQuaternion(v.x, v.y, v.z, 0.0);
    thiss = thiss * *this;

    return CVector3(thiss.x, thiss.y, thiss.z);
#endif
}

CQuaternion &CQuaternion::invert(){
    x = -x;
    y = -y;
    z = -z;

    return (*this);
}

void CQuaternion::fromRotationMatrix (CMatrix4 &mat){
    // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
    // article "Quaternion Calculus and Fast Animation".
    FMatrix4 &kRot = mat.m;
    F32 fTrace = kRot[0][0]+kRot[1][1]+kRot[2][2];
    F32 fRoot;

    if ( fTrace > 0.0 ){
        // |w| > 1/2, may as well choose w > 1/2
        fRoot = sqrt(fTrace + 1.0);  // 2w
        w = 0.5*fRoot;
        fRoot = 0.5/fRoot;  // 1/(4w)
        x = (kRot[2][1]-kRot[1][2])*fRoot;
        y = (kRot[0][2]-kRot[2][0])*fRoot;
        z = (kRot[1][0]-kRot[0][1])*fRoot;
    } else {
        // |w| <= 1/2
        static S32 s_iNext[3] = { 1, 2, 0 };
        S32 i = 0;
        if ( kRot[1][1] > kRot[0][0] )
            i = 1;
        if ( kRot[2][2] > kRot[i][i] )
            i = 2;
        S32 j = s_iNext[i];
        S32 k = s_iNext[j];

        fRoot = sqrt(kRot[i][i]-kRot[j][j]-kRot[k][k] + 1.0);
        F32* apkQuat[3] = { &x, &y, &z };
        *apkQuat[i] = 0.5*fRoot;
        fRoot = 0.5/fRoot;
        w = (kRot[k][j]-kRot[j][k])*fRoot;
        *apkQuat[j] = (kRot[j][i]+kRot[i][j])*fRoot;
        *apkQuat[k] = (kRot[k][i]+kRot[i][k])*fRoot;
    }
}

void CQuaternion::toRotationMatrix (CMatrix3& mat) const {
    F32 _x  = 2.0*x;
    F32 _y  = 2.0*y;
    F32 _z  = 2.0*z;
    F32 wx = _x*w;
    F32 wy = _y*w;
    F32 wz = _z*w;
    F32 xx = _x*x;
    F32 xy = _y*x;
    F32 xz = _z*x;
    F32 yy = _y*y;
    F32 yz = _z*y;
    F32 zz = _z*z;

    FMatrix3 &m = mat.m;

    m[0][0] = 1.0-(yy+zz);
    m[1][0] = xy-wz;
    m[2][0] = xz+wy;
    m[0][1] = xy+wz;
    m[1][1] = 1.0-(xx+zz);
    m[2][1] = yz-wx;
    m[0][2] = xz-wy;
    m[1][2] = yz+wx;
    m[2][2] = 1.0-(xx+yy);

	//mat.transpose();
}

void CQuaternion::toRotationMatrix (CMatrix4& mat) const {
    F32 _x  = 2.0*x;
    F32 _y  = 2.0*y;
    F32 _z  = 2.0*z;
    F32 wx = _x*w;
    F32 wy = _y*w;
    F32 wz = _z*w;
    F32 xx = _x*x;
    F32 xy = _y*x;
    F32 xz = _z*x;
    F32 yy = _y*y;
    F32 yz = _z*y;
    F32 zz = _z*z;

    FMatrix4 &m = mat.m;

    m[0][0] = 1.0-(yy+zz);
    m[1][0] = xy-wz;
    m[2][0] = xz+wy;
    m[0][1] = xy+wz;
    m[1][1] = 1.0-(xx+zz);
    m[2][1] = yz-wx;
    m[0][2] = xz-wy;
    m[1][2] = yz+wx;
    m[2][2] = 1.0-(xx+yy);

    m[3][0] = 0.0;
    m[3][1] = 0.0;
    m[3][2] = 0.0;
    m[0][3] = 0.0;
    m[1][3] = 0.0;
    m[2][3] = 0.0;
    m[3][3] = 1.0;

    //mat.transpose();
}

void CQuaternion::fromAxes (const CVector3& xAxis, const CVector3& yAxis, const CVector3& zAxis){
    CMatrix4 m;

    FMatrix4 &kRot = m.m;

    kRot[0][0] = xAxis.x;
    kRot[1][0] = xAxis.y;
    kRot[2][0] = xAxis.z;

    kRot[0][1] = yAxis.x;
    kRot[1][1] = yAxis.y;
    kRot[2][1] = yAxis.z;

    kRot[0][2] = zAxis.x;
    kRot[1][2] = zAxis.y;
    kRot[2][2] = zAxis.z;

    fromRotationMatrix(m);
}

void CQuaternion::fromAngleAxis (const CVector3& axis, const F32 angle){
    F32 halfAngle = 0.5 * angle;
    F32 Sin = sinf(halfAngle);

    x = Sin * axis.x;
    y = Sin * axis.y;
    z = Sin * axis.z;

    w = cosf(halfAngle);
}

void CQuaternion::lerp(F32 f, const CQuaternion& q){
    F32 norm;
    norm = x * q.x + y * q.y + z * q.z + w * q.w;

    bool bFlip = false;

    if(norm < 0.0f){
        norm = -norm;
        bFlip = true;
    }

    F32 inv_f;
    if(1.0f - norm < 0.000001f){
        inv_f = 1.0f - f;
    } else {
        F32 theta;
        theta = acos(norm);

        F32 s;
        s = 1.0f / sin(theta);

        inv_f = sin((1.0f - f) * theta) * s;
        f = sin(f * theta) * s;
    }

    if(bFlip) {
        f = -f;
    }

    x = inv_f * x + f * q.x;
    y = inv_f * y + f * q.y;
    z = inv_f * z + f * q.z;
    w = inv_f * w + f * q.w;
}

void CQuaternion::swapHand(){
    F32 t = y;
    y = z;
    z = -t;
}

void CQuaternion::save(CSerializer *ser) const {
	ser->writeFloat(x);
	ser->writeFloat(y);
	ser->writeFloat(z);
	ser->writeFloat(w);
}

void CQuaternion::load(CSerializer *ser){
	x = ser->readFloat();
	y = ser->readFloat();
	z = ser->readFloat();
	w = ser->readFloat();
}

