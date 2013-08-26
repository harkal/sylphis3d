/***************************************************************************
                          sceneobject.cpp  -  description
                             -------------------
    begin                : Mon Jun 3 2002
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

#include <assert.h>

#include <sceneobject.h>
#include <scenenode.h>
#include <scenemanager.h>
#include <matrix.h>
#include <renderer.h>
#include <frustum.h>
#include <serializer.h>

IMPLEMENT_SERIALIZABLE(CSceneObject);

CSceneObject::CSceneObject(){
}

CSceneObject::CSceneObject(CSceneManager *sm){
    assert(sm && "Instatiating CSceneObject without scene manager");
    //gcon.printf("New CSceneObject %p\n", this);
	mExtraData = 0;
	setFlags(0);
    setParent(0);
    setSceneManager(sm);
	mPosition = CVector3::ZERO;
	//mOrientation = CQuaternion::IDENTITY;
	mOrientation = CQuaternion(0.0, 0.0, 0.0, 1.0);
	CMatrix4 m ;
	mOrientation.toRotationMatrix(m);
    setPassClosedPortals(false);
}

CSceneObject::~CSceneObject(){
    //gcon.printf("Free CSceneObject %p\n", this);
#if 1
	if(getParent()){
		getParent()->removeChild(this);
	}
#endif
}

void CSceneObject::setSceneManager(CSceneManager *sm){
    mSceneManager = sm;
	if(mExtraData)
		delete [] mExtraData;
	mSceneManager->registerObject(this);
}

CSceneManager *CSceneObject::getSceneManager(){
    return mSceneManager;
}

void CSceneObject::setParent(CSceneNode *node){
	mParent = node;
}

CSceneNode *CSceneObject::getParent(){
	return mParent;
}

bool CSceneObject::passClosedPortals(){
    return mPassClosedPortals;
}

void CSceneObject::setPassClosedPortals(bool p){
    mPassClosedPortals = p;
}

void CSceneObject::setOrientation(const CQuaternion &q){
	mOrientation = q;
}

void CSceneObject::setOrientation(const F32 x, const F32 y, const F32 z, const F32 w){
	mOrientation.x = x;
	mOrientation.y = y;
	mOrientation.z = z;
	mOrientation.w = w;
}

CQuaternion CSceneObject::getOrientation() const {
	return mOrientation;
}

void CSceneObject::lookAt(const CVector3 &pos, bool localSpace){
    CVector3 dir = getDerivedPosition() - pos;
    normalize(dir);
    CVector3 xAxis(CVector3::UNIT_Y ^ dir);
    normalize(xAxis);
    CVector3 yAxis(dir ^ xAxis);
    normalize(yAxis);
    if(localSpace){
        
    } else {
        mOrientation.fromAxes(xAxis, yAxis, dir);
    }
}

void CSceneObject::setPosition(const CVector3 &p){
	mPosition = p;
}

void CSceneObject::setPosition(const F32 x, const F32 y, const F32 z){
	setPosition(CVector3(x, y, z));
}

CVector3 &CSceneObject::getPosition() {
	return mPosition;
}

CQuaternion CSceneObject::getDerivedOrientation() const {
	return mDerivedOrientation;
}

CVector3 CSceneObject::getDerivedPosition() const {
	return mDerivedPosition;
}

CMatrix4 CSceneObject::getTransformMatrix(){
	CMatrix4 m;

	mDerivedOrientation.toRotationMatrix(m);
    m.setTransform(mDerivedPosition);

	return m;
}

CAABoundBox &CSceneObject::getBound(){
    return mBound;
}

CAABoundBox &CSceneObject::getDerivedBound(){
    return mDerivedBound;
}



void CSceneObject::move(const CVector3 &v){
	mPosition += v;
}

void CSceneObject::moveLocal(const CVector3 &v){
	mPosition += mOrientation * v;
}

void CSceneObject::rotate(const CQuaternion &q){
    mOrientation = q * mOrientation;
}

void CSceneObject::rotate(const CVector3 &axis, F32 rad){
	CQuaternion q;
	q.fromAngleAxis(axis, rad);
    rotate(q);
}

void CSceneObject::roll(F32 rad){
		// Rotate around Z axis
		CVector3 axis = mOrientation * CVector3::UNIT_Z;
		rotate(axis, rad);
}

void CSceneObject::yaw(F32 rad){
		// Rotate around Y axis
		CVector3 axis = mOrientation * CVector3::UNIT_Y;
		rotate(axis, rad);
}

void CSceneObject::pitch(F32 rad){
		// Rotate around X axis
		CVector3 axis = mOrientation * CVector3::UNIT_X;
		rotate(axis, rad);
}

void CSceneObject::updateData(F64 time){
	if(mParent){
		mDerivedOrientation = mOrientation * mParent->mDerivedOrientation;
		mDerivedPosition = mParent->mDerivedPosition + (mParent->mDerivedOrientation * mPosition);
	} else {
		mDerivedOrientation = mOrientation;
		mDerivedPosition = mPosition;
	}
}

void CSceneObject::update(F64 time, bool startedHere){
    if(mPath){
        updatePath(time);
        setPosition(mPath->getAt(mPathPosition));
    }
    if(mLookAtPath){
        updateLookAtPath(time);
        lookAt(mLookAtPath->getAt(mLookAtPathPosition));
    }
	updateData(time);
    updateBound();
}

void CSceneObject::updateBound(){
    mDerivedBound = mBound;
    mDerivedBound.translate(getDerivedPosition());
	mSceneManager->spatialize(this);
}

void CSceneObject::propagateBoundToParent(){
    if (mParent){
        mParent->updateBound();
        mParent->propagateBoundToParent();
    }
}

void CSceneObject::onRender(CRenderer *renderer){
	if((!(getFlags() & NOCULL)) && mSceneManager->isCulled(this))
		return;

    if(getFlags() & NORENDER)
        return;

    render(renderer);
}


void CSceneObject::render(CRenderer *renderer){

}

void CSceneObject::postRenderUpdate(CRenderer *renderer){
	if((!(getFlags() & NOCULL)) && mSceneManager->isCulled(this))
		return;

    if(getFlags() & NORENDER)
        return;

	mLastOrientation = getDerivedOrientation();
    mLastPosition = getDerivedPosition();
}

void CSceneObject::save(CSerializer *ser) const {
	CRefObject::save(ser);
	    
	CSceneManager* mSceneManager;
	CSceneNode* mParent;

	mOrientation.save(ser);
	mPosition.save(ser);

	mDerivedOrientation.save(ser);
	mDerivedPosition.save(ser);

    mBound.save(ser);
    mDerivedBound.save(ser);
    ser->writeBool(mPassClosedPortals);

	// SERFIX : void *mExtraData;
	ser->writeInt(mFlags);
}

void CSceneObject::load(CSerializer *ser) {
	CRefObject::load(ser);

	mOrientation.load(ser);
	mPosition.load(ser);

	mDerivedOrientation.load(ser);
	mDerivedPosition.load(ser);

    mBound.load(ser);
    mDerivedBound.load(ser);
    mPassClosedPortals = ser->readBool();

	// SERFIX : void *mExtraData;
	mFlags = ser->readInt();
}

//
// CPathFollower
//

void CPathFollower::followPath(CInterpolatorVector3 &path, F32 startAt, F32 loopAt){
    mPath = &path;
    mPathPosition = startAt;
    mPathLoopPosition = loopAt;
}

void CPathFollower::lookAtPath(CInterpolatorVector3 &path, F32 startAt, F32 loopAt){
    mLookAtPath = &path;
    mLookAtPathPosition = startAt;
    mLookAtPathLoopPosition = loopAt;
}

void CPathFollower::stopFollowPath(){
    mPath = 0;
}

void CPathFollower::stopLookAtPath(){
    mLookAtPath = 0;
}



void CPathFollower::updatePath(F64 time){
    mPathPosition += time;
    if(mPathLoopPosition > 0.0){
        if(mPathPosition > mPathLoopPosition){
            mPathPosition -= mPathLoopPosition;
        }
    }
}

void CPathFollower::updateLookAtPath(F64 time){
    mLookAtPathPosition += time;
    if(mLookAtPathLoopPosition > 0.0){
        if(mLookAtPathPosition > mLookAtPathLoopPosition){
            mLookAtPathPosition -= mLookAtPathLoopPosition;
        }
    }
    
}

