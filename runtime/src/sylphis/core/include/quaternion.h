/***************************************************************************
                          quaternion.h  -  description
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

#ifndef QUATERNION_H
#define QUATERNION_H

#include <platform/types.h>

class CVector3;
class CMatrix3;
class CMatrix4;
class CSerializer;

class CQuaternion {
public: 
	CQuaternion(F32 _x = 0, F32 _y = 0, F32 _z = 0, F32 _w = 1.0);
	CQuaternion(const CQuaternion &q);
	~CQuaternion();

	CQuaternion operator+ (const CQuaternion& q);
	CQuaternion operator- (const CQuaternion& q);
	CQuaternion operator* (const CQuaternion& q) const;
	CQuaternion operator* (F32 f);
//	friend CQuaternion operator* (F32 f, const CQuaternion& q);
	CQuaternion operator- ();

	CVector3 operator* (const CVector3& v) const;

	CQuaternion &invert();
	void toRotationMatrix (CMatrix4& mat) const;
    void toRotationMatrix (CMatrix3& mat) const;
    void fromRotationMatrix (CMatrix4 &mat);
	void fromAngleAxis (const CVector3& axis, const F32 angle);
    void fromAxes (const CVector3& xAxis, const CVector3& yAxis, const CVector3& zAxis);

    void swapHand();

    void lerp(F32 f, const CQuaternion& q);

    F32 x;
    F32 y;
    F32 z;
    F32 w;

	static CQuaternion ZERO;
	static CQuaternion IDENTITY;

	void save(CSerializer *ser) const;
	void load(CSerializer *ser);
};

#endif
