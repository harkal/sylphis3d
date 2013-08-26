/***************************************************************************
                         scenemodel.h  -  description
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

#ifndef _SCENEMODEL_H_
#define _SCENEMODEL_H_

#include <string>
#include <vector>
#include <map>
#include <scenenode.h>
#include <renderable.h>

#include <skelmodel.h>
#include <mesh.h>
#include <skeletoninstance.h>
#include <skelanimation.h>
#include <animationblender.h>

#include <Python.h>

SmartPointer(CSceneModel);
class CSceneModel : public CSceneNode {
public:
                        CSceneModel(CSceneManager *sm, CSkelModel *m);
    virtual             ~CSceneModel();

    void                makeVerticesNormals(CSubMesh *sm, CVector4 *v, CVector3 *n, CVector3 *tang, CPlane *p, S32 stride);

	bool                hasAnim(const std::string &name);
	F32               getAnimWeight(const std::string &name);
	void                blendAnimCycle(const std::string &name, F32 weight, F32 delay);
	void                weightAnimCycle(const std::string &name, F32 weight, F32 delay);
	void                clearAnimCycle(const std::string &name, F32 delay);
	void                clearAllAnimCycle(F32 delay);

	void                executeAction(const std::string &name, F32 delayIn = 0.0, F32 delayOut = 0.0);
    void                executeAction(CSkelAnimation *anim, const std::string &name, F32 delayIn = 0.0, F32 delayOut = 0.0);
	void                waitAction();

	void                addChildAtBone(const std::string &name, CSceneObject *so);
	CSceneObject*       getChildAtBone(const std::string &name);

    virtual void        updateData(F64 time);
    virtual void        updateBound();
    virtual void        render(CRenderer *renderer);
protected:
	typedef std::map<CSceneObject *, CBoneInstancePtr> ChildToBoneMap;
	typedef std::map<std::string, CSceneObjectPtr> BoneToChildMap;
	BoneToChildMap      mBoneToChildMap;
	ChildToBoneMap      mChildToBoneMap;
    CRenderableMesh*    getAvailableRenderable();
    void                renderSubmesh(CMesh *mesh, CSubMesh *smesh);
    CSkelModelPtr       mModel;

    CSkeletonInstancePtr mSkeleton;
    CAnimationBlender    blender;
    list<CMeshPtr>       mMeshes;
    
    S32                  mRendsIndex;

    std::vector<CRenderableMeshPtr> mRends; // RenderableMesh pool

    void                 debugDrawSkeleton(CRenderer *r);
};

#endif
