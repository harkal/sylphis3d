/***************************************************************************
                          renderablemultimesh.cpp  -  description
                             -------------------
    begin                : Thu Jun 6 2002
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

#include <engine.h>
#include <scenemanager.h>
#include <renderablemultimesh.h>
#include <serializer.h>

IMPLEMENT_SERIALIZABLE(CRenderableMultiMesh);

CRenderableMultiMesh::CRenderableMultiMesh() : CSceneNode() {

}

CRenderableMultiMesh::CRenderableMultiMesh(CSceneManager *sm) : CSceneNode(sm) {
	mBound.setBounds(CVector3(-10, -10, -10), CVector3(10, 10, 10));
}

CRenderableMultiMesh::~CRenderableMultiMesh(){
    clearMeshes();
}

void CRenderableMultiMesh::setFrame(F32 frame){
    RMeshVector::iterator i;

	for (i = mMeshes.begin(); i != mMeshes.end(); i++)
		(*i)->setFrame(frame);
}

void CRenderableMultiMesh::addMesh(CRenderable *m){
    mMeshes.push_back(m);
	mShaders.push_back(m->getDefaultShader());
}

void CRenderableMultiMesh::addMesh(CRenderable *m, CMaterial *shader){
	mMeshes.push_back(m);
	mShaders.push_back(shader);
}

void CRenderableMultiMesh::addMesh(CRenderable *m, const std::string shader){
	addMesh(m, CEngine::getSingleton()->mMaterials.registerMaterial(shader));
}

CRenderableMultiMesh::RMeshVector &CRenderableMultiMesh::getMeshes(){
	return mMeshes;
}

void CRenderableMultiMesh::clearMeshes(){
    mMeshes.clear();
	mShaders.clear();
}

void CRenderableMultiMesh::render(CRenderer *renderer){
	CSceneNode::render(renderer);
    RMeshVector::iterator i;
	vector<CMaterialPtr>::iterator s;
    CRenderQueue *rq = getSceneManager()->getActiveRenderQueue();

	s = mShaders.begin();
	for (i = mMeshes.begin(); i != mMeshes.end(); i++, s++){
        CRenderableQueueMember *r = new CRenderableQueueMember();
		r->setRenderable(*i);
		r->setOrientation(getDerivedOrientation());
		r->setPosition(getDerivedPosition());
		r->setBounds(getDerivedBound());
        r->setNoShadows(getFlags() & NOSHADOW);
        r->setLastTransform(mLastOrientation, mLastPosition);
        rq->addRenderable(r, *s);
    }
}

void CRenderableMultiMesh::updateBound(){
	CSceneNode::updateBound();
}

void CRenderableMultiMesh::save(CSerializer *ser) const {
	CSceneNode::save(ser);

	ser->writeContainer(mMeshes);
	ser->writeContainer(mShaders);
}

#include <pointer.h>
void CRenderableMultiMesh::load(CSerializer *ser){
	CSceneNode::load(ser);

	ser->readContainerPtr(mMeshes);
	ser->readContainerPtr(mShaders);
}
