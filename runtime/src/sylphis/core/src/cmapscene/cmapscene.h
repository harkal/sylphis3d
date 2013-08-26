/***************************************************************************
                          cmapscene.h  -  description
                             -------------------
    begin                : Wed Jun 4 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#ifndef CMAPSCENE_H
#define CMAPSCENE_H

#include <set>
#include <list>
#include <map>
#include <scenemanager.h>
#include <shadowvolume.h>
#include <timer.h>
#include "cmap.h"

class CRenderer;
class CFrustum;
class CLight;
class CModel;

SmartPointer(CSMFModel);

#define MAX_MAP_FACES   65536

class CCMapScene : public CSceneManager  {
public: 
	CCMapScene();
	virtual ~CCMapScene();

	virtual void registerObject(CSceneObject *o);

	virtual void loadWorldMap(const string &name);

    virtual void unloadWorldMap();

    virtual void render(CRenderer *renderer);

	virtual void spatialize(CSceneObject *o);

	virtual bool isCulled(CSceneObject *o);

	virtual void collideRigidBodies();

	virtual CModel &getWorldMapModel(S32 n);

    virtual S32 getAreaPortal(const CVector3 &at) const;
    virtual void setAreaPortalOpen(S32 portalId, bool o);

    virtual void trace(CTraceResult *tr, CAABoundBox &bbox, const CVector3 &start,
                    const CVector3 &end);

    virtual void makeLightStatic(CLight *l);

	virtual void renderPerformaceInfo();

	void renderSky();
    
	typedef std::list<CSceneObject*> AreaObjectsSet;
	typedef std::set<S32> ObjectAreasSet;
    typedef std::map<CLight*, CShadowVolume> LightShadowMap;

private:
    typedef struct {
        CVector3 origin;
        S32 face;
        S32 sortkey;
    } rendface_t;

    typedef struct {
        S32 numfaces;
        rendface_t faces[16000];
    } facelist_t;

	CSMFModelPtr mSkyModel;
	
    std::vector<AreaObjectsSet> mAreaObjects;

	ObjectAreasSet *getObjectAreas(CSceneObject *o);

	CMAP *mCMAP;
    LightShadowMap mStaticLights;
    CRenderer *mRenderer;
    CFrustum *mFrustum;
	CVector3 *mOrigin;

	CLightPtr mSunLight;

	enum ECullType {
		FRUSTUM,
		BOUND
	};

	bool mCullType;
	CBound *mCullBound;

    char mFacesSeen[MAX_MAP_FACES];
    S32 faceSeenThreshold;
    bool mAreaVis[MAX_MAP_AREAS];
    void checkAreas_r(tArea *area, tArea *lastArea = 0);
    void filterFacesInRadius_r(S32 nodeNo, CLight &l);
    void filterFacesInRadius(CLight &l);
    void findSeenFacesPortals(CVector3 &o);
    static S32 face_cmp(const void *a, const void *b);
    void addAreaLeafs(tArea *area);
    void addLeafFaces(cleaf_t *leaf);
    void addBrushFaces(CVector3 &v, S32 firstFace, S32 numFaces, F32 radius);

    void renderWorld(CVector3 &o, facelist_t &faceList);
    void renderShadowVolume(CLight *l);
	void parseEntities();

    void pushFace(dface_t *face);

    facelist_t mFaceList;
    facelist_t mFaceListInRadius;

    // Performance monitoring
    CTimerAdvanced mSVTimer;
    CTimerAdvanced mPolyTimer;
	CTimerAdvanced mVisTimer;
};

#endif
