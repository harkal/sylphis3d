/***************************************************************************
                         bonetrack.cpp  -  description
                             -------------------
    begin                : Fri Dec 13 2002
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

