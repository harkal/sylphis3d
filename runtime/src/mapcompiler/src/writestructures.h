/***************************************************************************
    copyright            : (C) 2002 by Vassili Tenente
                           (C) 2002-2005 Harry Kalogirou (harkal@gmail.com)
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

#ifndef _WRITESTRUCTURES_H_
#define _WRITESTRUCTURES_H_

#include "globaldefs.h"

#include "vector.h"

enum  eLumps
{
    kEntities = 0,     // Stores player/object positions, etc...
    kTextures,         // Stores texture information
    kPlanes,           // Stores the splitting planes
    kBrushes,          // Stores the brushes info (for collision)
    kBrushSides,       // Stores the brush surfaces info  //double inderiction
    kNodes,            // Stores the BSP nodes
    kLeafs,            // Stores the leafs of the nodes
    kLeafBrushes,      // Stores the leaf's indices into the brushes// double inderiction
    kLeafFaces,        // Stores the leaf's indices into the faces
    kVertices,         // Stores the level vertices
    kNeighbors,
    kAreas,              //tssss mprikia kolame //sozei class Area  sto //Portal.h
    kAreasToPortals,     //double reindirection to areaPortals
    kAreaPortals,        // the areaPortals struct tBSPAreaPortal
    kElements,           // the Triangles
    kBrushEntities,      /// entities like doors etc...
    kMaxLumps          // A constant to store the number of lumps
};

struct tBSPLump{
        int offset;
        int length;
};
struct  tBSPHeader{
        char strID[4];     // This should always be 'CMAP'
        int version;
};

struct  tBSPVertex
{
        CVector3 vPosition;      // (x, y, z) position.
        float vTextureCoord[2];  // (u, v) texture coordinate
        CVector3 vNormal;        // (x, y, z) normal vector
};

struct tBSPFace
{
        int textureID;        // The index into the texture array
        int type;             // 1=polygon, 2=patch, 3=mesh, 4=billboard
        int vertexIndex;      // The index into this face's first vertex
        int numOfVerts;       // The number of vertices for this face
        int numMeshVerts;     // The number of mesh vertices
        int startElement;     // The start index in to the elements
        int elementsSize;     // The No of elements of this face
        float vNormal[3];     // The face normal.
        int normal;           // The normalIndex
        int size[2];          // The bezier patch dimensions.
};


struct  tBSPTexture
{
        char strName[64];   // The name of the texture w/o the extension
};

struct  tBSPNode
{
        int plane;      // The index into the planes array
        int front;      // The child index for the front node
        int back;       // The child index for the back node
        float mins[3];    // The bounding box min position.
        float maxs[3];    // The bounding box max position.
};

struct  tBSPLeaf
{
        int area;              // The area portal
        float mins[3];           // The bounding box min position
        float maxs[3];           // The bounding box max position
        int leafface;          // The first index into the face array
        int numOfLeafFaces;    // The number of faces for this leaf
        int element;           //The first index into the elements array
        int numOfElements;           //The first index into the elements array
        int leafBrush;         // The first index for into the brushes
        int numOfLeafBrushes;  // The number of brushes for this leaf
};

struct tBSPBrush
{
  int brushSide;           // The starting brush side for the brush
  int numOfBrushSides;     // Number of brush sides for the brush
  int textureID;           // The texture index for the brush
};

struct tBSPAreaPortal
{
  int areas[2];         //areas that joins
  float mins[3];
  float maxs[3];        //bound box
  int faceNo;
};


struct tBSPPlane
{
    float d;
    CVector3 normal;

    int onAxe;                  // 0 means on X,  1 means on Y, 2 means on Z, -1 means unspecified      
    int factor;                 // if this is  1 means potitive, -1 negative
};

struct tBSPElement{
  int verts[3];
};


struct tBSPArea{
        int startLeaf;
        int leafsSize;
        int startFace;
        int facesSize;
        int startPortal;
        int portalsSize;
        int startElement;
        int elementsSize;
};


//AYTA PROS TO PARON 
// ISOS NA THELOUME KAI LEAFS?
//MALLON OXI
struct tBSPBrushEntity{
        int startFace;
        int facesSize;
        int startElement;
        int elementsSize;
        int startBrush;
        int brushesSize;        
};

#endif //_WRITESTRUCTURES_H_

