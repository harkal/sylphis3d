/***************************************************************************
                         skelanimation.h  -  description
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
