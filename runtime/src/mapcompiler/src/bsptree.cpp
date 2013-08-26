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
#include "bspnode.h"
#include "bsptree.h"
#include "polygon.h"
#include "planemanager.h"
#include "shader.h"

#ifdef MEMPROF
#include "mmgr.h"
#endif


CBspTree::CBspTree(){
	polygons = 0;
}

CBTree<CBspNode *>::Node *CBspTree::getRoot(){
	return bTree.rootNode;
};

void CBspTree::setInitialValues(list<CPolygon *> *pol, CPlaneManager *planeManager){
	polygons          = pol;
	pManager = planeManager;
};

void CBspTree::reset(){
	if(!bTree.isEmpty())
		freeTree(getRoot());
	bTree.reset();
	bTree.init();
};

void CBspTree::freeTree(CBTree<CBspNode *>::Node *node){
	delete node->data;
	if(node->isLeaf()){
		return;
	}
	freeTree(node->child(0));
	freeTree(node->child(1));  
}

void CBspTree::buildFirstBsp()
{
	firstBsp = true;
	assert(polygons);

	reset();
	CBspNode *firstNode = new CBspNode();
	firstNode->addPolygons(polygons);
	///
	CAABoundBox &bbox = firstNode->getBounds();
	CVector3 max, min;
	max = bbox.getMax()+CVector3(20,20,20);
	min = bbox.getMin()-CVector3(20,20,20);

	CPolygon *maxPoly, *minPoly;
	CVector3 normalMax, normalMin;
	CShaderFlags *leakFlags = new CShaderFlags(0,1,0,0,0);
	leakFlags->setLeak(true);
	CBrushPlane *leakBrushPlane;

	for(int i=0;i<3;i++){
		normalMax = CVector3(0,0,0);
		normalMin = CVector3(0,0,0);
		normalMax[i] = -1;
		normalMin[i] = 1;

		leakBrushPlane = new CBrushPlane();
		leakBrushPlane->setShader(leakFlags);
		leakBrushPlane->setPlane(pManager->getPlane(pManager->addPlane(-max[i],normalMax)));
		maxPoly = new CPolygon(CPolygon::hugePolygon(*leakBrushPlane->getPlane()));
		leakBrushPlane->setPolygon(maxPoly);

		leakBrushPlane = new CBrushPlane();
		leakBrushPlane->setShader(leakFlags);
		leakBrushPlane->setPlane(pManager->getPlane(pManager->addPlane(min[i],normalMin)));      
		minPoly = new CPolygon(CPolygon::hugePolygon(*leakBrushPlane->getPlane()));
		leakBrushPlane->setPolygon(minPoly);

		polygons->push_back(maxPoly);
		polygons->push_back(minPoly);
		firstNode->addPolygon(maxPoly);
		firstNode->addPolygon(minPoly);      
	}
	///
	selectedPlanes.resize(pManager->countPlanes(), false);

	insertToTree(firstNode, bTree.rootNode);    
}


void CBspTree::buildSecondBsp(){
	firstBsp = false;
	assert(polygons);
	assert(pManager->countPlanes());

	reset();
	CBspNode *firstNode = new CBspNode();
	firstNode->addPolygons(polygons);
	///
	CAABoundBox &bbox = firstNode->getBounds();
	CVector3 max, min;
	max = bbox.getMax() + CVector3(20,20,20);
	min = bbox.getMin() - CVector3(20,20,20);

	CPolygon *maxPoly, *minPoly;
	CVector3 normalMax, normalMin;
	CShaderFlags *leakFlags = new CShaderFlags(0,1,0,0,0);
	leakFlags->setLeak(true);
	CBrushPlane *leakBrushPlane;

	for(int i=0;i<3;i++){
		normalMax = CVector3(0,0,0);
		normalMin = CVector3(0,0,0);
		normalMax[i] = -1;
		normalMin[i] = 1;

		leakBrushPlane = new CBrushPlane();
		leakBrushPlane->setShader(leakFlags);
		leakBrushPlane->setPlane(pManager->getPlane(pManager->addPlane(-max[i],normalMax)));
		maxPoly = new CPolygon(CPolygon::hugePolygon(*leakBrushPlane->getPlane()));
		leakBrushPlane->setPolygon(maxPoly);

		leakBrushPlane = new CBrushPlane();
		leakBrushPlane->setShader(leakFlags);
		leakBrushPlane->setPlane(pManager->getPlane(pManager->addPlane(min[i],normalMin)));      
		minPoly = new CPolygon(CPolygon::hugePolygon(*leakBrushPlane->getPlane()));
		leakBrushPlane->setPolygon(minPoly);

		polygons->push_back(maxPoly);
		polygons->push_back(minPoly);
		firstNode->addPolygon(maxPoly);
		firstNode->addPolygon(minPoly);      
	}
	///
	selectedPlanes.resize(pManager->countPlanes(), false);

	insertToTree(firstNode, bTree.rootNode);
}

void CBspTree::insertToTree(CBspNode *node, CBTree<CBspNode *>::Node *treeNode){
	const CPolygon *splitPolygon = chooseSplitPolygon(*node->getPolygons());
	if (!splitPolygon) {
		CBspLeaf *newLeaf = new CBspLeaf();
		if(firstBsp)
			newLeaf->addLeafPolygonsKeepKeep(node->getPolygons());
		else
			newLeaf->addLeafPolygons(node->getPolygons());

		treeNode->data = newLeaf;

		delete node;
		return;
	}

	CBrushPlane *spBrushPlane = splitPolygon->getBrushPlane();
	CPlane *bPlane = spBrushPlane->getPlane();
	int bestPlane = bPlane->getPlaneId();

	selectedPlanes[bestPlane] = true;  /*mark it selected */
	selectedPlanes[bestPlane ^ 1] = true;  /*mark it selected the oposite */

	node->setSplitBrushPlane(spBrushPlane);
	treeNode->data = node;

	CBspNode *nodes[2];
	nodes[0] = new CBspNode();
	nodes[1] = new CBspNode();
	brakePolygons(node, nodes[0], nodes[1]);

	for(int i = 0 ; i < 2 ; ++i){
		treeNode->insertToNotByRF(i, 0);
		insertToTree(nodes[i], treeNode->child(i));        
	}

	selectedPlanes[bestPlane] = false;
	selectedPlanes[bestPlane ^ 1] = false;
};

CPolygon *CBspTree::chooseSplitPolygon(const list<CPolygon *> poly){
	int bestScore = -10000;
	CPolygon *bestPoly=0;

	bool maybeHavePriors=true;

	list<CPolygon*>::const_iterator i;
	list<CPolygon*>::const_iterator pEnd = poly.end();
	for (i = poly.begin() ; i != pEnd; ++i) {
		CPolygon &currentPolygon = **i;
		const CPlane &currentPlane = *currentPolygon.getPlane();
		int planeId = currentPlane.getPlaneId();

		if (selectedPlanes[planeId] || selectedPlanes[planeId ^ 1]) {
			continue;
		}

		if(maybeHavePriors)
			if((*i)->isPrior())
				;
			else{
				if(bestPoly){
					return bestPoly;
				}
				maybeHavePriors=false;
			}

			int splits = 0;
			int front = 0;
			int back = 0;
			int on = 0;
			CPlane::eSide s;        

			list<CPolygon*>::const_iterator j;
			for ( j = poly.begin(); j != pEnd; ++j) {
				const CPolygon &secondLoopPolygon = **j;
				int secondPlaneId = secondLoopPolygon.getPlane().getPlaneId();

				if (secondPlaneId == planeId) {
					front++;
					on++;
				} else if (secondPlaneId ==(planeId ^ 1)){
					back++;
					on++;
				} else {
					s = secondLoopPolygon.planeSide(currentPlane, PLANES_ON_EPSILON);
					if (s == CPlane::CROSS) {
						splits++;
					} else if (s == CPlane::FRONT) {
						front++;
					} else if (s == CPlane::BACK) {
						back++;
					} else if (s == CPlane::ON) {
						on++;
					}
				}
			}

			int boost = 0;
			if (currentPolygon.isPrior())
				boost = 10000;

            if (currentPolygon.isPortal())
               boost = -10000;

			int score;
			if(firstBsp)
				score = 10000 + on - abs(front - back) + boost;
			else
				score = 10000 - splits + boost;

			if (currentPlane.getOnAxe() != -1)
				score = (int)((float)score*1.5f);

			if (score > bestScore) {
				bestScore = score;
				bestPoly = &currentPolygon;
			}
	}
	return bestPoly;
}

void CBspTree::brakePolygons(CBspNode *nodeToSplit, CBspNode *potitiveNode, CBspNode *negativeNode){
	CPlane *sPlane;
	int planeId;
	int opositeId;
	list<CPolygon *>::iterator pIter, pEnd;
	int toSplitPlaneId;

	sPlane = nodeToSplit->getSplitPlane();
	planeId = sPlane->getPlaneId();
	opositeId = planeId ^ 1;
	pIter = nodeToSplit->pBegin();
	pEnd = nodeToSplit->pEnd();
	for (;pIter!=pEnd; pIter++) {
		toSplitPlaneId = (*pIter)->getPlane()->getPlaneId();
		//isPrior = (*pIter)->isPrior();

		//if(isSplitPortal){
		//  if(polPortal == isSplitPortal)
		//    continue;
		// }

		if (planeId == toSplitPlaneId) {
			potitiveNode->addPolygon(*pIter);
			continue;
		} 
		else if (opositeId == toSplitPlaneId) {
			negativeNode->addPolygon(*pIter);
			continue;
		}

		CPlane::eSide side = (*pIter)->planeSide(*sPlane, PLANES_ON_EPSILON);

		if (side == CPlane::FRONT) {
			potitiveNode->addPolygon(*pIter);
		} 
		else if (side == CPlane::BACK) {
			negativeNode->addPolygon(*pIter);
		} 
		else if (side == CPlane::ON) {
			if((*pIter)->getPlane()->getNorma()*sPlane->getNorma() >= 0.0f){
				potitiveNode->addPolygon(*pIter);
			} else {
				negativeNode->addPolygon(*pIter);
			}
			continue;
		} 
		else if (side == CPlane::CROSS) {
			//edo to allaksa
			CPolygon *f = new CPolygon();
			CPolygon *b = new CPolygon();
			side = (*pIter)->splitByPlane(sPlane, f, b, 0.0f);
			assert(side == CPlane::CROSS);
			bool fsmall, bsmall;
			fsmall =f->isTooSmall();
			bsmall =b->isTooSmall();
			if(!fsmall && !bsmall){
				potitiveNode->addPolygon(f);
				negativeNode->addPolygon(b);              
				//keep tracking the polygons creation these will be deleted after flood fill
				polygons->push_back(f);              
				polygons->push_back(b);
				//new for detail forcing
				(*pIter)->getBrushPlane()->brake();              
				continue;
			}

			if (fsmall){                
				negativeNode->addPolygon(*pIter);                                
			}

			if(bsmall){
				potitiveNode->addPolygon(*pIter);            
			}
			delete f;
			delete b;              
		}
	}
}


list<CPolygon *> *CBspTree::getPolygons(){
	list<CPolygon *> *l = new list<CPolygon *>();
	getLeafPolygons(l, bTree.rootNode);
	return l;
}

void CBspTree::getLeafPolygons(list<CPolygon *> *l, CBTree<CBspNode *>::Node *node){
	if(node->isLeaf()){
		CBspLeaf *leaf = (CBspLeaf *)node->data;
		list<CPolygon *>::iterator pIter, pEnd;
		pIter = leaf->pBegin();
		pEnd = leaf->pEnd();
		while(pIter!=pEnd){
			l->push_front(*pIter);
			++pIter;
		}
		return;      
	}
	getLeafPolygons(l, node->child(0));
	getLeafPolygons(l, node->child(1));    
}

