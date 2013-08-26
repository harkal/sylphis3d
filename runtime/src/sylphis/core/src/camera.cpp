/***************************************************************************
                          camera.cpp  -  description
                             -------------------
    begin                : Tue Mar 12 2002
    copyright            : (C) 2002 by Harry Kalogirou
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

#include "globaldefs.h"
#include "camera.h"
#include <console.h>
#include <serializer.h>

IMPLEMENT_SERIALIZABLE(CCamera);

CCamera::CCamera(){
}

CCamera::CCamera(CSceneManager *sm) : CSceneNode(sm) {
    setActive(false);
}
CCamera::~CCamera(){
}

void CCamera::setFov(F32 f){
	if (f < 15.0)fov = 15.0;
	else if (f > 170.0)fov = 170.0;
	else fov = f;
}

F32 CCamera::getFov(){
	return fov;
}

void CCamera::updateFrustum(CMatrix4 &clip){
    mFrustum.makeFrustumPlanes(clip);
}

CFrustum &CCamera::getFrustum(){
    return mFrustum;
}

void CCamera::setActive(bool active){
    mActive = active;
}

bool CCamera::isActive(){
    return mActive;
}

void CCamera::render(CRenderer *renderer){
//	CQuaternion q = getDerivedOrientation();
//	CVector3 v = getDerivedPosition();
//	gcon.printf("%f %f %f -- %f %f %f\n", q.x, q.y, q.z, v.x, v.y, v.z);
	CSceneNode::render(renderer);
}

void CCamera::yawFixed(F32 rad){
	rotate(CVector3::UNIT_Y, rad);
}

void CCamera::pitchFixed(F32 rad){
	rotate(CVector3::UNIT_X, rad);
}

void CCamera::save(CSerializer *ser) const {
	CSceneNode::save(ser);
	ser->writeFloat(fov);
	mFrustum.save(ser);
	ser->writeBool(mActive);
}

void CCamera::load(CSerializer *ser){
	CSceneNode::load(ser);
	fov = ser->readFloat();
	mFrustum.load(ser);
	mActive = ser->readBool();
}

