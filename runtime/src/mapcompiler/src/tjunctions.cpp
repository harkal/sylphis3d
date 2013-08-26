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
#include <iostream>

#ifdef MEMPROF
#include "mmgr.h"
#endif

#include "tjunctions.h"
#include "polygon.h"


CTJunctions::CTJunctions():oneRPlane(CVector3(1, 0.3f, 0.2f)){
}

CTJunctions::CTJunctions(list<CPolygon *> *pol):oneRPlane(CVector3(1, 0.3f, 0.2f))
{
    setPolygons(pol);
}

void CTJunctions::setPolygons(list<CPolygon *> *pol){
  polygons = pol;
}

int CTJunctions::addVertex(CVertex &v)
{
    int noV = vertices.size();
    
    CVector3 &v1 = v.getRealCoor();
    
    for(int i=0;i<noV;i++){
      if(v1 == vertices[i]->v.getRealCoor()){
        return i;
      }
    }
    CProVertex *proV = new CProVertex();
    proV->v = v;
    proV->snapped=false;
    vertices.push_back(proV);
    return noV;
}

int CTJunctions::addEdge(CVertex &a, CVertex &b){
    /*float length = (b.getRealCoor()-a.getRealCoor()).length();
    int edgeNo = edges.size();
    CProEdge *edge;
    float edgeEpsilon=0.01f;
    for(int i=0;i<edgeNo;i++){
        edge = edges[i];
        
    }*/
    return 1;
}

CProVertex *CTJunctions::vertexToGraph(CVertex &v){
    int id = v.pvId();
    if(id == -1){
      id = addVertex(v);
    }
    v.setpvId(id);
    return vertices[id];
}

void CTJunctions::addPolygon(CPolygon *polygon)
{
  vector < CVertex > &pverts = polygon->getVertices();
  int noVerts;
  CPlane p;
  CVertex *previous;
  CVertex *next;
  CVertex *present;
  CProVertex *pre, *nex, *cur;
  noVerts = pverts.size();
  for(int i=0;i<noVerts;i++){
    previous = &pverts[(i-1+noVerts)%noVerts];
    next = &pverts[(i+1)%noVerts];
    present = &pverts[i];
    
    pre = vertexToGraph(*previous);
    previous = &pre->v;
    nex = vertexToGraph(*next);
    next = &nex->v;
    cur = vertexToGraph(*present);
    present = &cur->v;

    /*oneRPlane.setDistanse(oneRPlane.getNorma()*(present->getRealCoor()));
    float eps=0.01f;
    distanse = oneRPlane.distanseFromVertex(*previous);
    if(distanse>eps)
      cur->after.push_back(pre);
    else if(distanse<eps)
      cur->before.push_back(pre);
    
    distanse = oneRPlane.distanseFromVertex(*next);
    if(distanse>eps)
      cur->after.push_back(nex);
    else if(distanse<eps)
      cur->before.push_back(nex);
    */
    cur->before.push_back(pre);
    cur->after.push_back(nex);
    cur->neibor.push_back(pre);
    cur->neibor.push_back(pre);
    cur->neibor.push_back(nex);    
    //cur->before.push_back(nex);
    //cur->after.push_back(pre);
  }
  
}

void CTJunctions::fixxTJunctions(){
  //a,b,c stands for after, before & current
  vector <CProVertex *>::iterator citer, cend;
  citer=vertices.begin();
  cend = vertices.end();
  CProVertex *cpv;
  //for(int i=0;i<100;i++){
  while(citer!=cend){
    cpv = *citer;
    //if(!cpv->snapped){
      //if(moveItIfHaveTo(cpv))
        ;//cpv->snapped=true;
    //}
    ++citer;
  }
  //}
}

bool CTJunctions::moveItIfHaveTo(CProVertex *cpv){
  //i is the after
  vector <CProVertex *>::iterator i, aend;
  //j is the previous
  vector <CProVertex *>::iterator j, pend;
  
  CVector3 *bv, *cv, *av;
  CVector3 v1, v2, temp;
  float oneps = 0.001f;
  float v1Length, v2Length,dotp;
  cv = &cpv->v.getRealCoor();
  //for every previous and for every after of the previous
  // + for every after and for every previous of the after
    j = cpv->neibor.begin();
    pend = cpv->neibor.end();
    while(j!=pend){
      bv = &(*j)->v.getRealCoor();
      
      i = (*j)->neibor.begin();   
      aend = (*j)->neibor.end();
      while(i!=aend){
        av = &(*i)->v.getRealCoor();        
        
        v1 = ((*av)-(*cv));
        v1Length = v1.length();
        v1 = v1.normalized();
        v2 = ((*bv)-(*cv));
        v2Length = v2.length();
        v2 = v2.normalized();
        dotp = v1*v2;
        
        if((dotp>(-1.0f-oneps)) && (dotp < (-1.0f+oneps))){ // syneuthiaka
          temp = *av + ((*bv)-(*av))*(v1Length/(v1Length+v2Length));
          temp = (*cv-temp);
          //tempLength = temp.length();
          //if(tempLength<twops&&tempLength>twops){
           // ++i;
           // continue;
          //}
          if(!(*i)->snapped && !(*j)->snapped){
            (*i)->snapped=true;
            (*j)->snapped=true;
            (*i)->v.setRealCoor(*av+temp);
            (*j)->v.setRealCoor(*bv+temp);
            cpv->snapped=true;
          }
          else if(!(*i)->snapped && (*j)->snapped){
            (*i)->v.setRealCoor(*cv+((*cv)-(*bv))*(v1Length/v2Length));
            (*i)->snapped=true;
            //(*j)->snapped=true;
            cpv->snapped=true;
          }
          else if((*i)->snapped && !(*j)->snapped){
            (*j)->v.setRealCoor(*cv+((*cv)-(*av))*(v2Length/v1Length));
            //(*i)->snapped=true;
            (*j)->snapped=true;
            cpv->snapped=true;
          }
          else{
            if(!cpv->snapped){
              cpv->v.setRealCoor(*av + ((*bv)-(*av))*(v1Length/(v1Length+v2Length)));
              cpv->snapped=true;
            }
            else
              cout << "!!!warning could not snap vertex"<<endl;
          }
        }
        
        ++i;
      }
      ++j;
    } 
    return true;
  
}

void CTJunctions::createGraph(){
    list<CPolygon *>::iterator i, end = polygons->end() ;
 
    for(i = polygons->begin() ; i != end; ++i){
        addPolygon(*i);
    }
}

