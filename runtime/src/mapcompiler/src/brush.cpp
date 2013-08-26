/***************************************************************************
                          brush.cpp  -  description
                             -------------------
    begin                : Sun Jun 9 2002
    copyright            : (C) 2002 by  Vassili Tenente
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
#include <stack>

#ifdef MEMPROF
#include "mmgr.h"
#endif

#include "brush.h"
#include "polygon.h"
#include "boundbox.h"

//#define DEBUG 1


CBrush::CBrush(bool fb){
	opaque     = true;
	structural = true;
	rLeaf      = false;
	fromBegin  = fb;
	brushId    = -1;
}

CBrush::CBrush(CBrush *fatherBrush, bool fb){
	opaque     = fatherBrush->opaque;
	structural = fatherBrush->structural;
	rLeaf      = false;
	fromBegin  = fb;
	brushId    = fatherBrush->brushId;
}

CBrush::~CBrush(){
}

void CBrush::addBrushPlane(CBrushPlane &p, CPolygon *pol){
	planes.push_front(p);
	planes.front().setPolygon(pol);
	planes.front().setBrushId(brushId);
	pol->setBrushPlane(&planes.front());    
}

void CBrush::addBrushPlane(CBrushPlane *p, CPolygon *pol){
	planes.push_front(*p);
	planes.front().setPolygon(pol);
	planes.front().setBrushId(brushId);
	planes.front().setFather(p->getFather());
	pol->setBrushPlane(&planes.front());    
}

void CBrush::addBrushPlane(CBrushPlane *p){
	planes.push_front(*p);
}

CBrushPlane* CBrush::addBrushPlane(){
	planes.push_front(CBrushPlane());
	planes.front().setBrushId(brushId);
	return &planes.front();
}

int CBrush::getBrushPlanesNo(void) const {
	return static_cast<int>(planes.size());
}

list < CBrushPlane > &CBrush::getBrushPlanes(){
	return planes;
}

CBrushPlane & CBrush::getLastBrushPlane(){
	return planes.front();
}

CBrushPlane *CBrush::getLastBPPointer(){
	return &planes.front();
}

void CBrush::createPolygons(){
	list<CBrushPlane>::iterator biter, bend, biter2;
	biter = planes.begin();
	bend = planes.end();
	while(biter!=bend){
		assert(biter->getPlane());
		CPolygon *newP;

		newP = new CPolygon(CPolygon::hugePolygon(*biter->getPlane(), huge));

		biter2 = planes.begin();
		while(biter2!=bend){
			if(biter2!=biter)
				newP->splitIt(biter2->getPlane(), 0.0f);
			++biter2;
		}
		//newP->snapIt();
		newP->boundPolygon();
		newP->setBrushPlane(&(*biter));

		biter->setPolygon(newP);


		//now setting the brush parameters
		//from the shader parameters

		//one shader detail enough to make brush detail
		if(!biter->getShader()->isStructural())
			setStructural(false);

		//one not opaque enough to make brush not opaque
		if(!biter->getShader()->isOpaque())
			setOpaque(false);

		++biter;
	}
}

void CBrush::appendYourPolygons( list<CPolygon *> *polyList){
	list < CBrushPlane >::iterator bIter, bEnd;
	bEnd = planes.end();

	for(bIter = planes.begin() ; bIter != bEnd ; ++bIter){
		CPolygon *pol = bIter->getPolygon();
		polyList->push_back(pol);
	}
}


void CBrush::appendYourVisiblePolygons( list<CPolygon *> *polyList){
	list < CBrushPlane >::iterator bIter, bEnd;
	bIter = planes.begin();
	bEnd = planes.end();
	CPolygon *pol;
	while(bIter!=bEnd){
		if(bIter->isVisible()){
			pol = bIter->getPolygon();
			pol->setBrushPlane(&(*bIter));
			//pol->appendTessPolygons(polyList);
			polyList->push_back(pol);
		}
		++bIter;
	}
}

CPolygon * CBrush::getLargerPolygon(){
	list < CBrushPlane >::iterator bIter, bEnd;
	bIter          = planes.begin();
	bEnd           = planes.end();
	float howLarge = -huge;
	int index      = 0;
	int larger     = -1;
	float area;
	CBrushPlane *largerBp = 0;

	while(bIter!=bEnd){
		area = bIter->getPolygon()->area();
		if(area > howLarge){
			howLarge = area;
			larger = index;
			largerBp=&(*bIter);
		}

		++index;
		++bIter;
	}

	if(larger != -1)
		return largerBp->getPolygon();
	return 0;
}

void CBrush::freeIt() {
	CPolygon *polygon;
	list<CBrushPlane>::iterator biter, bend;
	biter = planes.begin();
	bend = planes.end();

	while(biter != bend){
		polygon = biter->getPolygon();
		if(polygon)
			delete polygon;

		++biter;    
	}
}

void CBrush::reBoundBrush() {
	boundBrush();
}

void CBrush::boundBrush() {
	mBounds.setBounds(CVector3(huge,huge,huge), CVector3(-huge,-huge,-huge));
	list<CBrushPlane>::iterator bIter, bEnd;
	bIter=planes.begin();
	bEnd = planes.end();
	CPolygon *pol;
	while(bIter!=bEnd){
		pol = bIter->getPolygon();
		if(!pol){
			++bIter;
			continue;
		}

		const CAABoundBox &polBounds = pol->getPolBounds();

		pol->boundPolygon();

		mBounds.addPoint(polBounds.getMin());
		mBounds.addPoint(polBounds.getMax());

		++bIter;
	}
}


/*
==================
BrushVolume

==================
*/
float CBrush::volume()
{
	CPolygon *w;
	CVector3 corner;
	float d, volume;
	/*if (planes.size()<4)
	return 0.0f; */

	// grab the first valid point as the corner 
	w = NULL;
	CBrushPlane *bp;
	list<CBrushPlane>::iterator bpIter, bpEnd;
	bpIter = planes.begin();
	bpEnd = planes.end();
	while(bpIter!=bpEnd){
		bp = &(*bpIter);
		w = bp->getPolygon();
		if (w)
			break;      
		++bpIter;
	}
	if (!w)
		return 0.0f;
	corner = w->getVertex(0).getRealCoor();
	// make tetrahedrons to all other faces
	volume = 0.0f;
	while(bpIter!=bpEnd){
		bp = &(*bpIter);
		w = bp->getPolygon();
		if (!w)
			continue;
		d = -bp->getPlane()->distanseFromVertex(corner);
		volume += (w->area()) * d;
		++bpIter;
	}

	volume /= 3;
	return volume;
}


void CBrush::setLeakBrush(CShaderFlags *leakFlag)
{
	list<CBrushPlane>::iterator pBegin, pEnd;
	pBegin = planesBegin();
	pEnd = planesEnd();
	while(pBegin != pEnd ){
		if( !pBegin->isLeak() )
		{
			pBegin->setShader(leakFlag);
		}    
		++pBegin;
	}
}


void CBrush::moveBrush(CVector3 &middle)
{
	list<CBrushPlane>::iterator pBegin, pEnd;
	pBegin = planesBegin();
	pEnd = planesEnd();
	while(pBegin != pEnd ){
		pBegin->translate(middle);
		++pBegin;
	}
}

bool CBrush::isInOrOn(CVector3 &v)
{
	list<CBrushPlane>::iterator planeI, planeEnd;
	planeI=planes.begin();
	planeEnd=planes.end();
	CPlane *plane;
	while(planeI!=planeEnd){
		plane = planeI->getPlane();
		if(plane->distanseFromVertex(v)>POINT_ON_PLANE_EPSILON)
			return false;
		++planeI;
	}
	return true;
}

bool CBrush::areAllVertsIn(CPolygon *p)
{
	vector<CVertex>::iterator v, e;
	list<CBrushPlane>::iterator planeI, planeEnd;
	e=p->vEnd();
	planeI=planes.begin();
	planeEnd=planes.end();
	CPlane *plane;
	while(planeI!=planeEnd){
		plane = planeI->getFather()->getPlane();
		for( v = p->vBegin() ; v != e ; ++v ){
			if(plane->distanseFromVertex(*v)>-POINT_ON_PLANE_EPSILON)
				return false;
		}
		++planeI;
	}
	return true;
}

bool CBrush::subPolygon(CPolygon *polygon)
{
	list<CBrushPlane>::iterator planeI, planeEnd;
	planeEnd=planes.end();

	//keeps for each edge the max factor M
	vector<float> maxM;
	//keeps if the vertice i is in brush or not
	vector<bool> inBrush;
	vector<bool> checked;
	CPlane *plane;
	vector<bool> initializedM;
	int noVerts = polygon->countVertices();
	vector<CVertex> &verts = polygon->getVertices();
	int i;
	float da;
	vector< vector<float> > ds;
	ds.resize( noVerts, vector<float>());
	//for every edge (edges # = noVerts) calc the M factor and keep the
	//bigger brushPlane for this edge
	bool noOneIn = true;
	maxM.resize( noVerts, 0.0f );
	initializedM.resize(noVerts, false);
	for( i = 0; i < noVerts; i++ ){

		inBrush.push_back(true);
		checked.push_back(false);
		planeI=planes.begin();
		while(planeI!=planeEnd){
			plane = planeI->getFather()->getPlane();
			da = plane->distanseFromVertex(verts[i]); 
			//keep the potition of the vertex 
			// i , in or not in brush
			if(da>POINT_ON_PLANE_EPSILON){
				inBrush[i] = false;
			}
			ds[i].push_back(da);
			++planeI;
		}
		if(inBrush[i])
			noOneIn = false;
	}

	if(noOneIn)
		return false;

	//these will be the new vertices of the polygon
	vector<CVertex> newVertices; 
	stack<int> vstack;
	vstack.push(0);
	int next, previous;
	while( true ){
		if(vstack.empty())
			break;
		//take the vertex i fromm the top of the stack
		i = vstack.top();
		vstack.pop();

		//if this vertex already checked do nothing
		if(checked[i])
			continue;

		// if not in brush then it is a vertex to keep    
		if(!inBrush[i]){
			newVertices.push_back(verts[i]);
			vstack.push((i+1)%noVerts);
			checked[i] = true;
			continue;
		}

		next     = (i+1)%noVerts;
		previous = (i+noVerts-1)%noVerts;

		// if both next and previous in brush then you should leave this vertex
		//behind and go on to the next vertex  
		if(inBrush[next] && inBrush[previous] && inBrush[i]){
			vstack.push(next);
			checked[i] = true;
			continue;    
		}
		//newVertices.push_back(verts[i]);
		if(!inBrush[previous] && inBrush[next]){
			//int pr2=(i+noVerts-2)%noVerts;
			CVector3 v2 = 
				(verts[i].getRealCoor()-verts[previous].getRealCoor())*(maximize(ds[previous], ds[i])) + verts[previous].getRealCoor();
			newVertices.push_back(CVertex(v2));
		}

		else if(!inBrush[next] && inBrush[previous]){
			CVector3 v2 = (verts[i].getRealCoor()-verts[next].getRealCoor())*((maximize(ds[next], ds[i]))) + verts[next].getRealCoor();
			newVertices.push_back(CVertex(v2));
		}
		else
			newVertices.push_back(verts[i]);

		vstack.push(next);
		checked[i] = true;
	}
	newVertices = CPolygon::removeCollinear(newVertices);
	if(newVertices.size() < 3)
		return true;
	polygon->setVertices(newVertices);
	return false;
}

float CBrush::maximize(vector< float > &out, vector<float> &in){
	size_t noBp = out.size();
	float max = -1;
	float tm;
	for(size_t i=0;i<noBp;i++){
		if(out[i]>POINT_ON_PLANE_EPSILON && in[i]<POINT_ON_PLANE_EPSILON){
			tm = fabs(out[i])/(fabs(out[i])+fabs(in[i]));
			if(tm>max && tm>=0.0f && tm<=1.0f)
				max = tm;
		}
	}
	return max;
}

void CBrush::polygonsInit(){
	planeIter=planes.begin();
}

CPolygon *CBrush::nextPolygon(){
	if(planeIter!=planes.end()){
		CPolygon *p = planeIter->getPolygon();
		p->setBrushPlane(&(*planeIter));
		++planeIter;
		return p;
	}
	return 0;
}

