/***************************************************************************
                         skeleton.cpp  -  description
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

#include <exception.h>
#include <skeleton.h>

using namespace std;

CBone::CBone(){
}

CBone::~CBone(){
}

void CBone::setName(const std::string &name){
    mName = name;
}

const std::string &CBone::getName(){
    return mName;
}

void CBone::addChildId(S32 id){
    mChildIds.push_back(id);
}

void CBone::setSkeleton(CSkeleton *s){
    mSkeleton = s;
}

void CBone::setParentId(S32 i){
    mParentId = i;
}

S32 CBone::getParentId(){
    return mParentId;
}

CSkeleton::CSkeleton(){
}

CSkeleton::~CSkeleton(){
}

CSkeleton::BoneVector &CSkeleton::getBones(){
    return mBones;
}

const S32 CSkeleton::EARLIEST_COMPATIBLE_FILE_VERSION = 699;
const S32 CSkeleton::CURRENT_FILE_VERSION = 910;

void CSkeleton::fileCorrupted(){
    throw CException("File corrupted");
}

void CSkeleton::loadBone(CFile &f){
    string boneName;
    boneName = f.readString();
    if(boneName == "")
        fileCorrupted();

    F32 x,y,z,w;
    x = f.readFloat();
    y = f.readFloat();
    z = f.readFloat();
    CVector3 trans(x, y, z);
    trans.swapHand();
    x = f.readFloat();
    y = f.readFloat();
    z = f.readFloat();
    w = f.readFloat();
    CQuaternion rot(x, y, z, w);
    rot.swapHand();

    x = f.readFloat();
    y = f.readFloat();
    z = f.readFloat();
    CVector3 transBoneSpace(x, y, z);
    transBoneSpace.swapHand();
    x = f.readFloat();
    y = f.readFloat();
    z = f.readFloat();
    w = f.readFloat();
    CQuaternion rotBoneSpace(x, y, z, w);
    rotBoneSpace.swapHand();

    S32 parentId = f.readInt();

    S32 childCount = f.readInt();
    if(childCount < 0)
        fileCorrupted();

    CBonePtr bone = new CBone();
    
    for(S32 i = 0 ; i < childCount ; i++){
        S32 childId = f.readInt();
        if(childId < 0)
            fileCorrupted();
        bone->addChildId(childId);
    }

    bone->setName(boneName);
    bone->setSkeleton(this);
    bone->setParentId(parentId);
    bone->setTranslation(trans);
    bone->setRotation(rot);
    bone->setTranslationBoneSpace(transBoneSpace);
    bone->setRotationBoneSpace(rotBoneSpace);

    mBones.push_back(bone);
	mBoneMap.insert(BoneMap::value_type(boneName.c_str(), bone));

    // If the bone has no parent, add it to the
    // rootbone vector too.
    if(bone->getParentId() == -1)
        mRootBones.push_back(bone);
}

CBone *CSkeleton::getBoneByName(const std::string &name){
	return mBoneMap[name];
}

void CSkeleton::load(const std::string &n, CVirtualFS *vfs){
    setName(n);

    CFile f(n);
    vfs->readFile(f);

    if(f.readChar() != 'C' || f.readChar() != 'S' || f.readChar() != 'F' || f.readChar() != 0)
        throw CException("Not skeleton file");

    S32 version = f.readInt();
    if(version < EARLIEST_COMPATIBLE_FILE_VERSION || version > CURRENT_FILE_VERSION)
        throw CException("File version not supported");

    S32 numBones = f.readInt();
    if(numBones <= 0)
        fileCorrupted();

    for(S32 bone = 0 ; bone < numBones ; bone++){
        loadBone(f);
    }
}
