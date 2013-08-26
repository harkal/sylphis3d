/***************************************************************************
                         skeleton.h  -  description
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
