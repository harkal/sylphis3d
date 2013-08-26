/***************************************************************************
                         skelanimation.h  -  description
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

#ifndef _SKELANIMATION_H_
#define _SKELANIMATION_H_

#include <vector>
#include <resource.h>
#include <bonetrack.h>

class CVirtualFS;

SmartPointer(CSkelAnimation);
class CSkelAnimation : public CResource {
public:
                    CSkelAnimation();
    virtual         ~CSkelAnimation();

    virtual void    load(const std::string &n, CVirtualFS *vfs);
    virtual void    addBoneTrack(CBoneTrack *bt);
    
    typedef std::vector<CBoneTrackPtr> BoneTrackVector;

    BoneTrackVector&    getBoneTracks(){ return mBoneTracks; }
    F32               getDuration(){ return mDuration; };
    void                setDuration(F32 t);
protected:
    static const S32    EARLIEST_COMPATIBLE_FILE_VERSION;
    static const S32    CURRENT_FILE_VERSION;
    BoneTrackVector     mBoneTracks;
    F32               mDuration;
};

//
// ********* Skeleton Controler ***********
//

SmartPointer(CBoneControler);
class CBoneControler : public CBoneTrack {
public:
                        CBoneControler();
    virtual             ~CBoneControler();

    virtual void        getAt(F32 time, CVector3 &trans, CQuaternion &rot);

    void                setTranslation(const CVector3 &t);
    const CVector3&     getTranslation() const;

    void                setRotation(const CQuaternion &q);
    const CQuaternion&  getRotation() const;
protected:
    CVector3            mTranslation;
    CQuaternion         mRotation;
};

SmartPointer(CSkelControler);
class CSkelControler :  public CSkelAnimation {
public:
    CSkelControler();
    virtual ~CSkelControler();


protected:
    
};

#endif
