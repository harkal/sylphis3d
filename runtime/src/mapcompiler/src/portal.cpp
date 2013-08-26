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
using namespace std;

#include "portal.h"
#include "bspnode.h"

#ifdef MEMPROF
#include "mmgr.h"
#endif


CPortal::CPortal():CPolygon(){
    leafs[0]= 0;
    leafs[1]= 0;
    areaPortal = false;
}
CPortal::CPortal(const CPolygon & poly):CPolygon(poly){
    //portalNo= pNo;
    leafs[0]= 0;
    leafs[1]= 0;
    areaPortal = false;
}

CPortal::CPortal(const vector < CVertex > &vertices):CPolygon(vertices)
{
    leafs[0]= 0;
    leafs[1]= 0;
    areaPortal = false;
}

CPortal::CPortal(const CPortal & port):CPolygon(port)
{
    leafs[0] = port.leafs[0];
    leafs[1] = port.leafs[1];
}

/*CPortal::CPortal(const int pNo)
{
portalNo= pNo;
leafs[0]= 0;
leafs[1]= 0;    
}*/
CBspLeaf *CPortal::getLeaf(const int LEAF_TYPE)
{
    return leafs[LEAF_TYPE];
}

void CPortal::setLeaf(const int LEAF_TYPE, CBspLeaf *lValue)
{
    leafs[LEAF_TYPE] = lValue;
}

void CPortal::youAreAccepted()
{
    if((!leafs[0]->isOpaque()) && (!leafs[1]->isOpaque())){
        leafs[FRONT_LEAF]->addPortal(this);
        leafs[BACK_LEAF]->addPortal(this);  
    }
    else{//add only the boundboxes
        leafs[FRONT_LEAF]->addBoundBox(this->getPolBounds());
        leafs[BACK_LEAF]->addBoundBox(this->getPolBounds());    
    }

    leafs[FRONT_LEAF]->addLeaf(leafs[BACK_LEAF]);
    leafs[BACK_LEAF]->addLeaf(leafs[FRONT_LEAF]);  
}

bool CPortal::hasBothLeafs(){
    if(leafs[FRONT_LEAF] && leafs[BACK_LEAF])
        return true;
    return false;
}

void CPortal::addWatchLeaf(CBspLeaf *leaf){
    if(!leafs[FRONT_LEAF]){
        leafs[FRONT_LEAF] = leaf;
        return;
    }
    if(!leafs[BACK_LEAF]){
        leafs[BACK_LEAF] = leaf;
        return;
    }  
    assert(false);
}

CVector3 CPortal::getMiddle(){
    CVector3 rv(0,0,0);
    for(VertsVector::iterator i = vBegin() ; i != vEnd() ; ++i){
        rv+=i->getRealCoor();
    }
    rv /= static_cast<float>(countVertices());
    return rv;
}

bool CAreaPortalMother::isInBound(CPortal *portal){
    CAABoundBox bounds = mOriginalPolygon->getPolBounds();
    bounds.expand(2.0f);
    return portal->isInBound(bounds);
}

void CAreaPortalMother::addPortal(CPortal *portal){
    portals.push_back(portal);
}


void CAreaPortalMother::addToAreasTheAreaPortals(vector<CArea *> &areas){
    CAreaPortal *areaPortal;

    setAreasNo(areas.size());
    findChildren();
    for(unsigned int i=1;i<areasNo;i++){
        for(unsigned int j=i;j<areasNo;j++){
            areaPortal = portalsChildren[i][j];
            if(!areaPortal)
                continue;

            areaPortal->boundPolygon();

            //HERE IS THE CODE FOR PORTAL SWAP AREAS SHOWS      
            CBspLeaf *oneFrontLeaf = areaPortal->getLeaf(FRONT_LEAF);//(areas[i]->lBegin());
            assert(!oneFrontLeaf->isBuggyBound());
            CVector3 middleOfLeaf = oneFrontLeaf->getMiddlePoint();
            CPlane *portalPlane = areaPortal->getPlane();
            float d = portalPlane->distanseFromVertex(middleOfLeaf);
            if(d>0){
                areaPortal->setArea(FRONT_AREA, areas[areaPortal->getLeaf(FRONT_LEAF)->getArea()]);
                areaPortal->setArea(BACK_AREA, areas[areaPortal->getLeaf(BACK_LEAF)->getArea()]);
            }
            else{
                areaPortal->setArea(BACK_AREA, areas[areaPortal->getLeaf(BACK_LEAF)->getArea()]);
                areaPortal->setArea(FRONT_AREA, areas[areaPortal->getLeaf(FRONT_LEAF)->getArea()]);
            }

            areas[i]->addAreaPortal(areaPortal);
            areas[j]->addAreaPortal(areaPortal);      
        }
    }
}

void CAreaPortalMother::findChildren(){
    CAreaPortal *areaPortal;

    for(unsigned int i=0;i<areasNo;i++){
        portalsChildren.push_back(vector<CAreaPortal *>(areasNo, 0));
    }

    vector<CPortal *>::iterator pIter, pEnd;
    CPortal *portal;
    int areaFront, areaBack;

    pIter = portals.begin();
    pEnd = portals.end();
    while(pIter != pEnd){
        portal = *pIter;
        areaFront = portal->getLeaf(FRONT_LEAF)->getArea();
        areaBack = portal->getLeaf(BACK_LEAF)->getArea();      

        if(!areaFront || !areaBack){
            ++pIter;
            cout << "WARNING : !areaFront || !areaBack" << endl;
            continue;
        }

#if 1
        if(areaFront==areaBack){
            ++pIter;
            cout << "WARNING : portal with same area both sides" << endl;
            continue;
        }
#endif

        if(areaBack<areaFront){
            int temp =areaBack;
            areaBack = areaFront;
            areaFront = temp;
        }

        areaPortal = portalsChildren[areaFront][areaBack];
        if( !areaPortal ){
            portalsChildren[areaFront][areaBack] = new CAreaPortal(portal);        
            //portalsChildren[areaFront][areaBack]->setBrushPlane(portal->getBrushPlane());
            //portalsChildren[areaFront][areaBack]->addPortal(portal);
        } else {
            portalsChildren[areaFront][areaBack]->addPolygon(portal);
        }

        ++pIter;
    }

}



void CAreaPortal::addPolygon(CPolygon *pol){
    //CVector3 testV(1, 0, 0);
    //if(portal->getPlane()->getPlaneId() == this->getPlane()->getPlaneId())
    if(pol->getPlane()->getPlaneId() != this->getPlane()->getPlaneId() &&
        pol->getPlane()->getPlaneId() != (this->getPlane()->getPlaneId() ^ 1)){    
    
        assert(false);
    }

    if(pol->getPlane()->getPlaneId() == (this->getPlane()->getPlaneId() ^ 1))
        pol->swapVertices();

    //if(portal->getPlane()->getPlaneId() == this->getPlane()->getPlaneId())
    CPolygon::smerge(this, pol);
    //CPolygon::smerge(this,portal,testV);
}


CArea *CAreaPortal::getArea(const int AREA_TYPE)
{
    return areas[AREA_TYPE];
}

void CAreaPortal::setArea(const int AREA_TYPE, CArea *areaVal)
{
    areas[AREA_TYPE] = areaVal;
}

