/***************************************************************************
                        actormover.h  -  description
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

#ifndef ACTOR_MOVER_H
#define ACTOR_MOVER_H

#include <vector.h>
#include <scenemanager.h>

#define MR_PREFECT	1
#define MR_UPSTARED 2
#define	MR_SLIDED	4

#define ACTION_FORWARD			1 
#define ACTION_BACKWARD			2 
#define ACTION_MOVE_LEFT		4 
#define ACTION_MOVE_RIGHT		8 
#define ACTION_TURN_LEFT		16 
#define ACTION_TURN_RIGHT		32 
#define ACTION_JUMP				128 
#define ACTION_ATTACK			256 

class CActor;
class CRigidBody;
#include <trace.h>

#define	MAX_TOUCHOBJECTS	10
typedef struct {
	S32 flags;
	S32	numtobjects;					//The number of touched objects
	CActor *tActors[MAX_TOUCHOBJECTS];	//The touched objects
	CVector3 end;
	CTraceResult groundtrace;
} moveresult_t;

class CActorMover {
public:
	CActorMover(CSceneManager *sm);
	~CActorMover();
	void playerMove(moveresult_t *mr, CRigidBody *body, S32 action, F64 time);
    void move(CRigidBody *body, const CVector3 &wishVel, CRigidBody *ground, F64 time);

	CVector3 mGravity;
	F32 mAirFric;
	F32 mFric;
	F32 mStopSpeed;
	F32 mMaxSpeed;
	F32 mAirAccel;
	F32 mAccel;
	F32 mStepSize;
protected:
	S32 moveSlide(moveresult_t *mr, CRigidBody *body, const CVector3 &pos, const CVector3 &vel);
	void wishSpeed(CRigidBody *body, CVector3 &velocity, S32 action);
	void accelerate(CRigidBody *body, CVector3 &dir, F32 velocity, F32 accel, F64 time);
	void walkMove(CRigidBody *body, S32 action, F64 time);
	CSceneManager *mSm;
};

#endif

