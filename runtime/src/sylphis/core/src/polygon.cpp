/***************************************************************************
                          polygon.cpp  -  description
                             -------------------
    begin                : ??? ??? 7 2003
    copyright            : (C) 2003 by Harry Kalogirou
    email                : harkal@sylphis3d.com
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
