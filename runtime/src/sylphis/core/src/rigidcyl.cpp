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
#include <rigidcyl.h>

extern dSpaceID space;
extern dWorldID world;

CRigidCylinder::CRigidCylinder(S32 parent, CSceneObject *so, F32 radius, F32 length ) : CRigidBody(parent, so) {
	if(parent == 0)parent = (S32)space;
	mGeomID = dCreateCCylinder((dSpaceID)parent, TO_PHYSICS(radius), TO_PHYSICS(length));
	mBodyID = dBodyCreate(world);
	dGeomSetBody(mGeomID, mBodyID);
	dGeomSetData(mGeomID, static_cast<void *>(this));
	mRadius = radius;
	mLength = length;
	setDensity(0.0001);
}

CRigidCylinder::~CRigidCylinder(){

}

void CRigidCylinder::setDimentions(F32 radius, F32 length){
	mRadius = radius;
	mLength = length;
	dGeomCCylinderSetParams(mGeomID, TO_PHYSICS(radius), TO_PHYSICS(length));
}

F32 CRigidCylinder::getRadius() {
	return mRadius;
}

F32 CRigidCylinder::getLength() {
	return mLength;
}

void CRigidCylinder::setDensity(F32 density){
	dMass m;
	dMassSetCappedCylinder(&m, TO_WORLD(density), 3, TO_PHYSICS(mRadius), TO_PHYSICS(mLength));
	dBodySetMass(mBodyID, &m);
}

void CRigidCylinder::setMass(F32 mass){
	dMass m;
	dMassSetCappedCylinderTotal(&m, mass, 3, TO_PHYSICS(mRadius), TO_PHYSICS(mLength));
	dBodySetMass(mBodyID, &m);
}

void CRigidCylinder::fitToVertices(CVector3 *v, S32 num){
	CAABoundBox bbox;
	bbox.setBounds(v[0], v[0]);
	for(S32 i = 1 ; i < num ; i++)
		bbox.addPoint(v[i]);

	setDimentions(bbox.getMax().x - bbox.getMin().x, bbox.getMax().y - bbox.getMin().y);
	mScenePositionOffset = (bbox.getMax() + bbox.getMin()) / 2;
}

