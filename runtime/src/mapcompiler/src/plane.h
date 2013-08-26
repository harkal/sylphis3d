/***************************************************************************
                          plane.h  -  description
                             -------------------
    begin                : Fri Jun 7 2002
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
#ifndef _PLANE_H_
#define _PLANE_H_

#include <vector>

#include "vertex.h"

using namespace std;

class CPolygon;

class CPlane
{
    /** distanse of plane from 0,0,0*/
    float d;
    /** the vertival normalized normal of the plain*/
    CVector3 normal;

    int onAxe;                  // 0 means on X,  1 means on Y, 2 means on Z, -1 means unspecified      
    int factor;                 // if this is  1 means potitive, -1 negative
    int planeId;                //unique for each plane in the real world keep tracking the equal planes
  public:
    //static const float epsilon = 0.01f;

    enum eSide
    {
        FRONT,
        BACK,
        CROSS,
        ON
    };
    enum pRelation
    {
        EQUAL,
        OPOSITE,
        NONE
    };

      CPlane(const CVector3 points[3]);
      CPlane(void);
      CPlane(const CVector3 & norma, const float &distanse);
      CPlane(vector < CVertex > &vertices);
      CPlane(vector < CVector3 > &points);
      CPlane(const CVector3 & norma);

    float distanseFromVertex(const CVector3 & v) const {
        if (onAxe != -1)
            return factor * v[onAxe] - d;

        return (v * normal - d);
    }

    inline float distanseFromVertex(const CVertex & v) const {
        return distanseFromVertex(v.getRealCoor());
    }

    inline int getOnAxe() const {
        return onAxe;
    }

	void translate(const CVector3 &v);

    CVector3 projectVector(CVector3 & vector) const;
    float getDistanse(void) const;
    const CVector3 &getNorma(void) const;
	CVector3 &getNorma(void);
    void setDistanse(const float &distanse);
    void setNorma(const CVector3 & norma);

    //static CPlane::pRelation planesRelation(CPlane & p1, CPlane & p2);
    static CPlane::pRelation planesRelation(const CPlane &p1, const CPlane &p2);    
    
    int getPlaneId() const;
    void setPlaneId(const int& id);
    
    //on the same plane or oposite returns true
    inline bool operator == (const CPlane &p1) const {
      return (p1.planeId == planeId)||(p1.planeId == (planeId^1));
    }
    
    //vector<Vertex> getHugeQuadeOnIt( const int &a );
    CPlane *inversed() const ;
    
    int getFactor() const { return factor; }

    vector < CVector3 > textureAxis(void);
    ~CPlane(void){};
};

#ifndef _NOINLINE
#include "plane.inl"
#endif

#endif //  _PLANE_H  ///:~
