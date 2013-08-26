/***************************************************************************
                          renderable.h  -  description
                             -------------------
    begin                : Wed Mar 27 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
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
