/***************************************************************************
                          cmap.cpp  -  description
                             -------------------
    begin                : Wed Jun 4 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#ifdef __WIN32
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <malloc.h>
#include <string.h> 
#include <math.h>

#include "cmap.h"
#include "brushmodel.h"

#include <engine.h>
#include <exception.h>
#include <vfs.h>
#include <plane.h>
#include <boundbox.h>
#include <scenemanager.h>
#include <trace.h>
#include <console.h>
#include <physics.h>

#include <gutil.h>

#ifdef _LINUX	//FIXME : This routines are in some .h file ?

F32 max( F32 x, F32 y){
    if(x>y)return x;
    return y;
}

F32 min( F32 x, F32 y){
    if(x>y)return y;
    return x;
}

#endif

CMAP::CMAP(){
    Raw = 0;
}

dmodel_t *CMAP::ProcessModels(model_t *models, S32 numofmodels){
	dmodel_t *dmodels;

	S32 i;
	
	dmodels = (dmodel_t *)malloc(numofmodels * sizeof(dmodel_t));
	
	for( i = 0; i < numofmodels ; i++ ){
		dmodels[i].mins = models[i].mins;
		dmodels[i].maxs = models[i].maxs, 
		dmodels[i].firstface = models[i].firstface;
		dmodels[i].numfaces = models[i].numfaces;
		dmodels[i].firstbrush = models[i].firstbrush;
		dmodels[i].numbrushes = models[i].numbrushes;

		dmodels[i].origin = CVector3(0.0, 0.0, 0.0);
	}
	
	return dmodels;
}

void CMAP::load(CFile &f) { 
	S32 i, size;
	byte *data;
	tHeader *header;
	tLump  *idxtable;
    CVector3 temp;
	
	f.loadData();

    Raw = (unsigned char *)f.getDataCopy();
	size = f.getSize();
	if (!Raw){
		throw CException("Error loading the CMAP file");
	}
	data = Raw;

	header = (tHeader *)data;

	if(*(S32*)header->strID!=CMAPHEADER_ID){
        free(Raw);
        Raw = 0;
		throw CException("Not a CMAP file");
	}
    
	if(header->version!=CMAPHEADER_VERSION){
        free(Raw);
        Raw = 0;
		throw CException("Unsupported version of cmap file " + f.getName());
	}

	idxtable = (tLump *)header + 1;

    entities = (char *)(data + idxtable[kEntities].offset);
	entities[idxtable[kEntities].length - 1] = 0; // Just to be sure
    
    texrefs = (tTexture *)(data + idxtable[kTextures].offset);	
	NumTexrefs = idxtable[kTextures].length / sizeof(tTexture);

    shaderrefs = new ctexref_t[NumTexrefs];
	for(i=0;i<NumTexrefs;i++){
        shaderrefs[i].shader = CEngine::getSingleton()->mMaterials.registerMaterial(texrefs[i].strName);
		//shaderrefs[i].contents = texrefs[i].contents;
		//shaderrefs[i].flags = texrefs[i].flags;
	}

    Planes = (CPlane *)(data + idxtable[kPlanes].offset);	
	NumPlanes = idxtable[kPlanes].length / sizeof(CPlane);

    tBrush *brushes = (tBrush *)(data + idxtable[kBrushes].offset);	
    NumBrushes = idxtable[kBrushes].length / sizeof(tBrush);
	Brushes = new dbrush_t[NumBrushes];
	for(i=0;i<NumBrushes;i++){
		Brushes[i].brushSide = brushes[i].brushSide;
		Brushes[i].numOfBrushSides = brushes[i].numOfBrushSides;
		Brushes[i].translate = CVector3::ZERO;
        //Brushes[i].shader = 0;
	}

    mActiveBrushBitVector = new unsigned char [(NumBrushes+7) >> 3];
    mBrushList = new S32[NumBrushes];

    S32 *brushsides = (S32 *)(data + idxtable[kBrushSides].offset);	
	NumBrushSides = idxtable[kBrushSides].length / sizeof(S32);
    BrushSides = new dbrushside_t[NumBrushSides];
	for(i=0;i<NumBrushSides;i++){
        BrushSides[i].planenum = brushsides[i];
        BrushSides[i].shader = 0;
	}

    tNode *nodes = (tNode *)(data + idxtable[kNodes].offset);	
	NumNodes = idxtable[kNodes].length / sizeof(tNode);
    Nodes = new cnode_t[NumNodes];
    for(i = 0 ; i < NumNodes ; i++){
        Nodes[i].plane = &Planes[nodes[i].plane];
        Nodes[i].children[0] = nodes[i].front;
        Nodes[i].children[1] = nodes[i].back;
		Nodes[i].bounds = new CAABoundBox();
        Nodes[i].bounds->setMax(CVector3(nodes[i].maxs[0],nodes[i].maxs[1],nodes[i].maxs[2]));
        Nodes[i].bounds->setMin(CVector3(nodes[i].mins[0],nodes[i].mins[1],nodes[i].mins[2]));
    }

    i = NumNodes - 1;
    Nodes[0].plane = &Planes[nodes[i].plane];
    Nodes[0].children[0] = nodes[i].front;
    Nodes[0].children[1] = nodes[i].back;
    Nodes[0].bounds->setMax(CVector3(nodes[i].maxs[0],nodes[i].maxs[1],nodes[i].maxs[2]));
    Nodes[0].bounds->setMin(CVector3(nodes[i].mins[0],nodes[i].mins[1],nodes[i].mins[2]));

    tLeaf *leaves = (tLeaf *)(data + idxtable[kLeafs].offset);	
	NumLeaves = idxtable[kLeafs].length / sizeof(tLeaf) + 1;
	mLeafList = new S32[NumLeaves];
    Leaves = new cleaf_t[NumLeaves];
    mLeafListNum = 0;
    for(i = 0 ; i < NumLeaves - 1; i++){
        S32 j = i + 1;
		cleaf_t &leaf = Leaves[j];
        leaf.area = leaves[i].area;
        leaf.firstleafbrush = leaves[i].leafBrush;
        leaf.firstleafface = leaves[i].leafface;
        leaf.numleafbrushes = leaves[i].numOfLeafBrushes;
        leaf.numleaffaces = leaves[i].numOfLeafFaces;
		leaf.bounds = new CAABoundBox();
        leaf.bounds->setMax(CVector3(leaves[i].maxs[0],leaves[i].maxs[1],leaves[i].maxs[2]));
        leaf.bounds->setMin(CVector3(leaves[i].mins[0],leaves[i].mins[1],leaves[i].mins[2]));
		leaf.bounds->getType();
    }

    lbrushes = (S32 *)(data + idxtable[kLeafBrushes].offset);	
	NumLbrushes = idxtable[kLeafBrushes].length / sizeof(S32);

    elems = (S32 *)(data + idxtable[kElements].offset);	
	NumElems = idxtable[kElements].length / sizeof(S32);

    tVertex *verts = (tVertex *)(data + idxtable[kVertices].offset);	
	NumVerts = idxtable[kVertices].length / sizeof(tVertex);
	Verts = new dvertex_t[NumVerts];
	for(i = 0 ; i < NumVerts ; i++){
		Verts[i].point = verts[i].point;
		Verts[i].v_norm = verts[i].v_norm;
		Verts[i].tex_st = verts[i].tex_st;
		Verts[i].tang = CVector3::ZERO;
	}

    neighbors = (S32 *)(data + idxtable[kNeighbors].offset);	
	// no size calculation (it is the same as vertices)
            
    tFace *faces = (tFace *)(data + idxtable[kLeafFaces].offset);	
	NumFaces = idxtable[kLeafFaces].length / sizeof(tFace);
    Faces = new dface_t[NumFaces];
    lfaces = new S32[NumFaces];
    for(i = 0 ; i < NumFaces ; i++){
        lfaces[i] = i;
        Faces[i].shader = faces[i].textureID;
		if(Faces[i].shader > NumTexrefs)
			Faces[i].shader = 0;
        Faces[i].facetype = faces[i].type;
        Faces[i].firstvert = faces[i].vertexIndex;
        Faces[i].numverts = faces[i].numOfVerts;
        Faces[i].firstelem = faces[i].startElement * 3;
        //assert(Faces[i].firstelem >= 0);
        Faces[i].numelems = faces[i].elementsSize * 3;
        //assert(Faces[i].numelems >= 0);
        Faces[i].v_norm = *(CVector3 *)faces[i].vNormal;
        Faces[i].plane = &Planes[faces[i].plane];
    }

    mAreaPortals = (tAreaPortal *)(data + idxtable[kAreaPortals].offset);
    NumAreaPortals = idxtable[kAreaPortals].length / sizeof(tAreaPortal);

    S32 *dd = (S32 *)(data + idxtable[kAreasToPortals].offset);
    NumOfAreasToPortals = idxtable[kAreasToPortals].length / sizeof(S32);

    mAreas = (tArea *)(data + idxtable[kAreas].offset);
    NumAreas = idxtable[kAreas].length / sizeof(tArea);

    mAreaPortalsAll = new cAreaPortal[NumOfAreasToPortals];
    for(i = 0 ; i < NumAreas ; ++i){
        tArea *area = &mAreas[i];
        for(S32 j = area->startPortal ; j < area->startPortal+area->numOfPortals ; j++){
            mAreaPortalsAll[j].bound.setBounds(mAreaPortals[dd[j]].min, mAreaPortals[dd[j]].max);
			mAreaPortalsAll[j].face = &Faces[mAreaPortals[dd[j]].polygonId];
			mAreaPortalsAll[j].open = true;
			mAreaPortalsAll[j].visible = false;
            mAreaPortalsAll[j].portalId = dd[j];
            if(mAreaPortals[dd[j]].areas[0] != i){
                mAreaPortalsAll[j].areas[0] = mAreaPortals[dd[j]].areas[1];
                mAreaPortalsAll[j].areas[1] = mAreaPortals[dd[j]].areas[0];
				mAreaPortalsAll[j].reverse = true;
            } else {
                mAreaPortalsAll[j].areas[0] = mAreaPortals[dd[j]].areas[0];
                mAreaPortalsAll[j].areas[1] = mAreaPortals[dd[j]].areas[1];
				mAreaPortalsAll[j].reverse = false;
            }
            if(mAreaPortalsAll[j].areas[0] != i)
                throw CException("Corrupted file (Improper portals)");
        }
    }

    // Build father links
    for(i = 0 ; i < NumNodes ; i++){
        cnode_t *node = &Nodes[i];

        for(S32 j = 0 ; j < 2 ; j++){
            if(node->children[j] > 0)
                Leaves[node->children[j]].father = node;
            else
                Nodes[-node->children[j]].father = node;
        }
    }

	F32 mins[2], maxs[2];

    for(i = 0 ; i < NumFaces - NumAreaPortals; i++){
		mins[0] = mins[1] = 99999;
		maxs[0] = maxs[1] = -99999;
        dface_t *face = &Faces[i];
        CMaterial *s = shaderrefs[face->shader].shader;
        for(S32 j = face->firstvert ; j < face->firstvert + face->numverts ; j++){
            U32 w, h;
		    s->getDimensions(&w, &h);
            Verts[j].tex_st.x /= w;
            Verts[j].tex_st.y /= h;			

			for (S32 k = 0; k < 2; k++) {
				if (Verts[j].tex_st[k] < mins[k]) {
					mins[k] = Verts[j].tex_st[k];
				}
				if (Verts[j].tex_st[k] > maxs[k]) {
					maxs[k] = Verts[j].tex_st[k];
				}
			}
        }

		mins[0] = floor(mins[0]);
		mins[1] = floor(mins[1]);
		for(S32 j = face->firstvert ; j < face->firstvert + face->numverts ; j++){
			Verts[j].tex_st.x -= mins[0];
			Verts[j].tex_st.y -= mins[1];	
		}
    }

	// Calculate tanget space
	for(i = 0 ; i < NumFaces - NumAreaPortals; i++){
        dface_t *face = &Faces[i];
		S32 e = face->firstelem;
        if(e < 0)
            continue; // Portal
		CVector3 &v0 = Verts[face->firstvert + elems[e]].point;
		CVector3 &v1 = Verts[face->firstvert + elems[e + 1]].point;
		CVector3 &v2 = Verts[face->firstvert + elems[e + 2]].point;
		CTexCoord &t0 = Verts[face->firstvert + elems[e]].tex_st;
		CTexCoord &t1 = Verts[face->firstvert + elems[e + 1]].tex_st;
		CTexCoord &t2 = Verts[face->firstvert + elems[e + 2]].tex_st;
		CVector3 &n0 = Verts[face->firstvert + elems[e]].v_norm;
		CVector3 &n1 = Verts[face->firstvert + elems[e+1]].v_norm;
		CVector3 &n2 = Verts[face->firstvert + elems[e+2]].v_norm;
		CVector3 &tang0 = Verts[face->firstvert + elems[e]].tang;
		CVector3 &tang1 = Verts[face->firstvert + elems[e+1]].tang;
		CVector3 &tang2 = Verts[face->firstvert + elems[e+2]].tang;

		calcTangentVector(v0, v1, v2, t0, t1, t2, n0, tang0);
		normalize(tang0);
		//calcTangentVector(v1, v2, v0, t1, t2, t0, n1, tang1);
		//calcTangentVector(v2, v0, v1, t2, t0, t1, n2, tang2);

		for(S32 k = face->firstvert ; k < face->firstvert + face->numverts ; k++)
			Verts[k].tang = tang0;
	}

	BrushModels = (tBrushModel *)(data + idxtable[kBrushModels].offset);	
    NumBrushModels = idxtable[kBrushModels].length / sizeof(tBrushModel);
	
	for(i = 0 ; i < NumBrushModels ; i++){
		CBrushModelPtr b = new CBrushModel();
		b->loadBrushEntity(*this, i);
		mBrushModels.push_back(b);
	}
} 
 
CMAP::~CMAP() {
	if(!Raw)
        return;
    
    free(Raw);

    for(S32 i = 0 ; i < NumNodes ; i++){
        delete Nodes[i].bounds;
    }
    for(S32 i = 1 ; i < NumLeaves ; i++){
        delete Leaves[i].bounds;
    }
    for(S32 i = 0 ; i < NumBrushModels ; i++){
		mBrushModels[i] = 0;
	}
    delete [] shaderrefs;
    delete [] Brushes;
    delete [] mActiveBrushBitVector;
    delete [] mBrushList;
    delete [] BrushSides;
    delete [] Nodes;
    delete [] mLeafList;
    delete [] Leaves;
    delete [] Verts;
    delete [] Faces;
    delete [] lfaces;
    delete [] mAreaPortalsAll;
} 
 

S32 CMAP::findLeaf(CVector3 &p) 
{ 
	cnode_t  *node; 
	S32 idx, count; 
	F32 dot; 
    
    idx = 0; 
 
    count = 0;
	while(idx <= 0) { 
        ++count;
		node=&Nodes[-idx]; 
		dot = node->plane->distance(p); 
		if (dot >= 0) 
            idx = node->children[0]; 
		  else 
            idx = node->children[1]; 
        if(count > 1000){
            assert("Entity in void" && 0);
            return 1;
        }
	}
 
    return idx; 
}

S32 CMAP::findArea(CVector3 &o){
    S32 leaf = findLeaf(o);
    assert(leaf > 0);
    return Leaves[leaf].area;
}

S32 CMAP::findLeaf(CBound &bbox){ //FIXME... den douleuei swsta
	cnode_t  *node;
	S32 idx; 
	F32 dot; 
    
    idx = 0; 
 
	while(idx <= 0) { 
		node=&Nodes[-idx]; 
		dot = bbox.distance(*node->plane); 
		if (dot >= 0) 
            idx = node->children[0]; 
		  else 
            idx = node->children[1]; 
	}
 
    return idx; 
}

S32 CMAP::findArea(CBound &bbox){
    return Leaves[findLeaf(bbox)].area;
}


void CMAP::setActiveArea(CAABoundBox *box){
    mBBox = box;
}

void CMAP::makeModelActive(S32 n){
	tBrushModel &bmu = BrushModels[n];

	for(S32 i = bmu.startBrush ; i < bmu.startBrush + bmu.brushesSize ; i++){
		S32 brush = i;
        S32 num = brush >> 3;
        unsigned char bit = 1 << (brush & 7);
        if((mActiveBrushBitVector[num]&bit) == 0){
            mActiveBrushBitVector[num] |= bit;
            mBrushList[mBrushListNum++] = brush;
        }
	}
}

void CMAP::markLeafBrushes(S32 l){
    if(l == 0)
        return;

    cleaf_t *leaf = &Leaves[l];

	CBound::EType t  = leaf->bounds->getType();
   if(!mBBox->collide(leaf->bounds))
        return;

    for(S32 i = leaf->firstleafbrush ;
            i < leaf->firstleafbrush + leaf->numleafbrushes ; i++){

        S32 brush = lbrushes[i];
        S32 num = brush >> 3;
        unsigned char bit = 1 << (brush & 7);
        if((mActiveBrushBitVector[num]&bit) == 0){
            mActiveBrushBitVector[num] |= bit;
            mBrushList[mBrushListNum++] = brush;
        }
    }

    
}
 
void CMAP::markActiveLeafs(S32 n){
    cnode_t *node = &Nodes[n];

    if(n == 0){
        mLeafListNum = 0;
        mBrushListNum = 0;
        memset(mActiveBrushBitVector, 0, (NumBrushes + 7) >> 3);
    }
    else 
        if(!mBBox->collide(node->bounds))
            return;

    n = node->children[0];
    if(n >= 0){
        markLeafBrushes(n);
    } else {
        markActiveLeafs(-n);
    }

    n = node->children[1];
    if(n >= 0){
        markLeafBrushes(n);
    } else {
        markActiveLeafs(-n);
    }   
}

void CMAP::trace(CTraceResult *tr, CAABoundBox &bbox, const CVector3 &start, const CVector3 &end){
    CVector3 p1, p2, col_end;
    F32 d1, d2;
    S32 col_bside = -1;
    F32 col_frac = 1.0;

    col_end = end;
    for(S32 i = 0 ; i < mBrushListNum ; i++){
        dbrush_t *brush = &Brushes[mBrushList[i]];

        d1 = 0.0;
        d2 = col_frac;

        p1 = start;
        p2 = col_end;

        dbrushside_t *bside = &BrushSides[brush->brushSide];

        S32 j;
        for(j = 0 ; j < brush->numOfBrushSides ; j++, bside++){
            CPlane &plane = Planes[bside->planenum];

            CAABoundBox startBox = bbox;
            startBox.translate(p1);
            CAABoundBox endBox = bbox;
            endBox.translate(p2);

            F32 dot1 = startBox.distance(plane);
            F32 dot2 = endBox.distance(plane);

            if(dot1 >= 0 && dot2 >= 0)
                break;

            if(dot1 < 0 && dot2 < 0)
                continue;

            F32 d = dot1 / (dot1 - dot2);
            CVector3 p = p1 + (p2 - p1) * d;
            d = d1 + (d2 - d1) * d;

            if(dot1 >= 0){
                p1 = p;
                d1 = d;
                col_bside = j + brush->brushSide;
            } else {
                p2 = p;
                d2 = d;
            }
        }
        if(j == brush->numOfBrushSides){
            if(col_frac == 0.0){
                tr->frac = 0.0;
                tr->mEnd = start;
                return;
            }
            col_end = p1;
            col_frac = d1;
        }
    }

    tr->frac = col_frac;
    tr->mEnd = col_end;
    if(col_bside != -1)
	    tr->plane = Planes[BrushSides[col_bside].planenum];

}
















