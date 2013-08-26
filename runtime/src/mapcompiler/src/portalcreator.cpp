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

#include <iostream>

#ifdef MEMPROF
#include "mmgr.h"
#endif

#include "portalcreator.h"
#include "portal.h"
#include "bspnode.h"
#include "brush.h"


CPortalCreator::CPortalCreator()
{
	initialPlanesNo = -1;
}

void CPortalCreator::setInitialValues( CBTree<CBspNode *>::Node *rNode, 
									  const int planesNo
									  )
{
	rootNode        = rNode;
	initialPlanesNo = planesNo;
}

bool CPortalCreator::acceptedPortalForNode(CPortal *portal, CBTree<CBspNode *>::Node *node){
	CBspLeaf *leaf = (CBspLeaf *) node->data;

    if(portal->hasBothLeafs()){
		return false;
    }

	list<CPolygon *>::iterator polIter, polEnd;
	polIter = leaf->polygonsBegin();
	polEnd = leaf->polygonsEnd();
	CPlane *polPlane;
	while(polIter!=polEnd){
        //if((*polIter)->isPortal()){
        //    ++polIter;
        //    continue;
        //}
		polPlane = (*polIter)->getPlane();
		CPlane::eSide side = portal->planeSide(*polPlane, PLANES_ON_EPSILON);      
		if(side == CPlane::CROSS)
			portal->splitIt(polPlane, 0.0f);
		++polIter;
	}

    if(portal->isTooSmall()){
		return false;
    }

	return true;
}

void CPortalCreator::passPortalThroughBsp(list<CPortal *>::iterator &portalI, CBTree<CBspNode *>::Node *node){
	CPortal *portal = *portalI;

	if(!portal)
		return;

	if (node->isLeaf()) {
		if(!acceptedPortalForNode(portal, node)){
			delete portal;
			*portalI=0;
		} else {
			portal->addWatchLeaf((CBspLeaf *)node->data);
		}
		return;
	}

	CPlane *sPlane = node->data->getSplitPlane();
	CPlane::eSide side = portal->planeSide(*sPlane, PLANES_ON_EPSILON);

	if (side == CPlane::FRONT) {
		passPortalThroughBsp(portalI, node->child(0));
	} else if (side == CPlane::BACK) {
		passPortalThroughBsp(portalI, node->child(1));
	} else if (side == CPlane::CROSS) {  //PLANE CROSS
		CPortal *front, *back;
		front = new CPortal();
		back = new CPortal();
		CPlane::eSide newSide = portal->splitByPlane(sPlane, front, back, 0.0f);

		assert(newSide == CPlane::CROSS);
		bool fsmall, bsmall;
		fsmall =front->isTooSmall();
		bsmall =back->isTooSmall();
		if(!fsmall && !bsmall){
			portalList.push_back(front);
			portalList.push_back(back);
			delete portal;
			*portalI=0;            
			return;;
		}
		delete front;                
		delete back;        
		if(fsmall){
			passPortalThroughBsp(portalI, node->child(1));
		} else {
			passPortalThroughBsp(portalI, node->child(0));        
		}
	} else {
		passPortalThroughBsp(portalI, node->child(0));
		passPortalThroughBsp(portalI, node->child(1));

		//to be found on means it is tooSmall so erase it
		//EOS THN ORA TO NA TO SVISO DOULEUEI
		//delete portal;
		//*portalI=0;
		//cout << "WARNING: 108 known warning from portal creator"<<endl;
		//assert(false);  
		return;
	}
}


void CPortalCreator::createForEachSplitNodeHugePolygon( CBTree<CBspNode *>::Node *node, vector<bool> &planesSelected){
	if(node->isLeaf())
		return;
	CBspNode *splitNode = node->data;
	CBrushPlane *sbPlane = splitNode->getSplitBrushPlane();
	CPlane *splitPlane = sbPlane->getPlane();
	int planeId = splitPlane->getPlaneId();
	if(planesSelected[planeId] || planesSelected[planeId^1]){
		createForEachSplitNodeHugePolygon(node->child(0), planesSelected);
		createForEachSplitNodeHugePolygon(node->child(1), planesSelected);    
		return;
	}

	CPortal *theNewPortal = new CPortal(CPolygon::hugePolygon(*splitPlane));
	theNewPortal->setBrushPlane(sbPlane);
	portalList.push_back( theNewPortal );

	planesSelected[planeId]=true;

	createForEachSplitNodeHugePolygon(node->child(0), planesSelected);
	createForEachSplitNodeHugePolygon(node->child(1), planesSelected);  
}

void CPortalCreator::calcPortals(){
	assert(initialPlanesNo>0);

	cout << "Start portals finding..." << endl;

	vector<bool> planesSelected(initialPlanesNo, false);

	createForEachSplitNodeHugePolygon(rootNode, planesSelected);    

	int portalNo=0;
	CPortal *theCurrentPortal;

	//these are to get what designer portal made
	int portalPortalNo = 0;

	for(list<CPortal *>::iterator i=portalList.begin();i!=portalList.end();i++){
		passPortalThroughBsp(i, rootNode);
		if(!(*i)){
			i = portalList.erase(i);
			--i;
			continue;
		}

		theCurrentPortal = *i;

		if(!(theCurrentPortal->getLeaf(FRONT_LEAF) && theCurrentPortal->getLeaf(BACK_LEAF))){
            //cout << "WARNING : Single sided portal!" << endl;
			delete *i;
			i = portalList.erase(i);
			--i;
			continue;
		}

		theCurrentPortal->boundPolygon();
        portalPortalNo += theCurrentPortal->isPortal() ? 1 : 0;
		theCurrentPortal->youAreAccepted();

		++portalNo;

		finalPortals.push_back(theCurrentPortal);    	        
	}
    cout << portalPortalNo << " User portals " << endl;
	portalList.clear();
	cout << portalNo<<" portals found "<<endl;
}


void CPortalCreator::calcPortalsForFirstBsp(){
	calcPortals();
}


void CPortalCreator::calcPortalsForSecondBsp(){
	calcPortals();
	//filterPortalsIntoTree();
	passBoundToFathers(rootNode);  
}

CAABoundBox &CPortalCreator::passBoundToFathers(CBTree<CBspNode *>::Node *node)
{
	if(!node->isLeaf()){
		CBspNode *thisNode = node->data;
		thisNode->addBoundBox(passBoundToFathers(node->child(0)));
		thisNode->addBoundBox(passBoundToFathers(node->child(1)));
	}
	return node->data->getBounds();
}


void CPortalCreator::freePortals(){
	list<CPortal *>::iterator fpIter, fpEnd;
	fpIter = finalPortals.begin();
	fpEnd = finalPortals.end();
	while(fpIter!=fpEnd){
		delete *fpIter;
		fpIter = finalPortals.erase(fpIter);
		--fpIter;
		++fpIter;
	}
}

void CPortalCreator::clearPortalBuffer()
{
	vector<CPortal *>::iterator pIter, pEnd;
	pIter = portalBuffer.begin();
	pEnd  = portalBuffer.end();
	while(pIter!=pEnd){
		delete *pIter;
		pIter = portalBuffer.erase(pIter);
		--pIter;
		++pIter;
	}
	//portalBuffer.clear();
}

void CPortalCreator::addToAreaLeafs(const int numberToCheck, CArea *area, CBTree<CBspNode *>::Node *node){
	if(node->isLeaf()){
		CBspLeaf *leaf = (CBspLeaf *)node->data;

		if(leaf->getArea() == numberToCheck && leaf->isTrueLeaf() ){
			area->addLeaf(leaf);
			//leaf->setIndex(areasUpLeafs.size());
			areasUpLeafs.push_back(leaf);
		}
		return;
	}
	addToAreaLeafs(numberToCheck, area, node->child(0));
	addToAreaLeafs(numberToCheck, area, node->child(1));
}


void CPortalCreator::findTheAreaLeafs(const int areaNo){
	for(int i=0;i<areaNo;i++){
		CArea *newArea = new CArea();
		addToAreaLeafs(i, newArea, rootNode);
		areas.push_back(newArea);
	}
}

