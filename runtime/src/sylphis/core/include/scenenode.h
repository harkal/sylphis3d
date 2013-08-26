/***************************************************************************
                          scenenode.h  -  description
                             -------------------
    begin                : Mon Jun 3 2002
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

#ifndef SCENENODE_H
#define SCENENODE_H

#include <list>
using namespace std;
#include "sceneobject.h"

SmartPointer(CSceneNode);
class CSceneNode : public CSceneObject  {
DEFINE_SERIALIZABLE(CSceneNode);
public: 
	CSceneNode();
	CSceneNode(CSceneManager *sm);
	virtual ~CSceneNode();

	void addChild(CSceneObject *so);
	void removeChild(CSceneObject *so);

	virtual void updateData(F64 time);
    virtual void updateBound();

    virtual void render(CRenderer *renderer);

	virtual void postRenderUpdate(CRenderer *renderer);

    virtual void save(CSerializer *ser) const;
    virtual void load(CSerializer *ser);

protected:
	list<CSceneObject*> mChildren;
};

#endif
