/***************************************************************************
                         skelanimation.cpp  -  description
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

#include <skelanimation.h>
#include <vfs.h>
#include <exception.h>

CSkelAnimation::CSkelAnimation(){

}

CSkelAnimation::~CSkelAnimation(){

}

void CSkelAnimation::addBoneTrack(CBoneTrack *bt){
    mBoneTracks.push_back(bt);
}

void CSkelAnimation::setDuration(F32 t){
    mDuration = t;
}

const S32 CSkelAnimation::EARLIEST_COMPATIBLE_FILE_VERSION = 699;
const S32 CSkelAnimation::CURRENT_FILE_VERSION = 910;

void CSkelAnimation::load(const std::string &n, CVirtualFS *vfs){
    setName(n);

    CFile f(n);
    vfs->readFile(f);

    if(f.readChar() != 'C' || f.readChar() != 'A' || f.readChar() != 'F' || f.readChar() != 0)
        throw CException("Not skeleton file");

    S32 version = f.readInt();
    if(version < EARLIEST_COMPATIBLE_FILE_VERSION || version > CURRENT_FILE_VERSION)
		throw CException("File version not supported");

    F32 duration = f.readFloat();
    if(duration < 0.0)
        throw CException("File corrupt");

    setDuration(duration);

    S32 numTracks = f.readInt();
    if(numTracks < 0)
        throw CException("File corrupt");

    for(S32 i = 0 ; i < numTracks ; i++){
        CBoneTrackPtr tr = new CBoneTrack();
        tr->load(&f);
        addBoneTrack(tr);
    }
}

//
// ********* Skeleton Controler ***********
//

CBoneControler::CBoneControler(){

}

CBoneControler::~CBoneControler(){

}

void CBoneControler::getAt(F32 time, CVector3 &trans, CQuaternion &rot){
    trans = mTranslation;
    rot = mRotation;
}

void CBoneControler::setTranslation(const CVector3 &t){
    mTranslation = t;
}

const CVector3& CBoneControler::getTranslation() const {
    return mTranslation;
}

void CBoneControler::setRotation(const CQuaternion &q){
    mRotation = q;
}

const CQuaternion& CBoneControler::getRotation() const {
    return mRotation;
}


CSkelControler::CSkelControler(){

}

CSkelControler::~CSkelControler(){

}



