/***************************************************************************
                          renderqueue.cpp  -  description
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

#include "renderqueue.h"

CRenderQueue::CRenderQueue(){
}
CRenderQueue::~CRenderQueue(){
}

void CRenderQueue::addRenderable(CRenderableQueueMember *r, CMaterial* s){
    std::pair<RenderableQueueMap::iterator, bool> res;

    std::vector<CRenderableQueueMemberPtr> n;
    res = mQueue.insert(RenderableQueueMap::value_type(s, n));
    res.first->second.push_back(r);
}

void CRenderQueue::clear(){
    mQueue.clear();
}

bool CRenderQueue::queueLess::operator()(const CMaterial* a, const CMaterial* b) const {
    // Sort by pointer
    return a < b;
}
