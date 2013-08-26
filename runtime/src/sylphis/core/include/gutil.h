/***************************************************************************
                          gutil.h  -  description
                             -------------------
    begin                : Wed Mar 20 2002
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

#ifndef _GUTIL_H_
#define _GUTIL_H_

#include <vector.h>

void calcTangentVector(const CVector3 &v0,
					   const CVector3 &v1,
					   const CVector3 &v2,
					   const CTexCoord &t0,
					   const CTexCoord &t1,
					   const CTexCoord &t2,
					   const CVector3 &n0,
					   CVector3 &tang);


void calcTangentVector(const CVector4 &v0,
					   const CVector4 &v1,
					   const CVector4 &v2,
					   const CTexCoord &t0,
					   const CTexCoord &t1,
					   const CTexCoord &t2,
					   const CVector3 &n0,
					   CVector3 &tang);

#ifdef _DEBUG
void checkGLError();
#else
#define checkGLError() 
#endif

void myglPopMatrix();
void myglPushMatrix();

F32 WrapToOne(F32 x);

#ifndef __GNUC__ //Already present in GCC
F64 random();
#endif

F64 randomInRange(F64 start, F64 stop);

bool isPowerOfTwo(U32 i);

#endif


