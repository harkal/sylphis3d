/***************************************************************************
                          boundbox.cpp  -  description
                             -------------------
    begin                : Sun Jun 9 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@gmx.net
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

