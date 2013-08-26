/***************************************************************************
                         rigidbody.h  -  description
                             -------------------
    begin                : Sat Nov 9 2002
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

#ifndef RIGIDBODYSTATIC_H
#define RIGIDBODYSTATIC_H

#include <rigidbody.h>

class CModel;

SmartPointer(CRigidBodyStatic);
class CRigidBodyStatic : public CRigidBody {
public:
	CRigidBodyStatic(S32 parent, CSceneObject *so = 0);
	virtual ~CRigidBodyStatic();

	eClass getClass() const{
		return (eClass)0;
	}

	void setMeshData(CVector3 *verts, U32 vertnum, S32 *elems, U32 elemsnum);
	void setMeshData(CModel *m);
	
    virtual void setSleep(bool s);

    virtual void addForce(const CVector3 &f);
    virtual void addTorque(const CVector3 &f);
    virtual void addLocalForce(const CVector3 &f);
    virtual void addLocalTorque(const CVector3 &f);
    virtual void addForceAt(const CVector3 &f, const CVector3 &pos);
    virtual void addForceAtLocal(const CVector3 &f, const CVector3 &pos);
    virtual void addLocalForceAt(const CVector3 &f, const CVector3 &pos);
    virtual void addLocalForceAtLocal(const CVector3 &f, const CVector3 &pos);
    
	virtual void makeImmovable();
	virtual void applyGravity(bool g);

	virtual void setPosition(const CVector3 &o);
	virtual void setPosition(const F32 x, const F32 y, const F32 z);
	virtual CVector3 getPosition();
	virtual void setOrientation(const CQuaternion &o);
	virtual void setOrientation(const F32 x, const F32 y, const F32 z, const F32 w);
	virtual CQuaternion getOrientation();
	virtual void setVelocity(const CVector3 &v);
	virtual CVector3 getVelocity();
	virtual void setAngVelocity(const CVector3 &v);
	virtual CVector3 getAngVelocity();

    virtual void update();
};

#endif


