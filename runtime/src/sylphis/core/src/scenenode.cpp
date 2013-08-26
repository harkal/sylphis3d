/***************************************************************************
                          scenenode.cpp  -  description
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

#include <scenemanager.h>
#include <scenenode.h>
#include <serializer.h>

IMPLEMENT_SERIALIZABLE(CSceneNode);

CSceneNode::CSceneNode() : CSceneObject() {
}
CSceneNode::CSceneNode(CSceneManager *sm) : CSceneObject(sm) {
}
CSceneNode::~CSceneNode(){
	//gcon.printf("Free CSceneNode %p\n", this);
    list<CSceneObject*>::iterator i;
	for (i = mChildren.begin(); i != mChildren.end(); i++)
		(*i)->setParent(0);
	mChildren.clear();
}

void CSceneNode::addChild(CSceneObject *so){
	mChildren.push_front(so);
	so->setParent(this);
	so->updateData(0.0);
}

void CSceneNode::removeChild(CSceneObject *so){
	mChildren.remove(so);
}


void CSceneNode::updateData(F64 time){
	CSceneObject::updateData(time);

	list<CSceneObject*>::iterator i, end;
	end = mChildren.end();
	S32 j = 0;
	for (i = mChildren.begin(); i != end; ++i, ++j){
		assert(*i != 0);
		(*i)->update(time, false);
	}
}

void CSceneNode::updateBound(){
	mDerivedBound = mBound;
    mDerivedBound.translate(getDerivedPosition());

    list<CSceneObject*>::iterator i;

	if(mChildren.size() == 0){
		mSceneManager->spatialize(this);
        return;
	}

    i = mChildren.begin();
    mDerivedBound.addBound((*i)->getDerivedBound());
	//mDerivedPosition = (*i)->getDerivedPosition();
	//mDerivedBound.addPoint(mDerivedPosition);//FIXME
    i++;
	for (; i != mChildren.end(); i++){
        mDerivedBound.addBound((*i)->getDerivedBound());
	}
	
	mSceneManager->spatialize(this);
}

void CSceneNode::render(CRenderer *renderer){
    list<CSceneObject*>::iterator i;

	for (i = mChildren.begin(); i != mChildren.end(); i++)
		(*i)->onRender(renderer);
}

void CSceneNode::postRenderUpdate(CRenderer *renderer){
	if((!(getFlags() & NOCULL)) && mSceneManager->isCulled(this))
		return;

    if(getFlags() & NORENDER)
        return;

	mLastOrientation = getDerivedOrientation();
    mLastPosition = getDerivedPosition();

	list<CSceneObject*>::iterator i;
	for (i = mChildren.begin(); i != mChildren.end(); i++)
		(*i)->postRenderUpdate(renderer);
}

void CSceneNode::save(CSerializer *ser) const {
	CSceneObject::save(ser);

	ser->writeContainer(mChildren);
}

void CSceneNode::load(CSerializer *ser){
	CSceneObject::load(ser);

	ser->readContainer(mChildren);
}

