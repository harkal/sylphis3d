/***************************************************************************
                         rigidbody.h  -  description
                             -------------------
    begin                : Sat Nov 9 2002
    copyright            : (C) 2002 by Harry Kalogirou
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
#include <renderable.h>
#include <renderablemultimesh.h>
#include <console.h>
#include <bound.h>

CRigidBody::CRigidBody(S32 parent, CSceneObject *so) : CControler(so) {
	mGeomID = 0;
	mBodyID = 0;
    disabled = false;
	setSolid(true);
	mVelocity = CVector3::ZERO;
    setDamping(0.01);
	mScenePositionOffset = CVector3::ZERO;
	mSceneObject = so;
    mListener = 0;
    mGroundBody = 0;
    self = 0;
	setSuspendMultiCollitions(true);
    setDamageFactor(1.0);
    setContents(CONTENTS_SOLID);
    setCollisionMask(MASK_ALL);
}

CRigidBody::~CRigidBody(){
    //gcon.printf("CRigidBody::Destructor\n");
	if(mGeomID)
		dGeomDestroy(mGeomID);
	if(mBodyID)
		dBodyDestroy(mBodyID);
}

void CRigidBody::setPosition(const CVector3 &o){
	dGeomSetPosition(mGeomID, TO_PHYSICS(o.x), TO_PHYSICS(o.y), TO_PHYSICS(o.z));
    //mPosition = o;
}

void CRigidBody::setPosition(const F32 x, const F32 y, const F32 z){
	dGeomSetPosition(mGeomID, TO_PHYSICS(x), TO_PHYSICS(y), TO_PHYSICS(z));
    mPosition.x = x;
    mPosition.y = y;
    mPosition.z = z;
}

CVector3 CRigidBody::getPosition(){
	const dReal* v = dGeomGetPosition(mGeomID);
	CVector3 o(TO_WORLD(v[0]), TO_WORLD(v[1]), TO_WORLD(v[2]));
	return o;
}

void CRigidBody::setOrientation(const CQuaternion &o){
	dQuaternion q;
	q[0] = o.w;
	q[1] = o.x;
	q[2] = o.y;
	q[3] = o.z;
	dGeomSetQuaternion(mGeomID, q);
}

void CRigidBody::setOrientation(const F32 x, const F32 y, const F32 z, const F32 w){
	dQuaternion q;
	q[0] = w;
	q[1] = x;
	q[2] = y;
	q[3] = z;
	dGeomSetQuaternion(mGeomID, q);
}

CQuaternion CRigidBody::getOrientation(){
	dReal v[4];
	dGeomGetQuaternion(mGeomID, v);
	CQuaternion o(v[1], v[2], v[3], v[0]);
	return o;
}

void CRigidBody::setVelocity(const CVector3 &v){
    if(!mBodyID){
        mVelocity = v;
        return;
    }
	dBodySetLinearVel(mBodyID, TO_PHYSICS(v.x), TO_PHYSICS(v.y), TO_PHYSICS(v.z));
    mVelocity = v;
}

void CRigidBody::setAngVelocity(const CVector3 &v){
	dBodySetAngularVel(mBodyID, v.x, v.y, v.z);
}

CVector3 CRigidBody::getVelocity(){
    if(!mBodyID){
        return mVelocity;
    }
	const dReal* v = dBodyGetLinearVel(mBodyID);
	CVector3 o(TO_WORLD(v[0]), TO_WORLD(v[1]), TO_WORLD(v[2]));
	return o;
}

CVector3 CRigidBody::getAngVelocity(){
	const dReal* v = dBodyGetAngularVel(mBodyID);
	CVector3 o(v[0], v[1], v[2]);
	return o;
}

void CRigidBody::setDamping(F32 d){
    if(mBodyID)
        dBodySetLinearDamping(mBodyID, d);
    mDamping = d;
}

F32 CRigidBody::getDamping(){
    return mDamping;
}

void CRigidBody::setAngDamping(F32 d){
    if(mBodyID)
        dBodySetAngularDamping(mBodyID, d);
}

F32 CRigidBody::getAngDamping(){
    if(mBodyID)
        return dBodyGetAngularDamping(mBodyID);
    return 0;
}

void CRigidBody::setMaxAngularVelocity(F32 mv){
    if(mBodyID)
        return dBodySetMaxAngularVel(mBodyID, TO_PHYSICS(mv));
}

void CRigidBody::setDamageFactor(F32 d){
    mDamageFactor = d;
}

F32 CRigidBody::getDamageFactor(){
    return mDamageFactor;
}

U32 CRigidBody::getContents(){
	return mContents;
}

void CRigidBody::setContents(U32 c){
	mContents = c;
}

U32 CRigidBody::getCollisionMask(){
	return mCollisionMask;
}

void CRigidBody::setCollisionMask(U32 c){
	mCollisionMask = c;
}

void CRigidBody::addForce(const CVector3 &f){
    dBodyAddForce(mBodyID, TO_PHYSICS(f.x), TO_PHYSICS(f.y), TO_PHYSICS(f.z));
}

void CRigidBody::addTorque(const CVector3 &f){
    dBodyAddTorque(mBodyID, f.x, f.y, f.z);
}

void CRigidBody::addLocalForce(const CVector3 &f){
    dBodyAddRelForce(mBodyID, TO_PHYSICS(f.x), TO_PHYSICS(f.y), TO_PHYSICS(f.z));
}

void CRigidBody::addLocalTorque(const CVector3 &f){
    dBodyAddRelTorque(mBodyID, f.x, f.y, f.z);
}

void CRigidBody::addForceAt(const CVector3 &f, const CVector3 &pos){
    dBodyAddForceAtPos(mBodyID, TO_PHYSICS(f.x), TO_PHYSICS(f.y), TO_PHYSICS(f.z), TO_PHYSICS(pos.x), TO_PHYSICS(pos.x), TO_PHYSICS(pos.z));
}

void CRigidBody::addForceAtLocal(const CVector3 &f, const CVector3 &pos){
    dBodyAddForceAtRelPos(mBodyID, TO_PHYSICS(f.x), TO_PHYSICS(f.y), TO_PHYSICS(f.z), TO_PHYSICS(pos.x), TO_PHYSICS(pos.x), TO_PHYSICS(pos.z));
}

void CRigidBody::addLocalForceAt(const CVector3 &f, const CVector3 &pos){
    dBodyAddRelForceAtPos(mBodyID, TO_PHYSICS(f.x), TO_PHYSICS(f.y), TO_PHYSICS(f.z), TO_PHYSICS(pos.x), TO_PHYSICS(pos.x), TO_PHYSICS(pos.z));
}

void CRigidBody::addLocalForceAtLocal(const CVector3 &f, const CVector3 &pos){
    dBodyAddRelForceAtRelPos(mBodyID, TO_PHYSICS(f.x), TO_PHYSICS(f.y), TO_PHYSICS(f.z), TO_PHYSICS(pos.x), TO_PHYSICS(pos.x), TO_PHYSICS(pos.z));
}

void CRigidBody::makeImmovable(){
    if(mGeomID)
		dGeomSetBody(mGeomID, 0);
	if(mBodyID){
		//dBodyDestroy(mBodyID); //FIXME
		mBodyID = 0;
	}
}

void CRigidBody::setSolid(bool s){
	mSolid = s;
}

bool CRigidBody::isSolid(){
	return mSolid;
}

void CRigidBody::setSuspendMultiCollitions(bool m){
	mSuspendMultiCollitions = m;
}

bool CRigidBody::getSuspendMultiCollitions(){
	return mSuspendMultiCollitions;
}

void CRigidBody::setSleep(bool s){
    if(mBodyID){
        if(s)
            dBodyDisable(mBodyID);
        else
		    dBodyEnable(mBodyID);
	}
}

void CRigidBody::applyGravity(bool g){
	if(mBodyID){
		dBodySetGravityMode(mBodyID, g ? 1 : 0);
	}
}

CAABoundBox dummyAABB;

CBound &CRigidBody::getBounds(){
    if(mSceneObject)
	    return mSceneObject->getDerivedBound();
    return dummyAABB;
}

void CRigidBody::setDensity(F32 density){
}

void CRigidBody::setMass(F32 mass){
}

F32 CRigidBody::getMass(){
    if(mBodyID){
        dMass m;
        dBodyGetMass(mBodyID, &m);
        return m.mass;
    }
    return 1.0;
}

void CRigidBody::fitToVertices(CVector3 *v, S32 num){
}

void CRigidBody::fitToMesh(CRenderable &rm){
    CVector4 *v = rm.getVertices();
    std::vector<CVector3> verts;
	for(S32 j = 0 ; j < rm.getNumOfVertices() ; j++)
        verts.push_back(v[j].toVec3());

	fitToVertices(&verts[0], rm.getNumOfVertices());
}

void CRigidBody::fitToModel(CRenderableMultiMesh &rm){
	CRenderableMultiMesh::RMeshVector &meshes = rm.getMeshes();
	CRenderableMultiMesh::RMeshVector::iterator i;
	std::vector<CVector3> verts;
	for(i = meshes.begin() ; i != meshes.end() ; i++){
		CVector4 *v = (*i)->getVertices();
		for(S32 j = 0 ; j < (*i)->getNumOfVertices() ; j++)
            verts.push_back(v[j].toVec3());
	}
	fitToVertices(&verts[0], verts.size());
}

void CRigidBody::noCollitionWith(CRigidBody &b){
	mExcudedInCollition.insert(&b);
}

bool CRigidBody::isCollitionExcluded(CRigidBody &b){
    if(mExcudedInCollition.find(&b) != mExcudedInCollition.end()){
		return true;
    } else {
		return false;
    }
}

void CRigidBody::update(){
    if(mSceneObject){
	    dReal r[4];
	    dGeomGetQuaternion(mGeomID, r);
	    //if(!disabled)
		    mSceneObject->setOrientation(r[1], r[2], r[3], r[0]);

	    const dReal *d;
	    d = dGeomGetPosition(mGeomID);
	    CVector3 off = mSceneObject->getOrientation() * mScenePositionOffset;
	    mSceneObject->setPosition(TO_WORLD(d[0]) - off.x, 
		    					  TO_WORLD(d[1]) - off.y, 
			    				  TO_WORLD(d[2]) - off.z);
    }

	dReal aabb[6];
	dGeomGetAABB(mGeomID, aabb);
	CVector3 Min(aabb[0],aabb[2],aabb[4]);
	CVector3 Max(aabb[1],aabb[3],aabb[5]);
    Min *= PHYSICS_TO_WORLD_RATIO;
    Max *= PHYSICS_TO_WORLD_RATIO;
    if(mSceneObject){
        mSceneObject->getBound().setMin(Min - mSceneObject->getPosition());
	    mSceneObject->getBound().setMax(Max - mSceneObject->getPosition());
    }
    mBBox.setMax(Max - mPosition);// + CVector3(100, 100, 100));
    mBBox.setMin(Min - mPosition);// - CVector3(100, 100, 100));
}

void CRigidBody::perStepUpdate(F64 stepTime){
    if(!mBodyID){
        setPosition(getPosition() + getVelocity() * stepTime);
        return;
    }
    if(mPath){
        updatePath(stepTime);
        CVector3 pos = mPath->getAt(mPathPosition);
        CVector3 nextPos = mPath->getAt(mPathPosition + stepTime);
        setPosition(pos);
        setVelocity((nextPos - pos)/stepTime);
    }
    if(mLookAtPath){
        updateLookAtPath(stepTime);
        //lookAt(mLookAtPath->getAt(mLookAtPathPosition));
    }
}

void CRigidBody::onCollide(CRigidBody *other){
    if(mListener)
	    mListener->handleCollition(other);
}

void CRigidBody::setGroundBody(CRigidBody *body){
    mGroundBody = body;
}

CRigidBody *CRigidBody::getGroundBody(){
    return mGroundBody;
}

void CRigidBody::draw() const {

}
