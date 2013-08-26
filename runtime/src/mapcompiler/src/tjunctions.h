/***************************************************************************
    copyright            : (C) 2002 by Vassili Tenente
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

#ifndef _TJUNCTIONS_H_
#define _TJUNCTIONS_H_

#include <vector>
#include <list>

#include "vector.h"
#include "vertex.h"
#include "plane.h"
using namespace std;

class CPolygon;

/**This class is to solve the holes that are generated
when a 
_____
  |   
  |
  |

happens at polygon splits
**/

struct CProVertex{
  CVertex v;
  vector<CProVertex *> after;
  vector<CProVertex *> before;
    vector<CProVertex *> neibor;
  bool snapped;
};

struct CProEdge{
  CProVertex *a, *b;
};

const CPlane planes[3] = 
{
    CPlane( CVector3(1, 0, 0)), 
    CPlane( CVector3(0, 1, 0)), 
    CPlane( CVector3(0, 0, 1))
};
const int planesNo = 3;


class CTJunctions{
  CPlane oneRPlane;
  vector <CProVertex *> vertices;
  vector<CProEdge *> edges;
  list<CPolygon *> *polygons;
  
    int addVertex(CVertex &v);
    void addPolygon(CPolygon *polygon);
    CProVertex *vertexToGraph(CVertex &v);
    bool moveItIfHaveTo(CProVertex *cpv);    
   int addEdge(CVertex &a, CVertex &b);    
    //void getThePolygons();
  public:
    CTJunctions();
    CTJunctions(list<CPolygon *> *pol);
    void CTJunctions::setPolygons(list<CPolygon *> *pol);
    void createGraph();
    void fixxTJunctions(); 
    CVertex &getVertex(const int id){return vertices[id]->v;}
    ~CTJunctions(){};
};

#endif //_TJUNCTIONS_H_

