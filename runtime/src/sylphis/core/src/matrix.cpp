/***************************************************************************
                          matrix.cpp  -  description
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
#include "matrix.h"
#include <math.h>
#include <memory.h>

#define ZERO_MATRIX(m)	memset((m), 0, sizeof((m)))
#define COPY_MATRIX(d,s)	memcpy((d), (s), sizeof((d)))



CMatrix4::CMatrix4() {
}

void CMatrix4::zero() {
	ZERO_MATRIX(this->m);
}

void CMatrix4::identity() {

	zero();
	
	this->m[0][0] = 1.0f;
	this->m[1][1] = 1.0f;
	this->m[2][2] = 1.0f;
	this->m[3][3] = 1.0f;
}

CMatrix4 &CMatrix4::rotateZ(F32 rad){
	F32 c,s;
	F32 Temp00, Temp01, Temp10, Temp11, Temp20, Temp21;

	s=(F32)sin(rad);
	c=(F32)cos(rad);

	Temp00=c*m[0][0]+s*m[0][1];
	Temp10=c*m[1][0]+s*m[1][1];
	Temp20=c*m[2][0]+s*m[2][1];
	Temp01=c*m[0][1]-s*m[0][0];
	Temp11=c*m[1][1]-s*m[1][0];
	Temp21=c*m[2][1]-s*m[2][0];

	m[0][0]=Temp00; m[0][1]=Temp01;
	m[1][0]=Temp10; m[1][1]=Temp11;
	m[2][0]=Temp20; m[2][1]=Temp21;
	
	return *this;
}

CMatrix4 &CMatrix4::rotateY(F32 rad){
	F32 c,s;
	F32 Temp00, Temp10, Temp20, Temp02, Temp12, Temp22;

	s=(F32)sin(rad);
	c=(F32)cos(rad);

	Temp00=c*m[0][0]+s*m[0][2];
	Temp10=c*m[1][0]+s*m[1][2];
	Temp20=c*m[2][0]+s*m[2][2];
	Temp02=c*m[0][2]-s*m[0][0];
	Temp12=c*m[1][2]-s*m[1][0];
	Temp22=c*m[2][2]-s*m[2][0];

	m[0][0]=Temp00; m[0][2]=Temp02;
	m[1][0]=Temp10; m[1][2]=Temp12;
	m[2][0]=Temp20; m[2][2]=Temp22;
	
	return *this;
}

CMatrix4 &CMatrix4::rotateX(F32 rad){
	F32 c,s;
	F32 Temp01, Temp11, Temp21, Temp02, Temp12, Temp22;

	s=(F32)sin(rad);
	c=(F32)cos(rad);

	Temp01=c*m[0][1]+s*m[0][2];
	Temp11=c*m[1][1]+s*m[1][2];
	Temp21=c*m[2][1]+s*m[2][2];
	Temp02=c*m[0][2]-s*m[0][1];
	Temp12=c*m[1][2]-s*m[1][1];
	Temp22=c*m[2][2]-s*m[2][1];

	m[0][1]=Temp01; m[0][2]=Temp02;
	m[1][1]=Temp11; m[1][2]=Temp12;
	m[2][1]=Temp21; m[2][2]=Temp22;
	
	return *this;
}

CMatrix4 &CMatrix4::rotate(CVector3 &a){
	F32 cos_b=(F32)cos(a.x);
	F32 sin_b=(F32)sin(a.x);
	F32 cos_c=(F32)cos(a.y);
	F32 sin_c=(F32)sin(a.y);
	F32 cos_a=(F32)cos(a.z);
	F32 sin_a=(F32)sin(a.z);

	m[0][0]=cos_a*cos_c-sin_a*sin_b*sin_c;
	m[0][1]=sin_a*cos_c+cos_a*sin_b*sin_c;
	m[0][2]=cos_b*sin_c;

	m[1][0]=-sin_a*cos_b;
	m[1][1]=cos_a*cos_b;
	m[1][2]=-sin_b;

	m[2][0]=-cos_a*sin_c-sin_a*sin_b*cos_c;
	m[2][1]=-sin_a*sin_c+cos_a*sin_b*cos_c;
	m[2][2]=cos_b*cos_c;
	
	return *this;
}

CMatrix4& CMatrix4::transpose() {
	#define matrix_swap(a,b) {F32 tmp=m[a][b]; m[a][b]=m[b][a]; m[b][a]=tmp;}
	matrix_swap(0,1);
	matrix_swap(0,2);
	matrix_swap(0,3);
	matrix_swap(1,2);
	matrix_swap(1,3);
	matrix_swap(2,3);
	#undef matrix_swap
	
	return *this;
}

void CMatrix4::setTransform(const CVector3& v){
	m[3][0] = v.x;
	m[3][1] = v.y;
	m[3][2] = v.z;
}

CMatrix4& CMatrix4::operator= (const CMatrix4& b) {
	memcpy(m, (b.m), sizeof(b.m));
	return *this;
}

CMatrix4& CMatrix4::operator*= (CMatrix4& b) {
	CMatrix4 temp = (*this) * b;
	*this = temp;
	return *this;
}

CMatrix4 operator*  (const CMatrix4& a_, const CMatrix4& b_) {
    CMatrix4 r;
#if 0
    r.m[0][0] = a_.m[0][0] * b_.m[0][0] + a_.m[0][1] * b_.m[1][0] + a_.m[0][2] * b_.m[2][0] + a_.m[0][3] * b_.m[3][0];
    r.m[0][1] = a_.m[0][0] * b_.m[0][1] + a_.m[0][1] * b_.m[1][1] + a_.m[0][2] * b_.m[2][1] + a_.m[0][3] * b_.m[3][1];
    r.m[0][2] = a_.m[0][0] * b_.m[0][2] + a_.m[0][1] * b_.m[1][2] + a_.m[0][2] * b_.m[2][2] + a_.m[0][3] * b_.m[3][2];
    r.m[0][3] = a_.m[0][0] * b_.m[0][3] + a_.m[0][1] * b_.m[1][3] + a_.m[0][2] * b_.m[2][3] + a_.m[0][3] * b_.m[3][3];

    r.m[1][0] = a_.m[1][0] * b_.m[0][0] + a_.m[1][1] * b_.m[1][0] + a_.m[1][2] * b_.m[2][0] + a_.m[1][3] * b_.m[3][0];
    r.m[1][1] = a_.m[1][0] * b_.m[0][1] + a_.m[1][1] * b_.m[1][1] + a_.m[1][2] * b_.m[2][1] + a_.m[1][3] * b_.m[3][1];
    r.m[1][2] = a_.m[1][0] * b_.m[0][2] + a_.m[1][1] * b_.m[1][2] + a_.m[1][2] * b_.m[2][2] + a_.m[1][3] * b_.m[3][2];
    r.m[1][3] = a_.m[1][0] * b_.m[0][3] + a_.m[1][1] * b_.m[1][3] + a_.m[1][2] * b_.m[2][3] + a_.m[1][3] * b_.m[3][3];

    r.m[2][0] = a_.m[2][0] * b_.m[0][0] + a_.m[2][1] * b_.m[1][0] + a_.m[2][2] * b_.m[2][0] + a_.m[2][3] * b_.m[3][0];
    r.m[2][1] = a_.m[2][0] * b_.m[0][1] + a_.m[2][1] * b_.m[1][1] + a_.m[2][2] * b_.m[2][1] + a_.m[2][3] * b_.m[3][1];
    r.m[2][2] = a_.m[2][0] * b_.m[0][2] + a_.m[2][1] * b_.m[1][2] + a_.m[2][2] * b_.m[2][2] + a_.m[2][3] * b_.m[3][2];
    r.m[2][3] = a_.m[2][0] * b_.m[0][3] + a_.m[2][1] * b_.m[1][3] + a_.m[2][2] * b_.m[2][3] + a_.m[2][3] * b_.m[3][3];

    r.m[3][0] = a_.m[3][0] * b_.m[0][0] + a_.m[3][1] * b_.m[1][0] + a_.m[3][2] * b_.m[2][0] + a_.m[3][3] * b_.m[3][0];
    r.m[3][1] = a_.m[3][0] * b_.m[0][1] + a_.m[3][1] * b_.m[1][1] + a_.m[3][2] * b_.m[2][1] + a_.m[3][3] * b_.m[3][1];
    r.m[3][2] = a_.m[3][0] * b_.m[0][2] + a_.m[3][1] * b_.m[1][2] + a_.m[3][2] * b_.m[2][2] + a_.m[3][3] * b_.m[3][2];
    r.m[3][3] = a_.m[3][0] * b_.m[0][3] + a_.m[3][1] * b_.m[1][3] + a_.m[3][2] * b_.m[2][3] + a_.m[3][3] * b_.m[3][3];
#else
	r.m[0][0] = a_.m[0][0] * b_.m[0][0] + a_.m[1][0] * b_.m[0][1] + a_.m[2][0] * b_.m[0][2] + a_.m[3][0] * b_.m[0][3];
    r.m[1][0] = a_.m[0][0] * b_.m[1][0] + a_.m[1][0] * b_.m[1][1] + a_.m[2][0] * b_.m[1][2] + a_.m[3][0] * b_.m[1][3];
    r.m[2][0] = a_.m[0][0] * b_.m[2][0] + a_.m[1][0] * b_.m[2][1] + a_.m[2][0] * b_.m[2][2] + a_.m[3][0] * b_.m[2][3];
    r.m[3][0] = a_.m[0][0] * b_.m[3][0] + a_.m[1][0] * b_.m[3][1] + a_.m[2][0] * b_.m[3][2] + a_.m[3][0] * b_.m[3][3];

    r.m[0][1] = a_.m[0][1] * b_.m[0][0] + a_.m[1][1] * b_.m[0][1] + a_.m[2][1] * b_.m[0][2] + a_.m[3][1] * b_.m[0][3];
    r.m[1][1] = a_.m[0][1] * b_.m[1][0] + a_.m[1][1] * b_.m[1][1] + a_.m[2][1] * b_.m[1][2] + a_.m[3][1] * b_.m[1][3];
    r.m[2][1] = a_.m[0][1] * b_.m[2][0] + a_.m[1][1] * b_.m[2][1] + a_.m[2][1] * b_.m[2][2] + a_.m[3][1] * b_.m[2][3];
    r.m[3][1] = a_.m[0][1] * b_.m[3][0] + a_.m[1][1] * b_.m[3][1] + a_.m[2][1] * b_.m[3][2] + a_.m[3][1] * b_.m[3][3];

    r.m[0][2] = a_.m[0][2] * b_.m[0][0] + a_.m[1][2] * b_.m[0][1] + a_.m[2][2] * b_.m[0][2] + a_.m[3][2] * b_.m[0][3];
    r.m[1][2] = a_.m[0][2] * b_.m[1][0] + a_.m[1][2] * b_.m[1][1] + a_.m[2][2] * b_.m[1][2] + a_.m[3][2] * b_.m[1][3];
    r.m[2][2] = a_.m[0][2] * b_.m[2][0] + a_.m[1][2] * b_.m[2][1] + a_.m[2][2] * b_.m[2][2] + a_.m[3][2] * b_.m[2][3];
    r.m[3][2] = a_.m[0][2] * b_.m[3][0] + a_.m[1][2] * b_.m[3][1] + a_.m[2][2] * b_.m[3][2] + a_.m[3][2] * b_.m[3][3];

    r.m[0][3] = a_.m[0][3] * b_.m[0][0] + a_.m[1][3] * b_.m[0][1] + a_.m[2][3] * b_.m[0][2] + a_.m[3][3] * b_.m[0][3];
    r.m[1][3] = a_.m[0][3] * b_.m[1][0] + a_.m[1][3] * b_.m[1][1] + a_.m[2][3] * b_.m[1][2] + a_.m[3][3] * b_.m[1][3];
    r.m[2][3] = a_.m[0][3] * b_.m[2][0] + a_.m[1][3] * b_.m[2][1] + a_.m[2][3] * b_.m[2][2] + a_.m[3][3] * b_.m[2][3];
    r.m[3][3] = a_.m[0][3] * b_.m[3][0] + a_.m[1][3] * b_.m[3][1] + a_.m[2][3] * b_.m[3][2] + a_.m[3][3] * b_.m[3][3];
#endif
    return r;
}

CMatrix4 operator*  (const CMatrix4& a, const F32 b) {
	CMatrix4 c;
	c.zero();

	for (S32 i=0; i<4; i++)
		for (S32 j=0; j<4; j++)
			for (S32 k=0; k<4; k++)
				c.m[i][j] += a.m[k][j] * b;
	return c;
}

CVector4 operator* (const CMatrix4& a, const CVector4& b) {
	CVector4 v;

    F32 m00 = a.m[0][0];
    F32 m01 = a.m[0][1];
    F32 m02 = a.m[0][2];
    F32 m03 = a.m[0][3];

    F32 m10 = a.m[1][0];
    F32 m11 = a.m[1][1];
    F32 m12 = a.m[1][2];
    F32 m13 = a.m[1][3];

    F32 m20 = a.m[2][0];
    F32 m21 = a.m[2][1];
    F32 m22 = a.m[2][2];
    F32 m23 = a.m[2][3];

    F32 m30 = a.m[3][0];
    F32 m31 = a.m[3][1];
    F32 m32 = a.m[3][2];
    F32 m33 = a.m[3][3];
	
	v.x = (m00 * b.x) + (m10 * b.y) + (m20 * b.z) + (m30 * b.w);
	v.y = (m01 * b.x) + (m11 * b.y) + (m21 * b.z) + (m31 * b.w);
	v.z = (m02 * b.x) + (m12 * b.y) + (m22 * b.z) + (m32 * b.w);
	v.w = (m03 * b.x) + (m13 * b.y) + (m23 * b.z) + (m33 * b.w);

    return v;
}

CMatrix4	CMatrix4::getInverted(){
	CMatrix4 outMatrix;
	F32 *out = (F32 *)&outMatrix.m[0][0];
	F32 *m = (F32 *)&this->m[0][0];

#define SWAP_ROWS(a, b) { F64 *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

  F64 wtmp[4][8];
  F64 m0, m1, m2, m3, s;
  F64 *r0, *r1, *r2, *r3;

  r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

  r0[0] = MAT(m,0,0), r0[1] = MAT(m,0,1),
  r0[2] = MAT(m,0,2), r0[3] = MAT(m,0,3),
  r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,

  r1[0] = MAT(m,1,0), r1[1] = MAT(m,1,1),
  r1[2] = MAT(m,1,2), r1[3] = MAT(m,1,3),
  r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,

  r2[0] = MAT(m,2,0), r2[1] = MAT(m,2,1),
  r2[2] = MAT(m,2,2), r2[3] = MAT(m,2,3),
  r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,

  r3[0] = MAT(m,3,0), r3[1] = MAT(m,3,1),
  r3[2] = MAT(m,3,2), r3[3] = MAT(m,3,3),
  r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

  /* choose pivot - or die */
  if (fabs(r3[0])>fabs(r2[0])) SWAP_ROWS(r3, r2);
  if (fabs(r2[0])>fabs(r1[0])) SWAP_ROWS(r2, r1);
  if (fabs(r1[0])>fabs(r0[0])) SWAP_ROWS(r1, r0);
  if (0.0 == r0[0]) {
    return *this;
  }

  /* eliminate first variable     */
  m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
  s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
  s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
  s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
  s = r0[4];
  if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
  s = r0[5];
  if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
  s = r0[6];
  if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
  s = r0[7];
  if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

  /* choose pivot - or die */
  if (fabs(r3[1])>fabs(r2[1])) SWAP_ROWS(r3, r2);
  if (fabs(r2[1])>fabs(r1[1])) SWAP_ROWS(r2, r1);
  if (0.0 == r1[1]) {
    return *this;
  }

  /* eliminate second variable */
  m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
  r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
  r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
  s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
  s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
  s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
  s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

  /* choose pivot - or die */
  if (fabs(r3[2])>fabs(r2[2])) SWAP_ROWS(r3, r2);
  if (0.0 == r2[2]) {
    return *this;
  }

  /* eliminate third variable */
  m3 = r3[2]/r2[2];
  r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
  r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
  r3[7] -= m3 * r2[7];

  /* last check */
  if (0.0 == r3[3]) {
    return *this;
  }

  s = 1.0/r3[3];              /* now back substitute row 3 */
  r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

  m2 = r2[3];                 /* now back substitute row 2 */
  s  = 1.0/r2[2];
  r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
  r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
  m1 = r1[3];
  r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
  r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
  m0 = r0[3];
  r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
  r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

  m1 = r1[2];                 /* now back substitute row 1 */
  s  = 1.0/r1[1];
  r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
  r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
  m0 = r0[2];
  r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
  r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

  m0 = r0[1];                 /* now back substitute row 0 */
  s  = 1.0/r0[0];
  r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
  r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

  MAT(out,0,0) = r0[4]; MAT(out,0,1) = r0[5],
  MAT(out,0,2) = r0[6]; MAT(out,0,3) = r0[7],
  MAT(out,1,0) = r1[4]; MAT(out,1,1) = r1[5],
  MAT(out,1,2) = r1[6]; MAT(out,1,3) = r1[7],
  MAT(out,2,0) = r2[4]; MAT(out,2,1) = r2[5],
  MAT(out,2,2) = r2[6]; MAT(out,2,3) = r2[7],
  MAT(out,3,0) = r3[4]; MAT(out,3,1) = r3[5],
  MAT(out,3,2) = r3[6]; MAT(out,3,3) = r3[7];

#undef MAT
#undef SWAP_ROWS

	return outMatrix;
}

void CMatrix4::invert(){
	CMatrix4 m = getInverted();
	*this = m;
}
