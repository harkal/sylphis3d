/***************************************************************************
                     splineinterpolator.cpp  -  description
                             -------------------
    begin                : Thu Apr 15 2004
    copyright            : (C) 2004 by Harry Kalogirou
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

#include <splineinterpolator.h>
#include <tokenizer.h>

CSplineInterpolatorVector3::CSplineInterpolatorVector3(){
    // Set up matrix
    // Hermite polynomial
    FMatrix4 &m = mCoeffs.m;
    m[0][0] = 2;
    m[0][1] = -2;
    m[0][2] = 1;
    m[0][3] = 1;
    m[1][0] = -3;
    m[1][1] = 3;
    m[1][2] = -2;
    m[1][3] = -1;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 1;
    m[2][3] = 0;
    m[3][0] = 1;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 0;
    
    mDirty =false;
}
CSplineInterpolatorVector3::~CSplineInterpolatorVector3(){
}

void CSplineInterpolatorVector3::insert(CVector3 &node, F32 nodeKey){
    CInterpolatorVector3::insert(node, nodeKey);
    mDirty = true;
}

void CSplineInterpolatorVector3::calcTangents(){
        // Catmull-Rom
        S32 i, nunodes;
        bool isClosed;

        nunodes = nodes.size();
        if (nunodes < 2){
            return;
        }

        if (nodes[0] == nodes[nunodes-1]){
            isClosed = true;
        } else {
            isClosed = false;
        }

        mTangents.resize(nunodes);

        for(i = 0; i < nunodes; ++i){
            if (i ==0){
                // Special case start
                if (isClosed) {
                    // Use nunodes-2 since nunodes-1 is the last point and == [0]
                    mTangents[i] = (nodes[1] - nodes[nunodes-2]) * 0.5;
                } else {
                    mTangents[i] = (nodes[1] - nodes[0]) * 0.5;
                }
            } else if (i == nunodes-1){
                // Special case end
                if (isClosed){
                    // Use same tangent as already calculated for [0]
                    mTangents[i] = mTangents[0];
                }else{
                    mTangents[i] = (nodes[i] - nodes[i-1]) * 0.5;
                }
            }else{
                mTangents[i] = (nodes[i+1] - nodes[i-1]) * 0.5;
            }
        }

    mDirty = false;
}

CVector3 CSplineInterpolatorVector3::intepolate(S32 index, F32 t){
        if ((index + 1) == nodes.size()){
            return nodes[index];
        }

        if (t == 0.0f){
            return nodes[index];
        }else if(t == 1.0f){
            return nodes[index + 1];
        }

        F32 t2, t3;
        t2 = t * t;
        t3 = t2 * t;
        CVector4 powers(t3, t2, t, 1);

        // ret = powers * mCoeffs * (point1, point2, tangent1, tangent2)
        CVector3& point1 = nodes[index];
        CVector3& point2 = nodes[index+1];
        CVector3& tan1 = mTangents[index];
        CVector3& tan2 = mTangents[index+1];
        CMatrix4 mpt;
        FMatrix4 &pt = mpt.m;

        pt[0][0] = point1.x;
        pt[0][1] = point1.y;
        pt[0][2] = point1.z;
        pt[0][3] = 1.0f;
        pt[1][0] = point2.x;
        pt[1][1] = point2.y;
        pt[1][2] = point2.z;
        pt[1][3] = 1.0f;
        pt[2][0] = tan1.x;
        pt[2][1] = tan1.y;
        pt[2][2] = tan1.z;
        pt[2][3] = 1.0f;
        pt[3][0] = tan2.x;
        pt[3][1] = tan2.y;
        pt[3][2] = tan2.z;
        pt[3][3] = 1.0f;

        CVector4 ret = mCoeffs * mpt * powers;
       
        return CVector3(ret.x, ret.y, ret.z);
}

CVector3 CSplineInterpolatorVector3::getAt(F32 key){
    S32 i;

    if(mDirty)
        calcTangents();

	if(keys.size() == 0)
		return CVector3(0.0, 0.0, 0.0);

	if(key < keys[0])
		return nodes[0];
		
	for(i = 0 ; i < keys.size() - 1 ; i++){
		if(key >= keys[i] && key < keys[i+1]){
			F32 s = keys[i];
			F32 e = keys[i+1];
			
			if(s == e)return nodes[i];
			
			F32 f = (key - s) / (e - s);
			return intepolate(i, f);
		}
	}
	
	return nodes[nodes.size() - 1];
}
