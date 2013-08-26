/***************************************************************************
    begin                : Sun Jun 9 2002
    copyright            : (C) 2002 by Vassili Tenente
                           (C) 2002-2005 Harry Kalogirou (harkal@gmail.com)
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

