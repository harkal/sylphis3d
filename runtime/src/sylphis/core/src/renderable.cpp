/***************************************************************************
                          renderable.cpp  -  description
                             -------------------
    begin                : Wed Mar 27 2002
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

#include <renderable.h>
#include <light.h>
#include <silhouette.h>
#include <gutil.h>
#include <openglext.h>
#include <engine.h>

//
// CRenderable
//

CRenderable::CRenderable(){
	mNoShadows = false;
}
CRenderable::~CRenderable(){
}

void CRenderable::setDefaultShader(CMaterial *shader){
	defShader = shader;
}

CMaterial *CRenderable::getDefaultShader(){
    return defShader;
}

//
// CRenderableElem
//

CRenderableElem::CRenderableElem(){
}
CRenderableElem::CRenderableElem(CVector3 *_a, CVector3 *_b, CVector3 *_c){
	a = _a;
	b = _b;
	c = _c;

	updatePlane();
}
CRenderableElem::~CRenderableElem(){
}

void CRenderableElem::render(CRenderer &r, const CMatrix4 &transform){
	renderWithShader(r, defShader);
}
CSilhouette* CRenderableElem::getSilhoutte(CVector3 &o){
    return 0;
}

void CRenderableElem::renderWithShader(CRenderer &r, CMaterial *shader){
}

void CRenderableElem::updatePlane(){
	plane = CPlane(*a, *b, *c);
}

//
// CRenderableMesh
//

CRenderableMesh::CRenderableMesh() {
    sil = new CSilhouette();
	mSilGen.self = this;
	mSilCache = CCache<CVector3, CSilAndVis, genSil>(50, mSilGen);
    mVertexBuffer = new CVertexBuffer();
    mVertexBuffer->addComponent(CVertexBuffer::POSITION);
    mVertexBuffer->addComponent(CVertexBuffer::NORMAL);
    mVertexBuffer->addComponent(CVertexBuffer::TANGENT);
    mVertexBuffer->addComponent(CVertexBuffer::TEX_COORD);

    mElems = new CIndexBuffer();
    reset();
}

CRenderableMesh::~CRenderableMesh(){
    delete sil;
	_free();
}

void CRenderableMesh::_free(){
	//if(vertices)delete [] vertices;
	//if(normals)delete [] normals;
	//if(tangent)delete [] tangent;
	//if(tex_st)delete [] tex_st;
	//if(elems)delete [] elems;
    if(elemsVis)delete [] elemsVis;
    if(elemEdges)delete [] elemEdges;
    if(elemNormals)delete [] elemNormals;
	if(elemPlanes)delete [] elemPlanes;
    if(neighbors)delete [] neighbors;
	if(relems)delete [] relems;

	reset();	
}

void CRenderableMesh::reset(){
    vertices.setupBuffer(mVertexBuffer, CVertexBuffer::POSITION);
    normals.setupBuffer(mVertexBuffer, CVertexBuffer::NORMAL);
    tangent.setupBuffer(mVertexBuffer, CVertexBuffer::TANGENT);
    tex_st.setupBuffer(mVertexBuffer, CVertexBuffer::TEX_COORD);
    elemsVis = 0;
    elemEdges = 0;
    elemNormals = 0;
	elemPlanes = 0;
    neighbors = 0;
	relems = 0;
	numVertices = 0;
	numElems = 0;
	
	currentFrame = 0.0f;
	numFrames = 0;
}

void CRenderableMesh::makeFrame(){

}

void CRenderableMesh::render(CRenderer &r, const CMatrix4 &transform){
    renderWithShader(r, defShader);
}

typedef CVector4 buffer_vertex_t;

//#define NO_CACHE
void CRenderableMesh::renderShadowVolume(CRenderer &r, CVector3 &o, S32 back){
#if 1
    if(mSystemVertices.size() == 0)
        return;
	makeFrame();
    //if(!elemsVis)
    //   return;
    CVector4 pos(o.x, o.y, o.z);
#ifdef NO_CACHE
	CSilhouette *sil = getSilhoutte(o);
	bool *vis = elemsVis;
#else
	CSilAndVis *silAndVis = &mSilCache.access(o);
	CSilhouette *sil = &silAndVis->mSil;
	bool *vis = silAndVis->mVis;
#endif
#if 0
	glColorMask(1,1,1,1);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(1);
    glBegin(GL_LINES);
    {for(S32 i = 0 ; i < sil->mStarts.size() ; i++){
        glColor3f(1.0, 1.0, 0.0);
        glVertex3fv(&sil->mStarts[i].x);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3fv(&sil->mEnds[i].x);
    }}
    glEnd();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
	glColorMask(0,0,0,0);
    glDepthMask(0);
#endif
    CVector4 temp;
    S32 count = 0;

    buffer_vertex_t *bufferVertex = (buffer_vertex_t *)r.getArrays()->mVertexBuffer->map() + r.getArrays()->numVerts;

    const S32 size = sil->mStarts.size();
    for(S32 i = 0 ; i < size ; ++i){
        r.pushElem(0 + count, 1 + count, 2 + count);
        r.pushElem(0 + count, 2 + count, 3 + count);
        count += 4;
    }
    U32 *elems = (U32 *)mElems->map(CIndexBuffer::READ_ONLY);
    if(r.isUseShadowVP()){
        for(S32 i = 0 ; i < size ; ++i){
            *bufferVertex = sil->mStarts[i];
            ++bufferVertex;

            *bufferVertex = sil->mEnds[i];
            ++bufferVertex;
            temp = sil->mEnds[i];
            temp.w = 0.0;

            *bufferVertex = temp;
            ++bufferVertex;
            temp = sil->mStarts[i];
            temp.w = 0.0;

            *bufferVertex = temp;
            ++bufferVertex;
        }

        for(S32 i = 0 ; i < numElems ; ++i){
            S32 k = i * 3;
            if(vis[i]){
                r.pushElem(2 + count, 1 + count, 0 + count);
                count += 3;
                r.pushElem(1 + count, 2 + count, 0 + count);
                count += 3;
                const CVector4 &v1 = mSystemVertices[elems[k]];
                const CVector4 &v2 = mSystemVertices[elems[k+1]];
                const CVector4 &v3 = mSystemVertices[elems[k+2]];
                *bufferVertex = v1;
                ++bufferVertex;
                *bufferVertex = v2;
                ++bufferVertex;
                *bufferVertex = v3;
                ++bufferVertex;

                temp = v1; temp.w = 0.0;
                *bufferVertex = temp;
                ++bufferVertex;

                temp = v2; temp.w = 0.0;
                *bufferVertex = temp;
                ++bufferVertex;

                temp = v3; temp.w = 0.0;
                *bufferVertex = temp;
                ++bufferVertex;
            }
        }
    } else {
        for(S32 i = 0 ; i < size ; ++i){
            *bufferVertex = sil->mStarts[i];
            ++bufferVertex;

            *bufferVertex = sil->mEnds[i];
            ++bufferVertex;
            temp = sil->mEnds[i] - o;
            temp.w = 0.0;

            *bufferVertex = temp;
            ++bufferVertex;
            temp = sil->mStarts[i] - o;
            temp.w = 0.0;

            *bufferVertex = temp;
            ++bufferVertex;
        }

        for(S32 i = 0 ; i < numElems ; ++i){
            S32 k = i * 3;
            if(vis[i]){
                r.pushElem(2 + count, 1 + count, 0 + count);
                count += 3;
                r.pushElem(1 + count, 2 + count, 0 + count);
                count += 3;
                const CVector4 &v1 = mSystemVertices[elems[k]];
                const CVector4 &v2 = mSystemVertices[elems[k+1]];
                const CVector4 &v3 = mSystemVertices[elems[k+2]];
                *bufferVertex = v1;
                ++bufferVertex;
                *bufferVertex = v2;
                ++bufferVertex;
                *bufferVertex = v3;
                ++bufferVertex;

                temp = v1 - o; temp.w = 0.0;
                *bufferVertex = temp;
                ++bufferVertex;

                temp = v2 - o; temp.w = 0.0;
                *bufferVertex = temp;
                ++bufferVertex;

                temp = v3 - o; temp.w = 0.0;
                *bufferVertex = temp;
                ++bufferVertex;
            }
        }
    }
    mElems->unmap();
    r.getArrays()->numVerts += count;
    pos.w = 0.0;
    r.flushShadowVolume(pos);
    r.clearArrays();
    
#endif
}

void CRenderableMesh::genSil::operator()(const CVector3 &o, CSilAndVis &sv){
	CSilhouette &s = sv.mSil;
	s.mEnds.clear();
    s.mStarts.clear();

    if(!self->elemPlanes)
        return;

	sv.mVis = new bool[self->numElems]; // Performance hit??
	bool *vis = sv.mVis;

    {for(S32 i = 0 ; i < self->numElems ; ++i){
        if(self->elemPlanes[i].distance(o) < 0.0)
            vis[i] = false;
        else
            vis[i] = true;
    }}

    // FIXME BUFFER SPEED
    U32 *elems = (U32 *)self->mElems->map(CIndexBuffer::READ_ONLY);
    {for(S32 i = 0 ; i < self->numElems ; i++){
        if(!vis[i])
            continue;

        S32 k = i * 3;
        for(S32 j = 0 ; j < 3 ; j++){
            //if(neighbors[k + j] == -1)
            //   continue;
            if(self->neighbors[k + j] < 0 ||vis[self->neighbors[k + j]] == false){
                s.mStarts.push_back(self->mSystemVertices[elems[k + j]]);
                s.mEnds.push_back(self->mSystemVertices[elems[k + (j+1)%3]]);
            }
        }
    }}
    self->mElems->unmap();
}

CSilhouette* CRenderableMesh::getSilhoutte(CVector3 &o){
#ifdef NO_CACHE
    sil->mEnds.clear();
    sil->mStarts.clear();

    if(!elemPlanes)
        return sil;
	if(!elemsVis)
		elemsVis = new bool[numElems];
    {for(S32 i = 0 ; i < numElems ; i++){
        if(elemPlanes[i].distance(o) < 0.0)
            elemsVis[i] = false;
        else
            elemsVis[i] = true;
    }}

    {for(S32 i = 0 ; i < numElems ; i++){
        if(!elemsVis[i])
            continue;

        S32 k = i * 3;
        for(S32 j = 0 ; j < 3 ; j++){
            //if(neighbors[k + j] == -1)
            //   continue;
            if(neighbors[k + j] < 0||elemsVis[neighbors[k + j]] == false){
                sil->mStarts.push_back(vertices[elems[k + j]]);
                sil->mEnds.push_back(vertices[elems[k + (j+1)%3]]);
            }
        }
    }}

    return sil;
#else
	return &mSilCache.access(o).mSil;
#endif
}

void CRenderableMesh::renderWithShader(CRenderer &r, CMaterial *shader){
    makeFrame();
    CArrays *a = r.getArrays();
    
    a->mVertexBuffer = mVertexBuffer;
    a->verts.setupBuffer(mVertexBuffer, CVertexBuffer::POSITION);
    a->normals.setupBuffer(mVertexBuffer, CVertexBuffer::NORMAL);
    a->tangent.setupBuffer(mVertexBuffer, CVertexBuffer::TANGENT);
    a->tex_st.setupBuffer(mVertexBuffer, CVertexBuffer::TEX_COORD);

    a->numVerts = numVertices;
    a->numElems = numElems * 3;

    a->mElems = mElems;
}

void CRenderableMesh::makeRElems(){
	if(!relems)relems = new CRenderableElem[numElems];
    U32 *elems = (U32 *)mElems->map(CIndexBuffer::READ_ONLY);
	for(S32 i = 0 ; i < numElems ; i++){
		S32 j = i * 3;
        relems[i] = CRenderableElem(&vertices[elems[j + 2]].toVec3(), 
                                    &vertices[elems[j + 1]].toVec3(), 
                                    &vertices[elems[j]].toVec3());
	}
	mElems->unmap();
	makeNormals();
}

void CRenderableMesh::makeNormals(){
	S32 *count = new S32[numVertices];
	S32 i;
	
	if(numFrames != 1) return;
	
	for(i = 0 ; i < numVertices ; i++){
		normals[i] = CVector3(0.0, 0.0, 0.0);
		count[i] = 0;
	}
	
    U32 *elems = (U32 *)mElems->map(CIndexBuffer::READ_ONLY);
	for(i = 0 ; i < numElems ; i++){
		S32 j = i * 3;
		CVector3 normal = relems[i].plane.normal;
		S32 e0 = elems[j];
		S32 e1 = elems[j + 1];
		S32 e2 = elems[j + 2];
		normals[e0] += normal; count[e0]++;
		normals[e1] += normal; count[e1]++;
		normals[e2] += normal; count[e2]++;

		calcTangentVector(vertices[e0], vertices[e1], vertices[e2],
						  tex_st[e0], tex_st[e1], tex_st[e2],
						  normals[e0], tangent[e0]);
		calcTangentVector(vertices[e1], vertices[e2], vertices[e0],
						  tex_st[e1], tex_st[e2], tex_st[e0],
						  normals[e1], tangent[e1]);
		calcTangentVector(vertices[e2], vertices[e0], vertices[e1],
						  tex_st[e2], tex_st[e0], tex_st[e1],
						  normals[e2], tangent[e2]);
	}
    mElems->unmap();
	
	for(i = 0 ; i < numVertices ; i++ ){
		normals[i] = normals[i] / count[i];
		normalize(tangent[i]);
	}

	delete [] count;
}

void CRenderableMesh::setFrame(F32 frame){
	currentFrame = frame;
	mSilCache.purgeData();
}

//#define DEBUG_NEIGH
void findUniqueVertices(const vector<CVector3> &original, vector<S32> &reorder);
class PolyEdge {
public:
    PolyEdge() : poly(-1), edge(-1) {}
    S32 poly;
    S32 edge;
};

template <class T>
class CSparseVector {
    class CNode {
    public:
        U32 mIndex;
        T mData;
    };
public:
    CSparseVector(const T &def){
        mDefaultData = def;
    }

    const T &operator[](U32 pos) const {
        return T();
    }

    T &operator[](U32 pos){
        for(typename TDataContainer::iterator i = mData.begin() ; i != mData.end() ; ++i){
            if((*i).mIndex == pos)
                return (*i).mData;
        }

        CNode n;
        n.mIndex = pos;
        n.mData = mDefaultData;
        mData.push_back(n);
        return mData.back().mData;
    }
protected:
    typedef list<CNode> TDataContainer;
    TDataContainer mData;
    T mDefaultData;
};

const S32 hashSize = 1024;
U32 hashValue(const CVector3 &v){
    S32 hash = (S32)(fabs(v.x)) + (S32)(fabs(v.y)) + (S32)(fabs(v.z)) / 64;
    hash &= hashSize - 1;
    return hash;
}

void findUniqueVertices(const vector<CVector3> &original, vector<S32> &reorder){
    typedef list<U32> TBucket;
    vector<TBucket> vertexHash;
    vertexHash.resize(hashSize);

#ifdef DEBUG_NEIGH
    cout << "++ Using hash table with size : "<<hashSize<<endl;
#endif

    reorder.clear();
    reorder.resize(original.size(), -1);

    vector<CVector3>::const_iterator i;
    U32 count = 0;
    for(i = original.begin() ; i != original.end() ; ++i, ++count){
        U32 hash = hashValue(*i);
        vertexHash[(hash - 1) & (hashSize - 1)].push_back(count);
        vertexHash[hash].push_back(count);
        vertexHash[(hash + 1) & (hashSize - 1)].push_back(count);
    }
    vector<TBucket>::iterator bi;
    U32 maxSize = 0;
    for(bi = vertexHash.begin() ; bi != vertexHash.end() ; ++bi){
        if((*bi).size() > maxSize){
            maxSize = (*bi).size();
        }
    }
#ifdef DEBUG_NEIGH
    cout << "++ Vertices : " << original.size() << " - Max in bucket : " << maxSize << endl;
#endif
    for(bi = vertexHash.begin() ; bi != vertexHash.end() ; ++bi){
        TBucket &bucket = *bi;
        for(list<U32>::iterator i = bucket.begin() ; i != bucket.end() ; ++i){
            list<U32>::iterator j = i;
            ++j;
            const CVector3 &v1 = original[*i];
            for(; j != bucket.end() ; ++j){
                const CVector3 &v2 = original[*j];
                if(v1 == v2){
                    S32 index = reorder[*i] >= 0 ? reorder[*i] : *i;
                    reorder[*j] = index;
                    assert(reorder[index] == -1);
                }
            }
        }
    }

    count = 0;
    S32 id = 0;
    for(vector<S32>::iterator i = reorder.begin() ; i != reorder.end() ; ++i, ++id){
        if(*i < 0){
            count++;
            *i = id;
        }
    }
#ifdef DEBUG_NEIGH
    cout << "++ Unique vertices :" << count << endl;
#endif

#if 0
    cout << "-- No hashing --" << endl;
    reorder.clear();
    reorder.resize(original.size(), -1);
    S32 x = 0;
    for(vector<CVector3>::const_iterator i = original.begin() ; i != original.end() ; ++i, ++x){
        vector<CVector3>::const_iterator j = i;
        ++j;
        S32 y = x + 1;
        for(; j != original.end() ; ++j, ++y){
            if(*i == *j){
                reorder[y] = x;
            }
        }
    }
    count = 0;
    for(vector<S32>::iterator i = reorder.begin() ; i != reorder.end() ; ++i){
        if(*i < 0){
            count++;
        }
    }
    cout << "Unique vertices :" << count << endl;
#endif;

}

void CRenderableMesh::makeNeighbors(){
    U32 i,k;

    vector<CVector3> originals;
    originals.reserve(numVertices);

    mSystemVertices.resize(numVertices);
    for(i = 0 ; i < numVertices ; ++i){
        mSystemVertices[i] = vertices[i];
        originals.push_back(mSystemVertices[i].toVec3());
    }
    
	// Map the element buffer
	U32 *elems = (U32 *)mElems->map(CIndexBuffer::READ_ONLY);
    
	mSystemElems.resize(numElems * 3);
	for(i = 0 ; i < numElems * 3 ; ++i){
		mSystemElems[i] = elems[i];
	}
    
    vector<S32> verticesReorder;

    findUniqueVertices(originals, verticesReorder);
    
    if(!neighbors)neighbors = new S32[numElems * 3];
    for(k = 0 ; k < numElems * 3 ; k++){
        neighbors[k] = -1;
    }
       
    typedef CSparseVector<PolyEdge> CharVector;
    vector<CharVector> conn(numVertices, CharVector(PolyEdge()));
    
    S32 v1,v2;
    for(i = 0 ; i < numElems ; ++i){
        for(k = 0 ; k < 3 ; ++k){
            U32 e = elems[i * 3 + ((k + 1) % 3)];
            v2 = verticesReorder[e];
            e = elems[i * 3 + k];
            v1 = verticesReorder[e];
            
            PolyEdge &ab = conn[v1][v2];
            PolyEdge &ba = conn[v2][v1];
            if(ab.edge >= 0){
                continue;
            }
            ab.edge = i * 3 + k;
            ab.poly = i;
            if(ba.edge >= 0){
                if(neighbors[ba.edge] == -1)
                    neighbors[ba.edge] = i;
                else
                    neighbors[ba.edge] = -2;
                if(neighbors[ab.edge] == -1)
                    neighbors[ab.edge] = ba.poly;
                else
                    neighbors[ab.edge] = -2;
            }
        }
    }

    mElems->unmap();
#if 0
    for(S32 i = 0 ; i < numElems ; ++i){
        cout << neighbors[i] << " ";
    }
    cout << endl;
#endif

}

void CRenderableMesh::makeElemNormals(){
    //if(!elemNormals)elemNormals = new CVector3[numElems];
	if(!elemPlanes)elemPlanes = new CPlane[numElems];
    U32 *elems = (U32 *)mElems->map(CIndexBuffer::READ_ONLY);
    for(S32 i = 0 ; i < numElems ; i++){
        S32 k = i * 3;
        CPlane p(vertices[elems[k]].toVec3(), vertices[elems[k+1]].toVec3(), vertices[elems[k+2]].toVec3());
        //elemNormals[i] = normals[elems[k]] + normals[elems[k + 1]] + normals[elems[k + 2]];
		elemPlanes[i] = p;
        //normalize(elemNormals[i]);
    }
    mElems->unmap();
}

//
//  CRenderableManager
//

#include <engine.h>

CRenderableManager::CRenderableManager(){
}

CRenderableManager::~CRenderableManager(){
}

void CRenderableManager::registerRenderable(CRenderable *r){
	resources.push_back(r);
}

CRenderable* CRenderableManager::findRenderable(const string name){
	return static_cast<CRenderable*>(find(name));
}

