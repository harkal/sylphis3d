/***************************************************************************
                         skelmodel.cpp  -  description
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

#include <tokenizer.h>
#include <skelmodel.h>
#include <vfs.h>
#include <modelmanager.h>
#include <assert.h>
#include <engine.h>

CSkelModel::CSkelModel(){
}

CSkelModel::CSkelModel(CModelManager *m) : CModel(m) {
}

CSkelModel::~CSkelModel(){
}

void CSkelModel::setSkeleton(CSkeleton *s){
    mSkeleton = s;
}

CSkeleton *CSkelModel::getSkeleton(){
    return mSkeleton;
}

void CSkelModel::addMesh(CMesh *m){
    mMeshes.push_back(m);
}

void CSkelModel::addSkelAnimation(CSkelAnimation *a, const string &alias){
    mSkelAnimations.push_back(a);
	//if(alias == "-"){
	mSkelAnimationsByName.insert(SkelAnimationMap::value_type(alias.c_str(), a));
	//}
}

CSkelAnimation *CSkelModel::getSkelAnimation(const string &name){
	return mSkelAnimationsByName[name];
}

void CSkelModel::addShader(CMaterial *s){
    mShaders.push_back(s);
}

CMaterial *CSkelModel::getShaderById(S32 i){
	assert(i >= 0);
    return mShaders[i];
}

void CSkelModel::load(const std::string &n, CVirtualFS *vfs){
    setName(n);

    CFileText f(n);
    vfs->readFile(f);

	string buffer = (char*)f.getData();
    
    CTokenizerNoComments tokenizer(buffer);

    string path;
    bool skeletonLoaded = false;
    while(tokenizer.nextToken()){
        const string key = tokenizer.lowerCase(tokenizer.getToken());
        tokenizer.nextToken();
        const string value = tokenizer.getToken();
        if(key == "path"){
            path = value;
            if(path[path.size()-1] != '/')
                path += '/';
        } else if(key == "skeleton"){
            setSkeleton(mManager->registerSkeleton(path + value));
            skeletonLoaded = true;
        } else if(key == "mesh"){
            if(!skeletonLoaded)
                throw CException("Failed because no skeleton active");
            addMesh(mManager->registerMesh(path + value));
        } else if(key == "animation"){
            if(!skeletonLoaded)
                throw CException("Failed because no skeleton active");
			tokenizer.nextToken();
			string alias = tokenizer.getToken();
			if (alias == "-"){
				alias = value;
			}
            addSkelAnimation(mManager->registerSkelAnimation(path + value), alias);
        } else if(key == "material"){
            mMeshes.back()->addShader(CEngine::getSingleton()->mMaterials.registerMaterial(path + value));      
			//addShader(mManager->registerShader(path + value));
        }
    }

    CResource::load(); // Mark us loaded
}

