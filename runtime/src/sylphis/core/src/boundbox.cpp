/***************************************************************************
                          boundbox.cpp  -  description
                             -------------------
    begin                : Sun Jun 9 2002
    copyright            : (C) 2002 by Harry Kalogirou
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

