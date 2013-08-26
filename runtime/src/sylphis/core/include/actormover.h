/***************************************************************************
                        actormover.h  -  description
                             -------------------
    begin                : Thu Nov 28 2002
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

