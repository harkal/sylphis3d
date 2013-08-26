/***************************************************************************
                        soundlistener.cpp  -  description
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
