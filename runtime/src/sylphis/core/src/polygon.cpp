/***************************************************************************
                          polygon.cpp  -  description
                             -------------------
    begin                : ??? ??? 7 2003
    copyright            : (C) 2003 by Harry Kalogirou
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

#include <polygon.h>

CPolygon::CPolygon(){
}
CPolygon::~CPolygon(){
}

void CPolygon::addVertex(const CVector3 &v){
    mVertices.push_back(v);
}

const CPolygon::VertexVector &CPolygon::getVertices() const {
    return mVertices;
}

CPlane::eSide CPolygon::planeSide(const CPlane &p, F32 epsilon) const {
    bool front = false;
    bool back = false;

    for(S32 i = 0 ; i < mVertices.size() ; i++){
        const CVector3 &v = mVertices[i];
        F32 d = p.distance(v);
        if (d < -epsilon){
            if (front)
                return CPlane::CROSS;
            back = true;
        } else if (d > epsilon){
            if (back)
                return CPlane::CROSS;
            front = true;
        }
    }
    if (back)
        return CPlane::BACK;
    if (front)
        return CPlane::FRONT;
    return CPlane::ON;
}

CPlane::eSide CPolygon::splitByPlane(const CPlane &p,
                                    CPolygon &f,
                                    CPolygon &b,
                                    F32 epsilon) const {

    std::vector<F32> dists(mVertices.size());
    std::vector<CPlane::eSide> sides(mVertices.size());
    VertexVector::iterator vi, vend;

    S32 front = 0;
    S32 back = 0;
    S32 on = 0;

    for (S32 i = 0; i < mVertices.size() ; i++) {
        const CVector3 &v = mVertices[i];
        F32 dist = p.distance(v);
        dists[i] = dist;
        if (dist > epsilon) {
            sides[i] = CPlane::FRONT;
            front++;
        } else if (dist < -epsilon) {
            sides[i] = CPlane::BACK;
            back++;
        } else {
            sides[i] = CPlane::ON;
            on++;
        }
    }
    if (front == 0) {
        b = *this;
        return CPlane::BACK;
    }
    if (back == 0) {
        f = *this;
        return CPlane::FRONT;
    }
    for (S32 i = 0; i < mVertices.size() ; i++) {
        const CVector3 &v1 = mVertices[i];
        if (sides[i] == CPlane::FRONT) {
            f.addVertex(v1);
        } else if (sides[i] == CPlane::BACK) {
            b.addVertex(v1);
        } else if (sides[i] == CPlane::ON) {
            f.addVertex(v1);
            b.addVertex(v1);
            continue;
        }
        S32 nexti = (i + 1)%mVertices.size();
        if (sides[nexti] == CPlane::ON || sides[i] == sides[nexti])
            continue;

        const CVector3 &v2 = mVertices[nexti];
        F32 l = dists[i] / (dists[i] - dists[nexti]);
        CVector3 m = v1 + ((v2 - v1) * l);

        f.addVertex(m);
        b.addVertex(m);
    }
    return CPlane::CROSS;
}
