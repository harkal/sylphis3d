/***************************************************************************
                      skeletoninstance.h  -  description
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

#ifndef _SKELETONINSTANCE_H_
#define _SKELETONINSTANCE_H_

#include <map>
#include <vector>

#include <vector.h>
#include <quaternion.h>
#include <matrix3.h>
#include <refobject.h>
#include <pointer.h>
#include <skeleton.h>
#include <boundbox.h>

SmartPointer(CBoneInstance);
SmartPointer(CSkeletonInstance);

class CBoneInstance : public CRefObject {
public:
    CBoneInstance(CBone *b = 0);
    virtual ~CBoneInstance();

    typedef std::vector<CBoneInstancePtr> BoneVector;

    void update();
    void reset();

	const std::string &getName();

    void setSkeleton(CSkeletonInstance *s);
    void addChild(CBoneInstance *b);
    void setParentBone(CBoneInstance *p);
    CBoneInstance *getParentBone();

    void setTranslation(const CVector3 &v){
        mTranslation = v;
    }
    const CVector3& getTranslation(){
        return mTranslation;
    }
    void setDerivedTranslation(const CVector3 &v){
        mDerivedTranslation = v;
    }
    const CVector3& getDerivedTranslation() const {
        return mDerivedTranslation;
    }
    void setTranslationBoneSpace(const CVector3 &v){
        mTranslationBoneSpace = v;
    }
    const CVector3& getTranslationBoneSpace() const {
        return mTranslationBoneSpace;
    }
    void setRotation(const CQuaternion &q){
        mRotation = q;
    }
    const CQuaternion& getRotation(){
        return mRotation;
    }
    void setDerivedRotation(const CQuaternion &q){
        mDerivedRotation = q;
    }
    const CQuaternion& getDerivedRotation(){
        return mDerivedRotation;
    }
    void setRotationBoneSpace(const CQuaternion &q){
        mRotationBoneSpace = q;
        mRotationBoneSpace.invert();
        mRotationBoneSpace.toRotationMatrix(mBoneSpaceMatrix);
        mRotationBoneSpace.invert();
        //mBoneSpaceMatrix.setTransform(mTranslationBoneSpace);
    }
    const CQuaternion& getRotationBoneSpace(){
        return mRotationBoneSpace;
    }
    const CMatrix3 &getBoneSpaceMatrix() const {
        return mBoneSpaceMatrix;
    }

    void fixate();
    void blend(F32 weight, const CVector3 &trans, const CQuaternion &rot);

protected:
    CBonePtr mBone;
    CBoneInstance*      mParentBone;
    CSkeletonInstancePtr mSkeleton;
    
    BoneVector mChildren;
    CVector3 mTranslation;
    CQuaternion mRotation;
    CVector3 mDerivedTranslation;
    CQuaternion mDerivedRotation;
    CVector3 mTranslationBoneSpace;
    CQuaternion mRotationBoneSpace;
    CMatrix3 mBoneSpaceMatrix;

    F32 mAccWeight;
    F32 mAccWeightAbs;
};


class CSkeletonInstance : public CRefObject {
public:
    CSkeletonInstance(CSkeleton *s = 0);
    virtual ~CSkeletonInstance();
    typedef std::vector<CBoneInstancePtr> BoneVector;
	typedef std::map<std::string, CBoneInstancePtr> BoneMap;

    void update();
    void reset();

	CBoneInstance *getBoneByName(const std::string &name);
    CBoneInstance *getBone(S32 i);

    BoneVector &getBones();

    void fixate();

	CAABoundBox &getBound();

protected:
    CSkeletonPtr mSkeleton;
    BoneVector mBones;
    BoneVector mRootBones;
	BoneMap mBoneMap;

	CAABoundBox mBound;
};

#endif
