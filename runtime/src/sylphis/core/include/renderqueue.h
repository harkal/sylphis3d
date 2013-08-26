/***************************************************************************
                          renderqueue.h  -  description
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

#ifndef RENDERQUEUE_H
#define RENDERQUEUE_H

#include "globaldefs.h"
#include <vector>
#include <map>

#include <material.h>
#include "renderablequeuemember.h"

class CRenderQueue {
public:
	CRenderQueue();
	~CRenderQueue();

    void addRenderable(CRenderableQueueMember *r, CMaterial *s);
    void clear();
protected:
    class queueLess {
    public:
        bool operator()(const CMaterial* a, const CMaterial* b) const;
    };
    typedef std::map<CMaterialPtr, std::vector<CRenderableQueueMemberPtr>, queueLess > RenderableQueueMap;
    RenderableQueueMap mQueue;

    friend class CRenderer;
};

#endif
