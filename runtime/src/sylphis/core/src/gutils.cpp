/***************************************************************************
                          gutil.cpp  -  description
                             -------------------
    begin                : Wed Mar 20 2002
    copyright            : (C) 2002 by Harry Kalogirou
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

#include <assert.h>
#include <vector.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <openglext.h>

#include <console.h>

#define TAN_1

#ifdef TAN_1
#define EPSILON 1.0e-7
#define DcVecSubtract(r, a, b)       ((r)[0] = (a)[0] - (b)[0], (r)[1] = (a)[1] - (b)[1], (r)[2] = (a)[2] - (b)[2])
void calcTangentVector(const CVector3 &v0,
                       const CVector3 &v1,
                       const CVector3 &v2,
                       const CTexCoord &t0,
                       const CTexCoord &t1,
                       const CTexCoord &t2,
                       const CVector3 &n0,
                       CVector3 &tang)
{
   CVector3 tangent;
   typedef struct {
      F64    v[3];
      F64    t[2];
   } tanSpaceVert;
   
   tanSpaceVert   verts[3];
   tanSpaceVert   tempVert;
   F64          dot;
   F64          interp;
   F64          tempVector[3];

      //========================================//
      // fill in the tangent space vertex array //
      //========================================//
      verts[0].v[0] = v0[0];
      verts[0].v[1] = v0[1];
      verts[0].v[2] = v0[2];
      verts[0].t[0] = t0[0];
      verts[0].t[1] = t0[1];

      verts[1].v[0] = v1[0];
      verts[1].v[1] = v1[1];
      verts[1].v[2] = v1[2];
      verts[1].t[0] = t1[0];
      verts[1].t[1] = t1[1];

      verts[2].v[0] = v2[0];
      verts[2].v[1] = v2[1];
      verts[2].v[2] = v2[2];
      verts[2].t[0] = t2[0];
      verts[2].t[1] = t2[1];

      //============================//
      // compute the tangent vector //
      //============================//
      {
         //=======================================================//
         // sort the vertices based on their t texture coordinate //
         //=======================================================//
         if (verts[0].t[1] < verts[1].t[1])
         {
            tempVert = verts[0];
            verts[0] = verts[1];
            verts[1] = tempVert;
         }
         if (verts[0].t[1] < verts[2].t[1])
         {
            tempVert = verts[0];
            verts[0] = verts[2];
            verts[2] = tempVert;
         }
         if (verts[1].t[1] < verts[2].t[1])
         {
            tempVert = verts[1];
            verts[1] = verts[2];
            verts[2] = tempVert;
         }

         //=======================================================================//
         // compute the parametric offset along edge02 to the middle t coordinate //
         //=======================================================================//
         if (fabs((verts[2].t[1] - verts[0].t[1])) < EPSILON)
         {
            interp = 1.0;
         }
         else
         {
            interp = (verts[1].t[1] - verts[0].t[1]) / (verts[2].t[1] - verts[0].t[1]);
         }

         //============================================================================//
         // use the interpolation paramter to compute the vertex position along edge02 //
         // that coresponds to the same t coordinate as v[1]                           //
         //============================================================================//
#define DcVecAddTo(a, b)             ((a)[0] += (b)[0], (a)[1] += (b)[1], (a)[2] += (b)[2])
#define DcVecSubtract(r, a, b)       ((r)[0] = (a)[0] - (b)[0], (r)[1] = (a)[1] - (b)[1], (r)[2] = (a)[2] - (b)[2])
#define DcVecMultByScalar(v, s)      ((v)[0] *= s, (v)[1] *= s, (v)[2] *= s)
#define DcVecLerp(r, a, b, c)        { DcVecSubtract(r, b, a);  \
                                       DcVecMultByScalar(r, c);   \
                                       DcVecAddTo(r, a); } 
#define DcVecNegate(a)               ((a)[0] = -(a)[0], (a)[1] = -(a)[1], (a)[2] = -(a)[2])
#define DcVecMagnitudeDouble(v)      ( sqrt((v)[0] * (v)[0] + (v)[1] * (v)[1] + (v)[2] * (v)[2]))
         DcVecLerp(tempVector, verts[0].v, verts[2].v, interp);
      
         //========================================//
         // compute the interpolated s coord value //
         //========================================//
         interp = verts[0].t[0] + (verts[2].t[0] - verts[0].t[0]) * interp;

         //===========================================================================//
         // the tangent vector is the ray from the middle vertex to the lerped vertex //
         //===========================================================================//
         DcVecSubtract(tangent, tempVector, verts[1].v);

         //=====================================================//
         // make sure the tangent points in the right direction //
         //=====================================================//
         if (interp < verts[1].t[0])
         {
            DcVecNegate(tangent);
         }

         //=============================================================//
         // make sure the tangent vector is perpendicular to the normal //
         //=============================================================//
#define DcVecDot3(a, b)              ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2])
         dot = DcVecDot3(n0, tangent);            
         tangent[0] = tangent[0] - n0[0] * dot;
         tangent[1] = tangent[1] - n0[1] * dot;
         tangent[2] = tangent[2] - n0[2] * dot;

         //==============================//
         // normalize the tangent vector //
         //==============================//
#define DcVecNormalizeDouble(v,def)  { F64 __mag = DcVecMagnitudeDouble(v); \
                                       if (__mag < EPSILON) {(v)[0] = (def)[0]; (v)[1] = (def)[1]; (v)[2] = (def)[2];} else \
                                       {__mag = 1.0/__mag;(v)[0] *= __mag; (v)[1] *= __mag; (v)[2] *= __mag;} }

         static F64 defTan[3] = {1.0, 0.0, 0.0};
         DcVecNormalizeDouble(tangent, defTan);
      }


    tang += tangent;
}

#endif

#ifdef TAN_2
void calcTangentVector(const CVector3 &v0,
                       const CVector3 &v1,
                       const CVector3 &v2,
                       const CTexCoord &t0,
                       const CTexCoord &t1,
                       const CTexCoord &t2,
                       const CVector3 &n0,
                       CVector3 &tang)
{
    F64 du1 = t1.x - t0.x;
    F64 dv1 = t1.y - t0.y;
    F64 du2 = t2.x - t0.x;
    F64 dv2 = t2.y - t0.y;

    F64 prod1 = (du1*dv2-dv1*du2);
    F64 prod2 = (du2*dv1-dv2*du1);

    if ((fabs(prod1) < 0.000001)||(fabs(prod2) < 0.000001))
        return;

    F64 x = du2 / prod1;
    F64 y = du1 / prod2;

    CVector3 vec1 = v1 - v0;
    CVector3 vec2 = v2 - v0;
    CVector3 tangent = (vec1 * x) + (vec2 * y);

    F64 component = (tangent * n0);
    tangent = tangent - (n0 * component);
    normalize(tangent);

    tangent = tangent ^ n0;
    //tangent.z = -tangent.z;

    tang += tangent;  // FIXME : I calculate the binormal and the the tangent!
}
#endif

#ifdef TAN_3
void calcTangentVector(const CVector3 &v0,
                       const CVector3 &v1,
                       const CVector3 &v2,
                       const CTexCoord &t0,
                       const CTexCoord &t1,
                       const CTexCoord &t2,
                       const CVector3 &n0,
                       CVector3 &tang)
{
    CVector3 vec1, vec2, tangent;
    CVector3 planes[3];

    for (S32 i = 0 ; i < 3 ; i++) {
        vec1[0] = v1[i]-v0[i];
        vec1[1] = t1[0]-t0[0];
        vec1[2] = t1[1]-t0[1];
        vec2[0] = v2[i]-v0[i];
        vec2[1] = t2[0]-t0[0];
        vec2[2] = t2[1]-t0[1];
        normalize(vec1);
        normalize(vec2);
        planes[i] = vec1 ^ vec2;
    }

    tangent[0] = -planes[0][1]/planes[0][0];
    tangent[1] = -planes[1][1]/planes[1][0];
    tangent[2] = -planes[2][1]/planes[2][0];

    normalize(tangent);

    tang += tangent;
}

#endif

void calcTangentVector(const CVector4 &v0,
                       const CVector4 &v1,
                       const CVector4 &v2,
                       const CTexCoord &t0,
                       const CTexCoord &t1,
                       const CTexCoord &t2,
                       const CVector3 &n0,
                       CVector3 &tang)
{
    calcTangentVector(v0.toVec3(),v1.toVec3(),v2.toVec3(),t0,t1,t2,n0,tang);
}

static const char *glErrorStrings[GL_OUT_OF_MEMORY - GL_INVALID_ENUM + 1] = {
    "invalid enumerant",
    "invalid value",
    "invalid operation",
    "stack overflow",
    "stack underflow",
    "out of memory",
};

const char *getGLErrorString(GLenum errorCode){
    if (errorCode == 0) {
	    return (const char *) "no error";
    }
    if ((errorCode >= GL_INVALID_ENUM) && (errorCode <= GL_OUT_OF_MEMORY)) {
	    return (const  char *) glErrorStrings[errorCode - GL_INVALID_ENUM];
    }
    return (const char *) "generic error";
}

void checkGLError(){
    GLuint error;

    error = glGetError();
    if(error){
        gcon.printf("OpenGL error : %s\n", getGLErrorString(error));
        assert(false);
    }
}


void myglPushMatrix(){
    //gcon.printf("glPushMatrix\n");
    glPushMatrix();
}

void myglPopMatrix(){
    //gcon.printf("glPopMatrix\n");
    glPopMatrix();
}

F32 WrapToOne(F32 x){
	return x - floor(x);
}

#ifndef __GNUC__ //already defined in GCC
F64 random(){
    return (F64)rand() / RAND_MAX;
}
#endif

F64 randomInRange(F64 start, F64 stop){
	return start + random() * (stop - start);
}

bool isPowerOfTwo(U32 i){
	return i && !(i & (i - 1));
}

U32 nextPowerOfTwo(U32 value){
    value--;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value++;
    return value;
}
