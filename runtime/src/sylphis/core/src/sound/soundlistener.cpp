/***************************************************************************
                        soundlistener.cpp  -  description
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

#include <al/al.h>
#include <al/alut.h>
#include <console.h>

#include <sound/soundlistener.h>

#include <serializer.h>

IMPLEMENT_SERIALIZABLE(CSoundListener);

CSoundListener::CSoundListener(){
}

CSoundListener::CSoundListener(CSoundEnv *se) : CSoundObject(se) {
}

CSoundListener::~CSoundListener(){

}

void CSoundListener::updateSound(){
    const CVector3 &v = getDerivedPosition();
	alListener3f(AL_POSITION, v.x, v.y, v.z);
	alListenerfv(AL_VELOCITY, &getVelocity().x);
	CVector3 ori[2];
	CQuaternion q = getDerivedOrientation();
	ori[0] = q * CVector3(0, 0, -1);
	ori[1] = q * CVector3(0, 1, 0);
	alListenerfv(AL_ORIENTATION, &ori[0].x);	
}

void CSoundListener::save(CSerializer *ser) const {
	CSoundObject::save(ser);
}

void CSoundListener::load(CSerializer *ser){
	CSoundObject::load(ser);
}
