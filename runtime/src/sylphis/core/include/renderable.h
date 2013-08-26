/***************************************************************************
                          renderable.h  -  description
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

#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <vector.h>
#include <plane.h>
#include <sceneobject.h>
#include <renderer.h>
#include <resource.h>
#include <resourcemanager.h>
#include <cache.h>

#include <map>

class CSceneManager;
#include <silhouette.h>

class  CEdge {
public:
    CEdge(){};
    CEdge(U32 _a, U32 _b) : a(_a), b(_b) {};
    U32 a;
    U32 b;

    bool operator== (const CEdge &op1){
        return ((a == op1.a) && (b == op1.b)) ||
        ((a == op1.b) && (b == op1.a));
    }

	U64 getHash() const {
		U64 ret = a;
		ret = ret << 16;
		return (ret << 16) + b;
	}

	class key_compare {
		bool operator()(const CEdge &a, const CEdge &b){
			return a.getHash() < b.getHash();
		}
	};
};

SmartPointer(CRenderable);
class CRenderable : public CResource {
public:
    CRenderable();
    virtual ~CRenderable();

	virtual void setFrame(F32 f) = 0;
	virtual void makeFrame(){};
    virtual void render(CRenderer &r, const CMatrix4 &transform) = 0;
    virtual void renderShadowVolume(CRenderer &r, CVector3 &o, S32 back){};
    virtual CSilhouette* getSilhoutte(CVector3 &o) = 0;
    virtual void renderWithShader(CRenderer &r, CMaterial *shader) = 0;

    void setDefaultShader(CMaterial *shader);
    CMaterial *getDefaultShader();
//protected:
    CMaterialPtr defShader;
	bool mNoShadows;

	virtual CVector4 *getVertices(){return 0;}
	virtual S32 getNumOfVertices(){return 0;}
};

SmartPointer(CRenderableElem);
class CRenderableElem : public CRenderable {
public:
	CRenderableElem();
	CRenderableElem(CVector3 *_a, CVector3 *_b, CVector3 *_c);
	virtual ~CRenderableElem();
	
	void updatePlane();

	virtual void setFrame(F32 f){};
    virtual void render(CRenderer &r, const CMatrix4 &transform);
    virtual CSilhouette* getSilhoutte(CVector3 &o);
	virtual void renderWithShader(CRenderer &r, CMaterial *shader);
	
	CVector3 *a, *b, *c;
	CPlane plane;
};

SmartPointer(CRenderableMesh);
class CRenderableMesh : public CRenderable {
public:
    CRenderableMesh();
    virtual ~CRenderableMesh();

    virtual void _free();
    virtual void reset();
    virtual void makeNormals();
    virtual void makeRElems();

    virtual void setFrame(F32 frame);	
    virtual void makeFrame();
    virtual void render(CRenderer &r, const CMatrix4 &transform);
    virtual CSilhouette* getSilhoutte(CVector3 &o);
    virtual void renderShadowVolume(CRenderer &r, CVector3 &o, S32 back);
    virtual void renderWithShader(CRenderer &r, CMaterial *shader);

	virtual CVector4*	getVertices(){return &mSystemVertices[0];}
	virtual S32			getNumOfVertices(){return numVertices;}
	virtual U32*		getElements(){return &mSystemElems[0];}
	virtual S32			getNumOfElements(){return numElems;}

    // System memory buffer
    vector<CVector4>	mSystemVertices;
	vector<U32>			mSystemElems;
protected:
    // Hardware buffers
    CVertexBufferPtr mVertexBuffer;
    
    CVertexBufferAccess<CVector4> vertices;
    CVertexBufferAccess<CVector3> normals;
	CVertexBufferAccess<CVector3> tangent;
    CVertexBufferAccess<CTexCoord> tex_st;

    CVector3 *elemNormals;
	CPlane *elemPlanes;
    
    CRenderableElem *relems;
    S32 numVertices;

    CIndexBufferPtr mElems;
    
    bool *elemsVis;
    U32 *elemEdges;
    S32 *neighbors;
    S32 numElems;

    F32 currentFrame;
    S32 numFrames;

    void makeNeighbors();
    void makeElemNormals();

	class CSilAndVis {
	public:
		CSilAndVis(){mVis = 0;};
		~CSilAndVis(){if(mVis)delete [] mVis;};
		CSilhouette mSil;
		bool *mVis;
	};

	class genSil {
    public:
		CRenderableMesh *self;
		void operator()(const CVector3 &o, CSilAndVis &sv);
	};

	genSil mSilGen;
	CCache<CVector3, CSilAndVis, genSil> mSilCache;
    CSilhouette *sil;

    friend class CMeshLoader3DS;
	friend class CMD3Model;
	friend class CSMFModel;
	friend class CBrushModel;
    friend class CSceneModel;
    friend class genSil;
};

class CRenderableManager : public CResourceManager {
public:
	CRenderableManager();
	~CRenderableManager();

	void registerRenderable(CRenderable *r);

	CRenderable* findRenderable(const string name);
};

#endif
