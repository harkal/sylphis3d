/***************************************************************************
                          boundbox.cpp  -  description
                             -------------------
    begin                : Sun Jun 9 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@gmx.net
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
#include "boundbox.h"
#include "plane.h"
#include "epsilon.h"

#ifdef MEMPROF
#include "mmgr.h"
#endif

const int minBound=10000;
CAABoundBox::CAABoundBox(const bool init){
   /* if(init){
      mMin.x=minBound;
      mMin.y=minBound;
      mMin.z=minBound;
      mMax.x=-minBound;
      mMax.y=-minBound;
      mMax.z=-minBound;
    }*/
}

CAABoundBox::~CAABoundBox(){
}

CAABoundBox::CAABoundBox(const CVector3 &v){
    setMin(v);
    setMax(v);
}

void CAABoundBox::setBounds(const CVector3 &min, const CVector3 &max){
    mMin = min;
    mMax = max;
}

void CAABoundBox::setMin(const CVector3 &min){
    mMin = min;
}

const CVector3 &CAABoundBox::getMin() const {
    return mMin;
}

void CAABoundBox::setMax(const CVector3 &max){
    mMax = max;
}

const CVector3 &CAABoundBox::getMax() const {
    return mMax;
}


void CAABoundBox::translate(const CVector3 &v){
    mMin += v;
    mMax += v;
}

void CAABoundBox::addPoint(const CVector3 &v){
    if(mMin.x > v.x)mMin.x = v.x;
    if(mMin.y > v.y)mMin.y = v.y;
    if(mMin.z > v.z)mMin.z = v.z;

    if(mMax.x < v.x)mMax.x = v.x;
    if(mMax.y < v.y)mMax.y = v.y;
    if(mMax.z < v.z)mMax.z = v.z;
}

bool CAABoundBox::isAllIn(const CAABoundBox &b) const{
    for(int i = 0 ; i < 3 ; i++){
//        if((mMin[i]-INBOUNDS_EPSILON) > b.getMax()[i] || (mMax[i]+INBOUNDS_EPSILON) < b.getMin()[i])        
     //  if(mMin[i] > b.getMax()[i] || mMax[i] < b.getMin()[i])
        if(mMin[i] > b.getMax()[i] || mMax[i] < b.getMin()[i])
            return false;
        if(mMin[i] < b.getMin()[i] || mMax[i] > b.getMax()[i])
          return false;
    }
    return true;
}

bool CAABoundBox::collide(const CAABoundBox &b) const{
    for(int i = 0 ; i < 3 ; i++){
        if(mMin[i] > b.getMax()[i] || mMax[i] < b.getMin()[i])
            return false;        
    }
    return true;
}

bool CAABoundBox::collide(const CVector3 &v) const {
    for(int i = 0 ; i < 3 ; i++){
        if(mMin[i] > v[i] || mMax[i] < v[i])
            return false;        
    }
    return true;    
}

void CAABoundBox::expand(float e){
    mMax = mMax + e;
    mMin = mMin - e;
}

