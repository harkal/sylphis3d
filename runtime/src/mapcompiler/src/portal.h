/***************************************************************************
    copyright            : (C) 2002 by Vassili Tenente
                           (C) 2002-2005 Harry Kalogirou (harkal@gmail.com)
  ***************************************************************************
    This file is part of "Sylphis3D Game Engine".

    "Sylphis3D Game Engine" is free software; you can redistribute it and/or 
    modify it under the terms of the GNU General Public License as 
    published by the Free Software Foundation; either version 2 of the 
    License, or (at your option) any later version, 
    plus a special exception:
      Linking "Sylphis3D Game Engine" statically or dynamically with other 
    modules is making a combined work based on this library. Thus, the 
    terms and conditions of the GNU General Public License cover the whole 
    combination. As a special exception, the copyright holders of this 
    library give you permission to link this library with independent 
    modules to produce an executable, regardless of the license terms of 
    these independent modules, and to copy and distribute the resulting 
    executable under terms of your choice, provided that you also meet, 
    for each linked independent module, the terms and conditions of the 
    license of that module. An independent module is a module which is 
    not derived from or based on this program. If you modify this program,
    you may extend this exception to your version of the library, but you 
    are not obligated to do so. If you do not wish to do so, delete this 
    exception statement from your version.

    "Sylphis3D Game Engine" is distributed in the hope that it will be 
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with "Sylphis3D Game Engine"; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
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

