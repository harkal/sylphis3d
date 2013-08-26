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

#ifndef _PORTALCREATOR_H_
#define _PORTALCREATOR_H_

#include <list>
#include <vector>

#include "tree.h"

using namespace std;

class CPortal;
class CBspNode;
class CArea;
class CPolygon;
class CAreaPortal;
class CAABoundBox;
class CBspLeaf;

class CPortalCreator{
public:
	CPortalCreator();
	void setInitialValues( CBTree<CBspNode *>::Node *rNode, const int planesNo);
	void calcPortalsForFirstBsp();

	void calcPortalsForSecondBsp();

	list<CPortal*> &getFinalPortals(){ return finalPortals; }
	void addAreaPortalsToAreas();
	void CPortalCreator::freePortals();

	void addToAreaLeafs(const int numberToCheck, CArea *area, CBTree<CBspNode *>::Node *node);
	CAABoundBox &passBoundToFathers(CBTree<CBspNode *>::Node *node);
	void findTheAreaLeafs(const int areaNo);
	vector<CArea *>::iterator aBegin(){ return areas.begin();}
	vector<CArea *>::iterator aEnd(){ return areas.end();}

	vector<CBspLeaf *>::iterator leafsUpBegin(){ return areasUpLeafs.begin(); }
	vector<CBspLeaf *>::iterator leafsUpEnd(){ return areasUpLeafs.end(); }    
	size_t countLeafs(){ return areasUpLeafs.size(); }
	CArea * getArea(const int index){ return areas[index]; }
	vector<CArea *> & getAreas(){ return areas; }

private:
	//keep all the portals created for memory clean up
	list<CPortal *> portalList;

	//keep only the good portals final true portals
	list<CPortal *> finalPortals;  

	CBTree<CBspNode *>::Node *rootNode;

	vector<CArea *> areas;
	vector<CBspLeaf *> areasUpLeafs;

	int initialPlanesNo;

	void calcPortals();
	bool acceptedPortalForNode(CPortal *portal, CBTree<CBspNode *>::Node *node);

	void passPortalThroughBsp(list<CPortal *>::iterator &portalI,
		CBTree<CBspNode *>::Node *node);
	void createForEachSplitNodeHugePolygon( CBTree<CBspNode *>::Node *node, vector<bool> &planesSelected);

	void filterPortalIntoTree(CBTree<CBspNode *>::Node *node, CPortal *portal);
	void filterPortalsIntoTree();
	vector<CPortal *> portalBuffer;
	void clearPortalBuffer();
};

#endif


