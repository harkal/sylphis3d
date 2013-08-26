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

#ifndef _PORTAL_H_
#define _PORTAL_H_

#include <list>
#include "polygon.h"

using namespace std;
class CBspLeaf;

const static int FRONT_LEAF = 1;
const static int BACK_LEAF  = 0;
const static int FRONT_AREA = 1;
const static int BACK_AREA  = 0;

class CPortal : public CPolygon {
public:
	CPortal();
	CPortal(const CPolygon & poly);
	CPortal(const CPortal & port);    
	CPortal(const vector < CVertex > &vertices);
	void setAreaPortal(bool inPB){areaPortal = inPB;}
	bool getAreaPortal(){ return areaPortal; }
	bool isTruePortal(){ 
		if(isPortal())
			return getAreaPortal();
		return false;
	}
	
	CVector3 getMiddle();
	CBspLeaf *getLeaf(const int LEAF_TYPE);
	void setLeaf(const int LEAF_TYPE, CBspLeaf *lValue);
	void youAreAccepted();
	bool hasBothLeafs();
	void addWatchLeaf(CBspLeaf *leaf);
	CPortal *swapped();
private :
	CBspLeaf *leafs[2];
	bool areaPortal;
};

class CArea;

class CAreaPortal : public CPortal {
public:
	CAreaPortal():CPortal() { 
		index = -1;
		areas[0] = 0;
		areas[1] = 0;        
	};
	CAreaPortal(CPortal *portal):CPortal(*portal){    
		areas[0] = 0;
		areas[1] = 0;    
		index = -1;
		setBrushPlane(portal->getBrushPlane());
	};  
	~CAreaPortal(){};

	CArea *getArea(const int AREA_TYPE);
	void setArea(const int AREA_TYPE, CArea *areaVal);

	void addPolygon(CPolygon *pol);

	int getIndex(){ return index; }
	void setIndex(const int in){ index = in; }
private:
	CArea *areas[2];
	int index;
};

class CAreaPortalMother {
public:
	CAreaPortalMother(CPolygon *originalPolygon){
        mOriginalPolygon = originalPolygon;
    };

	~CAreaPortalMother(){};

	void setAreasNo(size_t areaNo){
		areasNo = areaNo;
	}

    CPolygon *getOriginalPolygon(){
        return mOriginalPolygon;
    }

    bool isInBound(CPortal *portal);

	void findChildren();

	void addPortal(CPortal *portal);
	void addToAreasTheAreaPortals(vector<CArea *> &areas);

private:
    CPolygon *mOriginalPolygon;
	vector<CPortal *> portals;
	vector< vector<CAreaPortal *> > portalsChildren;
	size_t areasNo;
};

class CArea {
public:
	CArea(){};
	~CArea(){};

	void addLeaf(CBspLeaf *leaf){ leafs.push_back(leaf); }
	list<CBspLeaf *>::iterator lBegin(){ return leafs.begin();}
	list<CBspLeaf *>::iterator lEnd(){ return leafs.end();}
	vector<CAreaPortal *>::iterator apBegin(){ return areaPortals.begin();}
	vector<CAreaPortal *>::iterator apEnd(){ return areaPortals.end();}
	void addAreaPortal(CAreaPortal *areaP){ areaPortals.push_back(areaP); }
	size_t countLeafs(){ return leafs.size(); }
private:
	list<CBspLeaf *> leafs;
	vector<CAreaPortal *> areaPortals;
};

#endif

