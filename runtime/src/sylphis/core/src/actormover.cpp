/***************************************************************************
                       actormover.cpp  -  description
                             -------------------
    begin                : Thu Nov 28 2002
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

#include <vector.h>
#include <actormover.h>
#include <engine.h>

CActorMover::CActorMover(CSceneManager *sm){
	mSm = sm;
	mGravity = CVector3(0.0, -800.0, 0.0);
	mFric = 6.0;
	mAirFric = 0.0;
	mStopSpeed = 100.0;
	mMaxSpeed = 200.0;
	mAirAccel = 1.0;
	mAccel = 6.0;
	mStepSize = 30.0;
}

CActorMover::~CActorMover(){
}

void CActorMover::playerMove(moveresult_t *mr, CRigidBody *body, S32 action, F64 time){
#if 0
	CTraceResult gtrace,tr;
	CVector3 end;
	CVector3 delta;
	CVector3 start, start2; 
	S32 clip_flag, temp;
	CActor *tactor;

	assert(mSm);

	walkMove(body, action, time);
	delta = body->mVelocity * time;

	start = start2 = body->mPosition;

	clip_flag = moveSlide(mr, body, start, delta); 
	start = mr->end;
	// If we are going up don't step up
	if (body->mVelocity.y<=0.0f){
		// We had a possible step ahead (wall and ground)
        if ((clip_flag&2)&&(body->getGroundBody()) { 
			start2.y += mStepSize; 
			moveresult_t mr2;
			temp = moveSlide(&mr2, body, start2, delta); 
			start2 = mr2.end;
			// Step successfull not start_solid 
			if (temp!=4) { 
				clip_flag=temp; 
				// Push down to the ground
				start = start2;
				start.y -= mStepSize;
				//trace(&tr, start2, start, obj->mins, obj->maxs, obj, CONTENTS_SOLID|CONTENTS_PLAYERCLIP);
				mSm->trace(&tr, body->mBBox, start2, start);
				tr.mEnd.y += 0.1f;	// To avoid random stucks in solid
				mr->end = tr.mEnd;
			}
		} 
	}

	// Slightly below player
	end.x = mr->end.x;
	end.y = mr->end.y - 0.15f;
	end.z = mr->end.z;
	
	// Trace for ground
	mSm->trace(&gtrace, body->mBBox, mr->end, end);
	mr->groundtrace = gtrace;
	//memcpy(&mr->groundtrace, &gtrace, sizeof(moveresult_t));

	//mSm->getEngine().con.printf("%f ", gtrace.frac);
/*	if(gtrace.frac == 0.0){
		mr->end.y += 0.1;
	}
*/
	// We are airborn
	if(gtrace.frac == 1.0f){
		body->setOffGround(true);
	} else {
		if(body->getGround())body->mVelocity.y *= 0.1; //IMPACT
		body->setOffGround(false);
	}
    //gcon.printf("%d\n", body->isOffGround());
#endif
}

void CActorMover::move(CRigidBody *body, const CVector3 &wishVel, CRigidBody *ground, F64 time){
    CVector3 gvel;
    
    if(ground){
        gvel = ground->getVelocity();
    } else {
        gvel = CVector3::ZERO;
    }

    CVector3 wantPos = body->getPosition() + (wishVel+gvel) * time;
    CVector3 bePos = body->getPosition() + body->getVelocity() * time;

    CVector3 force = (wantPos - bePos) * 300.0 / time;
    body->addForce(force);

    //gcon.printf("%f %f\n", body->getVelocity().length(), wishVel.length());
}

void CActorMover::wishSpeed(CRigidBody *body, CVector3 &velocity, S32 action){
	CVector3 temp;

	if (action&ACTION_MOVE_RIGHT)
		temp.x = .8f;
	if (action&ACTION_MOVE_LEFT)
		temp.x = -.8f;

	if (action&ACTION_FORWARD)
		temp.z = -1.0f;
	if (action&ACTION_BACKWARD)
		temp.z = 1.0f;

	velocity = body->mOrientation * temp;
}

void CActorMover::accelerate(CRigidBody *body, CVector3 &dir, F32 speed, F32 accel, F64 time){
	F32 addSpeed, accelSpeed, currentSpeed;

	currentSpeed = body->mVelocity * dir;
	addSpeed = speed - currentSpeed;
	if (addSpeed <= 0) {
		return;
	}
	accelSpeed = accel * time * speed;
	if (accelSpeed > addSpeed) {
		accelSpeed = addSpeed;
	}
	
	body->mVelocity += dir * accelSpeed;
}

S32 CActorMover::moveSlide(moveresult_t *mr, CRigidBody *body, const CVector3 &pos, const CVector3 &vel){ 
	CPlane *plane; 
	CVector3 safe, start, end, delta, v; 
	F32 dot; 
	S32 i, clip_flag = 0; 
	CTraceResult tr;
	CVector3 oldorigin;
 
	start = safe = pos; 
	delta = vel;
	end = start + delta;
 
	//clipFlags = CONTENTS_TRIGGER | CONTENTS_SOLID | CONTENTS_PLAYERCLIP | CONTENTS_BODY;
	for(i=0;i<4;i++) {
			// Trace to where we are going
			mSm->trace(&tr, body->mBBox, start, end);

			//trace(&tr, start, end, obj->mins, obj->maxs, obj, clipFlags);
			

		if (tr.frac == 0.0) {  
		// Really stuck 
			if (i==0){
#if 0
				gcon.printf("actormove panic : stuck in solid\n");
#endif
				mr->end = safe;
				return 4;
			}
		// We are stuck due to previous slide
			start = safe;
			end = start + delta;
			continue; 
		} 
 
		// New safe position
		safe = tr.mEnd;

		// If moved all the way, break 
		if (tr.frac==1.0) break; 

		// Find new delta to move along 
		plane = &tr.plane;
 
		if (plane->normal.y > 0.8) clip_flag|=1; 
		if (plane->normal.y == 0.0) clip_flag|=2; 
 
		delta *= 1.0f - tr.frac;
		dot = plane->normal * delta;
		v = plane->normal * dot;
		delta -= v;
 
		if (delta[0]>-0.1&&delta[0]<0.1) delta[0]=0.0; 
		if (delta[1]>-0.1&&delta[1]<0.1) delta[1]=0.0; 
		if (delta[2]>-0.1&&delta[2]<0.1) delta[2]=0.0; 
 
		if (vel * delta < 0) { 
			delta = CVector3::ZERO; 
		} 
 
		v = plane->normal * 0.05f;
		start = v + tr.mEnd;
		end = start + delta;
	} 

//	Sys_Printf("%d\n",i);

	mr->end = safe;

	return clip_flag; 
}

void CActorMover::walkMove(CRigidBody *body, S32 action, F64 time){
#if 0
	F32 speedlen;
	F32 amount;
	F32 drop;
	F32 fric;
	CVector3 wishspeed;

	// If we are airborn apply gravity
	if(body->isOffGround())
		body->mVelocity += mGravity * time;
	
	// friction
	speedlen = body->mVelocity.length();
	if(speedlen > 0.0f){
		drop = 0;
	
		// Set the right friction size
		if(body->isOffGround()){
			fric = mAirFric;
		} else {
			fric = mFric;
		}
		
		amount = speedlen < mStopSpeed ? mStopSpeed : speedlen;
		drop += amount * fric * time;
	
		if(speedlen < drop)
			drop = speedlen;
		
		body->mVelocity *= 1 - drop / speedlen;
		//VectorMulScalar(obj->speed, 1 - drop / speedlen ,obj->speed);
	}
	
	// Apply user accelarations
	wishspeed = CVector3::ZERO;
	wishSpeed(body, wishspeed, action);
	wishspeed.y = 0.0f;

	if(wishspeed.x + wishspeed.z > 0.0)
		normalize(wishspeed);
	
	accelerate(body, wishspeed, mMaxSpeed, body->isOffGround() ? mAirAccel : mAccel, time);
#endif
}

