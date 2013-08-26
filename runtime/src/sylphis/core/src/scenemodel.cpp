/***************************************************************************
                         scenemodel.cpp  -  description
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

#include <list>
#include <string>
#include <exception.h>
#include <scenemanager.h>
#include <scenemodel.h>
#include <renderable.h>
#include <renderqueue.h>

#include <configfile.h>

CSceneModel::CSceneModel(CSceneManager *sm, CSkelModel *m) : CSceneNode(sm){
    if(!m)
        throw CException("ERROR: Null CSkelModel passed to CSceneModel");
    mModel = m;

    mSkeleton = new CSkeletonInstance(m->getSkeleton());

    vector<CMeshPtr>::iterator i;
    vector<CMeshPtr> &meshes = m->getMeshes();
    for(i = meshes.begin() ; i != meshes.end() ; i++){
        mMeshes.push_front(*i);
    }

	getBound().setBounds(CVector3(-10, -10, -10), CVector3(10, 10, 10));
}

CSceneModel::~CSceneModel(){
}

#include <engine.h>

void CSceneModel::addChildAtBone(const std::string &name, CSceneObject *so){

	CBoneInstance *b = mSkeleton->getBoneByName(name);
	
	mChildToBoneMap.insert(ChildToBoneMap::value_type(so, b));
	mBoneToChildMap.insert(BoneToChildMap::value_type(name, so));

	addChild(so);
}

CSceneObject *CSceneModel::getChildAtBone(const std::string &name){
	return mBoneToChildMap[name];
}

bool CSceneModel::hasAnim(const std::string &name){
	return mModel->getSkelAnimation(name) != 0;
}

F32 CSceneModel::getAnimWeight(const std::string &name){
	return blender.getAnimWeight(name);
}

void CSceneModel::blendAnimCycle(const std::string &name, F32 weight, F32 delay){
	blender.blendCycle(mModel->getSkelAnimation(name), name, weight, delay);
}

void CSceneModel::weightAnimCycle(const std::string &name, F32 weight, F32 delay){
	blender.weightCycle(name, weight, delay);
}

void CSceneModel::clearAnimCycle(const std::string &name, F32 delay){
	blender.clearCycle(name, delay);
}

void CSceneModel::clearAllAnimCycle(F32 delay){
	blender.clearAllCycle(delay);
}

void CSceneModel::executeAction(const std::string &name, F32 delayIn, F32 delayOut){
	blender.executeAction(mModel->getSkelAnimation(name), name, delayIn, delayOut);
}

void CSceneModel::executeAction(CSkelAnimation *anim, const std::string &name, F32 delayIn, F32 delayOut){
    blender.executeAction(anim, name, delayIn, delayOut);
}

void CSceneModel::waitAction(){
	blender.wait();
}

void CSceneModel::updateData(F64 time){
    CSceneObject::updateData(time);

    blender.update(time);
    blender.updateSkeleton(mSkeleton);

	mBound.addBound(mSkeleton->getBound());
	
	list<CSceneObject*>::iterator i;
	
	CVector3 pos = mDerivedPosition;
	CQuaternion rot = mDerivedOrientation;
	for (i = mChildren.begin(); i != mChildren.end(); i++){
		CBoneInstance *b = mChildToBoneMap[*i];
		if(b){
			mDerivedOrientation = rot * b->getDerivedRotation();
			mDerivedOrientation.invert();

			mDerivedPosition = pos + (rot * b->getDerivedTranslation());
		} else {
			mDerivedPosition = pos;
			mDerivedOrientation = rot;
		}
		(*i)->update(time, false);
	}
	mDerivedPosition = pos;
	mDerivedOrientation = rot;


}

void CSceneModel::updateBound(){
	CSceneNode::updateBound();
	return;	
	list<CSceneObject*>::iterator i;

	for(i = mChildren.begin(); i != mChildren.end(); ++i){
        mDerivedBound.addBound((*i)->getDerivedBound());
	}
}

void CSceneModel::render(CRenderer *renderer){
    CSceneNode::render(renderer);
    //return; 
    mRendsIndex = 0;
	getBound().setBounds(CVector3(10,10,10), CVector3(-10, -10, -10));    
    for(list<CMeshPtr>::iterator mesh = mMeshes.begin() ; mesh != mMeshes.end() ; mesh++){
        vector<CSubMeshPtr> &smeshes = (*mesh)->getSubMeshes();
        S32 i = 0;
        for(vector<CSubMeshPtr>::iterator smesh = smeshes.begin() ; smesh != smeshes.end() ; smesh++, i++){
            CSubMesh *sm = *smesh;

            renderSubmesh(*mesh, sm);
        }
    }
    //debugDrawSkeleton(renderer);
}

CRenderableMesh *CSceneModel::getAvailableRenderable(){
    if(mRendsIndex < mRends.size()){
        return mRends[mRendsIndex++];
    }
    
    CRenderableMesh *r = new CRenderableMesh();
    bool system = !gCfgFile.getAsInt("r_useSkinVBO");

    r->mVertexBuffer->allocate(2048 * (sizeof(CVector4) + sizeof(CVector3) * 2 + sizeof(CTexCoord)) , CVertexBuffer::DYNAMIC, system);
        
    r->vertices.setupBuffer(r->mVertexBuffer, CVertexBuffer::POSITION);
    r->normals.setupBuffer(r->mVertexBuffer, CVertexBuffer::NORMAL);
    r->tex_st.setupBuffer(r->mVertexBuffer, CVertexBuffer::TEX_COORD);
    r->tangent.setupBuffer(r->mVertexBuffer, CVertexBuffer::TANGENT);
    
    r->mElems->allocate(1024 * 10 * sizeof(U32), CIndexBuffer::DYNAMIC, true);

    r->elemPlanes = new CPlane[2048];
    r->neighbors = new S32[1024 * 10];
    r->elemsVis = new bool[2048];
	//r->mNoShadows = getFlags() & NOSHADOW;
    r->mSystemVertices.resize(2048);
    mRends.push_back(r);
    mRendsIndex++;

    return r;
}

#include <engine.h>

CVector3 normals[2000];
CVector3 tangent[2000];
const S32 striplen = sizeof(CVector4) + sizeof(CVector3) * 2 + sizeof(CTexCoord);
F32 buffer[ 2000 * striplen ];

template<class T>
void interleave(F32 *buffer, T *data, S32 size, S32 stride, S32 len){
    S32 pos = stride;
    for(S32 i = 0 ; i < len ; ++i){
        *(T *)((char *)buffer + pos) = data[i];
        pos += size;
    }
}

void CSceneModel::renderSubmesh(CMesh *mesh, CSubMesh *smesh){
    CRenderQueue *rq = getSceneManager()->getActiveRenderQueue();

    CRenderableMesh *rm = getAvailableRenderable();
	rm->setFrame(0.0); // Force a cache purge
    
    rm->numVertices = smesh->getVertices().size();
    
    makeVerticesNormals(smesh, &rm->mSystemVertices[0], (CVector3 *)((char*)buffer + rm->normals.getOffset(CVertexBuffer::NORMAL)), (CVector3 *)((char*)buffer + rm->normals.getOffset(CVertexBuffer::TANGENT)), &rm->elemPlanes[0], striplen);
    interleave(buffer, &rm->mSystemVertices[0], striplen, rm->vertices.getOffset(CVertexBuffer::POSITION), rm->numVertices);
    interleave(buffer, &smesh->getTexCoords()[0], striplen, rm->tex_st.getOffset(CVertexBuffer::TEX_COORD), rm->numVertices);
    rm->mVertexBuffer->uploadData(buffer, 0, rm->numVertices * striplen);
    
    rm->numElems = smesh->getElems().size() / 3;
    rm->mElems->uploadData(&smesh->getElems()[0], 0, rm->numElems * 3 * sizeof(S32));

    memcpy(rm->neighbors, &smesh->getNeighbors()[0], rm->numElems * 3 * sizeof(S32));
    
    CRenderableQueueMember *r = new CRenderableQueueMember();
    r->setLastTransform(mLastOrientation, mLastPosition);
	r->setRenderable(rm);
	r->setOrientation(getDerivedOrientation());
	r->setPosition(getDerivedPosition());

	//FIXME : Here I ivge the whole model's bound box on all submesses.
	//        This can probably optimaze performance by giving specific bound boxes
	r->setBounds(getDerivedBound());
    rq->addRenderable(r, mesh->getShaderById(smesh->getShader()));    
}

template<class T>
T *advance(T *p, S32 bytes){
    return (T *)((char *)p + bytes);
}

void CSceneModel::makeVerticesNormals(CSubMesh *sm, CVector4 *v, CVector3 *n, CVector3 *tang, CPlane *p, S32 stride){
    CSkeletonInstance::BoneVector &bv = mSkeleton->getBones();

    vector<CVector3> &vertices = sm->getVertices();
    vector<CVector3> &normals = sm->getNormals();
	vector<CVector3> &tangents = sm->getTangents();
    vector<CSubMesh::VInfluences> &infs = sm->getInfluences();
    vector<CVector3>::const_iterator vertex, normal, tangent;
    vector<CSubMesh::VInfluences>::const_iterator inf;

    vertex = vertices.begin();
    normal = normals.begin();
	tangent = tangents.begin();
    inf = infs.begin();

	F32 tri = 10;
    CVector4 *vi = v;
    CVector3 *ni = n;
    CPlane *pi = p;
    CVector3 *ti = tang;
    CMatrix3 rotmat;
	getDerivedOrientation().toRotationMatrix(rotmat);

    CSkeletonInstance::BoneVector::const_iterator bi;
    //made this not be a reference as gcc wouldn't accept it
    CSkeletonInstance::BoneVector::const_iterator be = bv.end();
    for(bi = bv.begin() ; bi != be ; ++bi){
        mBound.addPoint(rotmat * (*bi)->getDerivedTranslation());
    }

    memset(v, 0, sizeof(CVector4) * vertices.size());
    memset(n, 0, stride * vertices.size());

    const vector<CVector3>::const_iterator &verticesEnd = vertices.end();
    while(vertex != verticesEnd){
        vi->w = 1.0;
        CSubMesh::VInfluences::const_iterator i;
        const CSubMesh::VInfluences::const_iterator &ie = inf->end();
        for(i = inf->begin() ; i != ie ; ++i){
            const CBoneInstance &b = *bv[i->boneId];
            const F32 weight = i->weight;
            const CMatrix3 &m = b.getBoneSpaceMatrix();
            CVector3 t(m * *vertex);
            t += b.getTranslationBoneSpace();
            *vi += t * weight;
            
            t = m * *normal;
            *ni += t * weight;

			t = m * *tangent;
            *ti += t * weight;
        }
        normalize(*ni);
		normalize(*ti);

        ++vertex;
        ++normal;
        ++tangent;

        ++inf;
        ++vi;

        ni = advance(ni, stride);
        ti = advance(ti, stride);
    }
    vector<S32> &elems = sm->getElems();
    vector<S32>::const_iterator ei;
    const vector<S32>::const_iterator &eie = elems.end();
    ei = elems.begin();

    pi = p;
    while(ei != eie){
        const S32 a = *ei; ++ei;
        const S32 b = *ei; ++ei;
        const S32 c = *ei; ++ei;
        const CVector3 &va = *(CVector3*)&v[a];
        const CVector3 &vb = *(CVector3*)&v[b];
        const CVector3 &vc = *(CVector3*)&v[c];
        pi->normal = ((vc - va) ^ (vb - va)).normalized();
        pi->dist = va * pi->normal;
        pi->onAxe = -1;
        ++pi;
    }
    
}

void CSceneModel::debugDrawSkeleton(CRenderer *r){
    CSkeletonInstance::BoneVector &bv = mSkeleton->getBones();

    r->beginObjectTransformations();
    r->translate(getDerivedPosition());
    r->rotate(getDerivedOrientation());
    
	glDepthRange(0.0,0.001);

    CSkeletonInstance::BoneVector::const_iterator bi;
    //Made this not be a reference, see above or changelog
    CSkeletonInstance::BoneVector::const_iterator be = bv.end();
    for(bi = bv.begin() ; bi != be ; ++bi){
        const CBoneInstance &bone = **bi;

        if(!(*bi)->getParentBone())
            continue;

        CVector3 start = (*bi)->getParentBone()->getDerivedTranslation();

        r->debugDrawLine(start, (*bi)->getDerivedTranslation());

    }

    r->endObjectTransformations();
    glDepthRange(0.0,1.0);
}


