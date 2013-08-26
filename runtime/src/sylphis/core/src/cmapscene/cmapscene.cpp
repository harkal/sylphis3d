/***************************************************************************
                          cmapscene.cpp  -  description
                             -------------------
    begin                : Wed Jun 4 2002
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

#include <vector>
#include <algorithm>
using namespace std;

#include <engine.h>
#include <camera.h>
#include <renderer.h>
#include <vfs.h>
#include <light.h>
#include <frustum.h>
#include <tokenizer.h>
#include <configfile.h>
#include <shadowvolume.h>
#include <beamtree.h>
#include <polygon.h>
#include <scenemd3.h>
#include <openglext.h>
#include <physicsenvironment.h>
#include "cmapscene.h"
#include <trace.h>

#include <smfmodel.h>

#include <Python.h>
#include <ode/ode.h>

#include <rendersystem.h>

CColorf getAmbientSkyLight();

extern dSpaceID space;
CMaterialPtr mSpeedShader;

CCMapScene::CCMapScene() : mSVTimer(20), mPolyTimer(20), mVisTimer(20) {
	mCMAP = 0;
}
CCMapScene::~CCMapScene(){
	if(mCMAP)
		delete mCMAP;
}

void CCMapScene::registerObject(CSceneObject *o){
	o->setExtraData(static_cast<void *>(new ObjectAreasSet));
}

#define VertexCount 5
#define IndexCount 12

static dVector3 Size;
static CVector3 Vertices[VertexCount];
static S32 Indices[IndexCount];

S32 CCMapScene::getAreaPortal(const CVector3 &at) const {
    F32 minDist = mCMAP->mAreaPortalsAll[0].bound.distance(at);
    S32 portNo = 0;
    for(S32 i = 1 ; i < mCMAP->NumAreaPortals * 2; ++i){
        const cAreaPortal &portal = mCMAP->mAreaPortalsAll[i];
        F32 dist = portal.bound.distance(at);
        if(dist < minDist){
            minDist = dist;
            portNo = portal.portalId;
        }
    }   
    return portNo;
}

void CCMapScene::setAreaPortalOpen(S32 portalId, bool o){
    cAreaPortal *portal = &mCMAP->mAreaPortalsAll[0];
    for(S32 i = 0 ; i < mCMAP->NumAreas * 2; ++portal){
        if(portal->portalId == portalId){
            portal->open = o;
            ++i;
        }
        if(i == 2)
            return;
    }
}

std::vector<dGeomID> TriMeshes;

#include <particlesystem.h>

void CCMapScene::loadWorldMap(const string &name){
	gcon.printf("Loading world map %s\n", name.c_str());
    CTimerAdvanced timer;
    timer.start();
	CFile f(name);
	f.setVirtualFS(*CEngine::getSingleton()->vfs);

	try {
        mCMAP = new CMAP();
        gcon.printf("... Loading CMAP file\n");
		mCMAP->load(f);
        mAreaObjects.resize(mCMAP->NumAreas);
        registerObject(mRootSceneNode);

        gcon.printf("... Parsing actors\n");
        parseEntities();

#if 0
		dVector3 *verts = new dVector3[mCMAP->NumVerts];
		for(S32 i = 0 ; i < mCMAP->NumVerts ; i++){
			verts[i][0] = TO_PHYSICS(mCMAP->Verts[i].point.x);
			verts[i][1] = TO_PHYSICS(mCMAP->Verts[i].point.y);
			verts[i][2] = TO_PHYSICS(mCMAP->Verts[i].point.z);
		}
		for(S32 i = 0 ; i < mCMAP->NumLeaves ; i++){
			cleaf_t *leaf = &mCMAP->Leaves[i];
			if(leaf->area == 0)
				continue;
			S32 elemNum = 0;
			for(S32 j = leaf->firstleafface ; j < leaf->firstleafface + leaf->numleaffaces ; j++)
				elemNum += mCMAP->Faces[j].numelems;

			if(!elemNum)continue;
			S32 *elems = new S32[elemNum];
			elemNum = 0;

			for(S32 j = leaf->firstleafface ; j < leaf->firstleafface + leaf->numleaffaces ; j++){
				dface_t *face = &mCMAP->Faces[j];
				for(S32 k = face->firstelem ; k < face->firstelem + face->numelems ; k++)
					elems[elemNum++] = mCMAP->elems[k] + face->firstvert;
			}
			//assert(elemNum == 0);
			dTriMeshDataID Data = dGeomTriMeshDataCreate();
			dGeomTriMeshDataBuildSimple(Data, verts, mCMAP->NumVerts, elems, elemNum);
			dGeomID TriMesh = dCreateTriMesh(space, Data, 0, 0, 0);
            dGeomSetData(TriMesh, CPhysicsEnv::getSingleton().getStaticWorldBody());
		}
#else
		dVector3 *verts = new dVector3[mCMAP->NumVerts];
		for(S32 i = 0 ; i < mCMAP->NumVerts ; i++){
			verts[i][0] = TO_PHYSICS(mCMAP->Verts[i].point.x);
			verts[i][1] = TO_PHYSICS(mCMAP->Verts[i].point.y);
			verts[i][2] = TO_PHYSICS(mCMAP->Verts[i].point.z);
		}
		for(S32 i = 1 ; i < mCMAP->NumAreas ; i++){
			tArea *area = &mCMAP->mAreas[i];
			
			if(area->numOfElements <= 0)continue;

			S32 *elems = new S32[area->numOfElements * 3];
			S32 elemNum = 0;

			for(S32 j = area->startFace ; j < area->startFace + area->numOfFaces ; j++){
				dface_t *face = &mCMAP->Faces[mCMAP->lfaces[j]];
				for(S32 k = face->firstelem ; k < face->firstelem + face->numelems ; k++)
					elems[elemNum++] = mCMAP->elems[k] + face->firstvert;
			}
			
			dTriMeshDataID Data = dGeomTriMeshDataCreate();
			dGeomTriMeshDataBuildSimple(Data, &verts[0][0], mCMAP->NumVerts, elems, elemNum);
			dGeomID TriMesh = dCreateTriMesh(space, Data, 0, 0, 0);
            dGeomSetData(TriMesh, CPhysicsEnv::getSingleton()->getStaticWorldBody());
            TriMeshes.push_back(TriMesh);
		}
#endif
	} catch (CException e) {
        delete mCMAP;
		gcon.printf("CCMapScene::loadWorldMap : %s\n", e.getExplanation().c_str());
		throw CException("Failed to load map : \n" + e.getExplanation());
	}
    mSpeedShader = CEngine::getSingleton()->mMaterials.registerMaterial("speedShader");
    gcon.printf("CCMapScene::loadWorldMap : Loaded %s in %5.2f sec\n", name.c_str(), timer.stop());    
}

void CCMapScene::unloadWorldMap(){
    if(!mCMAP)
        return;

    delete mCMAP;
    mStaticLights.clear();
    mAreaObjects.clear();

    for(S32 i = 0 ; i < TriMeshes.size() ; ++i)
        dGeomDestroy(TriMeshes[i]);

    TriMeshes.clear();
}

extern PyObject *pyEngine;

void CCMapScene::parseEntities(){
	string buf = mCMAP->entities;

	PyObject *mdict = PyModule_GetDict(pyEngine);
	PyObject *func = PyDict_GetItemString(mdict, "addActors");
    if (func && PyCallable_Check(func)) {
		PyObject *rslt = PyObject_CallFunction(func, "(s)", mCMAP->entities);
		if(rslt){
			Py_XDECREF(rslt);
		} else {
			PyErr_Print();
			throw CException("addActors failed");	
		}
	} else {
		throw CException("Failed to call addActors()");
	}
}

CModel &CCMapScene::getWorldMapModel(S32 n){
	return *mCMAP->mBrushModels[n];
}

CCMapScene::ObjectAreasSet *CCMapScene::getObjectAreas(CSceneObject *o){
	return static_cast<ObjectAreasSet*>(o->getExtraData());
}

void checkBoundAreas_r(CMAP *mCMAP, tArea *area, CBound *bound, CCMapScene::ObjectAreasSet *areas, bool onlyOpen){
	for(S32 i = area->startPortal ; i < area->startPortal + area->numOfPortals ; i++){
        cAreaPortal *portal = &mCMAP->mAreaPortalsAll[i];

		if(!portal->open && onlyOpen)
			continue;

		if(!portal->bound.collide(bound))
			continue;

		S32 nextarea = portal->areas[1];
		if(includes(areas->begin(), areas->end(), &nextarea, (&nextarea) + 1))
			continue;

		areas->insert(nextarea);
		checkBoundAreas_r(mCMAP, &mCMAP->mAreas[nextarea], bound, areas, onlyOpen);
	}
}
#include <iostream>
using namespace std;
void CCMapScene::spatialize(CSceneObject *o){
	CVector3 areaVec = o->getDerivedPosition();
    S32 areaNo = mCMAP->findArea(areaVec);
	//S32 areaNo = mCMAP->findArea(o->getDerivedBound());
    tArea *area = &mCMAP->mAreas[areaNo];

	ObjectAreasSet *areas = getObjectAreas(o);

	ObjectAreasSet lastAreas, diffAreas, enter, left;
	lastAreas = *areas;
	areas->clear();
	areas->insert(areaNo);
    checkBoundAreas_r(mCMAP, area, &o->getDerivedBound(), areas, !o->passClosedPortals());

	set_difference(lastAreas.begin(), lastAreas.end(), 
		areas->begin(), areas->end(), inserter(left, left.begin()));	
	set_difference(areas->begin(), areas->end(),
		lastAreas.begin(), lastAreas.end(), inserter(enter, enter.begin()));	

	if(enter.size() == 0 && left.size() ==0)
		return;
	ObjectAreasSet::const_iterator i;
#if 0
	cout << endl << "lastAreas :";
	copy(lastAreas.begin(), lastAreas.end(), ostream_iterator<S32>(cout, " "));
	cout << endl << "areas :";
	copy(areas->begin(), areas->end(), ostream_iterator<const S32>(cout, " "));
	cout << endl << "diff :";
	copy(diffAreas.begin(), diffAreas.end(), ostream_iterator<const S32>(cout, " "));
	cout << endl << "left :";
	copy(left.begin(), left.end(), ostream_iterator<const S32>(cout, " "));
	cout << endl << "enter :";
	copy(enter.begin(), enter.end(), ostream_iterator<const S32>(cout, " "));
	cout << endl;
#endif
	for(i = enter.begin() ; i != enter.end() ; ++i){
		AreaObjectsSet &s = mAreaObjects[*i];
		s.push_front(o);
	}
	for(i = left.begin() ; i != left.end() ; ++i){
		AreaObjectsSet &s = mAreaObjects[*i];
		assert(find(s.begin(), s.end(), o) != s.end());
        //if(find(s.begin(), s.end(), o) != s.end())continue;
		s.remove(o);
	}
}

bool CCMapScene::isCulled(CSceneObject *o){
	if(mCullType == BOUND){
		return !o->getDerivedBound().collide(mCullBound);
	}
	ObjectAreasSet *s = getObjectAreas(o);
	if(!s)
		return true;
	ObjectAreasSet::const_iterator i;
	for(i = s->begin(); i != s->end() ; ++i){
		if(mAreaVis[*i]){
			CFrustum &frustum = mRenderer->getCamera()->getFrustum();
			return frustum.isCulled(o->getDerivedBound());
		}
	}
	return true;
}

void CCMapScene::collideRigidBodies(){
    
}

static S32 back;
static bool drawTrans;

void setupScissor(CRenderer *r, CLight *li){
	CMatrix4 &c = r->getMVPMatrix();

	CVector4 o(li->getDerivedPosition().x,
				li->getDerivedPosition().y,
				li->getDerivedPosition().z,
				1.0);
	CVector4 w;
	w = c * o;

	S32 view[4];
	glGetIntegerv(GL_VIEWPORT, (GLint *)view);

	w.x /= w.w;
	w.y /= w.w;
	w.x = view[2]*w.x/2 + view[2]/2;
	w.y = view[3]*w.y/2 + view[3]/2;
	
	glScissor((S32)(w.x - 100), (S32)(w.y - 100), 200, 200);
	//gcon.printf(" %f %f\n", w.x, w.y);
	glEnable(GL_SCISSOR_TEST);
}

S32 mSkyFlags = 0;
void setSkyFlags(S32 flags){
	mSkyFlags |= flags;
}

extern CRenderTexturePtr mSpeedBuffer;
CMatrix4 mLastMVPMatrix;

void CCMapScene::renderSky(){
	CRenderQueue rq;
	if(!mSkyModel)
		mSkyModel = CEngine::getSingleton()->models.registerSMFModel("models/sky.smf");

	CRenderTexturePtr rtex = dynamic_cast<CRenderTexture*>(CEngine::getSingleton()->textures.registerTexture("_bufferA"));
	CRenderTexturePtr ttex = dynamic_cast<CRenderTexture*>(CEngine::getSingleton()->textures.registerTexture("_bufferB"));
	CRenderTexturePtr tex1 = dynamic_cast<CRenderTexture*>(CEngine::getSingleton()->textures.registerTexture("_variabletexture"));

	CModel::MeshVector::iterator i;

	CModel::MeshVector &meshes = mSkyModel->getRMeshes();
	for (i = meshes.begin(); i != meshes.end(); ++i++){
        CRenderableQueueMember *r = new CRenderableQueueMember();
		r->setRenderable(*i);
		r->setOrientation(CQuaternion::IDENTITY);
		r->setPosition(CVector3(0,-4926.8,0) + mRenderer->getCamera()->getDerivedPosition());
		//r->setPosition(CVector3(0,-10.1,0) + mRenderer->getCamera()->getDerivedPosition());
		//r->setBounds();
        r->setNoShadows(true);
		r->setLastTransform(CQuaternion::IDENTITY, r->getPosition());
		rq.addRenderable(r, (*i)->getDefaultShader());
    }
	
	//mSunLight = 0;
	mRenderer->setLight(mSunLight);
	//mRenderer->setAmbientLight(mSunLight->getColor());
	mRenderer->setRenderQueue(&rq);
	glDepthRange(0.99, 1.0);
	mRenderer->flushRenderQueue();

#if 1
	gRSystem->pushWorldMatrix();
        gRSystem->resetWorldMatrix();
        gRSystem->pushProjMatrix();
        gRSystem->resetProjMatrix();

        glOrtho(0,1.0, 0 ,1.0,-999.0,999.0);
		glViewport(0,0, 128, 128);
        
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
	ttex->beginCapture();
	mRenderer->setOverrideShader(CEngine::getSingleton()->mMaterials.registerMaterial("sky_flat"));
	mRenderer->flushRenderQueue();
	mRenderer->setOverrideShader(0);
	ttex->endCapture();

	//mRenderer->downSample(rtex, ttex, 256, 256, 128, 128);
	mRenderer->downSample(ttex, rtex, 128, 128, 64, 64);
	mRenderer->downSample(rtex, ttex, 64, 64, 32, 32);
	mRenderer->downSample(ttex, rtex, 32, 32, 16, 16);
	mRenderer->downSample(rtex, ttex, 16, 16, 8, 8);
	mRenderer->downSample(ttex, rtex, 8, 8, 4, 4);
	mRenderer->downSample(rtex, ttex, 4, 4, 2, 2);
	mRenderer->downSample(ttex, tex1, 2, 2, 1, 1);

	tex1->beginCapture();
	float p[3];
	glReadPixels(0,0, 1, 1, GL_RGB, GL_FLOAT, p);
	//gcon.printf("%f %f %f\n", p[0], p[1], p[2]);
	p[1] = p[2] = p[0];
	tex1->endCapture();

	mAmbientLight = CVector4(p[0], p[1], p[2], 1.0);
	mAmbientLight *= 0.9;

	pair<S32,S32> d = mRenderer->getRenderingView().getDimentions();
        glViewport(0,0, d.first, d.second);
        gRSystem->popProjMatrix();
        gRSystem->popWorldMatrix();
#endif

	glDepthRange(0.0, 1.0);

	
}

void CCMapScene::render(CRenderer *renderer){
    mRenderer = renderer;
	CCamera *cam = renderer->getCamera();
    CRenderQueue rq;
	CRenderQueue shadowRenderQueue;

	bool doShadows = renderer->isShadows();
    
    glCullFace(GL_BACK);

    //glPolygonOffset(0.000000,-1.000000);
    //glEnable(GL_POLYGON_OFFSET_FILL);

	mVisTimer.start();
    CVector3 retVector = cam->getDerivedPosition();
    findSeenFacesPortals(retVector);

	renderer->setLight(0); // Ambient light
	renderer->setAmbientLight(mAmbientLight);
	renderer->setRenderQueue(&rq);
    setActiveRenderQueue(&rq);
	mCullType = FRUSTUM;
	mVisTimer.stop();
    mPolyTimer.start();
    renderer->ignoreNewLights(false);
    getRootSceneNode()->onRender(renderer);

#if 0
    gcon.printf("Lights : %d\n", renderer->getLights()->size());
#endif
	
	drawTrans = false;
    renderWorld(retVector, mFaceList);
    renderer->flushRenderQueue();
	// Do the transparent
	drawTrans = true;
    renderWorld(retVector, mFaceList);
	mPolyTimer.stop();

    //
    // Render the speed buffer
    //
    renderer->setLastMVPMatrix(mLastMVPMatrix);
    
    if(renderer->needSpeedRender()){
        mSpeedBuffer->beginCapture();
        renderer->setOverrideShader(mSpeedShader);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        drawTrans = false;
        renderWorld(retVector, mFaceList);
        renderer->flushRenderQueue();
		renderSky();
        renderer->setOverrideShader(0);
        gRSystem->popWorldMatrix();
        mSpeedBuffer->endCapture();
    }
    
//#define NOLIGHTS
#ifndef NOLIGHTS
	list<CLightPtr>* lights = renderer->getLights();
    list<CLightPtr>::iterator li;

    glDepthMask(0);
    
	renderer->ignoreNewLights(true);
    //glClear(GL_STENCIL_BUFFER_BIT);
    for(li = lights->begin() ; li != lights->end() ; ++li){
		if((*li)->getType() == CLight::sun){
			mSunLight = *li;
		}
		mSVTimer.start();
        filterFacesInRadius(**li);
		mSVTimer.stop();
		renderer->setLight(*li);
		renderer->setupScissor();
#if 0
        if((*li)->isShadowCaster() && doShadows){
			mSVTimer.start();
			glColorMask(0,0,0,0);
            glClearStencil(128);
			renderer->clearStencil();
            glEnable(GL_STENCIL_TEST);
            glDepthFunc(GL_LEQUAL);
			glStencilFunc(GL_ALWAYS, 1, 255);
			glStencilMask(255);
            glPolygonOffset(0.000000, 200.000000);
            glEnable(GL_POLYGON_OFFSET_FILL);

            setActiveRenderQueue(&shadowRenderQueue);
			renderer->setRenderQueue(&shadowRenderQueue);
            mCullType = BOUND;
			mCullBound = &(*li)->getDerivedBound();
			getRootSceneNode()->onRender(renderer);
			
            //glEnable(GL_BLEND);
            //glBlendFunc(GL_ONE, GL_ONE);
            //glColor3f(1.0,1.0,1.0);
            //glColorMask(1,1,1,1);
            
			renderShadowVolume(*li);
            renderer->renderShadowVolumes(*li, back);

			//glColorMask(0,0,0,0);

            glColorMask(1,1,1,1);			
			glStencilFunc(GL_GEQUAL, 128, 255);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glDepthFunc(GL_EQUAL);

            glDisable(GL_POLYGON_OFFSET_FILL);

			mSVTimer.stop();
		}
#endif
        // Draw scene again
        glCullFace(GL_BACK);
#if 1
		mPolyTimer.start();
		drawTrans = false;
        renderWorld(retVector, mFaceListInRadius);

		//setActiveRenderQueue(&rq);
		renderer->setRenderQueue(&rq);
		renderer->flushRenderQueue(*li);

		// Do the trans
        if(doShadows){
		    glDisable(GL_STENCIL_TEST);
        }
		drawTrans = true;
        renderWorld(retVector, mFaceListInRadius);
        //renderer->flushRenderQueue(*li);
		mPolyTimer.stop();
#endif
        
		glDisable(GL_SCISSOR_TEST);
    }
#endif

	renderSky();

    renderer->grabCurrentRender();
    glDepthMask(1);

	mSVTimer.reset();
	mPolyTimer.reset();
	mVisTimer.reset();
    
    rq.clear();

#ifndef NOLIGHTS 
	if(gCfgFile.getAsInt("sm_lightbounds")){
	glDisable( GL_TEXTURE_2D );
	CVector3 lightVector;
    for(li = lights->begin() ; li != lights->end() ; li++){
		lightVector = (*li)->getDerivedPosition();
        CVector3 &pos = lightVector;
		glTranslatef(pos.x, pos.y, pos.z);
		glDisable(GL_DEPTH_TEST);
		renderer->GLColor4(0.2, 0.2, 0.0);
		(*li)->getBound().render(renderer);
		glEnable(GL_DEPTH_TEST);
		renderer->GLColor4(1.0, 1.0, 0.0);
		(*li)->getBound().render(renderer);
		glTranslatef(-pos.x, -pos.y, -pos.z);
	}
	}
#endif

    if(gCfgFile.getAsInt("sm_debugportals")){
        //glDisable(GL_DEPTH_TEST);
        glDisable( GL_TEXTURE_2D );
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE,GL_ONE);
        glDisable(GL_CULL_FACE);
        for(S32 y = 0 ; y < mCMAP->NumOfAreasToPortals ; y++){
            dface_t *f = mCMAP->mAreaPortalsAll[y].face;
            if(!mCMAP->mAreaPortalsAll[y].visible)
                continue;

            glColor3f(0.0, 0.05, 0.0);

            glBegin(GL_POLYGON);//LINE_LOOP);
            for(S32 i = f->firstvert ; i < f->firstvert + f->numverts ; i++){
                glVertex3fv(&mCMAP->Verts[i].point.x);
            }
            glEnd();
            glDisable(GL_DEPTH_TEST);
            glColor3f(0.0, 1.0, 0.0);
            glBegin(GL_LINE_LOOP);
            for(S32 i = f->firstvert ; i < f->firstvert + f->numverts ; i++){
                glVertex3fv(&mCMAP->Verts[i].point.x);
            }
            glEnd();
            glEnable(GL_DEPTH_TEST);
        }
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }

	// Reset Portals
	for(S32 y = 0 ; y < mCMAP->NumOfAreasToPortals ; y++){
		mCMAP->mAreaPortalsAll[y].visible = false;
	}

	getRootSceneNode()->postRenderUpdate(renderer);

    mLastMVPMatrix = renderer->getModelViewMatrix();
}

typedef CVector4 buffer_position_t;

void CCMapScene::renderShadowVolume(CLight *l){
    dface_t *face;
    S32 f;
    CVector4 temp;

    //glDepthFunc(GL_LEQUAL);
    glDisable(GL_TEXTURE_2D);

    CVector3 pos = l->getEmissionPosition();
    CVector4 lightPos(pos.x, pos.y, pos.z, 0.0);

    LightShadowMap::iterator i;
    i = mStaticLights.find(l);
    if(i != mStaticLights.end()){
        CShadowVolume &v = mStaticLights[l];
        if(v.mVertices){
            mRenderer->getArrays()->mVertexBuffer = v.mVertices;
            memcpy(mRenderer->getArrays()->mElems->map(), &v.mElements[0], sizeof(U32) * v.mElements.size());
            mRenderer->getArrays()->numVerts = v.mVertices->getSize();
            mRenderer->getArrays()->numElems = v.mElements.size();
            mRenderer->flushShadowVolume();//lightPos);
            mRenderer->clearArrays();
            mRenderer->useDefaultArrays();
        }
    } else {
        for (f = 0; f < mFaceListInRadius.numfaces; f++){
            face = &mCMAP->Faces[mFaceListInRadius.faces[f].face];

            if(mCMAP->shaderrefs[face->shader].shader->isTransparent())
                continue;

            if(face->plane->distance(pos) < 0.0)
                continue;

            S32 off = face->numverts;

            S32 i;

            buffer_position_t *bufferVertex = (buffer_position_t *)mRenderer->getArrays()->mVertexBuffer->map() + mRenderer->getArrays()->numVerts;

            for(i = face->numverts -1 ; i > 1 ; i--){
                mRenderer->pushElem(i, i-1 , 0);
            }
            for(i = face->firstvert; i < face->firstvert + face->numverts; i++){
                *bufferVertex = mCMAP->Verts[i].point;
                ++bufferVertex;
            }

            for(i = face->numverts -1 ; i > 1 ; i--){
                mRenderer->pushElem(off + 0, off+i-1, off+i);
            }
            for(i = face->firstvert; i < face->firstvert + face->numverts; i++){
                temp = mCMAP->Verts[i].point;// - lightPos;
                temp.w = 0.0;
                *bufferVertex = temp;
                ++bufferVertex;
            }
            mRenderer->getArrays()->numVerts += face->numverts << 1;

            mRenderer->flushShadowVolume(lightPos);
            mRenderer->getArrays()->numElems = 0; //FIXME add method for this

            for(i = 0 ; i < face->numverts -1 ; i++){
                mRenderer->pushElem(off + i,i, off + i + 1);
                mRenderer->pushElem(off + i + 1,i, i + 1);
            }
            mRenderer->pushElem(off + i, i, off);
            mRenderer->pushElem(off, i, 0);

            mRenderer->flushShadowVolume(lightPos);
            mRenderer->clearArrays();
        }	
    }
    glDepthFunc(GL_LESS);
}

void CCMapScene::renderWorld(CVector3 &o, facelist_t &faceList){

    S32 f;
    CMaterialPtr material;
    S32 key;
    
    dface_t *face;

    glMatrixMode(GL_MODELVIEW);

    key = (U32)-1;
	S32 skyFlag = 0;
    //cout << "rendering " << mFaceList.numfaces << " faces\n";
    for (f=0; f < faceList.numfaces; ++f){
        S32 faceNo = faceList.faces[f].face;
        if(mFacesSeen[faceNo] == 0)
            continue;
        face = &mCMAP->Faces[faceNo];
#if 0
		if(mCMAP->shaderrefs[face->shader].shader->flags & SHADER_SKY){
			for(S32 i = face->firstvert ; i < face->firstvert + face->numverts ; i++){
				CVector3 d;
				d = mCMAP->Verts[i].point - o;
				if(d[0]<0)skyFlag |= CRenderer::SKY_LEFT; else	skyFlag |= CRenderer::SKY_RIGHT;
				if(d[2]<0)skyFlag |= CRenderer::SKY_FRONT; else	skyFlag |= CRenderer::SKY_BACK;
				if(d[1]>0)skyFlag |= CRenderer::SKY_UP; else		skyFlag |= CRenderer::SKY_DOWN;
			}
			setSkyShader(mCMAP->shaderrefs[face->shader].shader);
			setSkyFlags(skyFlag);
			continue;
		}
#endif
        bool isTrans = mCMAP->shaderrefs[face->shader].shader->isTransparent();
		if((isTrans||drawTrans)&&!(isTrans&&drawTrans))
			continue;

		//if(face->v_norm * (o - face->v_orig) < 0)continue;
        /* Look for faces that share rendering state */
        if (faceList.faces[f].sortkey != key){
            /* Flush the renderer and reset */
            if (f){
                mRenderer->flushWithMaterial(material, 0, 0);
                mRenderer->clearArrays();
            }
            material = mCMAP->shaderrefs[face->shader].shader;
            key = faceList.faces[f].sortkey;
        }

        pushFace(face);
#if 0
        switch (face->facetype){
        case FACETYPE_NORMAL:
        case FACETYPE_TRISURF:
            pushFace(face);
            break;
     /*    case FACETYPE_MESH:
            render_pushmesh(mFaceList->faces[f].face, origin);
            break;*/
            default:
            break;
        }
#endif 
    }
    mRenderer->flushWithMaterial(material, 0, 0);
    mRenderer->clearArrays();
}

typedef struct {
    CVector4 position;
    CVector3 normal;
    CVector3 tangent;
    CTexCoord tex_st;
} buffer_vertex_t;

void CCMapScene::pushFace(dface_t *face){
    CArrays *arrays = mRenderer->getArrays();

    S32 i, *elem;
    dvertex_t *vert;

    elem = &mCMAP->elems[face->firstelem];
    U32 *elems = (U32 *)arrays->mElems->map();
    for (i = 0; i < face->numelems; i++){
        elems[arrays->numElems++] = arrays->numVerts + *elem++;
    }

    vert = &mCMAP->Verts[face->firstvert];

    buffer_vertex_t *bufferVertex = (buffer_vertex_t *)arrays->mVertexBuffer->map() + arrays->numVerts;
    
    for (i = 0; i < face->numverts; ++i){
        bufferVertex->position = vert->point;
        bufferVertex->normal   = vert->v_norm;
        bufferVertex->tangent  = vert->tang;
        bufferVertex->tex_st   = vert->tex_st;
        ++vert;        
        ++bufferVertex;
    }
    arrays->numVerts += face->numverts;

}

#define CMAP_TESTVIS(from,to) true

void CCMapScene::addLeafFaces(cleaf_t *leaf){
    dface_t *face;
    S32 i, faceidx, listidx;;
    S32 firstFace = leaf->firstleafface;
    S32 numFaces = leaf->numleaffaces;

    for(i=0;i<numFaces;++i){

        faceidx = mCMAP->lfaces[firstFace + i];
        
        face=&mCMAP->Faces[faceidx];
        if(mFacesSeen[faceidx]) // REMOVE IT!
            continue;

        ++mFacesSeen[faceidx];
        listidx = mFaceList.numfaces;
        //mFaceList.faces[listidx].origin = CVector3(0.0, 0.0, 0.0);
        mFaceList.faces[listidx].face = faceidx;
        mFaceList.faces[listidx].sortkey = face->shader;
        mFaceList.numfaces = listidx + 1;
    }
}

void CCMapScene::addBrushFaces(CVector3 &v, S32 firstFace, S32 numFaces, F32 radius){
    dface_t *face;
    S32 i, faceidx, listidx;;

    for(i=0;i<numFaces;i++){
        faceidx = mCMAP->lfaces[firstFace + i];
        //faceidx = firstFace + i;
        // If it doesn't belong to the main model break
        //if(faceidx - mCMAP->Models[0].firstface > mCMAP->Models[0].numfaces)break;
        face=&mCMAP->Faces[faceidx];
/*
        if(shaders[mCMAP->shaderrefs[face->shader].shader].flags & SHADER_SKY){
            AddSkyFace(face);
            continue;
        }
*/
        if(mFacesSeen[faceidx])continue;
		//if(radius > 0.0)if((v - face->v_orig).length() > 2 * radius)continue;
        ++mFacesSeen[faceidx];
        listidx = mFaceList.numfaces;
        mFaceList.faces[listidx].origin = CVector3(0.0, 0.0, 0.0);
        mFaceList.faces[listidx].face = faceidx;
        mFaceList.faces[listidx].sortkey = face->shader;
        mFaceList.numfaces = listidx + 1;
    }
}

S32 CCMapScene::face_cmp(const void *a, const void *b){
    return ((rendface_t*)a)->sortkey - ((rendface_t*)b)->sortkey;
}

void CCMapScene::addAreaLeafs(tArea *area){
    cleaf_t *leaf = &mCMAP->Leaves[area->startLeaf+1];
    for(S32 i = 0 ; i < area->numOfLeafs ; ++i, ++leaf) {
        //if (!mFrustum->isCulled(*leaf->bounds)) {
            addLeafFaces(leaf);
        //}
    }
}

void CCMapScene::checkAreas_r(tArea *area, tArea *lastArea){
    for(S32 i = area->startPortal ; i < area->startPortal + area->numOfPortals ; i++){
        cAreaPortal *portal = &mCMAP->mAreaPortalsAll[i];

//		if(!mAreaVis[portal->areas[1]] || portal->visible)
  //          continue;

		if(!portal->open)
			continue;

		if(&mCMAP->mAreas[portal->areas[1]] == lastArea) //don't go back
			continue;

        if(!portal->reverse && portal->face->plane->distance(*mOrigin) < 0.0)
            continue;

        if(portal->reverse && portal->face->plane->distance(*mOrigin) > 0.0)
            continue;

        if(mFrustum->isCulled(portal->bound))
            continue;
#define EXACT_FRUSTUM
#ifdef EXACT_FRUSTUM
		// Add frustum planes
		dface_t *pFace = portal->face;
		for(S32 j = 0 ; j < pFace->numverts ; j++){
			S32 cur = j + pFace->firstvert;
			S32 next = ((j + 1) % pFace->numverts) + pFace->firstvert;

			if(portal->reverse){
				mFrustum->pushBackPlane(CPlane(*mOrigin, mCMAP->Verts[next].point, mCMAP->Verts[cur].point));
			} else {
				mFrustum->pushBackPlane(CPlane(*mOrigin, mCMAP->Verts[cur].point, mCMAP->Verts[next].point));
			}
		}
#endif

        faceSeenThreshold = 0;

		portal->visible = true;
        mAreaVis[portal->areas[1]] = true;
        tArea *nextArea = &mCMAP->mAreas[portal->areas[1]];
		//gcon.printf("%d ", portal->areas[1]);
        addAreaLeafs(nextArea);
        checkAreas_r(nextArea, area);

#ifdef EXACT_FRUSTUM
		mFrustum->popBackPlanes(pFace->numverts);
#endif
    }
}

void CCMapScene::findSeenFacesPortals(CVector3 &o){
    S32 areaNo = mCMAP->findArea(o);
    tArea *area = &mCMAP->mAreas[areaNo];

    {for(S32 i = 0 ; i < mCMAP->NumAreas ; i++)
        mAreaVis[i] = false;
    }
    memset(mFacesSeen, 0, sizeof(char) * MAX_MAP_FACES);
    
    mFaceList.numfaces = 0;
    
    mFrustum = &mRenderer->getCamera()->getFrustum();
	mOrigin = &mRenderer->getCamera()->getDerivedPosition();
    
    mAreaVis[areaNo] = true;
    addAreaLeafs(area);
    checkAreas_r(area);

	qsort((void*)mFaceList.faces, mFaceList.numfaces, sizeof(rendface_t),
          face_cmp);

#if 0
    gcon.printf("AREA : %d ", areaNo);
    for(S32 i = 0 ; i < mCMAP->NumAreas ; i++)
        if(mAreaVis[i])
            gcon.printf("%d ", i);

    gcon.printf("\n");
#endif
}

void CCMapScene::filterFacesInRadius_r(S32 nodeNo, CLight &l){
    cnode_t *node;

    if(nodeNo > 0){
        //It is a leaf
        cleaf_t *leaf = &mCMAP->Leaves[nodeNo];

		CAABoundBox *b = &l.getDerivedBound();
		if(!leaf->bounds->collide(b))
            return;

        dface_t *face;
        S32 i, faceidx, listidx;;
        S32 firstFace = leaf->firstleafface;
        S32 numFaces = leaf->numleaffaces;

		CVector3 orig = l.getEmissionPosition();
        for(i=0;i<numFaces;i++){
            faceidx = mCMAP->lfaces[firstFace + i];
        
            face=&mCMAP->Faces[faceidx];
            
            if(face->plane->distance(orig)<0.0)
				continue;
			if(b->distance(*face->plane) > 0.0)
				continue;
			
			listidx = mFaceListInRadius.numfaces;
            //mFaceListInRadius.faces[listidx].origin = CVector3(0.0, 0.0, 0.0);
            mFaceListInRadius.faces[listidx].face = faceidx;
            mFaceListInRadius.faces[listidx].sortkey = face->shader;
            mFaceListInRadius.numfaces = listidx + 1;
        }

        return;
    }

    node = &mCMAP->Nodes[-nodeNo];
	if(!node->bounds->collide(&l.getDerivedBound()))
        return;

	F32 dist = node->plane->distance(l.getEmissionPosition());

    if(dist <= 0){
        if(node->children[1] != 0)
            filterFacesInRadius_r(node->children[1], l);
        if(node->children[0] != 0)
            filterFacesInRadius_r(node->children[0], l);
    } else {
        if(node->children[0] != 0)
            filterFacesInRadius_r(node->children[0], l);
        if(node->children[1] != 0)
            filterFacesInRadius_r(node->children[1], l);
    }
}

void CCMapScene::filterFacesInRadius(CLight &l){
    mFaceListInRadius.numfaces = 0;

	LightShadowMap::iterator i;
    i = mStaticLights.find(&l);
    if(i != mStaticLights.end()){
        CShadowVolume &v = mStaticLights[&l];
        vector<S32>::iterator f;
        for(f = v.mLitPolygons.begin() ; f != v.mLitPolygons.end() ; f++){
			S32 listidx = mFaceListInRadius.numfaces;
            mFaceListInRadius.faces[listidx].face = *f;
            mFaceListInRadius.faces[listidx].sortkey = mCMAP->Faces[*f].shader;
            mFaceListInRadius.numfaces = listidx + 1;
		}
		return;
	}

    filterFacesInRadius_r(0, l);
}

void CCMapScene::trace(CTraceResult *tr, CAABoundBox &bbox, const CVector3 &start,
                    const CVector3 &end){

    CVector3 delta = end - start;
    CAABoundBox speedBox;

    for(S32 i = 0 ; i < 3 ; i++){
        if(delta[i] > 0){
            speedBox.getMin()[i] = start[i] + bbox.getMin()[i];
            speedBox.getMax()[i] = end[i] + bbox.getMax()[i];
        } else {
            speedBox.getMax()[i] = start[i] + bbox.getMax()[i];
            speedBox.getMin()[i] = end[i] + bbox.getMin()[i];
        }
    }

    mCMAP->setActiveArea(&speedBox);

    mCMAP->markActiveLeafs(0);

    mCMAP->trace(tr, bbox, start, end);
}

void CCMapScene::makeLightStatic(CLight *l){
    LightShadowMap::iterator i;
    i = mStaticLights.find(l);
    if(i != mStaticLights.end())
        return;

	l->update(0.0, true);   // Update the derived bound. Since makeLightStatic
                        // Is usualy called at the begining when bounds
                        // are crap
    filterFacesInRadius(*l);

    CShadowVolume &v = mStaticLights[l];

    dface_t *face;
        
    CBeamTree btree;
    btree.reset(l->getEmissionPosition());

    S32 allPolys = 0;
    for (S32 f = 0; f < mFaceListInRadius.numfaces; f++){
        face = &mCMAP->Faces[mFaceListInRadius.faces[f].face];

		if(face->plane->distance(l->getEmissionPosition()) < 0.0)
            continue;

        allPolys++;
        CPolygon p;
        for(S32 j = face->firstvert ; j < face->firstvert + face->numverts ; j++){
            p.addVertex(mCMAP->Verts[j].point);
        }
		if(mCMAP->shaderrefs[face->shader].shader->isTransparent()){
			btree.addPolygon(p, mFaceListInRadius.faces[f].face);
		} else {
		   btree.addShadowCastingPolygon(p, mFaceListInRadius.faces[f].face);
		}
    }
    btree.getShadowCastingPolygons(v.mPolygons);
	btree.getLitPolygons(v.mLitPolygons);

    S32 alledges = 0;
    S32 usededges = 0;
    for(vector<S32>::iterator i = v.mPolygons.begin() ; i != v.mPolygons.end() ; ++i){
        face = &mCMAP->Faces[*i];
        vector<S32> edges;
        for(S32 j = 0 ; j < face->numverts ; ++j){
            ++alledges;
            bool used = false;

            S32 n = mCMAP->neighbors[face->firstvert + j];
            if(n < 0){
                used = true;
            }
            else if(find(v.mPolygons.begin(), v.mPolygons.end(), n) == v.mPolygons.end()){
                used = true;
            }
                
            //used = true;
            if(used){
                usededges++;
                CShadowEdge e;
                e.start = mCMAP->Verts[face->firstvert + j].point;
                e.end = mCMAP->Verts[face->firstvert + ((j + 1) % face->numverts)].point;
                v.mEdges.push_back(e);

                edges.push_back(j);
            }
        }
        v.mUsedEdges.push_back(edges);
    }
    CVector4 temp;
    temp = l->getEmissionPosition();
    v.makeVertexBuffers(mCMAP, temp, l->getRadius().length());
#if 0
    gcon.printf("Used edges %d out of %d\n", usededges, alledges);
	gcon.printf("Light has %d(BT saved %d) shadow polygons and %d lit polygons\n", v.mPolygons.size(), allPolys - v.mPolygons.size(), v.mLitPolygons.size());
#endif
}

void CCMapScene::renderPerformaceInfo(){
	CSceneManager::renderPerformaceInfo();

	char text[256];
	F32 avrFrameLength = 1.0f / CEngine::getSingleton()->getFPS();
	F32 perS = mSVTimer.getAvrTime() / avrFrameLength * 100.0f;
	F32 perP = mPolyTimer.getAvrTime() / avrFrameLength * 100.0f;
	F32 perV = mVisTimer.getAvrTime() / avrFrameLength * 100.0f;
	sprintf(text, "Visibility : %f (%4.2f%%)\nShadows : %f (%4.2f%%)\nRendering : %f (%4.2f%%)\n", mVisTimer.getAvrTime(), perV, mSVTimer.getAvrTime(), perS, mPolyTimer.getAvrTime(), perP);
	CEngine::getSingleton()->getDefaultFont()->renderString(text);
}

