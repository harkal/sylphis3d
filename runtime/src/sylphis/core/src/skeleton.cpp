/***************************************************************************
                         skeleton.cpp  -  description
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
