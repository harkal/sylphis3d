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
#include <rigidbox.h>
#include <console.h>

extern dSpaceID space;
extern dWorldID world;

CRigidBox::CRigidBox(S32 parent, CSceneObject *so, const CVector3 &d ) : CRigidBody(parent, so) {
	if(parent == 0) parent = (S32)space;
	mGeomID = dCreateBox((dSpaceID)parent, TO_PHYSICS(d.x), TO_PHYSICS(d.y), TO_PHYSICS(d.z));
	mBodyID = dBodyCreate(world);
	dGeomSetBody(mGeomID, mBodyID);
	dGeomSetData(mGeomID, static_cast<void *>(this));
	mDimentions = d;
	setDensity(0.0001);
}

CRigidBox::~CRigidBox(){

}

void CRigidBox::setDimentions(const CVector3 &d){
	mDimentions = d;
	dGeomBoxSetLengths(mGeomID, TO_PHYSICS(d.x), TO_PHYSICS(d.y), TO_PHYSICS(d.z));
}

CVector3 CRigidBox::getDimentions() {
	return mDimentions;
}

void CRigidBox::setDensity(F32 density){
	if(!mBodyID)
		return;
	dMass m;
	dMassSetBox(&m, TO_WORLD(density), TO_PHYSICS(mDimentions.x), TO_PHYSICS(mDimentions.y), TO_PHYSICS(mDimentions.z));
	dBodySetMass(mBodyID, &m);
}

void CRigidBox::setMass(F32 mass){
	if(!mBodyID)
		return;
	dMass m;
	dMassSetBoxTotal(&m, mass, TO_PHYSICS(mDimentions.x), TO_PHYSICS(mDimentions.y), TO_PHYSICS(mDimentions.z));
	dBodySetMass(mBodyID, &m);
}

void CRigidBox::fitToVertices(CVector3 *v, S32 num){
    if(num <= 0){
        gcon.printError("WARNING : Trying to fit non-mesh object\n");
        return;
    }
	CAABoundBox bbox;
	bbox.setBounds(v[0], v[0]);
	for(S32 i = 1 ; i < num ; i++)
		bbox.addPoint(v[i]);

	setDimentions(bbox.getMax() - bbox.getMin());
	mScenePositionOffset = (bbox.getMax() + bbox.getMin()) / 2;
}

#include <debugdrawing.h>
void CRigidBox::draw() const {
    const dReal* v = dGeomGetPosition(mGeomID);
	CVector3 o(TO_WORLD(v[0]), TO_WORLD(v[1]), TO_WORLD(v[2]));
    const dReal* r = dGeomGetRotation(mGeomID);
    F32 sides[3];
    dGeomBoxGetLengths(mGeomID, sides);
    sides[0] = TO_WORLD(sides[0]);
    sides[1] = TO_WORLD(sides[1]);
    sides[2] = TO_WORLD(sides[2]);
    drawBox(&o.x, r, sides);
}

