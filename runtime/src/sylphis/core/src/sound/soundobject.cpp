/***************************************************************************
                        soundobject.cpp  -  description
                             -------------------
    begin                : Fri Jun 6 2003
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

#include <sound/soundenv.h>
#include <sound/soundobject.h>
#include <console.h>
#include <serializer.h>

IMPLEMENT_SERIALIZABLE(CSoundObject);

CSoundObject::CSoundObject(){

}

CSoundObject::CSoundObject(CSoundEnv *se) : CSceneObject(se->getSceneManager()) {
	//se->registerSoundObject(this);
	mListenerActor = 0;
	mSoundEnv = se;
}

CSoundObject::~CSoundObject(){
}

void CSoundObject::setVelocity(const CVector3 &v){
	mVelocity = v;
}

CVector3 &CSoundObject::getVelocity(){
	return mVelocity;
}

void CSoundObject::updateSound(){

}

void CSoundObject::updateData(F64 time){
	CVector3 pos = getDerivedPosition();
	CSceneObject::updateData(time);
    CVector3 vel = (getDerivedPosition() - pos) / (time * 16.0f);
	setVelocity(vel);
    //gcon.printf("Velocity : %f\n", vel.length());
	updateSound();
}

void CSoundObject::render(CRenderer *renderer){
}

void CSoundObject::setListener(CActor *a){
	mListenerActor = a;
}

void CSoundObject::save(CSerializer *ser) const {
	CSceneObject::save(ser);

    mVelocity.save(ser);
//    ser->writeSerializable(mSoundEnv);
//    ser->writeSerializable(mListenerActor);
}

void CSoundObject::load(CSerializer *ser){
	CSceneObject::load(ser);

    mVelocity.load(ser);
//    mSoundEnv = (CSoundEnv *)ser->readSerializable();
//    mListenerActor = (CActor *)ser->readSerializable();
}

