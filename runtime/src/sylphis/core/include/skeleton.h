/***************************************************************************
                         skeleton.h  -  description
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

#ifndef _SKELETON_H_
#define _SKELETON_H_

#include <list>
#include <map>
#include <string>

#include <vector.h>
#include <quaternion.h>
#include <refobject.h>
#include <resource.h>
#include <vfs.h>

SmartPointer(CBone);
SmartPointer(CSkeleton);

class CBone : public CRefObject {
public:
    CBone();
    virtual ~CBone();

    void setName(const std::string &name);
    const std::string &getName();

    void addChildId(S32 id);
    void setSkeleton(CSkeleton *s);

    void setParentId(S32 i);
    S32 getParentId();

    void setTranslation(const CVector3 &v){
        mTranslation = v;
    }
    const CVector3& getTranslation(){
        return mTranslation;
    }
    void setDerivedTranslation(const CVector3 &v){
        mTranslation = v;
    }
    const CVector3& getDerivedTranslation() const {
        return mTranslation;
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
        mRotation = q;
    }
    const CQuaternion& getDerivedRotation(){
        return mRotation;
    }
    void setRotationBoneSpace(const CQuaternion &q){
        mRotationBoneSpace = q;
    }
    const CQuaternion& getRotationBoneSpace(){
        return mRotationBoneSpace;
    }

protected:
    std::string mName;
    CSkeletonPtr mSkeleton;
    S32 mParentId;
    std::list<S32> mChildIds;
    CVector3 mTranslation;
    CQuaternion mRotation;
    CVector3 mDerivedTranslation;
    CQuaternion mDerivedRotation;
    CVector3 mTranslationBoneSpace;
    CQuaternion mRotationBoneSpace;
};


class CSkeleton : public CResource {
public:
    CSkeleton();
    virtual ~CSkeleton();

    virtual void load(const std::string &n, CVirtualFS *vfs);
    typedef std::vector<CBonePtr> BoneVector;
	typedef std::map<std::string, CBonePtr> BoneMap;
    BoneVector &getBones();
	CBone *getBoneByName(const std::string &name);
protected:
    void loadBone(CFile &f);
    void fileCorrupted();
    static const S32 EARLIEST_COMPATIBLE_FILE_VERSION;
    static const S32 CURRENT_FILE_VERSION;
    BoneVector mBones;
    BoneVector mRootBones;
	BoneMap mBoneMap;
};

#endif
