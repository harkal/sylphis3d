/***************************************************************************
                     rigidbodystatic.h  -  description
                             -------------------
    begin                : Sat Nov 9 2002
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

