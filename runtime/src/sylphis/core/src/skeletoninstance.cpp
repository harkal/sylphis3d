/***************************************************************************
                      skeletoninstance.cpp  -  description
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
#include <quaternion.h>
#include <skeletoninstance.h>
#include <console.h>

using namespace std;

CBoneInstance::CBoneInstance(CBone *b){
    mBone = b;
    reset();
}

CBoneInstance::~CBoneInstance(){
}

const std::string &CBoneInstance::getName(){
	return mBone->getName();
}

void CBoneInstance::update(){
    if(mAccWeight == 0.0){
        mTranslation = mBone->getTranslation();
        mRotation = mBone->getRotation();
    }

    S32 parentId = mBone->getParentId();
    if(parentId != -1){
        CBoneInstance *parent = mSkeleton->getBone(parentId);
        CQuaternion q = parent->getDerivedRotation();
        q.invert();
        mDerivedTranslation = parent->getDerivedTranslation() + (q * mTranslation);
        q.invert();
        mDerivedRotation = mRotation * q;
    } else {
        mDerivedTranslation = mTranslation;
        mDerivedRotation = mRotation;
    }

    CQuaternion q = mDerivedRotation;
    q.invert();
    mTranslationBoneSpace = q * mBone->getTranslationBoneSpace();
    mTranslationBoneSpace += mDerivedTranslation;
    q.invert();
    setRotationBoneSpace(mBone->getRotationBoneSpace() * q);//mDerivedRotation;

    BoneVector::iterator i;
    for(i = mChildren.begin() ; i != mChildren.end() ; i++){
        (*i)->update();
    }
}

void CBoneInstance::reset(){
    mAccWeight = mAccWeightAbs = 0.0;
}

void CBoneInstance::setSkeleton(CSkeletonInstance *s){
    mSkeleton = s;
}

void CBoneInstance::addChild(CBoneInstance *b){
    mChildren.push_back(b);
}

void CBoneInstance::setParentBone(CBoneInstance *p){
    mParentBone = p;
}

CBoneInstance *CBoneInstance::getParentBone(){
    return mParentBone;
}

CSkeletonInstance::CSkeletonInstance(CSkeleton *s){
    mSkeleton = s;

    CSkeleton::BoneVector &bv = s->getBones();
    mBones.reserve(bv.size());

    CSkeleton::BoneVector::iterator i;
    for(i = bv.begin() ; i != bv.end() ; i++){
        CBoneInstancePtr b = new CBoneInstance(*i);
        b->setSkeleton(this);
                
        if((*i)->getParentId() == -1){
            mRootBones.push_back(b);
            b->setParentBone(0);
        } else {
            b->setParentBone(mBones[(*i)->getParentId()]);
        }
        
        mBones.push_back(b);
		//gcon.printf("Bone : %s|\n", b->getName().c_str());
		mBoneMap.insert(BoneMap::value_type(b->getName().c_str(), b));
    }

    // Register the children
    S32 j = 0;
    for(i = bv.begin() ; i != bv.end() ; i++, j++){
        if((*i)->getParentId() == -1)
            continue;

        mBones[(*i)->getParentId()]->addChild(mBones[j]);
    }
}

void CBoneInstance::fixate(){
    if(mAccWeightAbs > 1.0 - mAccWeight){
        mAccWeightAbs = 1.0 - mAccWeight;
    }

    if(mAccWeightAbs > 0.0){
        if(mAccWeight == 0.0){
            mTranslation = mDerivedTranslation;
            mRotation = mDerivedRotation;

            mAccWeight = mAccWeightAbs;
        } else {
            F32 factor;
            factor = mAccWeightAbs / (mAccWeight + mAccWeightAbs);

            mTranslation.lerp(factor, mDerivedTranslation);
            mRotation.lerp(factor, mDerivedRotation);
            mAccWeight += mAccWeightAbs;
        }

        mAccWeightAbs = 0.0;
    }
}

void CBoneInstance::blend(F32 weight, const CVector3 &trans, const CQuaternion &rot){
    if(mAccWeightAbs == 0.0f) {
        mDerivedTranslation = trans;
        mDerivedRotation = rot;

        mAccWeightAbs = weight;
    } else {
        F32 factor;
        factor = weight / (mAccWeightAbs + weight);

        mDerivedTranslation.lerp(factor, trans);
        mDerivedRotation.lerp(factor, rot);

        mAccWeightAbs += weight;
    }
}

CSkeletonInstance::~CSkeletonInstance(){
}

CSkeletonInstance::BoneVector &CSkeletonInstance::getBones(){
    return mBones;
}

void CSkeletonInstance::update(){
    BoneVector::iterator i;

    for(i = mRootBones.begin() ; i != mRootBones.end() ; i++){
        (*i)->update();
    }
}

void CSkeletonInstance::reset(){
    BoneVector::iterator i;

    for(i = mBones.begin() ; i != mBones.end() ; i++){
        (*i)->reset();
    }
}

CBoneInstance *CSkeletonInstance::getBone(S32 i){
    return mBones[i];
}

CBoneInstance *CSkeletonInstance::getBoneByName(const std::string &name){
	return mBoneMap[name];
}

void CSkeletonInstance::fixate(){
    BoneVector::iterator i;
    
	mBound.setBounds(CVector3::ZERO, CVector3::ZERO);
    for(i = mBones.begin() ; i != mBones.end() ; i++){
		mBound.addPoint((*i)->getDerivedTranslation());
        (*i)->fixate();
    }
}

CAABoundBox &CSkeletonInstance::getBound(){
	return mBound;
}

