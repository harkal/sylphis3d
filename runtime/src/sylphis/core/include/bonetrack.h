/***************************************************************************
                         bonetrack.h  -  description
                             -------------------
    begin                : Fri Dec 13 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#ifndef _BONETRACK_H_
#define _BONETRACK_H_

#include <map>
#include <pointer.h>
#include <bonekeyframe.h>

class CFile;

SmartPointer(CBoneTrack);
class CBoneTrack : public CRefObject {
public:
                    CBoneTrack();
    virtual         ~CBoneTrack();

    void            setBoneId(S32 boneId);
    S32             getBoneId();

    void            addBoneKeyframe(CBoneKeyframe &kf);

    virtual void    getAt(F32 time, CVector3 &trans, CQuaternion &rot);

    void            load(CFile *f);

    typedef std::map<F32, CBoneKeyframe> BoneKeyframeMap;
protected:
    S32             mBoneId;
    BoneKeyframeMap mKeyframes;
};

#endif
