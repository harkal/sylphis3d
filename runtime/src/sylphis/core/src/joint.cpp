/***************************************************************************
                         joint.cpp  -  description
                             -------------------
    begin                : Sun Jul 20 2003
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


#include <rigidbody.h>
#include <joint.h>
extern dWorldID world;

CJoint::CJoint() : mID(0) {

}

CJoint::~CJoint() {
	if(mID)
		dJointDestroy(mID);
}

void CJoint::attach(CRigidBody *b1, CRigidBody *b2){
	dBodyID a = 0;
	dBodyID b = 0;

	if(b1)
		a = b1->getBodyID();
	if(b2)
		b = b2->getBodyID();
	dJointAttach(mID, a, b);
}

void CJoint::attachToWorld(CRigidBody *body){
	dJointAttach(mID, body->getBodyID(), 0);
}

dBodyID CJoint::getBody(S32 i){
	return dJointGetBody(mID, i);
}

//
// BALL
//

CJointBall::CJointBall(S32 w, S32 group){
	if(w == 0)w = (S32)world;
	mID = dJointCreateBall((dWorldID)w, (dJointGroupID)group);
}

void CJointBall::setAnchor(const CVector3 &a){
	dJointSetBallAnchor(mID, TO_PHYSICS(a.x), TO_PHYSICS(a.y), TO_PHYSICS(a.z));
}

CVector3 CJointBall::getAnchor() const {
	dVector3 a;
	dJointGetBallAnchor(mID, a);
	return TO_WORLD(CVector3(a[0], a[1], a[2]));
}

//
// HINGE
//

CJointHinge::CJointHinge(S32 w, S32 group){
	if(w == 0)w = (S32)world;
	mID = dJointCreateHinge((dWorldID)w, (dJointGroupID)group);
}

void CJointHinge::setAnchor(const CVector3 &a){
	dJointSetHingeAnchor(mID, TO_PHYSICS(a.x), TO_PHYSICS(a.y), TO_PHYSICS(a.z));
}

CVector3 CJointHinge::getAnchor() const {
	dVector3 a;
	dJointGetHingeAnchor(mID, a);
	return TO_WORLD(CVector3(a[0], a[1], a[2]));
}

void CJointHinge::setAxis(const CVector3 &a){
	dJointSetHingeAxis(mID, TO_PHYSICS(a.x), TO_PHYSICS(a.y), TO_PHYSICS(a.z));
}

CVector3 CJointHinge::getAxis() const {
	dVector3 a;
	dJointGetHingeAxis(mID, a);
	return TO_WORLD(CVector3(a[0], a[1], a[2]));
}

//
// DOUBLEHINGE
//

CJointDoubleHinge::CJointDoubleHinge(S32 w, S32 group){
	if(w == 0)w = (S32)world;
	mID = dJointCreateHinge2((dWorldID)w, (dJointGroupID)group);
}

void CJointDoubleHinge::setAnchor(const CVector3 &a){
	dJointSetHinge2Anchor(mID, TO_PHYSICS(a.x), TO_PHYSICS(a.y), TO_PHYSICS(a.z));
}

CVector3 CJointDoubleHinge::getAnchor() const {
	dVector3 a;
	dJointGetHinge2Anchor(mID, a);
	return TO_WORLD(CVector3(a[0], a[1], a[2]));
}

void CJointDoubleHinge::setAxis1(const CVector3 &a){
	//dJointSetHinge2Axis1(mID, TO_PHYSICS(a.x), TO_PHYSICS(a.y), TO_PHYSICS(a.z));
	dJointSetHinge2Axis1(mID, a.x, a.y, a.z);
}

CVector3 CJointDoubleHinge::getAxis1() const {
	dVector3 a;
	dJointGetHinge2Axis1(mID, a);
	return TO_WORLD(CVector3(a[0], a[1], a[2]));
}

void CJointDoubleHinge::setAxis2(const CVector3 &a){
	//dJointSetHinge2Axis2(mID, TO_PHYSICS(a.x), TO_PHYSICS(a.y), TO_PHYSICS(a.z));
	dJointSetHinge2Axis2(mID, a.x, a.y, a.z);
}

CVector3 CJointDoubleHinge::getAxis2() const {
	dVector3 a;
	dJointGetHinge2Axis2(mID, a);
	return TO_WORLD(CVector3(a[0], a[1], a[2]));
}

//
// SLIDER
//

CJointSlider::CJointSlider(S32 w, S32 group){
	if(w == 0)w = (S32)world;
	mID = dJointCreateSlider((dWorldID)w, (dJointGroupID)group);
}

void CJointSlider::setAnchor(const CVector3 &a){
}

CVector3 CJointSlider::getAnchor() const {
	return CVector3::ZERO;
}

void CJointSlider::setAxis(const CVector3 &a){
	dJointSetSliderAxis(mID, TO_PHYSICS(a.x), TO_PHYSICS(a.y), TO_PHYSICS(a.z));
}

CVector3 CJointSlider::getAxis() const {
	dVector3 a;
	dJointGetSliderAxis(mID, a);
	return TO_WORLD(CVector3(a[0], a[1], a[2]));
}

//
// Universal
//

CJointUniversal::CJointUniversal(S32 w, S32 group){
	if(w == 0)w = (S32)world;
	mID = dJointCreateUniversal((dWorldID)w, (dJointGroupID)group);
}

void CJointUniversal::setAnchor(const CVector3 &a){
	dJointSetUniversalAnchor(mID, TO_PHYSICS(a.x), TO_PHYSICS(a.y), TO_PHYSICS(a.z));
}

CVector3 CJointUniversal::getAnchor() const {
	dVector3 a;
	dJointGetUniversalAnchor(mID, a);
	return TO_WORLD(CVector3(a[0], a[1], a[2]));
}

void CJointUniversal::setAxis1(const CVector3 &a){
	dJointSetUniversalAxis1(mID, TO_PHYSICS(a.x), TO_PHYSICS(a.y), TO_PHYSICS(a.z));
}

CVector3 CJointUniversal::getAxis1() const {
	dVector3 a;
	dJointGetUniversalAxis1(mID, a);
	return TO_WORLD(CVector3(a[0], a[1], a[2]));
}

void CJointUniversal::setAxis2(const CVector3 &a){
	dJointSetUniversalAxis2(mID, TO_PHYSICS(a.x), TO_PHYSICS(a.y), TO_PHYSICS(a.z));
}

CVector3 CJointUniversal::getAxis2() const {
	dVector3 a;
	dJointGetUniversalAxis2(mID, a);
	return TO_WORLD(CVector3(a[0], a[1], a[2]));
}

