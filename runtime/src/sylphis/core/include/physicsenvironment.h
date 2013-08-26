/***************************************************************************
                     physicsenvironment.h  -  description
                             -------------------
    begin                : Thu Jun 13 2004
    copyright            : (C) 2004 by Harry Kalogirou
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

#ifndef _PHYSICS_ENVIRONMENT_H_
#define _PHYSICS_ENVIRONMENT_H_

#include <list>

#include <singleton.h>
#include <vector.h>
#include <rigidbody.h>

class CPhysicsEnv : public CSingleton<CPhysicsEnv> {
public:
    CPhysicsEnv();
    virtual ~CPhysicsEnv();

    // Useing the original method from CSingleton fails in "Release" build
    // This is because of a bug in the MSVC 7.0 compiler.
#if 1
    static CPhysicsEnv* getSingleton(){
		static CPhysicsEnv singleton;
		return &singleton;
	}
#endif

    void setAutoSleep(bool a);
    void setAutoSleepThresh(F32 linear, F32 angular);
    void setSimulationQuality(F32 q);
    void setUpdatesPerSecond(F64 u);
    F64 getUpdatesPerSecond();
    F64 getStepTime();

    void setGravity(const CVector3 &g);
    CVector3 getGravity();
    void wakeAll();
    void sleepAll();

    CRigidBody *getStaticWorldBody();

    typedef std::list<CRigidBodyPtr> RigidBodyList;

    void registerRigidBody(CRigidBody *rb);
	void unregisterRigidBody(CRigidBody *rb);
    RigidBodyList &getRigidBodies();

protected:
	RigidBodyList mRigidBodies;
    CRigidBodyPtr mStaticWorld;
    CVector3 mGravity;
    F64 mStepTime;
};

#endif


