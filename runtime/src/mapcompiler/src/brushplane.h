/***************************************************************************
                          brushplane.h  -  description
                             -------------------
    begin                : Sun Jun 9 2002
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
#ifndef _BRUSHLANE_H
#define _BRUSHLANE_H

#include <vector>

#include "vector.h"
using namespace std;

class CPolygon;
class CPlane;
class CShaderFlags;

class CBrushPlane {
public:  
	CBrushPlane();
	~CBrushPlane();

	CPolygon * getPolygon();
	void setPolygon(CPolygon *pol);
	CPlane * getPlane();
	void setPlane(CPlane *pl);

	void translate(const CVector3 &v);

	bool isVisible();
	void setVisible( bool v );

	void setShader(CShaderFlags *shaderFlags);
	CShaderFlags * getShader();

	float &getXOff();
	float &getYOff();
	float &getRotation();
	float &getScaleX();
	float &getScaleY();

	void setXOff(float &xo);
	void setYOff(float &yo);
	void setRotation(float &rot);
	void setScaleX(float &scx);
	void setScaleY(float &scy);

	void setBitValue1(const int &b);
	void setBitValue2(const int &b);
	void setBitValue3(const int &b);

	inline int getBrushId() const{return brushId;}
	inline void setBrushId(const int &bId){brushId=bId;}
	CVector3 & getXTextureMatrix();
	CVector3 & getYTextureMatrix();

	bool isLeak();
	void setTextureMatrix(const CVector3 matrice[2]);
	void calcTextureMatrix();

	//increase brakecounter
	void brake();
	//decrease brake counter
	void touch();

	bool canBeDetail();
	void merge(CPolygon *p);
	CPolygon *getMerged();
	CBrushPlane *getFather();
	void setFather(CBrushPlane *f);
	bool isFather();
private:
	float offset[2];

	float rotation;             //
	float scale[2];             // <------------------------
	//                         |
	//                         |
	//              |   textMatrix is these two prossesed together
	//              |
	CVector3 textMatrix[2];   //---------------

	int b1, b2, b3;

	CPolygon *polygon;
	CPlane   *plane;

	CShaderFlags *shader;

	//used by flood fill
	bool visible;

	//keep the number of brakes this face had
	int brakeCounter;

	CPolygon *merged;

	//the father brushplane //if zero it is the root
	CBrushPlane *father;
	int brushId;
};

#ifndef _NOINLINE
#include "brushplane.inl"
#endif

#endif


