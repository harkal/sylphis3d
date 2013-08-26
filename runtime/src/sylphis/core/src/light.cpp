/***************************************************************************
						light.cpp  -  description
                             -------------------
    begin                : Thu Apr 4 2002
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

#include <renderer.h>
#include <light.h>
#include <serializer.h>
#include <gutil.h>

const char table[] = {0,0,1,0,
					  1,1,0,1,
					  0,1,1,0,
					  0,1,1,0,
					  1,0,1,0,
					  1,0,1,1,
					  0,1,1,0,
					  1,1,0,1,
					  1,0,1,0,
					  1,0,1,0};
const S32 tablenum = 40;

IMPLEMENT_SERIALIZABLE(CLight);

CLight::CLight(){

}

CLight::CLight(CSceneManager *sm) : CSceneObject(sm) {
	mColor = CVector4(1.0, 1.0, 1.0, 1.0);
	setCenter(0.0, 0.0, 0.0);
    setRadius(100.0);
	setIsShadowCaster(true);
	setType(omni);
	mTime = 0.0;
	mColorScale = 1.0;
	setActive(true);
	setSpecular(true);
	setAttenuation(0); // The default
    mFixed = false; // Start as dynamic
}
CLight::~CLight(){
}

void CLight::setType(EType t){
    mType = t;
}

CLight::EType CLight::getType() const {
    return mType;
}

void CLight::setColor(CVector4 &color){
    mColor = color;
}

void CLight::setColor(CVector3 &color){
    mColor = color;
	mColorScaled = color * mColorScale;
}

CVector4 &CLight::getColor(){
	if(getType() == sun){
		return mColorScaled;// * (getDerivedOrientation() * CVector3(0,0,1) * CVector3::UNIT_Y);
	} else {
		return mColorScaled;
	}
}

void CLight::setFilter(CTexture *f){
	mFilter = f;
}

CTexture *CLight::getFilter(){
	return mFilter;
}

void CLight::setAttenuation(CTexture *f){
	mAttenuation = f;
}

CTexture *CLight::getAttenuation(){
	return mAttenuation;
}

void CLight::setRadius(F32 r){
	if(r < 0.0)return;
    setRadius(r, r, r);
}

void CLight::setRadius(F32 x, F32 y, F32 z){
	mRadius.x = x;
	mRadius.y = y;
	mRadius.z = z;
	mBound.setBounds(CVector3(-x,-y,-z), CVector3(x,y,z));
}

void CLight::setRadius(const CVector3 &v){
	setRadius(v.x, v.y, v.z);
}

void CLight::setCenter(const CVector3 &c){
	mCenter = c;
}

void CLight::setCenter(F32 x, F32 y, F32 z){
	setCenter(CVector3(x, y, z));
}

void CLight::setPosition(const CVector3 &p){
	CSceneObject::setPosition(p);
}

CVector3 CLight::getEmissionPosition(){
	if(getType() == sun){
		return getDerivedOrientation() * CVector3(0,0,1) * 6000.0;
	} else {
		return getDerivedPosition() + mCenter;
	}
}

CVector3 &CLight::getPosition(){
	return CSceneObject::getPosition();
}

CVector3 CLight::getDerivedPosition() const {
	return CSceneObject::getDerivedPosition();
}

CVector3 CLight::getDirection() const {
	return getDerivedOrientation() * CVector3(0,0,1);
}

const CVector3 &CLight::getRadius(){
    return mRadius;
}

const CVector3 &CLight::getCenter(){
    return mCenter;
}

void CLight::setIsShadowCaster(bool c){
	mCastShadows = c;
}

bool CLight::isShadowCaster(){
	return mCastShadows;
}

void CLight::setSpecular(bool c){
	mSpecular = c;
}

bool CLight::givesSpecular(){
	return mSpecular; 
}
/*
void CLight::fix(){
}

void CLight::unfix(){
}

bool CLight::isFixed(){
    return mFixed;
}
*/
bool CLight::isActive(){
	return mActive;
}

void CLight::setActive(bool a){
	mActive = a;
}

void CLight::setPhase(F32 f){
	mPhase = f;
}

void CLight::setFreq(F32 f){
	mFreq = f;
}

void CLight::updateData(F64 time){
    CSceneObject::updateData(time);

	mTime += time;
}

void CLight::render(CRenderer *renderer){
	if((S32)getType() & (S32)flickering){
		if(table[(S32)(WrapToOne(mTime*mFreq + mPhase) * tablenum)])
			setActive(false);
		else
			setActive(true);
	} 
	else if((S32)getType() & (S32)strobo){
		if(WrapToOne(mTime * mFreq + mPhase) < 0.5)
			setActive(false);
		else
			setActive(true);
	}
	else if((S32)getType() & (S32)pulsating){
		 mColorScale = sin(WrapToOne(mTime * mFreq + mPhase) * 2 * PI);
		 mColorScaled = mColor * mColorScale;
	}
	if(isActive())
		renderer->addLight(this);
}

void CLight::save(CSerializer *ser) const {
	CSceneObject::save(ser);

	ser->writeUnsignedInt((S32)mType);
	mColor.save(ser);
	mColorScaled.save(ser);
	ser->writeFloat(mColorScale);
	ser->writeBool(mActive);
	ser->writeSerializable(mFilter);
	ser->writeSerializable(mAttenuation);
    mRadius.save(ser);
	mCenter.save(ser);
	ser->writeBool(mCastShadows);
	ser->writeBool(mSpecular);
    ser->writeBool(mFixed);

	ser->writeFloat(mTime);
	ser->writeFloat(mPhase);
	ser->writeFloat(mFreq);
}

void CLight::load(CSerializer *ser) {
	CSceneObject::load(ser);

	mType = (EType)ser->readUnsignedInt();
	mColor.load(ser);
	mColorScaled.load(ser);
	mColorScale = ser->readFloat();
	mActive = ser->readBool();
	mFilter = (CTexture *)(ser->readSerializable());
	mAttenuation = (CTexture *)(ser->readSerializable());
    mRadius.load(ser);
	mCenter.load(ser);
	mCastShadows = ser->readBool();
	mSpecular = ser->readBool();
    mFixed = ser->readBool();

	mTime = ser->readFloat();
	mPhase = ser->readFloat();
	mFreq = ser->readFloat();
}


