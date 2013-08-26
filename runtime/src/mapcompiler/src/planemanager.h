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

#ifndef _PLANEMANAGER_H_
#define _PLANEMANAGER_H_

#include <vector>


using namespace std;

class CPlane;
class CVector3;
class CVertex;

class CPlaneManager{
  
  private:
    typedef vector<CPlane *> PlaneVector;
    PlaneVector planes;
    PlaneVector writePlanes;
    //keeps where a plane is written or -1  if not written
    vector<int> isWritten;
    
  public:
    CPlaneManager();
    ~CPlaneManager();
	void reset();
    void freePlanes();
    int addPlane(const CPlane &plane);
    int addPlane(vector < CVector3 > &points);
    int addPlane(vector < CVertex > &points);
    int addPlane(float d, CVector3 norma);
    int getPlaneId(CPlane *plane);
    int addPlane(CVector3 points[3]);
    size_t countPlanes(){return planes.size();};
    CPlane *getPlane(const int &id);
    CPlane *inversed(CPlane *plane);
    CPlane *inversed(const int &id);
    
    void initForWritting();
    int getWrittingPlaneId(const int &planeId);
    size_t countWritePlanes(){return writePlanes.size();};
    
    vector<CPlane *>::iterator planesBegin(){ return planes.begin();};
    vector<CPlane *>::iterator planesEnd(){return planes.end();};
    
    vector<CPlane *>::iterator writePlanesBegin(){ return writePlanes.begin();};
    vector<CPlane *>::iterator writePlanesEnd(){return writePlanes.end();};
};

#endif //_PLANEMANAGER_H_


