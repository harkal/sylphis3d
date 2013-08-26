/***************************************************************************
                          renderablemultimesh.h  -  description
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

#ifndef RENDERABLEMULTIMESH_H
#define RENDERABLEMULTIMESH_H

#include "sceneobject.h"
#include "renderable.h"
#include <vector>

class CSceneManager;
class CRenderer;

SmartPointer(CRenderableMultiMesh);
class CRenderableMultiMesh : public CSceneNode {
DEFINE_SERIALIZABLE(CRenderableMultiMesh);
public: 
	CRenderableMultiMesh();
	CRenderableMultiMesh(CSceneManager *sm);
	virtual ~CRenderableMultiMesh();

	typedef std::vector<CRenderablePtr> RMeshVector;
    virtual void setFrame(F32 frame);
    virtual void addMesh(CRenderable *m);
	virtual void addMesh(CRenderable *m, CMaterial *shader);
	virtual void addMesh(CRenderable *m, const std::string shader);
	virtual RMeshVector &getMeshes();
    virtual void clearMeshes();

    virtual void render(CRenderer *renderer);
	virtual void updateBound();

    virtual void save(CSerializer *ser) const;
    virtual void load(CSerializer *ser);
private:
    RMeshVector mMeshes;
	std::vector<CMaterialPtr> mShaders;
};

#endif
