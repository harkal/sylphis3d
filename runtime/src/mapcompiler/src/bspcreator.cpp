/***************************************************************************
    copyright            : (C) 2002 by Vassili Tenente
                           (C) 2002-2005 Harry Kalogirou (harkal@gmail.com)
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

#include <iostream>

#ifdef MEMPROF
#include "mmgr.h"
#endif

#include "bspcreator.h"
#include "entity.h"
#include "loader.h"
#include "polygon.h"
#include "bsptree.h"
#include "planemanager.h"
#include "filterer.h"
#include "tjunctions.h"

CBspCreator::CBspCreator(bool forceDetail){
	loader   = 0;
	pManager = 0;
	bspTree  = 0;
	forceDetailBrushes = forceDetail;
}

void CBspCreator::setPlaneManager(CPlaneManager *manager){
	pManager = manager;
}

CPlaneManager* CBspCreator::getPlaneManager(){
	return pManager;
}

void CBspCreator::setMapLoader(CMapLoader *mLoader){
	loader = mLoader;
}

CMapLoader *CBspCreator::getMapLoader(){
	return loader;
}


void CBspCreator::setForceDetail(bool forceD){
	forceDetailBrushes = forceD;
}

void CBspCreator::buildBsp(){
	if(!(loader&&pManager))
		return;
	if(!bspTree)
		bspTree = new CBspTree();

	list<CEntity *>::iterator eIter, eEnd;

	CEntity *worldEntity = loader->getEntity("classname", "worldspawn");

	list<CEntity *> *groupEntities = loader->getListWithEntities("classname", "func_group");
	if(groupEntities){
		for(eIter = groupEntities->begin() ; eIter != groupEntities->end() ; ++eIter){
			vector<CBrush>::iterator i;
			for(i = (*eIter)->bBegin() ; i != (*eIter)->bEnd() ; ++i){
				list<CBrushPlane>::iterator j;
				for(j = i->planesBegin() ; j != i->planesEnd() ; ++j){
					j->setBrushId(i->getBrushId());
				}
				worldEntity->addBrush(*i);
			}
			loader->deleteEntity(*eIter);
		}
	}


	list<CEntity *> *portalEntities = loader->getListWithEntities("classname", "portal");
	int noPortals = 1;
	list<CPolygon *> portalP;

	if(portalEntities){

		eIter = portalEntities->begin();
		eEnd = portalEntities->end();
		//we begin with one since 0 means no portal

		CPolygon *cp;
		while(eIter!=eEnd){
			(*eIter)->callCreatePolygons(); //now here are all the polygons of this entity
			//we just have to pick up the larger.
			cp = (*eIter)->getLargerPolygon();
			//and make it portal (its shaderFlags)
			//cout << "before setting portal shader"<<endl;
			if(!cp){
				++eIter;
				cout << "warning: portal with no brushes"<<endl;
				continue;
			}
			//cout << "noPortals="<<noPortals<<endl;
			CShaderFlags *newShaderFlags = new CShaderFlags(true,true,false,false,noPortals);
			CShaderFlags *giverShader = cp->getBrushPlane()->getShader();
			newShaderFlags->setPrior(giverShader->isPrior());
			//newShaderFlags->printFlags();
			++noPortals;
			newShaderFlags->setShaderId(0);
			cp->getBrushPlane()->setShader( newShaderFlags );

			//have it bounded     
			cp->boundPolygon(); 

			//polygons.push_back(cp);      
			//just add it to portals too to have it to check later the bounds with
			//all the portals , pass it to portalCreator
			//this must stay push_back !!!
			portalP.push_back(cp);

			++eIter;
		}
	}

	list<CPolygon *>::iterator portalIter, portalEnd;
	portalIter = portalP.begin();
	portalEnd = portalP.end();
	portalPolygons = vector<CPolygon*>(portalP.size());
	int i=0;
	while(portalIter!=portalEnd){
		portalPolygons[i] = *portalIter;
		++i;
		++portalIter;
	}
	cout << (noPortals-1) << " valid portals "<<endl;

	worldEntity->callCreatePolygons();
	worldEntity->appendStructural(&polygons);

	//inititalize the bspTree
	bspTree->setInitialValues(&polygons, pManager);
	initialPolygonsNo = polygons.size();

	cout << "=======================BSP 1==========================="<<endl;
	cout << initialPolygonsNo << " will be passed to BSP 1"<<endl;
	//build The first BSP
	bspTree->buildFirstBsp();

	//set the initial values to the filterer
	filterer.setInitialValues(pManager, bspTree->getRoot(),loader->getWorldEntity(), &portalPolygons, noPortals-1 );    
	filterer.doFilteringForFirstBsp();

	cout << "calculating portals..."<<endl;
	portalCreator.setInitialValues(bspTree->getRoot(), pManager->countPlanes());
	portalCreator.calcPortalsForFirstBsp();

	cout << "flood fill (leak check)...";

	CVector3 startPotition = loader->getPlayerStart()->getAsVector("origin");
	vector<CBspLeaf *> inLeafs;
	inLeafs.push_back(filterer.filterPointIntoTree(startPotition)->data);

	list<CEntity*> &lights = loader->getEntities();

    list<CEntity*>::iterator li;
    for(li = lights.begin() ; li != lights.end() ; ++li){
		CVector3 temp = (*li)->getAsVector("origin") + (*li)->getAsVector("light_center");
		inLeafs.push_back(filterer.filterPointIntoTree(temp)->data);
	}

	floodgate.setInitialValues(inLeafs, portalPolygons, loader->getFilePath(), startPotition);

	if(!floodgate.firstBspFloodPlace()){
		cout << "\n############### !!! LEAK !!!! ##############" << endl;
		exit(1);
	}

	cout<<" NO leak"<<endl;

	portalCreator.freePortals();

	if(forceDetailBrushes){
		cout << "forcing detail brushes" << endl;    
		filterer.forceDetailBrushes();
	}
	polygons.clear();

	worldEntity->appendStructuralAndVisible(&polygons);
	cout << polygons.size() << " polygons result for BSP 1" << endl;    

    ///
	///  SECOND BSP  ============================================================
    ///

	for(int i=0;i<(noPortals-1);i++){
		polygons.push_front(portalPolygons[i]);
	}

	bspTree->setInitialValues(&polygons, pManager);
	initialPolygonsNo = polygons.size();
	cout << "=======================BSP 2===========================" << endl;    
	cout << initialPolygonsNo << " will be passed to BSP 2 (" << noPortals << " portal polygons)" << endl;
	bspTree->buildSecondBsp();

	//set the initial values to the filterer
	filterer.setInitialValues(pManager, bspTree->getRoot(),loader->getWorldEntity(), &portalPolygons, noPortals-1);    
	filterer.filterFor2OnlyStructural();

	cout << "calculating portals..." << endl;
	portalCreator.setInitialValues(bspTree->getRoot(), pManager->countPlanes());
	portalCreator.calcPortalsForSecondBsp();
	cout << "filtering portals..." << endl;
	filterer.filterPortalsIntoTree();

	cout << "calculating areas..." << endl;

	//EDO PREPEI NA VRETHOUN TA KAINOURIA inLeafs !!! to see
	inLeafs[0] = filterer.filterPointIntoTree(startPotition)->data;
	floodgate.setInitialValues(inLeafs, portalPolygons, loader->getFilePath(), startPotition);        
	floodgate.secondBspFloodPlace();
    cout << floodgate.getAreaNo() << " AREAS FOUND" << endl;    

	filterer.filterFor2OnlyDetail();
    cout << "merging details..." << endl;
	filterer.findDetailAndMerge();

	cout << "calculating area portals ..." << endl;
	portalCreator.findTheAreaLeafs( floodgate.getAreaNo() );
	floodgate.addAreaPortalsToAreas(portalCreator.getAreas());

	//cout << "create vertices graph"<<endl;
}

void CBspCreator::cleanUpPolygons(int toPolNo){
	list<CPolygon *>::iterator piter = polygons.begin();
	list<CPolygon *>::iterator pend = polygons.end();    
	int i=0;
	while(piter!=pend){
		if(i<toPolNo){
			++piter;
			++i;
		}
		else{
			delete *piter;
			piter = polygons.erase(piter); 
			--piter;     
		}
		++piter;

	}
}

CPortalCreator &CBspCreator::getPortalCreator(){ 
	return portalCreator;
}    

CFloodgate &CBspCreator::getFloogate(){
	return floodgate; 
}

CBspTree *CBspCreator::getBspTree(){ 
	return bspTree; 
}

int CBspCreator::getInitialPolygonsNo(){ 
	return initialPolygonsNo; 
}

CTJunctions &CBspCreator::getTJ(){
	return tj;
}

CBspCreator::~CBspCreator(){
}



