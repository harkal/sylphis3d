
#ifndef _typedef_h_
#define _typedef_h_

#include <platform/types.h>

#define FALSE           0 
#define TRUE            1 
 
#define PI				3.141592f 
 
 
typedef unsigned char   byte; 

#ifdef __LINUX__
typedef unsigned char   BYTE; 
typedef U16  WORD; 
typedef unsigned long   DWORD; 
#endif

typedef U16  word; 
typedef unsigned long   dword; 
typedef F32 texcoord_t[2];

typedef F32	Vec4[4];
typedef F32	Vec3[3];
typedef F32	Vec2[2];
typedef Vec3	Mat3x3[3];
typedef byte	colour_t[4]; 

typedef S32		TriVec[3];	//vertex 1,2,3 of TriVec
typedef Vec2	TexVec;		//Texture U/V coordinates of vertex 

 

#endif



