/***************************************************************************
    copyright            : (C) 2003 by Vassili Tenente
                           (C) 2003-2005 Harry Kalogirou (harkal@gmail.com)
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

#include "filterer.h"
#include "polygon.h"
#include "brush.h"
#include "entity.h"
#include "bspnode.h"
#include "planemanager.h"
#include "brushplane.h"

CFilterer::CFilterer(){
	writeBounds=false;
	firstBsp = true;
	defaultShaderFlags = CShaderFlags(1,1,false,false,0);
	defaultShaderFlags.setShaderId(0);
}

void CFilterer::setInitialValues(CPlaneManager *pM, 
								 CBTree<CBspNode *>::Node *rNode, 
								 CEntity *we, vector<CPolygon *> *pPolygons, const int polNo)
{
	pManager    = pM;
	rootNode    = rNode;
	worldEntity = we;

	worldBounds = rootNode->data->getBounds();
	portalPolygons = pPolygons;
	portalPNo = polNo;

	worldMin = worldBounds.getMin() + CVector3(20, 20, 20);
	worldMax = worldBounds.getMax()+ CVector3(20, 20, 20);  
}

CPlane::eSide CFilterer::splitBrush(CBrush *brushToSplit, CBrush ** front, CBrush ** back, CBrushPlane *splitBrushPlane)
{
	float d, d_front, d_back;

	// check all points
	d_front = d_back = 0.0f;

	list < CBrushPlane >::iterator iter, bEnd;
	vector < CVertex >::iterator vIter, vEnd;

	CPlane *plane = splitBrushPlane->getPlane();
	bEnd = brushToSplit->planesEnd();
	CPolygon *polygon;
	for (iter=brushToSplit->planesBegin();iter != bEnd; iter++) {
		polygon = iter->getPolygon();
		if (!polygon)
			continue;
		vEnd = polygon->vEnd();
		for (vIter=polygon->vBegin(); vIter!=vEnd; vIter++) {
			d = plane->distanseFromVertex(*vIter);  /*the polygon of the brushplane */
			if (d > 0 && d > d_front)
				d_front = d;
			if (d < 0 && d < d_back)
				d_back = d;
		}
	}

	if (d_back > -PLANES_ON_EPSILON)            // PLANESIDE_EPSILON)
	{                           // only on front
		return CPlane::FRONT;
	}
	if (d_front < PLANES_ON_EPSILON)            // PLANESIDE_EPSILON)
	{                           // only on back
		return CPlane::BACK;
	}
	brushSplits++;

	CPolygon *p = new CPolygon();
	*p = CPolygon::hugePolygon(*plane);
	p->setBrushPlane(splitBrushPlane);
	CPlane *plane2;
	iter=brushToSplit->planesBegin();

	for (;iter!=bEnd; iter++) {
		plane2 = iter->getPlane();
		p->splitIt(plane2, 0.0f);   //0.0 is the EPSILON
	}

	if (p->isTooSmall()) {      // the brush isn't really split
		delete p;
		CPlane::eSide side;

		side = brushMostlyOnSide(brushToSplit, plane);

		if (side == CPlane::FRONT) {
			return CPlane::FRONT;
		}
		if (side == CPlane::BACK) {
			return CPlane::BACK;
		}
		assert(false);
	}

	CPolygon *frontP, *backP;
	*front = new CBrush(brushToSplit);
	*back = new CBrush(brushToSplit);

	brushesBuffer.push_back(*front);
	brushesBuffer.push_back(*back);

	iter=brushToSplit->planesBegin();
	for (;iter != bEnd; iter++) {
		polygon = iter->getPolygon();
		if (!polygon )
			continue;

		frontP = new CPolygon();
		backP = new CPolygon();
		//
		CPlane::eSide side =
			polygon->splitByPlane(plane, frontP,
			backP, 0.0f);
		if (side == CPlane::CROSS) {
			(*front)->addBrushPlane(&(*iter), new CPolygon(frontP->getVertices()));
			delete frontP;
			(*back)->addBrushPlane(&(*iter), new CPolygon(backP->getVertices()));            
			delete backP;
		} else if (side == CPlane::FRONT) {
			delete backP;
			(*front)->addBrushPlane(&(*iter), new CPolygon(polygon->getVertices()));            
			//edot thelei ftiaksimo me ta *bounds na ginei ksana calc to bound  tou polygon
			delete frontP;
			//polygon->setPolBounds(0);
		} else if (side == CPlane::BACK) {
			delete frontP;
			(*back)->addBrushPlane(&(*iter), new CPolygon(polygon->getVertices()));                        
			delete backP;
			//polygon->setPolBounds(0);
		}
		else{
			delete frontP;
			delete backP;
			assert(false);
		}
	}
#ifdef DEBUG
	cout << "all polygons at each brushPlane of brush were splitted" << endl;
#endif
	// see if we have valid polygons on both sides
	bool frontS = true;
	bool backS = true;

	(*front)->boundBrush();
	(*back)->boundBrush();

	const CAABoundBox &frontBounds = (*front)->getBounds();
	const CAABoundBox &backBounds = (*back)->getBounds();

		CVector3 frontMin = (*front)->getBounds().getMin();
		CVector3 frontMax = (*front)->getBounds().getMax();
		for (int j = 0; j < 3; j++) {
			if (frontMin[j] < worldMin[j] || frontMax[j] > worldMax[j]
			|| (*front)->getBrushPlanesNo() < 2) {
				frontS = false;
			}
		}

		CVector3 backMin = (*back)->getBounds().getMin();
		CVector3 backMax = (*back)->getBounds().getMax();
		for (int j = 0; j < 3; j++) {
			if (backMin[j] < worldMin[j]||backMax[j] > worldMax[j]
			|| (*back)->getBrushPlanesNo() < 2) {
				backS = false;
			}
		}

#ifdef DEBUG
	cout << "did not found out bounds" << endl;
#endif

	// add the midwinding to both sides (p is the midwinding)
	CBrushPlane bpInversed;
	bpInversed.setShader(&defaultShaderFlags);
	bpInversed.setPlane(pManager->inversed(plane));

	(*front)->addBrushPlane(bpInversed, new CPolygon(p->getVertices()));

	CBrushPlane bpNotInversed;
	bpNotInversed.setShader(&defaultShaderFlags);
	bpNotInversed.setPlane(plane);
	(*back)->addBrushPlane(bpNotInversed, new CPolygon(p->getVertices()));

	delete p;
#ifdef DEBUG
	cout << "middle was added to both" << endl;
#endif

	frontS = true;
	backS = true;
	if ((*front)->volume() < VOLUME_EPSILON)
		frontS = false;
	if ((*back)->volume() < VOLUME_EPSILON)
		backS = false;
	if (!(frontS && backS)) {
		tinyBrushes++;
		if (frontS) {
			return CPlane::FRONT;
		}
		if (backS) {
			return CPlane::BACK;
		}
		CPlane::eSide side;
		side = brushMostlyOnSide(brushToSplit, plane);
		if (side == CPlane::FRONT) {
			return CPlane::FRONT;
		}
		if (side == CPlane::BACK) {
			return CPlane::BACK;
		}

		assert(false);          /*the first was not tiny both of them are tiny , this certainly should not happen.(If happens will see...) */
		return CPlane::CROSS;    /*THIS SHOULD NEVER HAPPEN */
	}
#ifdef DEBUG
	cout << "no one had small volume SPLITTED SUCCEED" << endl;
#endif
	//return SPLITTED BY BOTH SIDES

#ifdef DEBUG
	cout << "front min bounds are :"<<frontBounds->getMin()[0]<<" "<<frontBounds->getMin()[1]<<"  "<<frontBounds->getMin()[2]<<endl;
	cout << "front max bounds are :"<<frontBounds->getMax()[0]<<" "<<frontBounds->getMax()[1]<<"  "<<frontBounds->getMax()[2]<<endl;   
	cout << "back min bounds are :"<<backBounds->getMin()[0]<<" "<<backBounds->getMin()[1]<<"  "<<backBounds->getMin()[2]<<endl;
	cout << "back max bounds are :"<<backBounds->getMax()[0]<<" "<<backBounds->getMax()[1]<<"  "<<backBounds->getMax()[2]<<endl;   
#endif

	return CPlane::ON;
}

CPlane::eSide CFilterer::brushMostlyOnSide(CBrush * brush, CPlane *plane)
{
	float d, max;
	CPlane::eSide side;
	list < CBrushPlane >::iterator iter, bEnd;
	vector < CVertex >::iterator vIter, vEnd;
	max = 0;
	side = CPlane::FRONT;
	bEnd = brush->planesEnd();
	CPolygon *pol;
	iter =brush->planesBegin();
	for (; iter != bEnd; iter++) {
		pol = iter->getPolygon();
		if (!pol)
			continue;
		vIter = pol->vBegin();
		vEnd = pol->vEnd();

		for (;vIter!=vEnd;vIter++) {
			d = plane->distanseFromVertex(*vIter);
			if (d > max) {
				max = d;
				side = CPlane::FRONT;
			}
			if (-d > max) {
				max = -d;
				side = CPlane::BACK;
			}
		}
	}
	return side;
}

void CFilterer::findDetailAndMerge(){
	mergeDetails(rootNode);
}

void CFilterer::mergeDetails(CBTree<CBspNode *>::Node *node){
    if(!node->isLeaf())	{
        mergeDetails(node->child(0));
        mergeDetails(node->child(1));
        return;
    }

    //NOW LEAF
    CBspLeaf *leaf = (CBspLeaf *)node->data;
    if(!leaf->getArea())
        return;
    vector<CBrush *>::iterator i, e;
    list < CBrushPlane >::iterator dpi, dpe;
    CPolygon *detailP, structuralP;

    e=leaf->bEnd();
    CBrush *db;//detail part brush
    //VertsVector::iterator v, ve;
    vector<CBrush *> allNb; //these are these and neigbor brushes
    vector<CBspLeaf *>::iterator li, le;
    vector<CBrush *>::iterator brushI, brushE;
    li=leaf->oLeafsBegin();
    le=leaf->oLeafsEnd();
    while(li!=le){
        (*li)->appendYourBrushes(allNb, worldEntity);
        li++;
    }
    leaf->appendYourBrushes(allNb, worldEntity);

    brushE = allNb.end();

    bool isAllInBrush;

    // this holds brushes which contains at least one vertex of a poly
    // but not all vertices
    list<CBrush *> brushesIn;

    list<CPolygon *>::iterator pi, pe;
    pi=leaf->polygonsBegin();
    pe=leaf->polygonsEnd();
    while(pi!=pe){
        if(!(*pi)->isKeep() || (*pi)->isLeak()){
            ++pi;
            continue;
        }
        for(brushI=allNb.begin(); brushI!=brushE;++brushI){
            assert((*brushI)->getBrushId()!=-1);
            assert((*pi)->getBrushId()!=-1);
            /*
            if((*brushI)->getBrushId() == (*i)->getBrushId())//if is the brush of this polygon do nothing
            continue;
            */ 
            if((*brushI)->getBrushId() == (*pi)->getBrushId())
                continue;
            if((*brushI)->isOpaque()){
                if((*brushI)->subPolygon(*pi)){
                    leaf->erasePolygon(pi);
                    break;
                }
            }
        }
        ++pi;
    }

    for(i=leaf->bBegin();i!=e;++i){
        brushesIn.clear();
        db  = *i;
        dpi = db->planesBegin();
        dpe = db->planesEnd();
        for( ; dpi != dpe ; ++dpi ){
            detailP = dpi->getPolygon();
            if(!detailP->isKeep())
                continue;

            isAllInBrush=false;
            for(brushI=allNb.begin(); brushI!=brushE;++brushI){
                assert((*brushI)->getBrushId()!=-1);
                assert((*i)->getBrushId()!=-1);
                if((*brushI)->getBrushId() == (*i)->getBrushId())//if is the brush of this polygon do nothing
                    continue;

                if((*brushI)->isOpaque()){
                    isAllInBrush = (*brushI)->subPolygon(detailP);
                    if(isAllInBrush)
                        break;
                }
            }

            if(isAllInBrush)
                continue;
            detailP->merge();
        }
    }
}

//CBrush filteringBrush;
int CFilterer::filterBrushIntoTree(CBrush * brush, CBTree<CBspNode *>::Node * node)
{

	//    if(brush.volume(*planes) < 1.0f || brush.getBrushPlanesNo()<4 ){
	//       return 0;
	//    }

	// All nodes either have 2 childs or no childs at all, in witch
	// the node is actualy a leaf - Harry
	//just for debugging
	bool hasLeft, hasRight;
	if (node->child(0))
		hasLeft = true;
	else
		hasLeft = false;
	if (node->child(1))
		hasRight = true;
	else
		hasRight = false;
	assert(!(hasRight ^ hasLeft)); //just for debuging

	if (node->isLeaf()) {            
		CBspLeaf *treeLeaf = (CBspLeaf *)node->data;      
		//here the brushBounds      
		// classify the leaf by the structural brush
		if ( filteringBrush->isStructural() ) {
			//treeLeaf->addBrush(brush, false);
			treeLeaf->addStructuralBrush(brush, filteringBrush->getBrushId());
			brush->setReachLeaf(true);
			if(!firstBsp)
				filteringBrush->setReachLeaf(true);

			if (filteringBrush->isOpaque() && !treeLeaf->isOpaque()) {
				opaqueNo++;
				treeLeaf->setOpaque(true);
				//node->areaportal = qfalse;
#ifdef DEBUG
				cout << "OPAQUE LEAF " << cp << endl;
#endif
			}
		}
		//NEW CODE AFTER 90 //FOR NON STRUCTURAL BRUSHES
		else{
			//assert(false);
			if(treeLeaf->isOpaque())
				return 0;

			brush->setReachLeaf(true);
			//treeLeaf->addDetailBrush(brush, filteringBrush->getBrushId(), writeBounds);        
			treeLeaf->addDetailBrush(brush, filteringBrush->getBrushId(), writeBounds);        
		}
		//NEW CODE AFTER 90 END
		return 1;
	}
	// split it by the node plane
	CBrush *front, *back;        /*this true is to clear with the memory */
	CPlane::eSide side =
		splitBrush(brush, &front, &back, node->data->getSplitBrushPlane());

	int c = 0;
	if (side == CPlane::ON) {
		c += filterBrushIntoTree(front, node->child(0));
		c += filterBrushIntoTree(back, node->child(1));        
	} else if (side == CPlane::FRONT) {
		c += filterBrushIntoTree(brush, node->child(0));
	} else if (side == CPlane::BACK) {
		c += filterBrushIntoTree(brush, node->child(1));
	} else if (side == CPlane::CROSS)
		assert(0 == "THe side was Plane::CROSS");

	return c;
}


/*void CFilterer::clearReachLeafBrushesBuffer(){
list<CBrush *>::iterator biter = reachLeafBrushes.begin();
list<CBrush *>::iterator bend = reachLeafBrushes.end();
while(biter!=bend){
(*biter)->freeIt();
delete *biter;
reachLeafBrushes.erase(biter);
--biter;
++biter;
}

assert(reachLeafBrushes.size()==0);
}*/

void CFilterer::clearBrushesBuffer(){
	list<CBrush *>::iterator biter = brushesBuffer.begin();
	list<CBrush *>::iterator bend = brushesBuffer.end();
	while(biter!=bend){
		if(!(*biter)->reachLeaf()){
			(*biter)->freeIt();
			delete *biter;
			biter = brushesBuffer.erase(biter);
			--biter;      
		}
		//else{
		//  reachLeafBrushes.push_back(*bIter);
		// }

		++biter;
	}

	// assert(brushesBuffer.size()==0);
}


void CFilterer::forceDetailBrushes()
{
	list<CBrushPlane>::iterator pIter, pEnd;

	bool forceItDetail;
	CBrush *brush;
	int brushesMadeDetail = 0;
	for(int i=0;i<worldEntity->getBrushesNo();i++) {
		brush = &worldEntity->getBrush(i);
		if(!brush->isStructural())
			continue;
		forceItDetail = true;
		pIter = brush->planesBegin();
		pEnd = brush->planesEnd();

		while(pIter!=pEnd){
			if(!pIter->canBeDetail()){
				forceItDetail = false;
				break;
			}
			++pIter;
		}

		if(forceItDetail){
			brush->setStructural(false);
			brushesMadeDetail++;
		}
	}
	cout << brushesMadeDetail<<" forced detail"<<endl;
}

void CFilterer::filterBrushes(bool type)
{
	if(type == STRUCTURAL)
		cout << "filter structural brushes"<<endl;
	else if(type == DETAIL)
		cout << "filter detail brushes"<<endl;
	int r;
	int c_unique, c_clusters;
	vector < CBrush >::iterator iter, bend;

	opaqueNo = 0;
	brushSplits = 0;
	tinyBrushes = 0;

	c_unique = 0;
	c_clusters = 0;

	iter=worldEntity->bBegin();    
	bend=worldEntity->bEnd();
	bool intype = !type;
	for ( ;iter != bend; iter++ ) {
		if( iter->isStructural() == intype )
			continue;

		c_unique++;

		//set which brush is filtered //must
		filteringBrush = &*iter;
		r = filterBrushIntoTree(filteringBrush, rootNode);
		clearBrushesBuffer();
		c_clusters += r;

		/*if (r)
		iter->visible=true;
		else
		iter->visible=false;*/
	}

	if( type == STRUCTURAL )
		cout << c_unique << " structural brushes " << endl;
	else if( type == DETAIL )
		cout << c_unique << " detail brushes " << endl;

	cout << brushSplits << " brush splits " << endl;

	if(tinyBrushes)
		cout <<"WARNING! "<< tinyBrushes << " tiny brushes generated" << endl;    
}


void CFilterer::filterFor2OnlyStructural(){
	writeBounds = true;
	firstBsp = false;
	filterBrushes(STRUCTURAL);
}

void CFilterer::filterFor2OnlyDetail(){
	writeBounds = true;
	firstBsp = false;
	filterBrushes(DETAIL);
}

void CFilterer::doFilteringForFirstBsp(){
	writeBounds = false;  
	firstBsp = true;

	filterBrushes(STRUCTURAL);
}

void CFilterer::doFilteringForSecondBsp(){
	writeBounds = true;
	firstBsp = false;
	filterBrushes(STRUCTURAL);
	filterBrushes(DETAIL);
}


CBTree<CBspLeaf *>::Node *CFilterer::filterPointIntoTree(CVector3 &point){
	float dist;
	CBTree<CBspNode *>::Node *node;
	CBspNode *nodedata;

	node = rootNode;
	CPlane *sPlane;
	while (true) {
		if (node->isLeaf()) {
			return (CBTree<CBspLeaf *>::Node *)node;
		}
		nodedata = node->data;
		sPlane = nodedata->getSplitPlane();
		dist = sPlane->distanseFromVertex(point);
		//returnP.push_back( current.devider );
		if (dist >= 0) {
			node = node->child(0);
			continue;
		} else {
			node = node->child(1);
			continue;
		}
		assert(false);
	}
	return 0;
}



void CFilterer::filterPortalIntoTree(CBTree<CBspNode *>::Node *node, CPolygon *portal){    
	if(node->isLeaf()){
		CBspLeaf *leaf = (CBspLeaf *)node->data;
		return;
	}

	CBspNode *sNode = node->data;
	CPlane *splitPlane =sNode->getSplitPlane();

	CPlane::eSide side = portal->planeSide(*splitPlane);

	if(side == CPlane::FRONT){
		filterPortalIntoTree(node->child(0), portal);
	} 
	else if(side == CPlane::BACK){
		filterPortalIntoTree(node->child(1), portal);
	}
	else if(side == CPlane::ON){   
		filterPortalIntoTree(node->child(0), portal);
		filterPortalIntoTree(node->child(1), portal);    

		//int splaneId = splitPlane->getPlaneId();
		//int pplaneId = portal->getPlane()->getPlaneId();
		//if(splaneId==pplaneId)
		//  filterPortalIntoTree(node->child(0), portal);
		//else if(splaneId==pplaneId ^ 1)
		// filterPortalIntoTree(node->child(1), portal);
	}
	else{
		CPolygon *front, *back;
		front = new CPolygon();
		back = new CPolygon();

		side = portal->splitByPlane(splitPlane, front, back, 0.0f);
		assert(side == CPlane::CROSS);
		assert(!(front->isTooSmall() && back->isTooSmall()));
		if(!front->isTooSmall()){
			filterPortalIntoTree(node->child(0), front);
		}    
		if(!back->isTooSmall()){
			filterPortalIntoTree(node->child(1), back);
		}

		delete front;
		delete back;
	}

}

void CFilterer::filterPortalsIntoTree(){
    return;
	for(int i=0;i<portalPNo;i++){
		portalCurrentlyFilter = i;
		CPolygon pol((*portalPolygons)[i]->getVertices());
		filterPortalIntoTree(rootNode, &pol);
	}    
}

