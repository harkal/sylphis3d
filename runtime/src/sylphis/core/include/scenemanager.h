/***************************************************************************
                          scenemanager.h  -  description
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

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "globaldefs.h"

#include <map>
#include <string>
#include <list>
using namespace std;

#include "plane.h"
#include "sceneobject.h"
#include "renderqueue.h"
#include "scenenode.h"
#include "light.h"
#include "rigidbody.h"
#include <particlesystem.h>

class CRenderer;
class CModel;
class CAABoundBox;
class CTraceResult;

class CSceneManager {
public:
    CSceneManager();
    virtual ~CSceneManager();

	virtual void registerObject(CSceneObject *o);

    virtual void loadWorldMap(const string &name);
    virtual void unloadWorldMap();

	virtual void setAmbientLight(const CVector3 &c);

    CSceneNode* getRootSceneNode();

    virtual void update(F64 time);

    virtual void render(CRenderer *renderer);

    virtual void render();

	virtual void spatialize(CSceneObject *o);

	virtual bool isCulled(CSceneObject *o);

	virtual void collideRigidBodies();

	virtual CModel &getWorldMapModel(S32 n) = 0;

    virtual S32 getAreaPortal(const CVector3 &at) const = 0;
    virtual void setAreaPortalOpen(S32 portalId, bool o) = 0;

    void setActiveRenderQueue(CRenderQueue *rq);
    CRenderQueue *getActiveRenderQueue();

    // Compatibility with the bsp renderer. Remove soon.
    CRenderQueue *getRenderQueue();

    void addClipPlane(const CPlane &p);
    void clearClipPlanes();

    virtual void trace(CTraceResult *tr, CAABoundBox &bbox, const CVector3 &start,
                    const CVector3 &end); 

    virtual void raytrace(CTraceResult *tr, const CVector3 &start,
                    const CVector3 &end, CRigidBody *skip); 

    virtual void makeLightStatic(CLight *l) = 0;

	virtual void renderPerformaceInfo();

    void    addParticleSystem(CParticleSystem *p);
protected:
    typedef map<string, CSceneObjectPtr, less<string> > SceneObjectList;
    SceneObjectList mSceneObjects;

    ParticleSystemList mParticleSystems;

    typedef map<string, CLightPtr, less<string> > LightList;
    LightList mLights;

	CVector4 mAmbientLight;

    CSceneNodePtr mRootSceneNode;

    vector<CPlane> mClipPlanes;

    CRenderQueue mRenderQueue;
    CRenderQueue *mActiveRenderQueue;
};

#endif
