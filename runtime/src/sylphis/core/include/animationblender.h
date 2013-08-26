/***************************************************************************
                      animationblender.h  -  description
                             -------------------
    begin                : Wed Dec 18 2002
    copyright            : (C) 2002, 2003 by Harry Kalogirou
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
