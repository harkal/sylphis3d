/***************************************************************************
                         joint.h  -  description
                             -------------------
    begin                : Sun Jul 20 2003
    copyright            : (C) 2003 by Harry Kalogirou
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


#ifndef _JOINT_H_
#define _JOINT_H_

#include <pointer.h>
#include <vector.h>
#include <physics.h>

#include <ode/ode.h>

class CRigidBody;

SmartPointer(CJoint);
class CJoint : public CRefObject {
public:
	enum eClass {
		JOINT_BALL = dJointTypeBall,
		JOINT_HINGE = dJointTypeHinge,
		JOINT_SLIDER = dJointTypeSlider,
		JOINT_CONTACT = dJointTypeContact,
		JOINT_UNIVERSAL = dJointTypeUniversal,
		JOINT_HINGE2 = dJointTypeHinge2,
		JOINT_FIXED = dJointTypeFixed,
		JOINT_AMOTOR = dJointTypeAMotor
	};
public:

	CJoint();
	virtual ~CJoint();

	void attach(CRigidBody *b1, CRigidBody *b2);
	void attachToWorld(CRigidBody *body);
	dBodyID getBody(S32 i);

	eClass getClass() const{
		return (eClass)dJointGetType(mID);
	}
protected:
	dJointID mID;
};

SmartPointer(CJointBall);
class CJointBall : public CJoint {
public:
	CJointBall(){}
	CJointBall(S32 world, S32 group = 0);

	void setAnchor(const CVector3 &a);
	CVector3 getAnchor() const;
};

SmartPointer(CJointHinge);
class CJointHinge : public CJoint {
public:
	CJointHinge(){}
	CJointHinge(S32 world, S32 group = 0);

	void setAnchor(const CVector3 &a);
	CVector3 getAnchor() const;

	void setAxis(const CVector3 &a);
	CVector3 getAxis() const;

	void setLoStop(F32 value) { dJointSetHingeParam(mID, dParamLoStop,value); }
	void setHiStop(F32 value) { dJointSetHingeParam(mID, dParamHiStop,value); }
	void setStopBounce(F32 value) { dJointSetHingeParam(mID, dParamBounce,value); }
	void setStopCFM(F32 value) { dJointSetHingeParam(mID, dParamCFM,value); }
	void setStopERP(F32 value) { dJointSetHingeParam(mID, dParamStopERP,value); }
	void setMotorVelocity(F32 value) { dJointSetHingeParam(mID, dParamVel,TO_PHYSICS(value)); }
	void setMotorMaxForce(F32 value) { dJointSetHingeParam(mID, dParamFMax,TO_PHYSICS(value)); }
	void setFudgeFactor( F32 value) { dJointSetHingeParam(mID, dParamFudgeFactor,value); }
	void setCFM(F32 value) { dJointSetHingeParam(mID, dParamCFM,value); }
	void setSuspensionCFM(F32 value) { dJointSetHingeParam(mID, dParamSuspensionCFM,value); }
	void setSuspensionERP(F32 value) { dJointSetHingeParam(mID, dParamSuspensionERP,value); }

	F32 getLoStop() const { return dJointGetHingeParam(mID, dParamLoStop); }
	F32 getHiStop() const { return dJointGetHingeParam(mID, dParamHiStop); }
	F32 getStopBounce() const { return dJointGetHingeParam(mID, dParamBounce); }
	F32 getStopCFM() const { return dJointGetHingeParam(mID, dParamCFM); }
	F32 getStopERP() const { return dJointGetHingeParam(mID, dParamStopERP); }
	F32 getMotorVelocity() const { return TO_WORLD(dJointGetHingeParam(mID, dParamVel)); }
	F32 getMotorMaxForce() const { return TO_WORLD(dJointGetHingeParam(mID, dParamFMax)); }
	F32 getFudgeFactor() const { return dJointGetHingeParam(mID, dParamFudgeFactor); }
	F32 getCFM() const { return dJointGetHingeParam(mID, dParamCFM); }
	F32 getSuspensionCFM() const { return dJointGetHingeParam(mID, dParamSuspensionCFM); }
	F32 getSuspensionERP() const { return dJointGetHingeParam(mID, dParamSuspensionERP); }
};

SmartPointer(CJointDoubleHinge);
class CJointDoubleHinge : public CJoint {
public:
	CJointDoubleHinge(){}
	CJointDoubleHinge(S32 world, S32 group = 0);

	void setAnchor(const CVector3 &a);
	CVector3 getAnchor() const;

	F32	getWeelAngle() const { return dJointGetHinge2Angle1(mID); }

	void setAxis1(const CVector3 &a);
	CVector3 getAxis1() const;
    void setAxis2(const CVector3 &a);
	CVector3 getAxis2() const;

	void setWeelVelocity(F32 value) { dJointSetHinge2Param(mID, dParamVel,TO_PHYSICS(value)); }
	void setWeelMaxForce(F32 value) { dJointSetHinge2Param(mID, dParamFMax,TO_PHYSICS(value)); }
	void setLoStop(F32 value) { dJointSetHinge2Param(mID, dParamLoStop,value); }
	void setHiStop(F32 value) { dJointSetHinge2Param(mID, dParamHiStop,value); }
	void setStopBounce(F32 value) { dJointSetHinge2Param(mID, dParamBounce,value); }
	void setStopCFM(F32 value) { dJointSetHinge2Param(mID, dParamCFM,value); }
	void setStopERP(F32 value) { dJointSetHinge2Param(mID, dParamStopERP,value); }
	void setMotorVelocity(F32 value) { dJointSetHinge2Param(mID, dParamVel2,TO_PHYSICS(value)); }
	void setMotorMaxForce(F32 value) { dJointSetHinge2Param(mID, dParamFMax2,TO_PHYSICS(value)); }
	void setFudgeFactor( F32 value) { dJointSetHinge2Param(mID, dParamFudgeFactor,value); }
	void setCFM(F32 value) { dJointSetHinge2Param(mID, dParamCFM,value); }
	void setSuspensionCFM(F32 value) { dJointSetHinge2Param(mID, dParamSuspensionCFM,value); }
	void setSuspensionERP(F32 value) { dJointSetHinge2Param(mID, dParamSuspensionERP,value); }

	F32 getWeelVelocity() const { return TO_WORLD(dJointGetHinge2Param(mID, dParamVel)); }
	F32 getWeelMaxForce() const { return TO_WORLD(dJointGetHinge2Param(mID, dParamFMax)); }
	F32 getLoStop() const { return dJointGetHinge2Param(mID, dParamLoStop); }
	F32 getHiStop() const { return dJointGetHinge2Param(mID, dParamHiStop); }
	F32 getStopBounce() const { return dJointGetHinge2Param(mID, dParamBounce); }
	F32 getStopCFM() const { return dJointGetHinge2Param(mID, dParamCFM); }
	F32 getStopERP() const { return dJointGetHinge2Param(mID, dParamStopERP); }
	F32 getMotorVelocity() const { return TO_WORLD(dJointGetHinge2Param(mID, dParamVel2)); }
	F32 getMotorMaxForce() const { return TO_WORLD(dJointGetHinge2Param(mID, dParamFMax2)); }
	F32 getFudgeFactor() const { return dJointGetHinge2Param(mID, dParamFudgeFactor); }
	F32 getCFM() const { return dJointGetHinge2Param(mID, dParamCFM); }
	F32 getSuspensionCFM() const { return dJointGetHinge2Param(mID, dParamSuspensionCFM); }
	F32 getSuspensionERP() const { return dJointGetHinge2Param(mID, dParamSuspensionERP); }
};

SmartPointer(CJointSlider);
class CJointSlider : public CJoint {
public:
	CJointSlider(){}
	CJointSlider(S32 world, S32 group = 0);

	void setAnchor(const CVector3 &a);
	CVector3 getAnchor() const;

	void setAxis(const CVector3 &a);
	CVector3 getAxis() const;

	void setLoStop(F32 value) { dJointSetSliderParam(mID, dParamLoStop,TO_PHYSICS(value)); }
	void setHiStop(F32 value) { dJointSetSliderParam(mID, dParamHiStop,TO_PHYSICS(value)); }
	void setStopBounce(F32 value) { dJointSetSliderParam(mID, dParamBounce,value); }
	void setStopCFM(F32 value) { dJointSetSliderParam(mID, dParamCFM,value); }
	void setStopERP(F32 value) { dJointSetSliderParam(mID, dParamStopERP,value); }
	void setMotorVelocity(F32 value) { dJointSetSliderParam(mID, dParamVel,TO_PHYSICS(value)); }
	void setMotorMaxForce(F32 value) { dJointSetSliderParam(mID, dParamFMax,TO_PHYSICS(value)); }
	void setFudgeFactor( F32 value) { dJointSetSliderParam(mID, dParamFudgeFactor,value); }
	void setCFM(F32 value) { dJointSetSliderParam(mID, dParamCFM,value); }
	void setSuspensionCFM(F32 value) { dJointSetSliderParam(mID, dParamSuspensionCFM,value); }
	void setSuspensionERP(F32 value) { dJointSetSliderParam(mID, dParamSuspensionERP,value); }

	F32 getLoStop() const { return TO_WORLD(dJointGetSliderParam(mID, dParamLoStop)); }
	F32 getHiStop() const { return TO_WORLD(dJointGetSliderParam(mID, dParamHiStop)); }
	F32 getStopBounce() const { return dJointGetSliderParam(mID, dParamBounce); }
	F32 getStopCFM() const { return dJointGetSliderParam(mID, dParamCFM); }
	F32 getStopERP() const { return dJointGetSliderParam(mID, dParamStopERP); }
	F32 getMotorVelocity() const { return TO_WORLD(dJointGetSliderParam(mID, dParamVel)); }
	F32 getMotorMaxForce() const { return TO_WORLD(dJointGetSliderParam(mID, dParamFMax)); }
	F32 getFudgeFactor() const { return dJointGetSliderParam(mID, dParamFudgeFactor); }
	F32 getCFM() const { return dJointGetSliderParam(mID, dParamCFM); }
	F32 getSuspensionCFM() const { return dJointGetSliderParam(mID, dParamSuspensionCFM); }
	F32 getSuspensionERP() const { return dJointGetSliderParam(mID, dParamSuspensionERP); }
};

SmartPointer(CJointUniversal);
class CJointUniversal : public CJoint {
public:
	CJointUniversal(){}
	CJointUniversal(S32 world, S32 group = 0);

	void setAnchor(const CVector3 &a);
	CVector3 getAnchor() const;

	void setAxis1(const CVector3 &a);
	CVector3 getAxis1() const;

	void setAxis2(const CVector3 &a);
	CVector3 getAxis2() const;
};

#endif
