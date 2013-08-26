/***************************************************************************
                        soundobject.cpp  -  description
                             -------------------
    begin                : Fri Jun 6 2003
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

