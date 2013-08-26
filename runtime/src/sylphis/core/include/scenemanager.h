/***************************************************************************
                          scenemanager.h  -  description
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
