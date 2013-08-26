/***************************************************************************
                       rigidcomplex.cpp  -  description
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
#include <rigidcomplex.h>

extern dSpaceID space;
extern dWorldID world;

CRigidComplex::CRigidComplex(S32 parent, CSceneObject *so, const CVector3 &d ) : CRigidBody(parent, so) {
    if(parent == 0)parent = (S32)space;
    mParent = space;
#if 0
	mGeomID = dCreateBox((dSpaceID)parent, TO_PHYSICS(d.x), TO_PHYSICS(d.y), TO_PHYSICS(d.z));
	dGeomSetBody(mGeomID, mBodyID);
	dGeomSetData(mGeomID, static_cast<void *>(this));
	mDimentions = d;
	setDensity(0.0001);
#endif
	mBodyID = dBodyCreate(world);
    dMassSetZero(&mMass);
}

CRigidComplex::~CRigidComplex(){

}

void CRigidComplex::addBox(F32 mass, F32 x, F32 y, F32 z){
    mLastTran = dCreateGeomTransform(mParent);
    dGeomTransformSetCleanup (mLastTran, 1);

    mLastGeom = dCreateBox(mParent, TO_PHYSICS(x), TO_PHYSICS(y), TO_PHYSICS(z));
    dMassSetBox(&mLastMass, mass, TO_PHYSICS(x), TO_PHYSICS(y), TO_PHYSICS(z));
    
    dGeomTransformSetGeom(mLastTran, mLastGeom);
    mGeomIDs.push_back(mLastTran);
    mGeomIDs.push_back(mLastGeom);
}

void CRigidComplex::addSphere(F32 mass, F32 radius){
    mLastTran = dCreateGeomTransform(mParent);
    dGeomTransformSetCleanup (mLastTran, 1);

    mLastGeom = dCreateSphere(mParent, TO_PHYSICS(radius));
    dMassSetSphere(&mLastMass, mass, TO_PHYSICS(radius));

    dGeomTransformSetGeom(mLastTran, mLastGeom);
    mGeomIDs.push_back(mLastTran);
    mGeomIDs.push_back(mLastGeom);
}

void CRigidComplex::addCapsule(F32 mass, F32 radius, F32 length){
    mLastGeom = dCreateCCylinder(0, TO_PHYSICS(radius), TO_PHYSICS(length));
    dMassSetCappedCylinder(&mLastMass, mass, 3, TO_PHYSICS(radius), TO_PHYSICS(length));
}

void CRigidComplex::translateLast(const CVector3 &v){
    dGeomSetPosition (mLastGeom, TO_PHYSICS(v.x) *2.0, TO_PHYSICS(v.y)*2.0, TO_PHYSICS(v.z)*2.0);
    dMassTranslate (&mLastMass, TO_PHYSICS(v.x)*2.0, TO_PHYSICS(v.y)*2.0, TO_PHYSICS(v.z)*2.0);
}

void CRigidComplex::addLastMass(){
    dMassAdd(&mMass,&mLastMass);
}

void CRigidComplex::fixate(){
    for(GeomVector::iterator i = mGeomIDs.begin() ; i != mGeomIDs.end() ; ++i, ++i){
        dGeomSetBody(*i, mBodyID);
    }
    for(GeomVector::iterator i = mGeomIDs.begin() ; i != mGeomIDs.end() ; ++i){
        dGeomSetData(*i, static_cast<void *>(this));
    }
    mGeomID = mGeomIDs[0];
    for(S32 i = 1 ; i < mGeomIDs.size() ; i += 2){
        const dReal* v = dGeomGetPosition(mGeomIDs[i]);
        dGeomSetPosition(mGeomIDs[i], v[0] - mMass.c[0], v[1] - mMass.c[1], v[2] - mMass.c[2]);
    }
    mScenePositionOffset = CVector3(TO_WORLD(mMass.c[0]),TO_WORLD(mMass.c[1]),TO_WORLD(mMass.c[2]));
    dMassTranslate (&mMass, -mMass.c[0],-mMass.c[1],-mMass.c[2]);
    dBodySetMass(mBodyID, &mMass);
}

void CRigidComplex::setDimentions(const CVector3 &d){
	mDimentions = d;
	dGeomBoxSetLengths(mGeomID, TO_PHYSICS(d.x), TO_PHYSICS(d.y), TO_PHYSICS(d.z));
}

CVector3 CRigidComplex::getDimentions() {
	return mDimentions;
}

void CRigidComplex::setDensity(F32 density){
	
}

void CRigidComplex::setMass(F32 mass){
}

void CRigidComplex::fitToVertices(CVector3 *v, S32 num){
	
}

#include <debugdrawing.h>
void CRigidComplex::draw() const {
    for(S32 i = 0 ; i < mGeomIDs.size() ; i+=2){
    dGeomID tran = mGeomIDs[i];
    dGeomID geom = mGeomIDs[i+1];

    const dReal* v = dGeomGetPosition(tran);
    const dReal* r = dGeomGetRotation(tran);

    const dReal* v2 = dGeomGetPosition(geom);
    const dReal* r2 = dGeomGetRotation(geom);

    CVector3 o(TO_WORLD(v[0]), TO_WORLD(v[1]), TO_WORLD(v[2]));

    dVector3 actual_pos;
    dMatrix3 actual_R;

    dMULTIPLY0_331 (actual_pos,r,v2);
    actual_pos[0] = TO_WORLD(actual_pos[0] + v[0]);
    actual_pos[1] = TO_WORLD(actual_pos[1] + v[1]);
    actual_pos[2] = TO_WORLD(actual_pos[2] + v[2]);
    dMULTIPLY0_333 (actual_R,r,r2);

    F32 sides[3];
    dGeomBoxGetLengths(geom, sides);
    sides[0] = TO_WORLD(sides[0]);
    sides[1] = TO_WORLD(sides[1]);
    sides[2] = TO_WORLD(sides[2]);
    drawBox(actual_pos, actual_R, sides);
    }
}
