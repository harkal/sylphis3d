/***************************************************************************
                        modelmanager.cpp  -  description
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

#include <engine.h>
#include <exception.h>
#include <model.h>
#include <skelmodel.h>
#include <md3model.h>
#include <smfmodel.h>
#include <skeleton.h>
#include <mesh.h>
#include <skelanimation.h>
#include <modelmanager.h>

CModelManager::CModelManager(void){

}

CModelManager::~CModelManager(void){

}

CModel *CModelManager::registerModel(CModel *model){
    resources.push_back(model);
    return model;
}

CMD3Model *CModelManager::registerMD3Model(const std::string fname){
    CModel *found = findModel(fname);
    if(found)return static_cast<CMD3Model*>(found);

    gcon.printString("Loading MD3 model " + fname + "\n");

	CMD3ModelPtr model = new CMD3Model(this);
    
    model->load(fname, CEngine::getSingleton()->vfs);

    registerModel(model);

    return model;
}

CSMFModel *CModelManager::registerSMFModel(const std::string fname){
    CModel *found = findModel(fname);
    if(found)return static_cast<CSMFModel*>(found);

    gcon.printString("Loading SMF model " + fname + "\n");

	CSMFModelPtr model = new CSMFModel(this);
    
    model->load(fname, CEngine::getSingleton()->vfs);

    registerModel(model);

    return model;
}

CSkelModel *CModelManager::registerSkelModel(const std::string fname){
    CModel *found = findModel(fname);
    if(found)return static_cast<CSkelModel *>(found);

    gcon.printString("Loading skeletal model " + fname + "\n");

    CSkelModelPtr model = new CSkelModel(this);
    
    try {
        model->load(fname, CEngine::getSingleton()->vfs);
	} catch (CException e) {
        CEngine::getSingleton()->con.printString("Failed to load : " + fname + "\n  ");
        CEngine::getSingleton()->con.printString(e.getExplanation() + "\n");
		return 0;
	}

    registerModel(model);

    return model;
}

CModel *CModelManager::findModel(const std::string name){
    return static_cast<CModel*>(find(name));
}



CSkeleton *CModelManager::registerSkeleton(CSkeleton *skeleton){
    resources.push_back(skeleton);
    return skeleton;
}

CSkeleton *CModelManager::registerSkeleton(const std::string fname){
    CSkeleton *found = findSkeleton(fname);
    if(found)return found;

    gcon.printString("Loading skeleton " + fname + "\n");

    CSkeletonPtr skeleton = new CSkeleton();
    
    try {
        skeleton->load(fname, CEngine::getSingleton()->vfs);
	} catch (CException e) {
        gcon.printString("Failed to load : " + fname + "\n  ");
        gcon.printString(e.getExplanation() + "\n");
		return 0;
	}

    registerSkeleton(skeleton);

    return skeleton;
}

CSkeleton *CModelManager::findSkeleton(const std::string name){
    return static_cast<CSkeleton*>(find(name));
}

CMesh *CModelManager::registerMesh(CMesh *mesh){
    resources.push_back(mesh);
    return mesh;
}

CMesh *CModelManager::registerMesh(const std::string fname){
    CMesh *found = findMesh(fname);
    if(found)return found;

    gcon.printString("Loading mesh " + fname + "\n");

    CMeshPtr mesh = new CMesh();
    
    try {
        mesh->load(fname, CEngine::getSingleton()->vfs);
	} catch (CException e) {
        gcon.printString("Failed to load : " + fname + "\n  ");
        gcon.printString(e.getExplanation() + "\n");
		return 0;
	}

    registerMesh(mesh);

    return mesh;
}

CMesh *CModelManager::findMesh(const std::string name){
    return static_cast<CMesh*>(find(name));
}

CSkelAnimation *CModelManager::registerSkelAnimation(CSkelAnimation *skelAnimation){
    resources.push_back(skelAnimation);
    return skelAnimation;
}

CSkelAnimation *CModelManager::registerSkelAnimation(const std::string fname){
    CSkelAnimation *found = findSkelAnimation(fname);
    if(found)return found;

    gcon.printString("Loading skelAnimation " + fname + "\n");

    CSkelAnimationPtr skelAnimation = new CSkelAnimation();
    
    try {
        skelAnimation->load(fname, CEngine::getSingleton()->vfs);
	} catch (CException e) {
        gcon.printString("Failed to load : " + fname + "\n  ");
        gcon.printString(e.getExplanation() + "\n");
		return 0;
	}

    registerSkelAnimation(skelAnimation);

    return skelAnimation;
}

CSkelAnimation *CModelManager::findSkelAnimation(const std::string name){
    return static_cast<CSkelAnimation*>(find(name));
}

