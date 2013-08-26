/***************************************************************************
                          filterer.h  -  description
                             -------------------
    begin                : Mon Jul 7 2003
    copyright            : (C) 2003 by Vassili Tenente
    email                :   p98009@rainbow.cs.unipi.gr
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
#ifndef _FILTERER_H_
#define _FILTERER_H_

#include <list>
#include "plane.h"
#include "tree.h"
#include "boundbox.h"
#include "shader.h"

using namespace std;

class CBrush;
class CBspNode;
class CBspLeaf;
class CPlaneManager;
class CEntity;
class CBrushPlane;
class CShaderFlags;

class CFilterer{
public:
	CFilterer();
	~CFilterer(){	};
	void setInitialValues(CPlaneManager *pM, 
		CBTree<CBspNode *>::Node *rNode, CEntity *we, vector<CPolygon *> *pPolygons, const int polNo);
	void doFilteringForFirstBsp();
	void doFilteringForSecondBsp();
	CBTree<CBspLeaf *>::Node *filterPointIntoTree(CVector3 &point);
	void filterPortalsIntoTree();
	void filterPortalIntoTree(CBTree<CBspNode *>::Node *node, CPolygon *portal);
	void forceDetailBrushes();
	void filterFor2OnlyDetail();
	void filterFor2OnlyStructural();
	void mergeDetails(CBTree<CBspNode *>::Node *node);
	void findDetailAndMerge();

private:
	const static bool STRUCTURAL  = true; 
	const static bool DETAIL      = false; 

	CEntity *worldEntity;
	CBTree<CBspNode *>::Node *rootNode;

	CPlaneManager *pManager;

	vector<CPolygon *> *portalPolygons;
	int portalPNo;
	int portalCurrentlyFilter;

	//these variables are used with the filterBrushIntoTree fuction
	bool        writeBounds;
	CBrush      *filteringBrush;
	int         opaqueNo;
	int         brushSplits;
	CAABoundBox worldBounds;
	int         tinyBrushes;
	CVector3    worldMin, worldMax;
	CShaderFlags defaultShaderFlags;

	int filterBrushIntoTree(CBrush * brush, CBTree<CBspNode *>::Node * node);
	void filterBrushes(bool which);

	CPlane::eSide splitBrush(CBrush *brushToSplit, CBrush ** front, CBrush ** back, CBrushPlane *splitBrushPlane);
	CPlane::eSide brushMostlyOnSide(CBrush * brush, CPlane *plane);

	list<CBrush *> brushesBuffer;

	void clearBrushesBuffer();

	void filterAllBrushesIntoTree();
	bool firstBsp;     
};

#endif

