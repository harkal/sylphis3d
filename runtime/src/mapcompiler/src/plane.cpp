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

#include <cassert>

#include "polygon.h"
#include "plane.h"
#include "epsilon.h"

#ifdef MEMPROF
#include "mmgr.h"
#endif

#ifdef _NOINLINE
#include "plane.inl"
#endif

using namespace std;

CPlane::CPlane()
{
}

CPlane::CPlane(const CVector3 & norma, const float &distanse)
{
    setNorma(norma);
    d = distanse;
}

CPlane::CPlane(const CVector3 & norma)
{
    setNorma(norma.normalized());
}

CPlane::CPlane(vector < CVertex > &verts)
{
    assert(verts.size() > 2);

    setNorma(((verts[1].getRealCoor() -
               verts[0].getRealCoor()) ^ (verts[2].getRealCoor() -
                                          verts[0].getRealCoor())).
             normalized());
    d = (normal * verts[0].getRealCoor());
}

CPlane *CPlane::inversed() const {
    return new CPlane(-normal, -d);
}

CPlane::CPlane(const CVector3  points[3])
{
    setNorma(((points[1] - points[0]) ^ (points[2] -
                                         points[0])).normalized());
    d = (normal * points[0]);
}

CPlane::CPlane(vector < CVector3 > &points)
{
    assert(points.size() > 2);

    setNorma(((points[1] - points[0]) ^ (points[2] -
                                         points[0])).normalized());
    d = (normal * points[0]);
}

/*
CPlane::pRelation CPlane::planesRelation(CPlane & p1, CPlane & p2)
{
    if(
        fabs(p1.getNorma()[0] - p2.getNorma()[0]) < PLANES_ON_EPSILON
      &&fabs(p1.getNorma()[1] - p2.getNorma()[1]) < PLANES_ON_EPSILON
      &&fabs(p1.getNorma()[2] - p2.getNorma()[2]) < PLANES_ON_EPSILON
      &&fabs(p1.getDistanse() - p2.getDistanse()) < PLANES_ON_EPSILON
     )
        return CPlane::EQUAL;
    return CPlane::NONE;
}*/

CPlane::pRelation CPlane::planesRelation(const CPlane &p1, const CPlane &p2)
{   
    if(
        fabs(p1.getNorma()[0] - p2.getNorma()[0]) < PLANES_ON_EPSILON
      &&fabs(p1.getNorma()[1] - p2.getNorma()[1]) < PLANES_ON_EPSILON
      &&fabs(p1.getNorma()[2] - p2.getNorma()[2]) < PLANES_ON_EPSILON
      &&fabs(p1.getDistanse() - p2.getDistanse()) < PLANES_ON_EPSILON
     )
        return CPlane::EQUAL;
    return CPlane::NONE;
}

void CPlane::translate(const CVector3 &v){
	d = (normal * d  + v) * normal;
}

CVector3 CPlane::projectVector(CVector3 & vector) const {
    float esGin = normal * vector;
    CVector3 rv = vector - normal * esGin;
    //return vector - normal * distanseFromVertex(vector);
    return rv;
}

float CPlane::getDistanse(void) const {
    return d;
}

const CVector3 &CPlane::getNorma(void) const {
    return normal;
}

CVector3 &CPlane::getNorma(void) {
    return normal;
}

void CPlane::setDistanse(const float &newDistanse)
{
    d = newDistanse;
}

void CPlane::setNorma(const CVector3 & norma)
{
    normal = norma;
    int n = 0;
 
    for (int i = 0; i < 3; i++) {
        if (normal[i] < -PLANES_ON_EPSILON) {
            factor = -1;
            onAxe = i;
        } else if (normal[i] > PLANES_ON_EPSILON) {
            factor = 1;
            onAxe = i;
        } else
            ++n;

    }
    if (n != 2)
        onAxe = -1;             // 0 means on X,  1 means on Y, 2 means on Z, -1 means unspecified
}


vector < CVector3 > CPlane::textureAxis(void)
{
    vector < CVector3 > axis;
    axis.push_back(CVector3(0, 0, 1));
    axis.push_back(CVector3(1, 0, 0));
    axis.push_back(CVector3(0, -1, 0));
    axis.push_back(CVector3(0, 0, -1));
    axis.push_back(CVector3(1, 0, 0));
    axis.push_back(CVector3(0, -1, 0));
    axis.push_back(CVector3(1, 0, 0));
    axis.push_back(CVector3(0, 1, 0));
    axis.push_back(CVector3(0, 0, -1));
    axis.push_back(CVector3(-1, 0, 0));
    axis.push_back(CVector3(0, 1, 0));
    axis.push_back(CVector3(0, 0, -1));
    axis.push_back(CVector3(0, 1, 0));
    axis.push_back(CVector3(1, 0, 0));
    axis.push_back(CVector3(0, 0, -1));
    axis.push_back(CVector3(0, -1, 0));
    axis.push_back(CVector3(1, 0, 0));
    axis.push_back(CVector3(0, 0, -1));

	//for(int a=0;a<axis.size();a++){
	//	swap(&axis[a]);
	//}

    int bestAxis;
    float dp, best;
    int i;

    bestAxis = 0;
    best = 0.0f;
    
    CVector3 tempN(normal.x, -normal.z, normal.y);
	for (i = 0; i < 6; i++) {
        dp = tempN * axis[i * 3];
        if (dp > best) {
            best = dp;
            bestAxis = i;
        }
    }

    vector < CVector3 > rv;

    rv.push_back(axis[bestAxis * 3 + 1]);
    rv.push_back(axis[bestAxis * 3 + 2]);

    return rv;

}

void CPlane::setPlaneId(const int& id){
  planeId = id;
}

