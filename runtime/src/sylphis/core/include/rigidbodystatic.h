/***************************************************************************
                         rigidbody.h  -  description
                             -------------------
    begin                : Sat Nov 9 2002
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

#ifndef RIGIDBODYSTATIC_H
#define RIGIDBODYSTATIC_H

#include <rigidbody.h>

class CModel;

SmartPointer(CRigidBodyStatic);
class CRigidBodyStatic : public CRigidBody {
public:
	CRigidBodyStatic(S32 parent, CSceneObject *so = 0);
	virtual ~CRigidBodyStatic();

	eClass getClass() const{
		return (eClass)0;
	}

	void setMeshData(CVector3 *verts, U32 vertnum, S32 *elems, U32 elemsnum);
	void setMeshData(CModel *m);
	
    virtual void setSleep(bool s);

    virtual void addForce(const CVector3 &f);
    virtual void addTorque(const CVector3 &f);
    virtual void addLocalForce(const CVector3 &f);
    virtual void addLocalTorque(const CVector3 &f);
    virtual void addForceAt(const CVector3 &f, const CVector3 &pos);
    virtual void addForceAtLocal(const CVector3 &f, const CVector3 &pos);
    virtual void addLocalForceAt(const CVector3 &f, const CVector3 &pos);
    virtual void addLocalForceAtLocal(const CVector3 &f, const CVector3 &pos);
    
	virtual void makeImmovable();
	virtual void applyGravity(bool g);

	virtual void setPosition(const CVector3 &o);
	virtual void setPosition(const F32 x, const F32 y, const F32 z);
	virtual CVector3 getPosition();
	virtual void setOrientation(const CQuaternion &o);
	virtual void setOrientation(const F32 x, const F32 y, const F32 z, const F32 w);
	virtual CQuaternion getOrientation();
	virtual void setVelocity(const CVector3 &v);
	virtual CVector3 getVelocity();
	virtual void setAngVelocity(const CVector3 &v);
	virtual CVector3 getAngVelocity();

    virtual void update();
};

#endif


