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


