/***************************************************************************
                        modelmanager.cpp  -  description
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

