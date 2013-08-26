#ifndef __CMAP_H_
#define __CMAP_H_

#include "typedefs.h"

#include <vector.h>
#include <material.h>
#include <boundbox.h>
#include "brushmodel.h"

#define CMAPHEADER_ID  (*(S32*)"CMAP")
#define CMAPHEADER_VERSION 17

#include "cmapfile.h"
using namespace CMapFile;

enum
{
    FACETYPE_NORMAL   = 1,
    FACETYPE_MESH     = 2,
    FACETYPE_TRISURF  = 3,
    FACETYPE_FLARE    = 4
};

enum {
	WENTITIES,
	WSHADERREFS,//
	WPLANES,//
	WNODES,//
	WLEAFS,//
	WLFACES,//
	WLBRUSH,//
	WMODELS,//
	WBRUSHES,//
	WBRUSHSIDES,//
	WVERTS,//
    WNEIGHBORS,//
	WELEMS,//
	WFOGZONE,
	WFACES,//
	WLIGHTMAPS,//
	WLIGHTGRID,
	WVISIBILITY,
	WNUM_LUMPS
};

#define	MAX_MAP_MODELS		1024 
#define	MAX_MAP_BRUSHES		8192 
#define	MAX_MAP_ENTITIES	2048 
#define	MAX_MAP_ENTSTRING	0x40000 
#define	MAX_MAP_TEXINFO		8192 
 
#define	MAX_MAP_AREAS		256 
#define	MAX_MAP_AREAPORTALS	1024 
#define	MAX_MAP_PLANES		65536 
#define	MAX_MAP_NODES		65536 
#define	MAX_MAP_BRUSHSIDES	65536 
#define	MAX_MAP_LEAFS		65536 
#define	MAX_MAP_VERTS		65536 
#define	MAX_MAP_FACES		65536 
#define	MAX_MAP_LEAFFACES	65536 
#define	MAX_MAP_LEAFBRUSHES 	65536 
#define	MAX_MAP_PORTALS		65536 
#define	MAX_MAP_EDGES		128000 
#define	MAX_MAP_SURFEDGES	256000 
#define	MAX_MAP_LIGHTING	0x200000 
#define	MAX_MAP_VISIBILITY	0x100000 

#define	SURF_NODAMAGE			0x1		// never give falling damage
#define	SURF_SLICK				0x2		// effects game physics
#define	SURF_SKY				0x4		// lighting from environment map
#define	SURF_LADDER				0x8
#define	SURF_NOIMPACT			0x10	// don't make missile explosions
#define	SURF_NOMARKS			0x20	// don't leave missile marks
#define	SURF_FLESH				0x40	// make flesh sounds and effects
#define	SURF_NODRAW				0x80	// don't generate a drawsurface at all
#define	SURF_HINT				0x100	// make a primary bsp splitter
#define	SURF_SKIP				0x200	// completely ignore, allowing non-closed brushes
#define	SURF_NOLIGHTMAP			0x400	// surface doesn't need a lightmap
#define	SURF_POINTLIGHT			0x800	// generate lighting info at vertexes
#define	SURF_METALSTEPS			0x1000	// clanking footsteps
#define	SURF_NOSTEPS			0x2000	// no footstep sounds
#define	SURF_NONSOLID			0x4000	// don't collide against curves with this set
#define SURF_LIGHTFILTER		0x8000	// act as a light filter during q3map -light
#define	SURF_ALPHASHADOW		0x10000	// do per-pixel light shadow casting in q3map
#define	SURF_NODLIGHT			0x20000	// don't dlight even if solid (solid lava, skies)
#define SURF_NOPICMIP			0x40000
#define SURF_NOMIPMAP			0x80000


typedef struct {
	S32 offset;
	S32 size;
}idxrec_t;

typedef struct 
{
	S32 offset;
	S32 size;
}lump_t ;

typedef struct {
	S32 id ,ver;
	lump_t lumps [WNUM_LUMPS];
}dheader_t;


typedef struct {
	CVector3	mins, maxs;
	S32	firstface, numfaces;
	S32 firstbrush, numbrushes;
} model_t;

typedef struct {
	CVector3	mins, maxs;
	CVector3	origin;
	S32	firstface, numfaces;
	S32 firstbrush, numbrushes;
} dmodel_t;

 
typedef struct { 
	CVector3 point;
	CVector2 tex_st;
    CVector3 v_norm;
	CVector3 tang;
} dvertex_t; 
 

typedef struct { 
	CVector3 normal; 
	F32	dist; 
} dplane_t; 
 

typedef struct {
    char name[64];
    S32 flags;
	S32 contents;
} dtexref_t;

typedef struct {
	CMaterialPtr shader;
	S32 flags;
	S32 contents;
} ctexref_t;

typedef struct { 
	S32		planenum; 
	S32		children[2];
	S32		mins[3];
	S32		maxs[3]; 
} dnode_t;

class cnode_t {
public:
    CAABoundBox *bounds;
	cnode_t *father;
    CPlane *plane;
    S32 children[2];
};

typedef struct { 
    S32 shader;    
    S32 facetype;  
    S32 firstvert, numverts;
    S32 firstelem, numelems;
    CVector3 orig;
//    F32 radius;
    CVector3 v_norm;
    CPlane *plane;
//    S32 mesh_cp[2];
} dface_t; 
 
typedef struct {  
	S32		cluster; 
	S32		area; 
 
	S32		mins[3];			// for frustum culling 
	S32		maxs[3]; 
 
	S32	firstleafface, numleaffaces; 
	S32 firstleafbrush, numleafbrushes; 
} dleaf_t;

class cleaf_t {
public:
	CAABoundBox *bounds; // STUPID MSVC7.0 DOESN'T INITIALISE THE VCT CORRECTLY
						 // SO I USE A POINTER
    cnode_t *father;
	S32		cluster;
	S32		area;

	S32	firstleafface, numleaffaces;
	S32 firstleafbrush, numleafbrushes;
};

typedef struct {
    S32 areas[2];
    CAABoundBox bound;
	dface_t *face;
    S32 portalId;
	bool reverse;

	bool open;
	bool visible;
} cAreaPortal;
 
typedef struct { 
	S32	planenum;
	S32	shader; 
} dbrushside_t; 
 
typedef struct { 
	S32	brushSide; 
	S32	numOfBrushSides; 
	S32	shader; 
	CVector3 translate;
} dbrush_t; 
 
 
typedef struct { 
	S32	numclusters; 
	S32 rowsize;
	byte data[1];
} dvis_t; 

typedef struct { 
	byte *raw[6]; 
	S32 cache_offsets[6]; 
	S32 width, height; 
} BSPSky; 
  
class CFile;
class CAABoundBox;
class CTraceResult;

class CMAP { 
public:
    CMAP();
	~CMAP();
	void load(CFile &f);	
	dmodel_t *ProcessModels(model_t *models, S32 numofmodels);
	S32 findLeaf(CVector3 &p);
	S32 findLeaf(CBound &bbox);
    S32 findArea(CVector3 &o);
	S32 findArea(CBound &bbox);

	tTexture	  *texrefs;
	ctexref_t	  *shaderrefs;
	dmodel_t      *Models; 
    CPlane        *Planes; 
	cnode_t       *Nodes; 
	cleaf_t       *Leaves; 
	char			*entities;
	S32 *lfaces ;
	S32 *lbrushes ;
	dbrush_t      *Brushes; 
	dbrushside_t  *BrushSides; 
	dvertex_t     *Verts; 
    S32 * neighbors;
	S32 * elems ;
	dface_t       *Faces; 

    cAreaPortal   *mAreaPortalsAll;
    tAreaPortal   *mAreaPortals;
    tArea         *mAreas;
	tBrushModel   *BrushModels;

	std::vector<CBrushModelPtr> mBrushModels;
		
	dvis_t        *vis; 
	S32          *LightMaps;
	
	char		**EntityIndexes;
	S32			NumEntityIndexes;

	BSPSky         Sky; 

	S32 NumTexrefs, NumModels, NumPlanes, NumNodes, NumLeaves; 
	S32 NumBrushes, NumBrushSides; 
	S32 NumFaces, NumVerts, NumAreaPortals, NumAreas;
	S32 NumLfaces,NumLbrushes,NumVis,NumLightmap;
	S32 NumElems;
	S32 NumOfAreasToPortals;
	S32 NumBrushModels;
	byte *Raw;
    unsigned char *mActiveBrushBitVector;
    
    void setActiveArea(CAABoundBox *box);
    void markActiveLeafs(S32 n);

	void makeModelActive(S32 n);

    void trace(CTraceResult *tr, CAABoundBox &bbox, const CVector3 &start, const CVector3 &end);
private:
    S32 *mLeafList;
    S32 mLeafListNum;

    S32 *mBrushList;
    S32 mBrushListNum;

    CAABoundBox *mBBox;

    void markLeafBrushes(S32 l);
}; 

#endif


