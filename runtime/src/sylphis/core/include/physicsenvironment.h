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


