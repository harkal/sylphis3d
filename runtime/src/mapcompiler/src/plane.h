/***************************************************************************
                          plane.h  -  description
                             -------------------
    begin                : Fri Jun 7 2002
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
