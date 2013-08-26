
#ifndef _CMAPFILE_H_
#define _CMAPFILE_H_

namespace CMapFile {

	enum  eLumps {
        kEntities = 0,
        kTextures,
        kPlanes,
        kBrushes,
        kBrushSides,
        kNodes,
        kLeafs,
        kLeafBrushes,
        kLeafFaces,
        kVertices,
        kNeighbors,
        kAreas,
        kAreasToPortals,
        kAreaPortals,
        kElements,
		kBrushModels,
        kMaxLumps
    };

	typedef struct sHeader {
        char strID[4];
        S32 version;
	} tHeader;

	typedef struct sLump {
        S32 offset;
        S32 length;
	} tLump;

	typedef struct sVertex { 
		CVector3 point;
		CVector2 tex_st;
		CVector3 v_norm;  
	} tVertex; 

	typedef struct sNode {
        S32 plane;
        S32 front;
        S32 back;
        F32 mins[3];
        F32 maxs[3];
	} tNode;

	typedef struct sLeaf {
        S32 area;
        F32 mins[3];
        F32 maxs[3];
        S32 leafface;
        S32 numOfLeafFaces;
        S32 element;
        S32 numOfElements;
        S32 leafBrush;
        S32 numOfLeafBrushes;
	} tLeaf;

	typedef struct sFace {
		S32 textureID;
        S32 type;
        S32 vertexIndex;
        S32 numOfVerts;
        S32 numMeshVerts;
        S32 startElement;
        S32 elementsSize;
        F32 vNormal[3];
        S32 plane;
        S32 size[2];
	} tFace;

	typedef struct sTexture {
        char strName[64];
	} tTexture;

	typedef struct sBrush {
        S32 brushSide;
        S32 numOfBrushSides;
        S32 textureID;
	} tBrush;

	typedef struct sArea {
	  S32 startLeaf;
        S32 numOfLeafs;
        S32 startFace;
        S32 numOfFaces;
        S32 startPortal;
        S32 numOfPortals;
        S32 startElement;
        S32 numOfElements;
	} tArea;

	typedef struct sAreaPortal {
        S32 areas[2];
        CVector3 min;
        CVector3 max;
	  S32 polygonId;
	} tAreaPortal;

	typedef struct sBrushModel {
        S32 startFace;
        S32 facesSize;
        S32 startElement;
        S32 elementsSize;
        S32 startBrush;
        S32 brushesSize;        
	} tBrushModel;
}

#endif

