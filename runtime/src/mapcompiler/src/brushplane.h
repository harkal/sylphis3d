/***************************************************************************
                          brushplane.h  -  description
                             -------------------
    begin                : Sun Jun 9 2002
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


