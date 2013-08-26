/***************************************************************************
                          boundbox.cpp  -  description
                             -------------------
    begin                : Sun Jun 9 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
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

#include <math.h>

#include "boundbox.h"
#include <plane.h>

IMPLEMENT_SERIALIZABLE(CAABoundBox);

CAABoundBox::CAABoundBox(){
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

CVector3 &CAABoundBox::getMin(){
    return mMin;
}

void CAABoundBox::setMax(const CVector3 &max){
    mMax = max;
}

CVector3 &CAABoundBox::getMax(){
    return mMax;
}

F32 CAABoundBox::distance(const CPlane &p) const {
    CVector3 v;
    for(S32 i = 0 ; i < 3 ; i++){
        if(p.normal[i] < 0)
            v[i] = mMax[i];
        else
            v[i] = mMin[i];
    }
    return p.distance(v);
}

F32 CAABoundBox::distance(const CVector3 &v) const {
    F32 sqrDist = 0;
    F32 f;
    for(S32 i = 0 ; i < 3 ; i++){
        if(v[i] < mMin[i]){
            f = v[i] - mMin[i];
            sqrDist += f * f;
        } else if (v[i] > mMax[i]){
            f = v[i] - mMax[i];
            sqrDist += f * f;
        }
    }
    return sqrt(sqrDist);
}

bool CAABoundBox::isBehind(const CPlane &p){
    return false;
}

void CAABoundBox::translate(const CVector3 &v){
    mMin += v;
    mMax += v;
}

void CAABoundBox::addPoint(const CVector3 &v){
    if(mMin.x > v.x)mMin.x = v.x;
    if(mMax.x < v.x)mMax.x = v.x;

    if(mMin.y > v.y)mMin.y = v.y;
    if(mMax.y < v.y)mMax.y = v.y;

    if(mMin.z > v.z)mMin.z = v.z;
    if(mMax.z < v.z)mMax.z = v.z;
}

void CAABoundBox::addBound(CAABoundBox &bbox){
    /*CVector3 &v = bbox.getMin();
    if(mMin.x > v.x )mMin.x = v.x;
    if(mMin.y > v.y)mMin.y = v.y;
    if(mMin.z > v.z)mMin.z = v.z;

    v = bbox.getMax();
    if(mMax.x < v.x)mMax.x = v.x;
    if(mMax.y < v.y)mMax.y = v.y;
    if(mMax.z < v.z)mMax.z = v.z;*/
    addPoint(bbox.getMin());
    addPoint(bbox.getMax());
}

bool CAABoundBox::collide(CBound *b){
	if(b->getType() == AABOX){
		CAABoundBox *bb = static_cast<CAABoundBox *>(b);
		for(S32 i = 0 ; i < 3 ; i++){
			if(mMin[i] > bb->getMax()[i] || mMax[i] < bb->getMin()[i])
				return false;        
		}
		return true;
	}
    return false;
}

bool CAABoundBox::collide(const CVector3 &v){
	for(S32 i = 0 ; i < 3 ; i++){
		if(mMin[i] > v[i] || mMax[i] < v[i])
			return false;        
	}
	return true;
}

#include <renderer.h>

void CAABoundBox::render(CRenderer *r){
	//CVector3 

	//	r->GLVertex3(mMin.x,mMin.y,mMin.z);
	//	r->GLVertex3(mMax.x,mMax.y,mMax.z);
	//	r->GLEnd();

	r->GLBeginLine();
	//Top plane
	r->GLVertex3(mMax[0],mMax[1],mMax[2]);
	r->GLVertex3(mMin[0],mMax[1],mMax[2]);

	r->GLVertex3(mMax[0],mMax[1],mMax[2]);
	r->GLVertex3(mMax[0],mMin[1],mMax[2]);

	r->GLVertex3(mMin[0],mMin[1],mMax[2]);
	r->GLVertex3(mMin[0],mMax[1],mMax[2]);

	r->GLVertex3(mMin[0],mMin[1],mMax[2]);
	r->GLVertex3(mMax[0],mMin[1],mMax[2]);

	//Bottom plane
	r->GLVertex3(mMax[0],mMax[1],mMin[2]);
	r->GLVertex3(mMin[0],mMax[1],mMin[2]);

	r->GLVertex3(mMax[0],mMax[1],mMin[2]);
	r->GLVertex3(mMax[0],mMin[1],mMin[2]);

	r->GLVertex3(mMin[0],mMin[1],mMin[2]);
	r->GLVertex3(mMin[0],mMax[1],mMin[2]);

	r->GLVertex3(mMin[0],mMin[1],mMin[2]);
	r->GLVertex3(mMax[0],mMin[1],mMin[2]);

	//Sides
	r->GLVertex3(mMin[0],mMax[1],mMin[2]);
	r->GLVertex3(mMin[0],mMax[1],mMax[2]);

	r->GLVertex3(mMax[0],mMax[1],mMin[2]);
	r->GLVertex3(mMax[0],mMax[1],mMax[2]);

	r->GLVertex3(mMin[0],mMin[1],mMin[2]);
	r->GLVertex3(mMin[0],mMin[1],mMax[2]);

	r->GLVertex3(mMax[0],mMin[1],mMin[2]);
	r->GLVertex3(mMax[0],mMin[1],mMax[2]);
	r->GLEnd();

}

void CAABoundBox::save(CSerializer *ser) const {
	CBound::save(ser);
	mMin.save(ser);
	mMax.save(ser);
}

void CAABoundBox::load(CSerializer *ser){
	CBound::load(ser);
	mMin.load(ser);
	mMax.load(ser);
}

