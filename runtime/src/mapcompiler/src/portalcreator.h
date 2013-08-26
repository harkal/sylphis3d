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


