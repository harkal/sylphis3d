/***************************************************************************
                       rigidbox.cpp  -  description
                             -------------------
    begin                : Tue Jul 15 2002
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

#include <boundbox.h>
#include <rigidcapsule.h>

extern dSpaceID space;
extern dWorldID world;

//#define SIM

CRigidCapsule::CRigidCapsule(S32 parent, CSceneObject *so, const CVector3 &d ) : CRigidBody(parent, so) {
	if(parent == 0)parent = (S32)space;

#ifdef SIM
	mGeomID = dCreateGeomTransform ((dSpaceID)parent);
	dGeomTransformSetCleanup (mGeomID, 1);
	F32 radius = sqrt(d.x * d.x + d.z * d.z) / 2.0;
	//F32 radius = d.x / 2;
	dGeomID mGeomSphere = dCreateSphere(0, TO_PHYSICS(radius));
	dGeomTransformSetGeom (mGeomID, mGeomSphere);
	dGeomSetPosition (mGeomSphere, 0, TO_PHYSICS(-d.y/2 + radius), 0);

	mGeomID2 = dCreateGeomTransform ((dSpaceID)parent);
	dGeomTransformSetCleanup (mGeomID2, 1);
	dGeomID mGeomBox = dCreateBox(0, TO_PHYSICS(d.x), TO_PHYSICS(d.y - radius), TO_PHYSICS(d.z));
	dGeomTransformSetGeom (mGeomID2, mGeomBox);
	dGeomSetPosition (mGeomBox, 0, TO_PHYSICS(radius / 2), 0);
#else
    F32 length = d.y - d.x * 2;
    mGeomID = dCreateCCylinder((dSpaceID)parent, TO_PHYSICS(d.x), TO_PHYSICS(length));
#endif

	mBodyID = dBodyCreate(world);
	dGeomSetBody(mGeomID, mBodyID);
#ifdef SIM
	dGeomSetBody(mGeomID2, mBodyID);
#endif
	dGeomSetData(mGeomID, static_cast<void *>(this));
#ifdef SIM
	dGeomSetData(mGeomID2, static_cast<void *>(this));
#endif
	mDimentions = d;
    mDimentions.y = length;
	setDensity(0.0001);
    dBodySetAutoDisableFlag(mBodyID, 0);

	mGroundBox.setBounds(CVector3(-d.x, -d.y / 2 - 5.0, -d.z), CVector3(d.x, -d.y / 2 + 20.0, d.z));
}

CRigidCapsule::~CRigidCapsule(){

}

void CRigidCapsule::setDimentions(const CVector3 &d){
	mDimentions = d;
	assert(true);
	dGeomBoxSetLengths(mGeomID, TO_PHYSICS(d.x), TO_PHYSICS(d.y), TO_PHYSICS(d.z));
}

CVector3 CRigidCapsule::getDimentions() {
	return mDimentions;
}

void CRigidCapsule::setDensity(F32 density){
	if(!mBodyID)
		return;
	dMass m;
	dMassSetCappedCylinder(&m, TO_WORLD(density), 3, TO_PHYSICS(mDimentions.x), TO_PHYSICS(mDimentions.y));
	dBodySetMass(mBodyID, &m);
}

void CRigidCapsule::setMass(F32 mass){
	if(!mBodyID)
		return;
	dMass m;
	dMassSetCappedCylinderTotal(&m, TO_WORLD(mass), 3, TO_PHYSICS(mDimentions.x), TO_PHYSICS(mDimentions.y));
	dBodySetMass(mBodyID, &m);
}

void CRigidCapsule::fitToVertices(CVector3 *v, S32 num){
	CAABoundBox bbox;
	bbox.setBounds(v[0], v[0]);
	for(S32 i = 1 ; i < num ; i++)
		bbox.addPoint(v[i]);

	setDimentions(bbox.getMax() - bbox.getMin());
	mScenePositionOffset = (bbox.getMax() + bbox.getMin()) / 2;
}

