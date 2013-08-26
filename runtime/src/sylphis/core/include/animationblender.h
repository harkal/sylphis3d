/***************************************************************************
                      animationblender.h  -  description
                             -------------------
    begin                : Wed Dec 18 2002
    copyright            : (C) 2002, 2003 by Harry Kalogirou
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

#ifndef _ANIMATIONBLENDER_H_
#define _ANIMATIONBLENDER_H_

#include <list>
#include <string>
#include <refobject.h>
#include <pointer.h>
#include <skelanimation.h>

#include <python/waitable.h>

SmartPointer(CAnimation);
class CAnimation : public CRefObject {
public:
    enum  EState {
        STATE_NONE,
        STATE_SYNC,
        STATE_ASYNC,
        STATE_IN,
        STATE_STEADY,
        STATE_OUT
    };
	CAnimation(CSkelAnimation *sa = 0);
    virtual ~CAnimation();

    F32 getDuration();
	virtual void setBlend(F32 fullWeight, F32 blendDelay){};
    EState getState(){ return mState; }
	void setState(EState state){ mState = state; }
	void setClearing(bool c){ mClearing = c; }
	bool getClearing(){ return mClearing; }
    F32 getWeight(){ return mWeight; }
    F32 getTime(){ return mTime; }
	void setName(const std::string &name){ mName = name; }
	const std::string &getName(){ return mName; }
    CSkelAnimation *getSkelAnimation();

    virtual bool update(F32 time){return false;};
protected:
	std::string mName;
    CSkelAnimationPtr mSkelAnimation;
    F32 mTime;
    F32 mWeight;
	bool mClearing;
    EState mState;
};

SmartPointer(CAnimationCycle);
class CAnimationCycle : public CAnimation {
public:
    CAnimationCycle(CSkelAnimation *sa = 0);
    virtual ~CAnimationCycle();

    virtual void setBlend(F32 fullWeight, F32 blendDelay);
    virtual bool update(F32 time);
protected:
    F32 mBlendDelay;
    F32 mFullWeight;
};

SmartPointer(CAnimationAction);
class CAnimationAction : public CAnimation {
public:
    CAnimationAction(CSkelAnimation *sa = 0);
    virtual ~CAnimationAction();

	virtual void execute(F32 delayIn, F32 delayOut);
    virtual bool update(F32 time);
protected:
    F32 mDelayIn;
    F32 mDelayOut;
	F32 mDelayTarget;
};

class CSkeletonInstance;

class CAnimationBlender : public CWaitable {
public:
    CAnimationBlender();
    ~CAnimationBlender();

    typedef std::list<CAnimationCyclePtr> AnimationCycleList;
	typedef std::list<CAnimationActionPtr> AnimationActionList;

    void update(F32 time);
    void addAnimationCycle(CAnimationCycle *a);
	void addAnimationAction(CAnimationAction *a);

	F32 getAnimWeight(const std::string &name);
    void blendCycle(CSkelAnimation *anim, const std::string &name, F32 weight, F32 delay);
	void weightCycle(const std::string &name, F32 weight, F32 delay);
	void clearCycle(const std::string &name, F32 delay);
	void clearAllCycle(F32 delay);

	void executeAction(CSkelAnimation *anim, const std::string &name, F32 delayIn, F32 delayOut);

    void updateSkeleton(CSkeletonInstance *s);
protected:
    AnimationCycleList mAnimationCycles;
	AnimationActionList mAnimationActions;
    F32 mAnimationTime;
    F32 mAnimationDuration;
};

#endif
