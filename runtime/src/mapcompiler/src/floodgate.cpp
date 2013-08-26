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

#include <vector>
#include <fstream>
#include <iostream>

#ifdef MEMPROF
#include "mmgr.h"
#endif

#include "floodgate.h"
#include "portal.h"
#include "bspnode.h"

CFloodgate::CFloodgate(){
}

void CFloodgate::setInitialValues(vector<CBspLeaf *> &sLeafs, 
								  vector<CPolygon *> &portals,                                  
								  string &fileP, CVector3 &playerStart)
{
	startLeafs = sLeafs;
	portalPNo = portals.size();
    mPortals = portals;
	filePath = fileP;
	startPoint = playerStart;
	size_t place=filePath.rfind(".map");
	if(place !=-1)
		filePath=filePath.replace(place, 4, ".lin", 4);
	else
		filePath+=".lin";
}

bool CFloodgate::firstBspFloodPlace(){
	return findBPPolygons();
}

void CFloodgate::secondBspFloodPlace(){
	findAreas();
}

void CFloodgate::addAreaPortalsToAreas(vector<CArea *> &areas){
	for(vector<CAreaPortalMother *>::iterator i = areaPortalMothers.begin();
		i != areaPortalMothers.end() ; ++i){

		(*i)->addToAreasTheAreaPortals(areas);
	}    
}


void CFloodgate::findAreas(){
	areaPortalMothers.resize( portalPNo, 0 );
	for(int i=0 ; i < portalPNo ; ++i){
		areaPortalMothers[i] = new CAreaPortalMother(mPortals[i]);
	}

	CBspLeaf *leaf, *frontLeaf, *backLeaf;
	stack < CBspLeaf * >leafs;
	CPortal *portal;
	int currentArea;
	stack<CBspLeaf *> posAreasLeafs;
	list<CPortal *>::iterator portalIter, portalEnd;
	//CArea *area;   
	currentArea = 0;
	//this is the area 0 to add the opaque brushes
	//    areas.push_back(new CArea());
	int leafsForThisArea;

	// FIXME
	//for(vector<CBspLeaf *>::iterator i = startLeafs.begin() ; 
	//	i != startLeafs.end() ; ++i)
	//	leafs.push(*i);

	leafs.push(startLeafs.front());

	goto findThatArea;
findThatArea:
	currentArea++;
	leafsForThisArea = 0;

    int num = 0;
    int innum = 0;
	while (true) {
		if (leafs.empty())
			break;
		leaf = leafs.top();
		leafs.pop();

		if(leaf->getArea() || leaf->isOpaque())
			continue;

		if(leaf->isLeak())
			assert(false);
		
		leaf->setArea(currentArea);
		leaf->setVisible(true);
		leafsForThisArea++;
		
		portalIter = leaf->portalsBegin();
		portalEnd = leaf->portalsEnd();
		while( portalIter != portalEnd ){
			portal = *portalIter;

			frontLeaf = portal->getLeaf(FRONT_LEAF);
			backLeaf = portal->getLeaf(BACK_LEAF);

            num++;
            int inPortal = -1;
            for(int i = 0 ; i < portalPNo ; ++i){
                if(areaPortalMothers[i]->isInBound(portal)){
                    inPortal = i;
                    break;
                }
            }
			if(inPortal != -1){
                innum++;
				portal->setAreaPortal(true);
				areaPortalMothers[inPortal]->addPortal(portal);

				posAreasLeafs.push(frontLeaf);
				posAreasLeafs.push(backLeaf);
            } else {
	    		leafs.push(frontLeaf);
			    leafs.push(backLeaf);		    	
            }
			++portalIter;
		}
	}
	if(leafsForThisArea==0){
		currentArea--;
	}
	while(true){
		if (posAreasLeafs.empty())
			break;
		leaf = posAreasLeafs.top();
		posAreasLeafs.pop();

		if(!leaf->isVisible() && !leaf->isOpaque()){
			leafs.push(leaf);
			goto findThatArea;
		}
	}
	goto ret;

ret:    
	currentArea++;
	areaNo = currentArea;
}


void CFloodgate::alertForLeaking(CBspLeaf * leakLeaf)
{
	ofstream out(filePath.c_str(), ios::out | ios::trunc);
	CVector3 middlePointOfNode;
	list<CPortal *>::iterator portalIter, portalEnd;
	CPortal *portal;
	CBspLeaf *fLeaf, *bLeaf;
	CBspLeaf *currentLeaf;

	currentLeaf = leakLeaf;
	cout <<leakLeaf->getDepth()<< " LEAK DEPTH"<<endl;
	int lessNo = leakLeaf->getDepth()+1;
	int finish = 0;
	CPortal *minPortal = 0;
	bool first=true;

	vector<CBspLeaf *>::iterator li, le;
	li = startLeafs.begin();
	le = startLeafs.end();
	while(li!=le){
		(*li)->setDepth(1);
		++li;
	}
	while(true){
		if(currentLeaf->getDepth()<=finish)
			break;

		portalIter = currentLeaf->portalsBegin();
		portalEnd = currentLeaf->portalsEnd();
		//currentLeaf = 0;
		currentLeaf->setDepth(lessNo+1000);
		minPortal = 0;
		while(portalIter!=portalEnd){
			portal = *portalIter;
			fLeaf = portal->getLeaf(FRONT_LEAF);
			bLeaf = portal->getLeaf(BACK_LEAF);

			if(fLeaf->getDepth()<lessNo && fLeaf->getDepth()!=0) {
				//distanse = (currentLeaf->getMiddlePoint()-fLeaf->getMiddlePoint()).length();
				//if(distanse>maxD){
				currentLeaf = fLeaf;
				lessNo = fLeaf->getDepth()+1;
				minPortal = portal;
				//maxD = distanse;
				//}
			}
			if(bLeaf->getDepth()<lessNo && bLeaf->getDepth()!=0){
				//distanse = (currentLeaf->getMiddlePoint()-bLeaf->getMiddlePoint()).length();
				//if(distanse>maxD){
				currentLeaf = bLeaf;
				lessNo = bLeaf->getDepth()+1;
				minPortal = portal;
				//maxD = distanse;
				//}
			}

			++portalIter;
		}

		//if(minPortal && !first){
		if(minPortal){
			middlePointOfNode = minPortal->getMiddle();
			out << " \n";
			//for(int i=0;i<3;i++){
			out << " "<<middlePointOfNode[0];
			out << " "<<-middlePointOfNode[2];
			out << " "<<middlePointOfNode[1];                       
		} else {
			break;
		}
		first = false;
	}
	CVector3 mp = currentLeaf->getMiddlePoint();
	/*out << " \n";
	out << " "<<startPoint[0];
	out << " "<<-startPoint[2];
	out << " "<<startPoint[1];*/
	out << " \n";
	out << " "<<mp[0];
	out << " "<<-mp[2];
	out << " "<<mp[1];
	out.close();
}


bool CFloodgate::findBPPolygons(){    
	CBspLeaf *leaf;
	list <CBspLeaf *> nodesToCheck;
	vector <CBspLeaf *>::iterator i;

	list<CPolygon *>::iterator piter, pend;
	list<CPortal *>::iterator portalIter, portalEnd;

	CPolygon *pol;
	CBrushPlane *polBP;
	CPortal *portal;
	CBspLeaf *fLeaf, *bLeaf;

	for(i = startLeafs.begin() ; i != startLeafs.end() ; ++i){
		nodesToCheck.push_back(*i);
	}
	int currentDepth = 1;
	int nodeNo=0;
	size_t noToChangeDepth = 1;

	while (true) {
		if (nodesToCheck.size() == 0)
			break;

		leaf = nodesToCheck.front();
		nodesToCheck.pop_front();

		if(leaf->isVisible())
			continue;

		//NOT DEPTH ADDED TILL NOW OR BIGGER DEPTH ADDED
		if(!leaf->getDepth()){
			leaf->setDepth(currentDepth);
		}

		//keep tracking the path in case leak happen
		//NOT ANY MORE
		//leakPath.push(leaf);

		if(leaf->isLeak()){
			alertForLeaking(leaf);
			return false;
		}

		piter = leaf->pBegin();
		pend = leaf->pEnd();
		while(piter != pend){
			pol = *piter;
			polBP = pol->getBrushPlane();

			//it is touched       
			polBP->touch();

			if(polBP->isVisible()){
				++piter;
				continue;
			}
			polBP->setVisible(true);
			//sPolygons->push_back(polBP->getPolygon());
			++piter;
		}
		leaf->setVisible(true);
		portalIter = leaf->portalsBegin();
		portalEnd = leaf->portalsEnd();
		//int select = 3;
		while(portalIter!=portalEnd){
			portal = *portalIter;

			fLeaf = portal->getLeaf(FRONT_LEAF);
			bLeaf = portal->getLeaf(BACK_LEAF);
			if(!fLeaf->isVisible() && !fLeaf->isOpaque()){
				//if(!select)
				//  fLeaf->setDepth(1000);
				nodesToCheck.push_back(fLeaf);
				//--select;
			}
			if(!bLeaf->isVisible()&& !bLeaf->isOpaque()){
				nodesToCheck.push_back(bLeaf);
				// if(!select)
				//  fLeaf->setDepth(1000);
				// --select;
			}
			++portalIter;
		}


		nodeNo++;
		if(nodeNo == noToChangeDepth){
			noToChangeDepth = nodesToCheck.size();
			currentDepth++;
		}

	}
	return true;
}



