/***************************************************************************
                          skelmodel.h  -  description
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

#ifndef _SKELMODEL_H_
#define _SKELMODEL_H_

#include <string>
#include <vector>
#include <resource.h>
#include <pointer.h>
#include <material.h>
#include <skeleton.h>
#include <mesh.h>
#include <model.h>
#include <skelanimation.h>

class CVirtualFS;

class CModelManager;

SmartPointer(CSkelModel);
class CSkelModel : public CModel {
public:
    CSkelModel();
    CSkelModel(CModelManager *m);
    virtual ~CSkelModel();

    void setSkeleton(CSkeleton *s);
    CSkeleton *getSkeleton();

    void addMesh(CMesh *m);
    std::vector<CMeshPtr> &getMeshes(){ return mMeshes; }
    void addSkelAnimation(CSkelAnimation *a, const string &alias);
    std::vector<CSkelAnimationPtr> &getSkelAnimations(){ return mSkelAnimations; }
	CSkelAnimation *getSkelAnimation(const string &name);
    void addShader(CMaterial *s);
    CMaterial *getShaderById(S32 i);

    virtual void load(const std::string &n, CVirtualFS *vfs);

protected:
	typedef std::map<std::string, CSkelAnimationPtr> SkelAnimationMap;
    
    CSkeletonPtr mSkeleton;
    std::vector<CMeshPtr> mMeshes;
    std::vector<CSkelAnimationPtr> mSkelAnimations;
	SkelAnimationMap mSkelAnimationsByName;
    std::vector<CMaterialPtr> mShaders;
    
    friend class CSceneModel;
};



#endif

