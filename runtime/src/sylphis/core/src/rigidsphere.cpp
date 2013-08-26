/***************************************************************************
                       rigidsphere.cpp  -  description
                             -------------------
    begin                : Tue Jul 15 2002
    copyright            : (C) 2003 by Harry Kalogirou
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

#include <rigidsphere.h>

extern dSpaceID space;
extern dWorldID world;

#define RAY

CRigidSphere::CRigidSphere(S32 parent, CSceneObject *so, F32 radius) : CRigidBody(parent, so) {
	if(radius <= 0.0)
		radius = 10.0;
	if(parent == 0)parent = (S32)space;

#ifdef RAY
	mSpeedRay = dCreateRay((dSpaceID)parent, 1.0);
	dGeomRaySetClosestHit(mSpeedRay, true);
	dGeomSetBody(mSpeedRay, mBodyID);
	dGeomSetData(mSpeedRay, static_cast<void *>(this));
#endif

	mGeomID = dCreateSphere((dSpaceID)parent, TO_PHYSICS(radius));
	mBodyID = dBodyCreate(world);
	dGeomSetBody(mGeomID, mBodyID);
	dGeomSetData(mGeomID, static_cast<void *>(this));
	mRadius = radius;
	setDensity(0.0001);

	hasLast = false;
}

CRigidSphere::~CRigidSphere(){
#ifdef RAY
	dGeomDestroy(mSpeedRay);
#endif
}

void CRigidSphere::update(){
	CRigidBody::update();
}

#include <console.h>
void CRigidSphere::perStepUpdate(F64 stepTime){
    CRigidBody::perStepUpdate(stepTime);
	if(!hasLast){
		hasLast = true;
		mLastPos = getPosition();
		//return;
	}
	const CVector3 &start = getPosition();//mLastPos;
	CVector3 end = start + getVelocity() * stepTime;//getPosition();
	CVector3 delta = end - start;

#ifdef RAY
	F32 len = delta.length();
	if(len>0.0)
		delta /= len;
	
	dGeomRaySet(mSpeedRay, TO_PHYSICS(start.x), TO_PHYSICS(start.y), TO_PHYSICS(start.z), 
                           delta.x, delta.y, delta.z);

    if(len < 2 * mRadius){
        len = 0;
    }

    //gcon.printf("Ray length %f\n", len);
    dGeomRaySetLength(mSpeedRay, TO_PHYSICS(len));
	
#endif
	mLastPos = end;
	//gcon.printf("%f %f %f - %f %f %f\n", start.x, start.y, start.z, delta.x, delta.y, delta.z);
}

void CRigidSphere::setRadius(F32 r){
	if(r <= 0.0)
		return;
	mRadius = r;
	dGeomSphereSetRadius(mGeomID, TO_PHYSICS(r));
}

F32 CRigidSphere::getRadius() const {
	return mRadius;
}

void CRigidSphere::setDensity(F32 density){
	dMass m;
	dMassSetSphere(&m, TO_WORLD(density), TO_PHYSICS(mRadius));
	dBodySetMass(mBodyID, &m);
}

void CRigidSphere::setMass(F32 mass){
	dMass m;
	dMassSetSphereTotal(&m, mass, TO_PHYSICS(mRadius));
	dBodySetMass(mBodyID, &m);
}

void CRigidSphere::fitToVertices(CVector3 *v, S32 num){
	F64 s[3];
	s[0] = s[1] = s[2] = 0.0;
	for(S32 i = 0 ; i < num ; i++){
		s[0] += v[i].x;
		s[1] += v[i].y;
		s[2] += v[i].z;
	}
	CVector3 center(s[0] / num,	s[1] / num, s[2] / num);
	mScenePositionOffset = center;
	F32 radius = 0.0;
	for(S32 i = 0 ; i < num ; ++v, ++i){
		F32 len = (*v - center).length();
		if(len > radius)
			radius = len;
	}
	gcon.printf("RADIUS: %f\n", radius);
	setRadius(radius);
}
