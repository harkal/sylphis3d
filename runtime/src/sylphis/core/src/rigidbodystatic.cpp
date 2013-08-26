/***************************************************************************
                     rigidbodystatic.h  -  description
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
 
#include <vector>

#include <rigidbodystatic.h>
#include <model.h>

extern dSpaceID space;
extern dWorldID world;

CRigidBodyStatic::CRigidBodyStatic(S32 parent, CSceneObject *so) : CRigidBody(parent, so) {

}

CRigidBodyStatic::~CRigidBodyStatic(){
}

void CRigidBodyStatic::setMeshData(CVector3 *verts, U32 vertnum, S32 *elems, U32 elemsnum){
	dTriMeshDataID Data = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSimple(Data, &verts[0].x, vertnum, elems, elemsnum);
	mGeomID = dCreateTriMesh(space, Data, 0, 0, 0);
	//mBodyID = dBodyCreate(world);
	//dGeomSetBody(mGeomID, mBodyID);
	dGeomSetData(mGeomID, static_cast<void *>(this));
}

void CRigidBodyStatic::setMeshData(CModel *m){
	CModel::MeshVector &meshes = m->getRMeshes();

	// Convert the vertices from CVector4 to CVector3
	U32 numVerts = meshes[0]->getNumOfVertices();
	U32 numElems = meshes[0]->getNumOfElements() * 3;
	dVector3 *verts = new dVector3[numVerts]; //FIXME: delete this!!!!!
	CMatrix4 mat;
	getOrientation().toRotationMatrix(mat);
	mat.setTransform(getPosition());
	for(U32 i = 0 ; i < numVerts ; ++i){
		CVector4 vert = meshes[0]->getVertices()[i];
		vert = mat * vert;
		verts[i][0] = TO_PHYSICS(vert.x);
		verts[i][1] = TO_PHYSICS(vert.y);
		verts[i][2] = TO_PHYSICS(vert.z);
	}
	S32 *elems = new S32[numElems];//FIXME: delete this
	for(U32 i = 0 ; i < numElems ; ++i){
		elems[i] = meshes[0]->getElements()[i];
		assert(elems[i] < numVerts);
	}
	setMeshData((CVector3*)verts, numVerts, elems, numElems);
}

void CRigidBodyStatic::setPosition(const CVector3 &o){
	mPosition = o;
}

void CRigidBodyStatic::setPosition(const F32 x, const F32 y, const F32 z){
	mPosition.x = x;
	mPosition.y = y;
	mPosition.z = z;
}

CVector3 CRigidBodyStatic::getPosition(){
    return mPosition;
}

void CRigidBodyStatic::setOrientation(const CQuaternion &o){
	mOrientation = o;
}

void CRigidBodyStatic::setOrientation(const F32 x, const F32 y, const F32 z, const F32 w){
	mOrientation.x = x;
	mOrientation.y = y;
	mOrientation.z = z;
	mOrientation.w = w;
}

CQuaternion CRigidBodyStatic::getOrientation(){
    return mOrientation;
}

void CRigidBodyStatic::setVelocity(const CVector3 &v){
}

void CRigidBodyStatic::setAngVelocity(const CVector3 &v){
}

CVector3 CRigidBodyStatic::getVelocity(){
    return CVector3::ZERO;
}

CVector3 CRigidBodyStatic::getAngVelocity(){
    return CVector3::ZERO;
}

void CRigidBodyStatic::addForce(const CVector3 &f){
}

void CRigidBodyStatic::addTorque(const CVector3 &f){
}

void CRigidBodyStatic::addLocalForce(const CVector3 &f){
}

void CRigidBodyStatic::addLocalTorque(const CVector3 &f){
}

void CRigidBodyStatic::addForceAt(const CVector3 &f, const CVector3 &pos){
}

void CRigidBodyStatic::addForceAtLocal(const CVector3 &f, const CVector3 &pos){
}

void CRigidBodyStatic::addLocalForceAt(const CVector3 &f, const CVector3 &pos){
}

void CRigidBodyStatic::addLocalForceAtLocal(const CVector3 &f, const CVector3 &pos){
}

void CRigidBodyStatic::makeImmovable(){
}

void CRigidBodyStatic::setSleep(bool s){
}

void CRigidBodyStatic::applyGravity(bool g){
}

void CRigidBodyStatic::update(){
}

