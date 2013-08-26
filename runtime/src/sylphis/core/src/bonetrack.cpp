/***************************************************************************
                         bonetrack.cpp  -  description
                             -------------------
    begin                : Fri Dec 13 2002
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

#include <bonetrack.h>
#include <bonekeyframe.h>
#include <vector.h>
#include <quaternion.h>
#include <vfs.h>
#include <exception.h>

CBoneTrack::CBoneTrack(){
}

CBoneTrack::~CBoneTrack(){
}

void CBoneTrack::setBoneId(S32 boneId){
    mBoneId = boneId;
}

S32 CBoneTrack::getBoneId(){
    return mBoneId;
}

void CBoneTrack::addBoneKeyframe(CBoneKeyframe &kf){
    mKeyframes.insert(std::make_pair(kf.getTime(), kf));
}

void CBoneTrack::getAt(F32 time, CVector3 &trans, CQuaternion &rot){
    BoneKeyframeMap::iterator keyframeAfter;

    keyframeAfter = mKeyframes.upper_bound(time);

    if(keyframeAfter == mKeyframes.end()){
        keyframeAfter--;
        trans = keyframeAfter->second.getTranslation();
        rot = keyframeAfter->second.getRotation();
        return;
    } 
    
    if(keyframeAfter == mKeyframes.begin()){
        trans = keyframeAfter->second.getTranslation();
        rot = keyframeAfter->second.getRotation();
        return;
    }
    
    CBoneKeyframe &after = (keyframeAfter--)->second;
    CBoneKeyframe &before = keyframeAfter->second;

    F32 f = (time - before.getTime()) / (after.getTime() - before.getTime());

    trans = before.getTranslation();
    trans.lerp(f, after.getTranslation());

    rot = before.getRotation();
    rot.lerp(f, after.getRotation());
}

void CBoneTrack::load(CFile *f){
    setBoneId(f->readInt());

    S32 numKeyframes = f->readInt();
    if(numKeyframes < 0)
        throw CException("File is corrupted");

    for(S32 i = 0 ; i < numKeyframes ; i++){
        F32 time = f->readFloat();
        F32 x,y,z,w;
        x = f->readFloat();
        y = f->readFloat();
        z = f->readFloat();
        CVector3 trans(x, y, z);
        trans.swapHand();
        x = f->readFloat();
        y = f->readFloat();
        z = f->readFloat();
        w = f->readFloat();
        CQuaternion rot(x, y, z, w);
        rot.swapHand();
        
        CBoneKeyframe kf;
        kf.setTime(time);
        kf.setTranslation(trans);
        kf.setRotation(rot);
        addBoneKeyframe(kf);
    }
}

