/***************************************************************************
                       rigidsphere.cpp  -  description
                             -------------------
    begin                : Tue Jul 15 2002
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
