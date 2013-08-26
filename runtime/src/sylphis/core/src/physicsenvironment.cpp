/***************************************************************************
                     physicsenvironment.h  -  description
                             -------------------
    begin                : Thu Jun 13 2004
    copyright            : (C) 2004 by Harry Kalogirou
    email                : harkal@sylphis3d.com
   ***************************************************************************
    This file is part of "Sylphis3D Game Engine".

    "Sylphis3D Game Engine" is free software; you can redistribute it and/or 
    modify it under the terms of the GNU General Public License as 
    published by the Free Software Foundation; either version 2 of the 
    License, or (at your option) any later version, 
    plus a special exception:
      Linking "Sylphis3D Game Engine" statically or dynamically with other 
    modules is making a combined work based on this library. Thus, the 
    terms and conditions of the GNU General Public License cover the whole 
    combination. As a special exception, the copyright holders of this 
    library give you permission to link this library with independent 
    modules to produce an executable, regardless of the license terms of 
    these independent modules, and to copy and distribute the resulting 
    executable under terms of your choice, provided that you also meet, 
    for each linked independent module, the terms and conditions of the 
    license of that module. An independent module is a module which is 
    not derived from or based on this program. If you modify this program,
    you may extend this exception to your version of the library, but you 
    are not obligated to do so. If you do not wish to do so, delete this 
    exception statement from your version.

    "Sylphis3D Game Engine" is distributed in the hope that it will be 
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with "Sylphis3D Game Engine"; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
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

