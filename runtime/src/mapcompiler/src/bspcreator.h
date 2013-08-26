

#ifndef _BSPCREATOR_H_
#define _BSPCREATOR_H_

#include <vector>
#include <list>

#ifdef MEMPROF
#include "mmgr.h"
#endif

#include "boundbox.h"
#include "filterer.h"
#include "portalcreator.h"
#include "floodgate.h"
#include "tjunctions.h"

using namespace std;

class CPlaneManager;
class CMapLoader;
class CPolygon;
class CBspTree;

class CBspCreator{
public:
	CBspCreator(bool forceDetail=false);
	~CBspCreator();    

	void buildBsp();

	void setForceDetail(bool forceD);
	void setPlaneManager(CPlaneManager *manager);
	CPlaneManager* getPlaneManager();
	void setMapLoader(CMapLoader *loader);
	CMapLoader *getMapLoader();

	list<CPolygon *>::iterator pBegin(){return polygons.begin();};
	list<CPolygon *>::iterator pEnd(){return polygons.end();};

	CPortalCreator &getPortalCreator();
	CFloodgate    &getFloogate();

	CBspTree *getBspTree();

	int getInitialPolygonsNo();
	CTJunctions &getTJ();
private:
	CPlaneManager *pManager;
	CMapLoader      *loader;
	CTJunctions     tj;

	list<CPolygon *> polygons;
	vector<CPolygon *> portalPolygons;
	//keep up the noPolygons before first BSP
	int initialPolygonsNo;
	//deletes all the polygons after the place toPolNo at the polygons vector
	void cleanUpPolygons(int toPoNo);

	CBspTree          *bspTree;

	//used for filtering things into tree
	CFilterer filterer;    
	CPortalCreator portalCreator;
	CFloodgate floodgate;

	bool forceDetailBrushes;

	void fixxTJunctions(list<CPolygon *> &polygons);   

};

#endif 

