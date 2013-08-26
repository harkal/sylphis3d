/***************************************************************************
                          frustum.cpp  -  description
                             -------------------
    begin                : Fri Aug 30 2002
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

#include <frustum.h>
#include <boundbox.h>
#include <assert.h>
#include <serializer.h>

IMPLEMENT_SERIALIZABLE(CFrustum);

CFrustum::CFrustum(){
	mPlanes.resize(6);
}
CFrustum::~CFrustum(){
}

void CFrustum::makeFrustumPlanes(CMatrix4 &m){

    //m.transpose();
    F32 *c = &m.m[0][0];
   // Extract the RIGHT plane
   mPlanes[PRIGHT].normal.x = c[3]-c[0];
   mPlanes[PRIGHT].normal.y = c[7]-c[4];
   mPlanes[PRIGHT].normal.z = c[11]-c[8];
   mPlanes[PRIGHT].dist = c[15]-c[12];

   // Extract the LEFT plane
   mPlanes[PLEFT].normal.x = c[3]+c[0];
   mPlanes[PLEFT].normal.y = c[7]+c[4];
   mPlanes[PLEFT].normal.z = c[11]+c[8];
   mPlanes[PLEFT].dist = c[15]+c[12];

   // Extract the BOTTOM plane
   mPlanes[PBOTTOM].normal.x = c[ 3] + c[ 1];
   mPlanes[PBOTTOM].normal.y = c[ 7] + c[ 5];
   mPlanes[PBOTTOM].normal.z = c[11] + c[ 9];
   mPlanes[PBOTTOM].dist = c[15] + c[13];

   // Extract the TOP plane
   mPlanes[PTOP].normal.x = c[ 3] - c[ 1];
   mPlanes[PTOP].normal.y = c[ 7] - c[ 5];
   mPlanes[PTOP].normal.z = c[11] - c[ 9];
   mPlanes[PTOP].dist = c[15] - c[13];

   // Extract the FAR plane
   mPlanes[PFAR].normal.x = c[ 3] - c[ 2];
   mPlanes[PFAR].normal.y = c[ 7] - c[ 6];
   mPlanes[PFAR].normal.z = c[11] - c[10];
   mPlanes[PFAR].dist = c[15] - c[14];

   // Extract the NEAR plane
#if 1
   mPlanes[PNEAR].normal.x = c[ 3] + c[ 2];
   mPlanes[PNEAR].normal.y = c[ 7] + c[ 6];
   mPlanes[PNEAR].normal.z = c[11] + c[10];
   mPlanes[PNEAR].dist = c[15] + c[14];
#endif

   for(S32 i = 0 ; i < 6 ; i++){
        F32 l = mPlanes[i].normal.length();
        mPlanes[i].normal /= -l;
        mPlanes[i].dist /= l;
   }
    //m.transpose();  
}

void CFrustum::pushBackPlane(const CPlane &p){
	mPlanes.push_back(p);
}

void CFrustum::popBackPlanes(S32 i){
	//mPlanes.resize(mPlanes.size() - i);
	for(S32 t = 0 ; t < i ; t++)
		mPlanes.pop_back();
}

CPlane &CFrustum::getPlane(S32 t){
	return mPlanes[t];
}

bool CFrustum::isCulled(CAABoundBox &b){
    F32 dot;
	tPlaneList::iterator i;
    
	for(i = mPlanes.begin() ; i != mPlanes.end() ; i++){
        dot = -(*i).dist;
        if((*i).normal.x > 0)
            dot += (*i).normal.x * b.getMin().x;
          else dot += (*i).normal.x * b.getMax().x;

        if((*i).normal.y > 0)
            dot += (*i).normal.y * b.getMin().y;
          else dot += (*i).normal.y * b.getMax().y;

        if((*i).normal.z > 0)
            dot += (*i).normal.z * b.getMin().z;
          else dot += (*i).normal.z * b.getMax().z;

        if(dot > 0)return true;
    }
    
    return false;
}

F32 CFrustum::distance(const CPlane &p) const {
	assert(0 && "Not implemented");
	return 0;
}

F32 CFrustum::distance(const CVector3 &v) const {
	assert(0 && "Not implemented");
	return 0;
}

bool CFrustum::collide(CBound *b) {
	if(b->getType() == AABOX){
		CAABoundBox *bb = static_cast<CAABoundBox *>(b);
		return !isCulled(*bb);
	}
	return false;
}

void CFrustum::translate(const CVector3 &v){
    NOT_SUPPORTED;
}

void CFrustum::save(CSerializer *ser) const {
	CBound::save(ser);
	ser->writeContainerSimple(mPlanes);
}

void CFrustum::load(CSerializer *ser){
	CBound::load(ser);
	ser->readContainerSimple(mPlanes);
}

