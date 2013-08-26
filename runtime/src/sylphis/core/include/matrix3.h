/***************************************************************************
                          matrix3.h  -  description
                             -------------------
    begin                : Apr 22 2002
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

#ifndef _MATRIX3_H_
#define _MATRIX3_H_

#include "vector.h"
#include "plane.h"

typedef F32 FMatrix3[3][3];

class CMatrix3 {
public:
	CMatrix3();
	
	void zero();
	void identity();

	CMatrix3 &rotateZ(F32 rad);
	CMatrix3 &rotateY(F32 rad);
	CMatrix3 &rotateX(F32 rad);
	CMatrix3 &rotate(CVector3 &a);

	CMatrix3	getInverted();
	void	invert();
	CMatrix3 &transpose();
	
	CMatrix3& operator=  (const CMatrix3& b);
	CMatrix3& operator*= (CMatrix3& b);
	friend CMatrix3 operator* (const CMatrix3& a, const CMatrix3& b);
	friend CMatrix3 operator* (const CMatrix3& a, const F32 b);
	friend CVector3 operator* (const CMatrix3& a, const CVector3& b);
	friend CVector4 operator* (const CMatrix3& a, const CVector4& b);

	F32 m[3][3];
};

inline CVector3 operator* (const CMatrix3& a, const CVector3& b) {
	CVector3 v;
	
    v.x = (a.m[0][0] * b.x) + (a.m[1][0] * b.y) + (a.m[2][0] * b.z);
	v.y = (a.m[0][1] * b.x) + (a.m[1][1] * b.y) + (a.m[2][1] * b.z);
	v.z = (a.m[0][2] * b.x) + (a.m[1][2] * b.y) + (a.m[2][2] * b.z);

    return v;
}

#endif


