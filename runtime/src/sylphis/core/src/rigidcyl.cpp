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
#include <rigidcyl.h>

extern dSpaceID space;
extern dWorldID world;

CRigidCylinder::CRigidCylinder(S32 parent, CSceneObject *so, F32 radius, F32 length ) : CRigidBody(parent, so) {
	if(parent == 0)parent = (S32)space;
	mGeomID = dCreateCCylinder((dSpaceID)parent, TO_PHYSICS(radius), TO_PHYSICS(length));
	mBodyID = dBodyCreate(world);
	dGeomSetBody(mGeomID, mBodyID);
	dGeomSetData(mGeomID, static_cast<void *>(this));
	mRadius = radius;
	mLength = length;
	setDensity(0.0001);
}

CRigidCylinder::~CRigidCylinder(){

}

void CRigidCylinder::setDimentions(F32 radius, F32 length){
	mRadius = radius;
	mLength = length;
	dGeomCCylinderSetParams(mGeomID, TO_PHYSICS(radius), TO_PHYSICS(length));
}

F32 CRigidCylinder::getRadius() {
	return mRadius;
}

F32 CRigidCylinder::getLength() {
	return mLength;
}

void CRigidCylinder::setDensity(F32 density){
	dMass m;
	dMassSetCappedCylinder(&m, TO_WORLD(density), 3, TO_PHYSICS(mRadius), TO_PHYSICS(mLength));
	dBodySetMass(mBodyID, &m);
}

void CRigidCylinder::setMass(F32 mass){
	dMass m;
	dMassSetCappedCylinderTotal(&m, mass, 3, TO_PHYSICS(mRadius), TO_PHYSICS(mLength));
	dBodySetMass(mBodyID, &m);
}

void CRigidCylinder::fitToVertices(CVector3 *v, S32 num){
	CAABoundBox bbox;
	bbox.setBounds(v[0], v[0]);
	for(S32 i = 1 ; i < num ; i++)
		bbox.addPoint(v[i]);

	setDimentions(bbox.getMax().x - bbox.getMin().x, bbox.getMax().y - bbox.getMin().y);
	mScenePositionOffset = (bbox.getMax() + bbox.getMin()) / 2;
}

