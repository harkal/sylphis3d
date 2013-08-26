/***************************************************************************
                         bonekeyframe.h  -  description
                             -------------------
    begin                : Fri Dec 13 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#ifndef _BONEKEYFRAME_H_
#define _BONEKEYFRAME_H_

#include <vector.h>
#include <quaternion.h>

class CBoneKeyframe {
public:
    CBoneKeyframe();
    ~CBoneKeyframe();

    void setTranslation(const CVector3 &v){
        mTranslation = v;
    }
    const CVector3& getTranslation(){
        return mTranslation;
    }
    void setRotation(const CQuaternion &q){
        mRotation = q;
    }
    const CQuaternion& getRotation(){
        return mRotation;
    }
    void setTime(F32 time){
        mTime = time;
    }
    F32 getTime(){
        return mTime;
    }
protected:
    CVector3 mTranslation;
    CQuaternion mRotation;
    F32 mTime;
};

#endif
