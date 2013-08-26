/***************************************************************************
                          resourcemanager.h  -  description
                             -------------------
    begin                : Wed May 29 2002
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

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <list>
using namespace std;

#include "resource.h"

class CResourceManager {
public: 
    typedef list<CResourcePtr> ResourceList;

	CResourceManager();
	virtual ~CResourceManager();

	CResource*				find(const string &name);

	virtual void			loadAll();
	virtual void			freeAll();
    virtual void			freeUnused();
	virtual ResourceList*	getResources();
protected:
	ResourceList resources;
	bool dirty;
};

#endif

