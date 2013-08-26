/***************************************************************************
                        rigidsphere.h  -  description
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

#ifndef RIGIDSPHERE_H
#define RIGIDSPHERE_H

#include <rigidbody.h>

class CRigidSphere : public CRigidBody {
public:
	CRigidSphere(S32 parent, CSceneObject *so = 0, F32 radius = 10.0);
	virtual ~CRigidSphere();

	void setRadius(F32 r);
	F32 getRadius() const;

	virtual void setDensity(F32 density);
    virtual void setMass(F32 mass);

	virtual void update();
	virtual void perStepUpdate(F64 stepTime);

	virtual void fitToVertices(CVector3 *v, S32 num);
protected:
	CVector3 mLastPos;
	bool hasLast;
	F32 mRadius;
};

#endif


