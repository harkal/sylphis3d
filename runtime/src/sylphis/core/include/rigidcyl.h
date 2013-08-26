/***************************************************************************
                        rigidcyl.h  -  description
                             -------------------
    begin                : Tue Jul 15 2003
    copyright            : (C) 2003 by Harry Kalogirou
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

#ifndef RIGIDCYL_H
#define RIGIDCYL_H

#include <rigidbody.h>


class CRigidCylinder : public CRigidBody {
public:
	CRigidCylinder(S32 parent, CSceneObject *so = 0, F32 radius = 10.0, F32 length = 20.0);
	virtual ~CRigidCylinder();

	void setDimentions(F32 radius, F32 length);
	F32 getRadius();
	F32 getLength();

	virtual void setDensity(F32 density);
    virtual void setMass(F32 mass);
	virtual void fitToVertices(CVector3 *v, S32 num);
protected:
	F32 mRadius;
	F32 mLength;
};

#endif


