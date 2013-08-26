/***************************************************************************
                      skeletoninstance.h  -  description
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
