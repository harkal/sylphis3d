/***************************************************************************
                          vertex.cpp  -  description
                             -------------------
    begin                : Fri Jun 7 2002
    copyright            : (C) 2002 by  Vassili Tenente
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

#include "vertex.h"



using namespace std;

CVertex::CVertex(void)
{
  rvId = -1;
}

CVertex::CVertex(const float &x, const float &y, const float &z)
{
    real_coor.x = x;
    real_coor.y = y;
    real_coor.z = z;
    rvId = -1;
}

//sets the real coor by vector
void CVertex::setRealCoor(const CVector3 & realCoor)
{
    real_coor = realCoor;
}


//get the real coors individual
float CVertex::getRealX(void) const {
    return real_coor.x;
}

float CVertex::getRealY(void) const {
    return real_coor.y;
}

float CVertex::getRealZ(void) const {
    return real_coor.z;
}


//  sets the real coor individual
void CVertex::setRealX(const float &x)
{
    real_coor.x = x;
}

void CVertex::setRealY(const float &y)
{
    real_coor.y = y;
}

void CVertex::setRealZ(const float &z)
{
    real_coor.z = z;
}

void CVertex::setU(const float u)
{
    UV[0] = u;
}

void CVertex::setV(const float v)
{
    UV[1] = v;
}

float CVertex::getU() const {
    return UV[0];
}

float CVertex::getV() const {
    return UV[1];
}

float CVertex::getUV(int i) const {
    return UV[i];
}

float* CVertex::getTheUV(){
    return UV;
}

float &CVertex::getXYZ(const int &xyz)
{
    return real_coor[xyz];
}
void CVertex::setXYZ(const int &index, const float &xyz)
{
    real_coor[index] = xyz;
}

void CVertex::print()
{
   cout << "(" << real_coor.x<< "," << real_coor.y<< "," << real_coor.z<<")" << endl;
};

