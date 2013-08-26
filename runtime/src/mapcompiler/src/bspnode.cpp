/***************************************************************************
                          bspnode.cpp  -  description
                             -------------------
    begin                : Fri Dec 6 2002
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

#include "bspnode.h"

#include "polygon.h"
#include <iostream>
#include "portal.h"
#include "brush.h"
#include "entity.h"

#ifdef MEMPROF
#include "mmgr.h"
#endif

CBspNode::CBspNode()
{ 
    bounded = false;
    bounds.setBounds(CVector3(huge,huge,huge), CVector3(-huge,-huge,-huge));
};
    
CBspNode::CBspNode(list<CPolygon *> *polys, CBrushPlane *sp)
{
  bounded = false;  
  bounds.setBounds(CVector3(huge,huge,huge), CVector3(-huge,-huge,-huge));
  addPolygons(polys);
  setSplitBrushPlane(sp);
        //setPortalNo(pNo);
};

CBspNode::~CBspNode(){
}

void CBspNode::freePolygons(){
	int count = 0;
	for(list<CPolygon *>::iterator i = polygons.begin() ; i != polygons.end() ; ++i){
		if(*i)count++;
		delete *i;
	}
	cout << "Free polygons :" << count << endl;
}

list<CPolygon *> *CBspNode::getPolygons(){return &polygons; }

void CBspNode::addPolygon(CPolygon *polygon){        
  if(polygon->isPrior())
    polygons.push_front(polygon);
  else
    polygons.push_back(polygon);
    addPolygonBound(polygon);
};

void CBspNode::addPolygonWithOutBound(CPolygon *polygon){
  if(polygon->isPrior())
    polygons.push_front(polygon);
  else
    polygons.push_back(polygon);
};

void CBspNode::addBoundBox(const CAABoundBox &bbox){
  if(bounded){
    bounds.addPoint(bbox.getMin());
    bounds.addPoint(bbox.getMax());
  }
  else{
    bounds.setBounds(bbox.getMin(), bbox.getMax());
    bounded = true;
  }
}

bool CBspNode::isBuggyBound()
{
  if(!bounded)
    return true;
    
  for(int i=0;i<3;i++){
    if(bounds.getMax()[i]<bounds.getMin()[i])
      return true;
  }
  return false;
}

void CBspNode::addPolygonBound(CPolygon *polygon){
	// FIXME: Check if this is nessecary
	polygon->boundPolygon();
	addBoundBox(polygon->getPolBounds());
}

list<CPolygon *>::iterator CBspNode::pBegin()
{ 
    return polygons.begin();
}

list<CPolygon *>::iterator CBspNode::pEnd()
{ 
    return polygons.end();
} 
    
void CBspNode::addPolygons(list<CPolygon *> *pols){
  list<CPolygon *>::iterator pIter, pEnd;
  pIter = pols->begin();
  pEnd = pols->end();
  while(pIter!=pEnd){
    addPolygon(*pIter);
    ++pIter;
  }
};

bool CBspLeaf::isTrueLeaf(){
    if(area)
        return true;
    return (brushes.size() || sBrushes.size());
};
    
list<CPolygon *>::iterator CBspNode::polygonsBegin(){
  return polygons.begin();
}
list<CPolygon *>::iterator CBspNode::polygonsEnd(){
  return polygons.end();
}


CVector3 CBspNode::getMiddlePoint(){
    CVector3 min = bounds.getMin();
    CVector3 max = bounds.getMax();
    return (min+max)/2.0f;
}

void CBspNode::printBounds()
{
  cout << "bounds: "<<bounds.getMin().x<<" "<<bounds.getMin().y<<" "<<bounds.getMin().z<<endl;
}

void CBspLeaf::printStructuralBrushes()
{
  vector<int>::iterator s, send;
  s = sbBegin();
  send = sbEnd();
  cout <<"structural brushes:"<<endl;
  while(s!=send)
  {
    cout << *s<<endl;
    ++s;
  }
}

void CBspLeaf::addStructuralBrush(CBrush *brush, const int brushId){
      sBrushes.push_back(brushId);
        const CAABoundBox &box = brush->getBounds();
        addBoundBox(&box);
        allBrushes.push_back(brush);
        
        
       // brushes.push_back(brush);
      /*
      vector<CBrushPlane>::iterator bpIter, bpEnd;
      bpIter = brush->planesBegin();
      bpEnd = brush->planesEnd();
      CPolygon *polygon;
      while(bpIter!=bpEnd){        
        polygon = bpIter->getPolygon();

        assert(polygon );
        addBoundBox(polygon->boundPolygon());
        ++bpIter;
     }*/
}

void CBspLeaf::addDetailBrush(CBrush *brush, const int brushId, bool writeBounds){
     //CBrush *newBrush = new CBrush();
      //brushes.push_back(newBrush);
      assert(!brush->isStructural());
      sBrushes.push_back(brushId);
      brushes.push_back(brush);
      allBrushes.push_back(brush);
      
     /* //new code for structural subpolygon when a brush is over it
        if(brush->isOpaque()){
          list<CPolygon *>::iterator pi, pe;
          pi=polygonsBegin();
          pe=polygonsEnd();
          while(pi!=pe){
            if(brush->subPolygon((*pi))){
              erasePolygon(pi);
            }
            ++pi;
          }
        }
       */ 
      //detailBrushes.push_back(father);
      if(!writeBounds)
        return;
      const CAABoundBox &bbox = brush->getBounds();
      addBoundBox(&bbox);
}
    
void CBspLeaf::addPortal(CPortal *portal){
      portals.push_back(portal);
      addPolygonBound(portal);
}

//these one clears the portal polygons
void CBspLeaf::addLeafPolygons(list<CPolygon *> *pols){
        list<CPolygon *>::iterator pIter, pEnd;
        pIter = pols->begin();
        pEnd = pols->end();
        while(pIter!=pEnd){
          if((*pIter)->isLeak())
            leak = true;
          //if(!(*pIter)->isKeep()){
          //  addPolygonBound(*pIter);
          //  ++pIter;
          //  continue;
          // }
    
          addPolygon(*pIter);
          ++pIter;
        }
};

void CBspLeaf::addLeafPolygonsKeepKeep(list<CPolygon *> *pols){
        list<CPolygon *>::iterator pIter, pEnd;
        pIter = pols->begin();
        pEnd = pols->end();
        while(pIter!=pEnd){
          if((*pIter)->isLeak())
            leak = true;
          addPolygon(*pIter);
          ++pIter;
        }
};
    
    
list<CPortal *> &CBspLeaf::getPortals(){ return portals; } 
    
list<CPortal *>::iterator CBspLeaf::portalsBegin(){ return portals.begin(); }
list<CPortal *>::iterator CBspLeaf::portalsEnd(){ return portals.end(); }


CBspLeaf::~CBspLeaf(){
  vector<CBrush *>::iterator bIter, bEnd;
  bIter = brushes.begin();
  bEnd = brushes.end();
  while(bIter!=bEnd){
    if(!(*bIter)->isFromBegin()){
      (*bIter)->freeIt();
      delete *bIter;
      bIter = brushes.erase(bIter);
      --bIter;
    }
    ++bIter;
  }
}

void CBspLeaf::addLeaf(CBspLeaf *leaf)
{
    otherLeafs.push_back(leaf);
}

void CBspLeaf::appendYourBrushes(vector<CBrush *> &abrushes, CEntity* entity)
{
  /*int index = abrushes.size();
  abrushes.reserve(abrushes.size()+allBrushes.size());
  abrushes.reserve(abrushes.size()+allBrushes.size());
  CBrush *templData = (CBrush *)&abrushes[index];
          
   memcpy(templData,&(allBrushes[0]), allBrushes.size());*/
   /*vector<CBrush *>::iterator b, e;
   b = allBrushes.begin();
   e=allBrushes.end();
   while(b!=e){
     abrushes.push_back(*b);
     ++b;
   }*/
   vector<int>::iterator b, e;
   b=sbBegin();
   e=sbEnd();
   while(b!=e){
     abrushes.push_back(&entity->getBrush(*b));
     ++b;
   }
}

