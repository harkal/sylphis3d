/***************************************************************************
                          brush.h  -  description
                             -------------------
    begin                : Sun Jun 9 2002
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
#ifndef _BRUSH_H
#define _BRUSH_H

#include <list>

#include "boundbox.h"
#include "brushplane.h"

using namespace std;

class CAABoundBox;

class CBrush {
public:
	CBrush(bool fb=false);
	CBrush(CBrush *fatherBrush, bool fb=false);
	~CBrush();

	void addBrushPlane(CBrushPlane *p);
	void addBrushPlane(CBrushPlane & p, CPolygon *pol);
	void CBrush::addBrushPlane(CBrushPlane *p, CPolygon *pol);

	CBrushPlane* CBrush::addBrushPlane();

	int getBrushPlanesNo(void) const;

	list<CBrushPlane>::iterator planesBegin(){return planes.begin();};
	list<CBrushPlane>::iterator planesEnd() {return planes.end();};

	list < CBrushPlane > &getBrushPlanes(void);
	CBrushPlane & getLastBrushPlane(void);

	void freeIt();
	void createPolygons();
	float volume();

	CPolygon * getLargerPolygon();
	void appendYourPolygons( list<CPolygon *> *polyList);
	void appendYourVisiblePolygons( list<CPolygon *> *polyList);
	bool isStructural(){return structural;}

	void setStructural(bool isS){structural = isS;}
	void setOpaque( bool op ){opaque = op;};
	bool isOpaque(){ return opaque; };
	void markVisibleSides();

	void boundBrush();
	void reBoundBrush();
	const CAABoundBox &getBounds() const {return mBounds;}
	CBrushPlane *getLastBPPointer();

	void setLeakBrush(CShaderFlags *leakFlag);

	void setReachLeaf(bool c){ rLeaf =c; };
	bool reachLeaf(){ return rLeaf; };
	bool isFromBegin(){ return fromBegin; }
	void setBrushId(const int bId){ brushId = bId; }
	int getBrushId(){ return brushId; }
	void moveBrush(CVector3 &middle);
	bool areAllVertsIn(CPolygon *p);
	bool isInOrOn(CVector3 &v);

	/// !!WARNING 
	/// This function changes the vertices of the polygon p
	/// with others less or equal to its depending which of them are in the brush
	/// @returns true if a subpolygon does not exist-at this point the p vertices are not changed-
	/// ans false when there is a subpolygon and p becomes the subpolygon
	bool subPolygon(CPolygon *p);
	float maximize(vector< float > &out, vector<float> &in);

	///these are to iterate the polygons
	///to work the createPolygons must have called first
	void polygonsInit();
	CPolygon *nextPolygon();
	list<CBrushPlane>::iterator planeIter;
private:
	list<CBrushPlane> planes;
	bool structural;
	bool opaque;

	CAABoundBox mBounds;

	//this lets you know if this brush was created by splitBrush
	bool rLeaf;

	//this is to know which brush is from the begining
	bool fromBegin;
	int brushId;
};

#endif


