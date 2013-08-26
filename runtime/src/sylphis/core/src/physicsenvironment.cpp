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

#include <physicsenvironment.h>
#include <rigidbodystatic.h>
#include <physics.h>
#include <console.h>
#include <ode/ode.h>

dWorldID world;
dSpaceID space;

CPhysicsEnv::CPhysicsEnv(){
    world = dWorldCreate();
	space = dSimpleSpaceCreate(0);
    //space = dHashSpaceCreate(0);

    setUpdatesPerSecond(60.0);

    dWorldSetCFM (world,0.001);
	dWorldSetERP (world, 0.6);

    mStaticWorld = (CRigidBody *)new CRigidBodyStatic(0);
    mStaticWorld->makeImmovable();
}

CPhysicsEnv::~CPhysicsEnv(){

}

void CPhysicsEnv::setAutoSleep(bool a){
    dWorldSetAutoDisableFlag(world, a ? 1 : 0);
}

void CPhysicsEnv::setAutoSleepThresh(F32 linear, F32 angular){
    dWorldSetAutoDisableLinearThreshold(world, TO_PHYSICS(linear));
    dWorldSetAutoDisableAngularThreshold(world, TO_PHYSICS(angular));
}

void CPhysicsEnv::setSimulationQuality(F32 q){
    dWorldSetQuickStepNumIterations(world, (S32)(190.0 * q) + 10.0);
}

void CPhysicsEnv::setUpdatesPerSecond(F64 u){
    mStepTime = 1.0 / u;
}

F64 CPhysicsEnv::getUpdatesPerSecond(){
    return 1.0 / mStepTime;
}

F64 CPhysicsEnv::getStepTime(){
    return mStepTime;
}

void CPhysicsEnv::setGravity(const CVector3 &g){
    dWorldSetGravity(world, TO_PHYSICS(g.x), TO_PHYSICS(g.y), TO_PHYSICS(g.z));
    wakeAll();
    mGravity = g;
}

CVector3 CPhysicsEnv::getGravity(){
    return mGravity;
}

void CPhysicsEnv::wakeAll(){
    for(RigidBodyList::iterator i = mRigidBodies.begin() ; i != mRigidBodies.end() ; ++i){
        (*i)->setSleep(false);
    }
}

void CPhysicsEnv::sleepAll(){
    for(RigidBodyList::iterator i = mRigidBodies.begin() ; i != mRigidBodies.end() ; ++i){
        (*i)->setSleep(true);
    }
}

CRigidBody *CPhysicsEnv::getStaticWorldBody(){
    //gcon.printf("Getting world body\n");
    return mStaticWorld;
}

void CPhysicsEnv::registerRigidBody(CRigidBody *rb){
	assert(rb);
	mRigidBodies.push_back(rb);
	//getEngine().con.printf("rigids %d\n", mRigidBodies.size());
}

void CPhysicsEnv::unregisterRigidBody(CRigidBody *rb){
	mRigidBodies.remove(rb);
	//gcon.printf("rigids %d\n", mRigidBodies.size());
}

CPhysicsEnv::RigidBodyList &CPhysicsEnv::getRigidBodies(){
    return mRigidBodies;
}

