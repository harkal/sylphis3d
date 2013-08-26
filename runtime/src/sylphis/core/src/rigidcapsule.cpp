/***************************************************************************
                       rigidbox.cpp  -  description
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

#include <boundbox.h>
#include <rigidcapsule.h>

extern dSpaceID space;
extern dWorldID world;

//#define SIM

CRigidCapsule::CRigidCapsule(S32 parent, CSceneObject *so, const CVector3 &d ) : CRigidBody(parent, so) {
	if(parent == 0)parent = (S32)space;

#ifdef SIM
	mGeomID = dCreateGeomTransform ((dSpaceID)parent);
	dGeomTransformSetCleanup (mGeomID, 1);
	F32 radius = sqrt(d.x * d.x + d.z * d.z) / 2.0;
	//F32 radius = d.x / 2;
	dGeomID mGeomSphere = dCreateSphere(0, TO_PHYSICS(radius));
	dGeomTransformSetGeom (mGeomID, mGeomSphere);
	dGeomSetPosition (mGeomSphere, 0, TO_PHYSICS(-d.y/2 + radius), 0);

	mGeomID2 = dCreateGeomTransform ((dSpaceID)parent);
	dGeomTransformSetCleanup (mGeomID2, 1);
	dGeomID mGeomBox = dCreateBox(0, TO_PHYSICS(d.x), TO_PHYSICS(d.y - radius), TO_PHYSICS(d.z));
	dGeomTransformSetGeom (mGeomID2, mGeomBox);
	dGeomSetPosition (mGeomBox, 0, TO_PHYSICS(radius / 2), 0);
#else
    F32 length = d.y - d.x * 2;
    mGeomID = dCreateCCylinder((dSpaceID)parent, TO_PHYSICS(d.x), TO_PHYSICS(length));
#endif

	mBodyID = dBodyCreate(world);
	dGeomSetBody(mGeomID, mBodyID);
#ifdef SIM
	dGeomSetBody(mGeomID2, mBodyID);
#endif
	dGeomSetData(mGeomID, static_cast<void *>(this));
#ifdef SIM
	dGeomSetData(mGeomID2, static_cast<void *>(this));
#endif
	mDimentions = d;
    mDimentions.y = length;
	setDensity(0.0001);
    dBodySetAutoDisableFlag(mBodyID, 0);

	mGroundBox.setBounds(CVector3(-d.x, -d.y / 2 - 5.0, -d.z), CVector3(d.x, -d.y / 2 + 20.0, d.z));
}

CRigidCapsule::~CRigidCapsule(){

}

void CRigidCapsule::setDimentions(const CVector3 &d){
	mDimentions = d;
	assert(true);
	dGeomBoxSetLengths(mGeomID, TO_PHYSICS(d.x), TO_PHYSICS(d.y), TO_PHYSICS(d.z));
}

CVector3 CRigidCapsule::getDimentions() {
	return mDimentions;
}

void CRigidCapsule::setDensity(F32 density){
	if(!mBodyID)
		return;
	dMass m;
	dMassSetCappedCylinder(&m, TO_WORLD(density), 3, TO_PHYSICS(mDimentions.x), TO_PHYSICS(mDimentions.y));
	dBodySetMass(mBodyID, &m);
}

void CRigidCapsule::setMass(F32 mass){
	if(!mBodyID)
		return;
	dMass m;
	dMassSetCappedCylinderTotal(&m, TO_WORLD(mass), 3, TO_PHYSICS(mDimentions.x), TO_PHYSICS(mDimentions.y));
	dBodySetMass(mBodyID, &m);
}

void CRigidCapsule::fitToVertices(CVector3 *v, S32 num){
	CAABoundBox bbox;
	bbox.setBounds(v[0], v[0]);
	for(S32 i = 1 ; i < num ; i++)
		bbox.addPoint(v[i]);

	setDimentions(bbox.getMax() - bbox.getMin());
	mScenePositionOffset = (bbox.getMax() + bbox.getMin()) / 2;
}

