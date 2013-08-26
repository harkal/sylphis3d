/***************************************************************************
                          bspnode.h  -  description
                             -------------------
    begin                : Mon Jun 17 2002
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
#ifndef _BSPNODE_
#define _BSPNODE_

#include <vector>
#include <list>

#include "shader.h"
#include "brushplane.h"
#include "boundbox.h"

using namespace std;

class CPolygon;
class CPortal;
class CBrush;
class CEntity;

class CBspNode{
public:
	CBspNode();
	CBspNode(list<CPolygon *> *polys, CBrushPlane *sp);
	~CBspNode();

	void freePolygons();
	list<CPolygon *> *getPolygons();

	void addPolygon(CPolygon *polygon);
	void addPolygonWithOutBound(CPolygon *polygon);
	void addPolygonBound(CPolygon *polygon);
	void addBoundBox(const CAABoundBox &bbox);

	list<CPolygon *>::iterator pBegin();
	list<CPolygon *>::iterator pEnd();

	void addPolygons(list<CPolygon *> *pols);
	bool isBuggyBound();

	void setSplitBrushPlane(CBrushPlane *sp)
	{
		splitPlane=sp;
	};
	CPlane *getSplitPlane(){ return splitPlane->getPlane(); };
	CBrushPlane *getSplitBrushPlane(){ return splitPlane; }

	int getPortalNo(){return splitPlane->getShader()->isPortal(); };

	list<CPolygon *>::iterator polygonsBegin();
	list<CPolygon *>::iterator polygonsEnd();
	void erasePolygon(list<CPolygon *>::iterator &i){i = polygons.erase(i);--i;}
	CVector3 getMiddlePoint();

	void pushPolygon(CPolygon *p){ polygons.push_back(p);}
	CAABoundBox &getBounds(){return bounds;}

	void printBounds();
private:
	list<CPolygon *> polygons;

	CAABoundBox bounds;
	CBrushPlane *splitPlane;

	bool bounded;
};

class CBspLeaf : public CBspNode {
public:
	CBspLeaf():CBspNode(){
		opaque = false;
		leak   = false;
		visible= false;
		area   = 0;
		index  = -1;
		depth = 0;
		areaPortalsReach=0;
	}
	~CBspLeaf();

	bool isTrueLeaf();
	int getDepth(){ return depth; }
	void setDepth(const int d){depth = d;}
	int getArea(){ return area; }
	void setArea(const int &a){ area = a; }
	bool isOpaque(){ return opaque; }
	void setOpaque(bool op){ opaque = op; }
	bool isVisible(){ return visible; }
	void setVisible( bool v ){ visible = v; }

	void addDetailBrush(CBrush *brush,const int brushId, bool writeBounds);
	void addStructuralBrush(CBrush *brush, const int brushId);
	void addPortal(CPortal *portal);

	bool isLeak(){ return leak; }
	void setLeak(bool l){ leak = l; }

	//these one clears the portal polygons
	void addLeafPolygons(list<CPolygon *> *pols);
	void addLeafPolygonsKeepKeep(list<CPolygon *> *pols);

	list<CPortal *> &getPortals();

	void addLeaf(CBspLeaf *leaf);
	list<CPortal *>::iterator portalsBegin();
	list<CPortal *>::iterator portalsEnd();

	vector<int>::iterator sbBegin(){ return sBrushes.begin(); }
	vector<int>::iterator sbEnd(){ return sBrushes.end(); }    
	vector<CBrush *>::iterator bBegin(){ return brushes.begin(); }
	vector<CBrush *>::iterator bEnd(){ return brushes.end(); }
	vector<CBrush *>::iterator abBegin(){ return allBrushes.begin(); }
	vector<CBrush *>::iterator abEnd(){ return allBrushes.end(); }
	vector<CBspLeaf *>::iterator oLeafsBegin(){return otherLeafs.begin(); }
	vector<CBspLeaf *>::iterator oLeafsEnd(){return otherLeafs.end(); }
	void setIndex(const int in){index = in;}
	int getIndex(){ return index; }
	void printStructuralBrushes();
	void appendYourBrushes(vector<CBrush *> &abrushes, CEntity* entity);
private:
	vector<CBrush *> brushes;
	vector<int> sBrushes;
	list<CPortal *> portals;
	vector<CBrush *> allBrushes;
	bool opaque;
	//used at flooding fo leak check
	bool leak;
	//used for flooding
	bool visible;
	int area;
	int index;

	//used for finding less leak path
	int depth;
	bool *areaPortalsReach;  
	vector<CBspLeaf *> otherLeafs;
};

#endif


