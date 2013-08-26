/***************************************************************************
                          scenemanager.cpp  -  description
                             -------------------
    begin                : Mon Jun 3 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#include <engine.h>
#include <ode/ode.h>
#include <renderer.h>
#include <scenemanager.h>
#include <trace.h>
#include <python/mthread.h>
#include <physics.h>
#include <physicsenvironment.h>

extern dWorldID world;
extern dSpaceID space;
static dJointGroupID contactgroup;
CTimerAdvanced mConTimer;
CTimerAdvanced mPhyTimer;


CSceneManager::CSceneManager(){
    // Setup the root node of the scene
    mRootSceneNode = new CSceneNode(this);
    mRootSceneNode->setFlags(CSceneObject::NOCULL);
    mRootSceneNode->setParent(0);
    setAmbientLight(CVector3(1.0,1.0,1.0));

    mActiveRenderQueue = &mRenderQueue;

    contactgroup = dJointGroupCreate (0);

    CPhysicsEnv::getSingleton()->setGravity(CVector3(0.0, -600.0, 0.0));
    CPhysicsEnv::getSingleton()->setAutoSleep(false);
    CPhysicsEnv::getSingleton()->setAutoSleepThresh(10.0, 10.0);
    CPhysicsEnv::getSingleton()->setSimulationQuality(1.0);

    mConTimer = CTimerAdvanced(20);
    mPhyTimer = CTimerAdvanced(20);
}

CSceneManager::~CSceneManager(){
    dJointGroupDestroy (contactgroup);
    dSpaceDestroy (space);
    dWorldDestroy (world);
}

void CSceneManager::registerObject(CSceneObject *o){

}

void CSceneManager::loadWorldMap(const string &name){

}

void CSceneManager::unloadWorldMap(){

}

void CSceneManager::setAmbientLight(const CVector3 &c){
    mAmbientLight = c;
}

CSceneNode* CSceneManager::getRootSceneNode(){
    return mRootSceneNode;
}

#define MAX_CONTACTS 128
typedef std::map<CRigidBody *, S32> ColMapSecond;
typedef std::map<CRigidBody *, ColMapSecond> ColMap;
ColMap collitions;

const S32 numpFrames = 10;
bool signalCollition(CRigidBody *r1, CRigidBody *r2){
    ColMap::iterator i;
    ColMapSecond::iterator j;
    bool ret = false;

    //if(r1 && !r1->getSuspendMultiCollitions())
    //	ret = true;

    i = collitions.find(r1);
    if(i == collitions.end()){
        collitions[r1][r2] = numpFrames;
        return true;
    }
    j = i->second.find(r2);
    if(j == i->second.end()){
        collitions[r1][r2] = numpFrames;
        return true;
    }
    collitions[r1][r2] = numpFrames;
    return ret;
}

//
// FIXME : WORLD/PHYSICS COORD SYSTEM MIXUP
//
static bool checkGround(dGeomID geom, const CVector3 &contactPoint){
    F32 bound[6];
    dGeomGetAABB(geom, bound);
    CAABoundBox AABound;
    F32 height = (bound[3] - bound[2]) * 0.1;
    AABound.setBounds(CVector3(bound[0], bound[2] - height, bound[4]), CVector3(bound[1], bound[2] + height, bound[5]));
    CVector3 point(TO_PHYSICS(contactPoint.x),TO_PHYSICS(contactPoint.y),TO_PHYSICS(contactPoint.z));
    if(AABound.collide(point)){
        return true;
    }
    return false;
}

dContact contact[MAX_CONTACTS];

static void colCallback(void *data, dGeomID o1, dGeomID o2){
    CSceneManager *sm = static_cast<CSceneManager *>(data);
    S32 ttt = 0;

    S32 i;
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    //if(!b1 && !b2)
    //	  return;
    if (b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)) return;

    CRigidBody *r1 = 0;
    CRigidBody *r2 = 0;

    r1 = static_cast<CRigidBody *>(dGeomGetData(o1));
    r2 = static_cast<CRigidBody *>(dGeomGetData(o2));

    if(r1 == r2)
        return;

    if(b1 && !b2 && !dBodyIsEnabled(b1))
        return;
    if(b2 && !b1 && !dBodyIsEnabled(b2))
        return;
    if(b1 && b2 && !dBodyIsEnabled(b1) && !dBodyIsEnabled(b2))
        return;

    if(r1 && r2 && (r1->isCollitionExcluded(*r2) || r2->isCollitionExcluded(*r1)))
        return;

	if (dGeomGetClass(o1) == dTriMeshClass && dGeomGetClass(o2) == dTriMeshClass){
		return;
	}
    
    CSurfaceResponse sr;
    if(r1)sr = r1->mSR;
    if(r2){
        sr.bounce += r2->mSR.bounce;
        sr.bounceCutoff += r2->mSR.bounceCutoff;
        sr.softness += r2->mSR.softness;
    }

    contact[0].surface.mode = dContactBounce | dContactSoftCFM;
    contact[0].surface.mu = dInfinity;
    contact[0].surface.mu2 = 0;
    contact[0].surface.bounce = sr.bounce;
    contact[0].surface.bounce_vel = sr.bounceCutoff;
    contact[0].surface.soft_cfm = sr.softness;

    dContact *con = &contact[1];
    while(con < &contact[MAX_CONTACTS / 4]){
        *con = contact[0];
        ++con;
        *con = contact[0];
        ++con;
        *con = contact[0];
        ++con;
        *con = contact[0];
        ++con;
    }

    if (S32 numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact))) {
            //gcon.printf("%d\n", numc);

            dMatrix3 RI;
            dRSetIdentity (RI);
            const dReal ss[3] = {0.02,0.02,0.02};

            if(!(r1->getCollisionMask() & r2->getContents()) || !(r2->getCollisionMask() & r1->getContents()))
                numc = 0;
            for (i=0; i < numc; ++i) {
                CVector3 pos(TO_WORLD(contact[i].geom.pos[0]), TO_WORLD(contact[i].geom.pos[1]),
                    TO_WORLD(contact[i].geom.pos[2]));

                dGeomID ray = 0;
                if (dGeomGetClass(o1) == dRayClass){
                    ray = o1;
                }
                if (dGeomGetClass(o2) == dRayClass){
                    ray = o2;
                }
                if(ray && false){
                    r1 = static_cast<CRigidBody *>(dGeomGetData(ray));
                    assert(r1);
                    CVector3 n(contact[i].geom.normal[0], contact[i].geom.normal[1],
                        contact[i].geom.normal[2]);
                    r1->setPosition(pos + n * 20);

                    CVector3 vel = r1->getVelocity();
                    F32 d = vel * n;
                    vel = vel - n * 2 * d;
                    r1->setVelocity(vel * 0.3);
                    //r1->setVelocity(CVector3::ZERO);
                } else {
                    if(checkGround(o1, pos))
                        r1->setGroundBody(r2);
                    if(checkGround(o2, pos))
                        r2->setGroundBody(r1);
                }

                dJointID c = dJointCreateContact(world, contactgroup, contact + i);
                dJointAttach (c,b1,b2);
            }

            if(signalCollition(r1, r2) && signalCollition(r2, r1)){
                assert(r1);
                assert(r2);
                if(r2->isSolid())
                    r1->onCollide(r2);
                if(r1->isSolid())
                    r2->onCollide(r1);
            } else {
                if(!r1->getSuspendMultiCollitions())
                    r1->onCollide(r2);
                if(!r2->getSuspendMultiCollitions())
                    r2->onCollide(r1);
            }

            if(!r1->isSolid())
                return;
            if(!r2->isSolid())
                return;


        }
}

void clearMap(){
    ColMap::iterator i,tempi;
    ColMapSecond::iterator j,tempj;

    for(i = collitions.begin() ; i != collitions.end() ;){
        for(j = i->second.begin() ; j != i->second.end() ;){
            S32 &b = j->second;
            if(!b){
                tempj = j;
                tempj++;
                i->second.erase(j);
                j = tempj;
                continue;
            }
            b--;
            ++j;
        }
        if(i->second.size() == 0){
            tempi = i;
            tempi++;
            collitions.erase(i);
            i = tempi;
            continue;
        }
        ++i;
    }
}

void CSceneManager::collideRigidBodies(){

}

static F64 timeLeft = 0.0;
static F64 accTime = 0.0;

void CSceneManager::addParticleSystem(CParticleSystem *p){
    mParticleSystems.push_back(p);
}

void CSceneManager::update(F64 time){
    CSceneObject *o = getRootSceneNode();
    CPhysicsEnv::RigidBodyList &bodies = CPhysicsEnv::getSingleton()->getRigidBodies();

    if(!CEngine::getSingleton()->isRunning()){
        CMThread::getSingleton()->update();
        return;
    }

    CPhysicsEnv::RigidBodyList::iterator i;
    for(i = bodies.begin(); i != bodies.end(); ++i){
       (*i)->setGroundBody(0);
    }

    

    S32 count = 0;
    F64 updateTime = CEngine::getSingleton()->getFrameTime();
    if(time > 0.0){
        timeLeft += time;
        F64 dt;
        dt = CPhysicsEnv::getSingleton()->getStepTime();
        CClient *client = CEngine::getSingleton()->getClient();
        CPhysicsEnv::RigidBodyList::iterator i;
		if(timeLeft > 1.0)timeLeft = 1.0;
        while(timeLeft > 0.0){
            
#if 1
            if(timeLeft < dt)
                break;
#else
            if(timeLeft < dt)
                dt = timeLeft;
#endif

            timeLeft -= dt;
            for(i = bodies.begin(); i != bodies.end(); ++i){
                (*i)->perStepUpdate(dt);
            }
			mConTimer.start();

			ParticleSystemList::iterator psi, psiEnd;
			psiEnd = mParticleSystems.end();
			for(psi = mParticleSystems.begin() ; psi != psiEnd ; ){
				if(!(*psi)->update(dt)){
					*psi = 0;
					psi = mParticleSystems.erase(psi);
					psiEnd = mParticleSystems.end(); // Just to be sure
				} else {
					++psi;
				}
			}

			dSpaceCollide(space, this, &colCallback);
			updateTime += dt;
            CEngine::getSingleton()->setWorldTime(accTime);
            mConTimer.stop();

            mPhyTimer.start();
            
            dWorldQuickStep(world, dt);
            //dWorldStepFast1(world, dt, 10);
            
            CEngine::getSingleton()->getClient()->updateActors(dt);
            mPhyTimer.stop();

            CMThread::getSingleton()->update();
            getRootSceneNode()->update(dt, true);

            accTime += dt;

            dJointGroupEmpty (contactgroup);
            count++;
        }
        //if(count == 0)
        //	client->updateActors(dt + dt);
    }
    mConTimer.reset();
    mPhyTimer.reset();
    clearMap();

    if(!count)
        return;
    
	CPhysicsEnv::RigidBodyList::iterator bodiesEnd = bodies.end();
    for(CPhysicsEnv::RigidBodyList::iterator i = bodies.begin(); i != bodiesEnd; ++i){
        CRigidBody *b = *i;
        //if(b->isDisabled())continue;
        b->update();
    }
    //getRootSceneNode()->update(time, true);
}

void CSceneManager::render(CRenderer *renderer){
    getRootSceneNode()->onRender(renderer);

    renderer->setRenderQueue(getRenderQueue());
    renderer->flushRenderQueue();
    getRenderQueue()->clear();
}

void CSceneManager::addClipPlane(const CPlane &p){
    mClipPlanes.push_back(p);
}

void CSceneManager::clearClipPlanes(){
    mClipPlanes.clear();
}

void CSceneManager::setActiveRenderQueue(CRenderQueue *rq){
    mActiveRenderQueue = rq;
}

CRenderQueue *CSceneManager::getActiveRenderQueue(){
    return mActiveRenderQueue;
}

CRenderQueue *CSceneManager::getRenderQueue(){
    return getActiveRenderQueue();
}

void CSceneManager::render(){
}

void CSceneManager::spatialize(CSceneObject *o){
}

bool CSceneManager::isCulled(CSceneObject *o){
    return false;
}

void CSceneManager::trace(CTraceResult *tr, CAABoundBox &bbox, const CVector3 &start, const CVector3 &end){
    dGeomID mTraceRay = 0;
    CVector3 delta = end - start;
    F32 len = delta.length();
    mTraceRay = dCreateRay(space, len);
    delta /= len;
    dGeomRaySet(mTraceRay, start.x, start.y, start.z, delta.x, delta.y, delta.z);
}


void CSceneManager::raytrace(CTraceResult *tr, const CVector3 &start, const CVector3 &end, CRigidBody *skip){

    CODETrace ot;
    CVector3 s(TO_PHYSICS(start)), e(TO_PHYSICS(end));
    dRayTrace(&s.x, &e.x, skip->getGeomID(), space, &ot);

    tr->frac = ot.mFrac;
    tr->mEnd = TO_WORLD(CVector3(ot.mEnd[0], ot.mEnd[1], ot.mEnd[2]));
    tr->plane.normal = CVector3(ot.mPlane[0], ot.mPlane[1], ot.mPlane[2]);
    if(ot.mGeomID){
        tr->body = static_cast<CRigidBody*>(dGeomGetData(ot.mGeomID));
    } else {
        tr->body = 0;
    }
}

void CSceneManager::renderPerformaceInfo(){
    char text[256];
    F32 avrFrameLength = 1.0f / CEngine::getSingleton()->getFPS();
    F32 perC = mConTimer.getAvrTime() / avrFrameLength * 100.0f;
    F32 perP = mPhyTimer.getAvrTime() / avrFrameLength * 100.0f;
    sprintf(text, "Contacts : %f (%4.2f%%)\nPhysics : %f (%4.2f%%)\n", mConTimer.getAvrTime(), perC, mPhyTimer.getAvrTime(), perP);
    CEngine::getSingleton()->getDefaultFont()->renderString(text);
}
