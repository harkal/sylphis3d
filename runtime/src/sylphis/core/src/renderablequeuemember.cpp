/***************************************************************************
                          renderablequeuemember.cpp  -  description
                             -------------------
    begin                : Thu Jun 6 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#include "renderablequeuemember.h"

CRenderableQueueMember::CRenderableQueueMember(){
    setNoShadows(false);
}

CRenderableQueueMember::~CRenderableQueueMember(){
    mRenderable = 0;
}

void CRenderableQueueMember::setOrientation(const CQuaternion &q){
	mOrientation = q;
}

CQuaternion &CRenderableQueueMember::getOrientation() {
	return mOrientation;
}

void CRenderableQueueMember::setPosition(const CVector3 &p){
	mPosition = p;
}

CVector3 &CRenderableQueueMember::getPosition() {
	return mPosition;
}

void CRenderableQueueMember::setBounds(const CAABoundBox &bbox){
    mBound = bbox;
}

CAABoundBox &CRenderableQueueMember::getBounds(){
    return mBound;
}

CMatrix4 CRenderableQueueMember::getTransformMatrix() const {
	CMatrix4 m;

	mOrientation.toRotationMatrix(m);
	m.setTransform(mPosition);

	return m;
}

CMatrix4 CRenderableQueueMember::getInverseTransformMatrix() const {
	CMatrix4 m;

	CQuaternion q = mOrientation;
	q.invert();
	q.toRotationMatrix(m);
	m.setTransform(-mPosition);

	return m;
}

void CRenderableQueueMember::setLastTransform(const CQuaternion &q, const CVector3 &v){
    mLastOrientation = q;
    mLastPosition = v;
}

CMatrix4 CRenderableQueueMember::getLastTransformMatrix() const {
    CMatrix4 m;

	mLastOrientation.toRotationMatrix(m);
	m.setTransform(mLastPosition);

	return m;
}

void CRenderableQueueMember::setRenderable(CRenderable *r){
    mRenderable = r;
}

CRenderable *CRenderableQueueMember::getRenderable(){
    return mRenderable;
}

void CRenderableQueueMember::setNoShadows(bool b){
    mNoShadows = b;
}

bool CRenderableQueueMember::getNoShadows() const {
    return mNoShadows;
}
