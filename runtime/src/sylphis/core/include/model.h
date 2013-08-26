/***************************************************************************
                           model.h  -  description
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

#ifndef _MODEL_H_
#define _MODEL_H_

#include <string>
#include <vector>
#include <resource.h>
#include <pointer.h>
#include <renderable.h>

class CVirtualFS;
class CModelManager;

SmartPointer(CModel);
class CModel : public CResource {
public:
    CModel();
    CModel(CModelManager *m);
    virtual ~CModel();

	typedef std::vector<CRenderableMeshPtr> MeshVector;
	virtual MeshVector& getRMeshes();

    virtual void load(const std::string &n, CVirtualFS *vfs);

	F32 radius; //FIXME HACK!!!!!
protected:
	CModelManager *mManager;
	MeshVector mRMeshes;
};


#endif
