/***************************************************************************
                          quaternion.h  -  description
                             -------------------
    begin                : Mon Jun 3 2002
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

#ifndef QUATERNION_H
#define QUATERNION_H

#include <platform/types.h>

class CVector3;
class CMatrix3;
class CMatrix4;
class CSerializer;

class CQuaternion {
public: 
	CQuaternion(F32 _x = 0, F32 _y = 0, F32 _z = 0, F32 _w = 1.0);
	CQuaternion(const CQuaternion &q);
	~CQuaternion();

	CQuaternion operator+ (const CQuaternion& q);
	CQuaternion operator- (const CQuaternion& q);
	CQuaternion operator* (const CQuaternion& q) const;
	CQuaternion operator* (F32 f);
//	friend CQuaternion operator* (F32 f, const CQuaternion& q);
	CQuaternion operator- ();

	CVector3 operator* (const CVector3& v) const;

	CQuaternion &invert();
	void toRotationMatrix (CMatrix4& mat) const;
    void toRotationMatrix (CMatrix3& mat) const;
    void fromRotationMatrix (CMatrix4 &mat);
	void fromAngleAxis (const CVector3& axis, const F32 angle);
    void fromAxes (const CVector3& xAxis, const CVector3& yAxis, const CVector3& zAxis);

    void swapHand();

    void lerp(F32 f, const CQuaternion& q);

    F32 x;
    F32 y;
    F32 z;
    F32 w;

	static CQuaternion ZERO;
	static CQuaternion IDENTITY;

	void save(CSerializer *ser) const;
	void load(CSerializer *ser);
};

#endif
