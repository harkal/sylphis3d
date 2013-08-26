/***************************************************************************
                          scenenode.cpp  -  description
                             -------------------
    begin                : Mon Jun 3 2002
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

