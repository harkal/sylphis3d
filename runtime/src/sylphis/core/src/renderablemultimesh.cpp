/***************************************************************************
                          renderablemultimesh.cpp  -  description
                             -------------------
    begin                : Thu Jun 6 2002
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

#include <engine.h>
#include <scenemanager.h>
#include <renderablemultimesh.h>
#include <serializer.h>

IMPLEMENT_SERIALIZABLE(CRenderableMultiMesh);

CRenderableMultiMesh::CRenderableMultiMesh() : CSceneNode() {

}

CRenderableMultiMesh::CRenderableMultiMesh(CSceneManager *sm) : CSceneNode(sm) {
	mBound.setBounds(CVector3(-10, -10, -10), CVector3(10, 10, 10));
}

CRenderableMultiMesh::~CRenderableMultiMesh(){
    clearMeshes();
}

void CRenderableMultiMesh::setFrame(F32 frame){
    RMeshVector::iterator i;

	for (i = mMeshes.begin(); i != mMeshes.end(); i++)
		(*i)->setFrame(frame);
}

void CRenderableMultiMesh::addMesh(CRenderable *m){
    mMeshes.push_back(m);
	mShaders.push_back(m->getDefaultShader());
}

void CRenderableMultiMesh::addMesh(CRenderable *m, CMaterial *shader){
	mMeshes.push_back(m);
	mShaders.push_back(shader);
}

void CRenderableMultiMesh::addMesh(CRenderable *m, const std::string shader){
	addMesh(m, CEngine::getSingleton()->mMaterials.registerMaterial(shader));
}

CRenderableMultiMesh::RMeshVector &CRenderableMultiMesh::getMeshes(){
	return mMeshes;
}

void CRenderableMultiMesh::clearMeshes(){
    mMeshes.clear();
	mShaders.clear();
}

void CRenderableMultiMesh::render(CRenderer *renderer){
	CSceneNode::render(renderer);
    RMeshVector::iterator i;
	vector<CMaterialPtr>::iterator s;
    CRenderQueue *rq = getSceneManager()->getActiveRenderQueue();

	s = mShaders.begin();
	for (i = mMeshes.begin(); i != mMeshes.end(); i++, s++){
        CRenderableQueueMember *r = new CRenderableQueueMember();
		r->setRenderable(*i);
		r->setOrientation(getDerivedOrientation());
		r->setPosition(getDerivedPosition());
		r->setBounds(getDerivedBound());
        r->setNoShadows(getFlags() & NOSHADOW);
        r->setLastTransform(mLastOrientation, mLastPosition);
        rq->addRenderable(r, *s);
    }
}

void CRenderableMultiMesh::updateBound(){
	CSceneNode::updateBound();
}

void CRenderableMultiMesh::save(CSerializer *ser) const {
	CSceneNode::save(ser);

	ser->writeContainer(mMeshes);
	ser->writeContainer(mShaders);
}

#include <pointer.h>
void CRenderableMultiMesh::load(CSerializer *ser){
	CSceneNode::load(ser);

	ser->readContainerPtr(mMeshes);
	ser->readContainerPtr(mShaders);
}
