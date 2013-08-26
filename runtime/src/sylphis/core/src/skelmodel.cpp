/***************************************************************************
                         skelmodel.cpp  -  description
                             -------------------
    begin                : Wed Dec 11 2002
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

