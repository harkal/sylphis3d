/***************************************************************************
    begin                : Sun Jun 9 2002
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

#ifndef _BSPWRITER_H_
#define _BSPWRITER_H_

#include <string>
#include <vector>
#include <list>
#include "writestructures.h"
#include "tree.h"
#include "brush.h"
#include "planemanager.h"

using namespace std;

class CBspCreator;
class CMapLoader;
class CPolygon;
class CBspNode;
class CEntity;
class CTJunctions;

const static bool WRITE_ELEMENTS = true;
const static bool NO_ELEMENTS    = false;

const static bool TEXTURE_COOR   = true;
const static bool NO_TEXTURE_COOR = false;

const static bool TRANSLATE      = true;
const static bool NO_TRANSLATE   = false;

const static bool USE_TJ         = true;
const static bool NO_USE_TJ      = false;

const static bool USE_MERGE         = true;
const static bool NO_USE_MERGE      = false;

class CBspWriter{

public:
	CBspWriter( CBspCreator *theBsp, const bool write=false,const string &fileP="", bool neighbors = true);

	void writeIt();
	void rewriteEntities();
	~CBspWriter(){};
private:
	vector<char> memData;
	CBspCreator *bsp;
	CMapLoader *loader;
	CPlaneManager *pManager;
	string filePath;    
	CTJunctions *tj;
	vector<tBSPNode> tNodes;
    vector<CAABoundBox> mLightBounds;
    bool mFindNeighbors;

	void initWrite();


	template < class T >        
		void writeLump(const int lumpNo, const T *data, const int dataNo)
	{
		int bytesNo=dataNo*sizeof(T);
		int offset = memData.size();

		memData.resize(offset+bytesNo);

		T *templData = (T*)&memData[offset];

		memcpy(templData,data, bytesNo);

		tBSPLump *lumpP = (tBSPLump*)&memData[sizeof(tBSPHeader)+lumpNo*sizeof(tBSPLump)];
		lumpP->offset=offset;
		lumpP->length=bytesNo;                 
	}

	void writeMemData();

	void writeShaders();
	void writePlanes();
	void writeBrushesAndMuchMore();
	void writeNodes();
	void writeEntities();
    void writeNeighbors();
	void writeAll();
	vector<int> *writeBrush(CBrush &brush, vector<tBSPBrush> *brushes, vector<int> *brushSides);

	void addPolygon(CPolygon *polygon, 
		vector<tBSPFace> *faces, 
		vector<tBSPElement> *elements,
		vector<tBSPVertex> *vertices, 
		bool writeElements, 
		bool calcUV,
		bool useTJ );

	void addPolygon_tess(CPolygon *polygon, 
		vector<tBSPFace> *faces, 
		vector<tBSPElement> *elements,
		vector<tBSPVertex> *vertices, 
		bool writeElements, 
		bool calcUV,
		bool useTJ );

	void addBrushPolygons(CBrush *brush, 
		vector<tBSPFace> *faces, 
		vector<tBSPElement> *elements,
		vector<tBSPVertex> *vertices,
		bool calcUV,
		bool useMergePolygon,
		bool useTJ);

	void addMergePolygon(CPolygon *polygon, 
		vector<tBSPFace> *faces, 
		vector<tBSPElement> *elements,
		vector<tBSPVertex> *vertices, 
		bool writeElements, 
		bool calcUV, 
		bool usetj);

	void processModelEntity(CEntity *modelEntity);

	void calcUVForPolygon(CPolygon *polygon, CBrushPlane *brushPlane=0);
	int addNode(CBTree<CBspNode *>::Node *node);
	void mergeVertices(tBSPVertex *tVertices, const int theLastVerticesNo);
	void calcUVForVertex(CVertex &v, CBrushPlane *bPlane);
};


#endif

