/***************************************************************************
                          meshloader.h  -  description
                             -------------------
    begin                : Wed Mar 27 2002
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

#ifndef MESHLOADER3DS_H
#define MESHLOADER3DS_H

#include <utility>
using namespace std;

#include <vfs.h>
#include <renderable.h>
#include <tokenizer.h>

class CMeshLoader3DS {
public: 
	CMeshLoader3DS(CVirtualFS &v);
	~CMeshLoader3DS();

	void load(CRenderableMesh &mesh, const string &name);
protected:
	void expect(CTokenizer &t, const string &token);
	pair<string, F32> getValuePair(CTokenizer &t);
	CVirtualFS *vfs;
};

#endif
