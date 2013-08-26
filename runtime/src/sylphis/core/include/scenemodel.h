/***************************************************************************
                         scenemodel.h  -  description
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
