/***************************************************************************
                          scenemd3.h  -  description
                             -------------------
    begin                : Wed Dec 11 2002
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

#include <md3model.h>
#include <scenemd3.h>
#include <engine.h>
#include <scenemanager.h>
#include <serializer.h>

IMPLEMENT_SERIALIZABLE(CSceneMD3);

CSceneMD3::CSceneMD3(){

}

CSceneMD3::CSceneMD3(CSceneManager *sm) : CRenderableMultiMesh(sm) {
}

CSceneMD3::~CSceneMD3(){

}

void CSceneMD3::setModel(CModel *model){
	CModel::MeshVector &meshes = model->getRMeshes();
	CModel::MeshVector::iterator i;

	for(i = meshes.begin() ; i != meshes.end() ; i++){
        addMesh(*i);
		mBound.addPoint(CVector3(model->radius, model->radius, model->radius));
		mBound.addPoint(CVector3(-model->radius, -model->radius, -model->radius));
		//mBound.setMin(CVector3(-5,-5,-5));
		//mBound.setMax(CVector3(5, 5, 5));
	}
}






