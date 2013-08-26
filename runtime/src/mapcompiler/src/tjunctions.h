/***************************************************************************
    copyright            : (C) 2002 by Vassili Tenente
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

