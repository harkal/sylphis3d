
#define CMAPHEADER_ID  (*(int*)"CMAP")
#define CMAPHEADER_VERSION 17


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
        int version;
	} tHeader;

	typedef struct sLump {
        int offset;
        int length;
	} tLump;

	typedef struct sNode {
        int plane;
        int front;
        int back;
        float mins[3];
        float maxs[3];
	} tNode;

	typedef struct { 
		float point[3];
		float tex_st[2];
		float norm[3];
	} tVertex; 

	typedef struct sLeaf {
        int area;
        float mins[3];
        float maxs[3];
        int leafface;
        int numOfLeafFaces;
        int element;
        int numOfElements;
        int leafBrush;
        int numOfLeafBrushes;
	} tLeaf;

	typedef struct sFace {
  	  int textureID;
        int type;
        int vertexIndex;
        int numOfVerts;
        int numMeshVerts;
        int startElement;
        int elementsSize;
        float vNormal[3];
        int plane;
        int size[2];
	} tFace;

	typedef struct sTexture {
        char strName[64];
	} tTexture;

	typedef struct sBrush {
        int brushSide;
        int numOfBrushSides;
        int textureID;
	} tBrush;

	typedef struct sArea {
		int startLeaf;
        int numOfLeafs;
        int startFace;
        int numOfFaces;
        int startPortal;
        int numOfPortals;
        int startElement;
        int numOfElements;
	} tArea;

	typedef struct sAreaPortal {
        int areas[2];
        float min[3];
        float max[3];
        int polygonId;
	} tAreaPortal;

