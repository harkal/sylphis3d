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
#include <rigidbox.h>
#include <console.h>

extern dSpaceID space;
extern dWorldID world;

CRigidBox::CRigidBox(S32 parent, CSceneObject *so, const CVector3 &d ) : CRigidBody(parent, so) {
	if(parent == 0) parent = (S32)space;
	mGeomID = dCreateBox((dSpaceID)parent, TO_PHYSICS(d.x), TO_PHYSICS(d.y), TO_PHYSICS(d.z));
	mBodyID = dBodyCreate(world);
	dGeomSetBody(mGeomID, mBodyID);
	dGeomSetData(mGeomID, static_cast<void *>(this));
	mDimentions = d;
	setDensity(0.0001);
}

CRigidBox::~CRigidBox(){

}

void CRigidBox::setDimentions(const CVector3 &d){
	mDimentions = d;
	dGeomBoxSetLengths(mGeomID, TO_PHYSICS(d.x), TO_PHYSICS(d.y), TO_PHYSICS(d.z));
}

CVector3 CRigidBox::getDimentions() {
	return mDimentions;
}

void CRigidBox::setDensity(F32 density){
	if(!mBodyID)
		return;
	dMass m;
	dMassSetBox(&m, TO_WORLD(density), TO_PHYSICS(mDimentions.x), TO_PHYSICS(mDimentions.y), TO_PHYSICS(mDimentions.z));
	dBodySetMass(mBodyID, &m);
}

void CRigidBox::setMass(F32 mass){
	if(!mBodyID)
		return;
	dMass m;
	dMassSetBoxTotal(&m, mass, TO_PHYSICS(mDimentions.x), TO_PHYSICS(mDimentions.y), TO_PHYSICS(mDimentions.z));
	dBodySetMass(mBodyID, &m);
}

void CRigidBox::fitToVertices(CVector3 *v, S32 num){
    if(num <= 0){
        gcon.printError("WARNING : Trying to fit non-mesh object\n");
        return;
    }
	CAABoundBox bbox;
	bbox.setBounds(v[0], v[0]);
	for(S32 i = 1 ; i < num ; i++)
		bbox.addPoint(v[i]);

	setDimentions(bbox.getMax() - bbox.getMin());
	mScenePositionOffset = (bbox.getMax() + bbox.getMin()) / 2;
}

#include <debugdrawing.h>
void CRigidBox::draw() const {
    const dReal* v = dGeomGetPosition(mGeomID);
	CVector3 o(TO_WORLD(v[0]), TO_WORLD(v[1]), TO_WORLD(v[2]));
    const dReal* r = dGeomGetRotation(mGeomID);
    F32 sides[3];
    dGeomBoxGetLengths(mGeomID, sides);
    sides[0] = TO_WORLD(sides[0]);
    sides[1] = TO_WORLD(sides[1]);
    sides[2] = TO_WORLD(sides[2]);
    drawBox(&o.x, r, sides);
}

