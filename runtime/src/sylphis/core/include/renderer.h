/***************************************************************************
                          renderer.h  -  description
                             -------------------
    begin                : Sun Mar 10 2002
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

#ifndef RENDERER_H
#define RENDERER_H

#include <map>

#include <vector.h>
#include <material.h>
#include <camera.h>
#include <renderingview.h>
#include <light.h>
#include <matrix.h>
#include <timer.h>

#include <texture.h>
#include <vertexbuffer.h>
#include <indexbuffer.h>
#include <rendertexture.h>


class CEngine;
class CRenderQueue;
class CRenderChunk;
class CMaterial;
class CMaterialPass;

typedef struct {
    CVector4 position;
    CVector3 normal;
    CVector3 tangent;
    CTexCoord tex_coord;
} vertex_t;

class CArrays {
public:
    CVertexBufferPtr mVertexBuffer;

    CVertexBufferAccess<CVector4> verts;
    CVertexBufferAccess<CVector3> normals;
	CVertexBufferAccess<CVector3> tangent;
    CVertexBufferAccess<CTexCoord> tex_st;

	CColor *colors;
	CVector3 *env_st;
	CVector3 *spec_st;

	S32 numVerts;
    S32 numElems;
	//U32 *elems;
    CIndexBufferPtr mElems;
};

class COverlayQuad {
public:
    CVector3 points[4];
    CTexCoord tex_st[4];
    CColor colors[4];
    CMaterialPtr shader;
};

class CRenderer {
public:
	CRenderer();
	virtual ~CRenderer();
	virtual void startup();
	void shutdown();

    void flushShadowVolume(CVector4 lightPos);
    void flushShadowVolume();

    virtual bool isSupported() = 0;
    virtual const char *getName() = 0;
	
	void setRenderingView(CRenderingTarget &v);
    void setRenderingView(CTexture &v);
	const CRenderingTarget &getRenderingView() const;

	void addLight(CLight *l);
	void ignoreNewLights(bool i);
    list<CLightPtr>* getLights();
	void clearLights();
	
	void useCamera(CCamera *c);
	CCamera *getCamera();

	void setSkyShader(CMaterial *s);
	void setSkyFlags(S32 flags);
	
	void beginFrame();
	void endFrame();

    void setOverrideShader(CMaterial *s);
    CMaterial* getOverrideShader();

    void setLastMVPMatrix(const CMatrix4 &m);
    CMatrix4 &getLastMVPMatrix();

    CMatrix4 &getModelViewMatrix();
    CMatrix4 &getProjectionMatrix();
    CMatrix4 &getMVPMatrix();

    void beginObjectTransformations();
    void endObjectTransformations();
    void clearTransformations();
	void translate(const CVector3 &o);
	void rotate(const CVector3 &r);
    void rotate(const CQuaternion &q);
    void rotate(const CMatrix4 &m);
	void scale(CVector3 s);
	void GLBeginQuad();
	void GLBeginLine();
	void GLEnableTexture2D();
	void GLDisableTexture2D();
	void GLEnd();
	void GLColor4(F32 r, F32 g, F32 b, F32 a = 1.0);
	void GLVertex3(F32 x, F32 y, F32 z);
	void GLVertex2(F32 x, F32 y);
	void GLTexCoord2(F32 s, F32 t);
    void setScissor(U32 x, U32 y, U32 w, U32 h);
	
	void setup3DMode(CRenderingTarget *rt, F32 ratio = 0.0);
	void setup2DMode(CRenderingTarget *rt);

    void setRenderQueue(CRenderQueue *rq);
    CRenderQueue *getRenderQueue();

    void flushRenderQueue(CLight *light = 0);

    void renderShadowVolumes(CLight *light, S32 back);

	void setLight(CLight *light);
	CLight *getLight();

	void projectPoint(const CVector3 &c, CVector3 &p);

	void setupScissor();
	void clearStencil();

    void setAmbientLight(const CColorf &a);

    void renderChunk(CRenderChunk *rc);

    void drawPic(S32 x, S32 y, S32 w, S32 h, CMaterial *shader, F32 s1 = 0.0, F32 t1 = 0.0, F32 s2 = 1.0, F32 t2 = 1.0, CColor *c = 0);
	
	void pushRaw(CVector3 *points, CVector3 *normals, CVector3 *tangent, CTexCoord *tm_st, U32 *elems, S32 numVerts, S32 numElems);
    
    void renderMaterialPass(CMaterialPass *pass, F64 time, CMatrix4 *WorldSpaceMat, CMatrix4 *invWorldSpaceMat);
    void flushWithMaterial(CMaterial *material, CMatrix4 *WorldSpaceMat, CMatrix4 *invWorldSpaceMat);

    CArrays *getArrays();
    void clearArrays();
    void drawArrays();

    void useDefaultArrays();

    void pushVertex(const CVector3 &v);
    void pushVertex(const CVector4 &v);
    void pushVertex(const F32 x, const F32 y, const F32 z);
    void pushVertex(const F32 x, const F32 y, const F32 z, const F32 w);
    void pushElem(const S32 a, const S32 b, const S32 c);

    virtual void debugDrawLine(const CVector3 &start, const CVector3 &end, CColor *c = 0);

    void enter2DMode();
    void exit2DMode();

	U32 getTextureDataSize();

    void grabCurrentRender();

	F32 getEPS();

    void setUseShadowVP(bool b);
    bool isUseShadowVP();

    void setUseHDR(bool b);
    bool isUseHDR();

	void setMaxLuminance(F32 l);
	void setMinLuminance(F32 l);
	F32  getMaxLuminance() const;
	F32  getMinLuminance() const;

    void setMotionBlurAmount(F32 amount);
    F32  getMotionBlurAmount();   

	void setShadows(bool b);
	bool isShadows();

    bool needSpeedRender();

    void renderOverlay2D();

    // FOG
    void setEnableFog(bool b);
    void setFogColor(const CVector3 &color);
    void setFogBounds(F32 start, F32 end);

    void  setNearDepth(F32 d);
    F32 getNearDepth();

	virtual CTexture *registerAttenuation(const std::string &name);

	virtual void renderPerformaceInfo();
    float calcAverageLuma(CTexture2D *tex = 0);
    void makeBloomTexture();

	virtual void downSample(CTexture2D* in, CRenderTexture* out, U32 inw, U32 inh, U32 outw, U32 outh) const;
	virtual void scaleDown(CTexture2D* in, CRenderTexture* out, int outWidth, CRenderTexture* tempTex = 0) const;
protected:
    
	void addEdge(const CVector3 &v1, const CVector3 &v2);
	//F64 evaluateFunc(const shaderfunc_t &func, F64 time);

    CColorf             mAmbientLight;

	CLightPtr           mLight;
	CVector3            objectSpaceLight;
	CVector3            objectSpaceEye;

	CEngine*            eng;
	CRenderingTarget*   renderingView;

	CArrays             arrays;

    CVertexBufferPtr    mVertexBuffer;
    CIndexBufferPtr     mElemBuffer;

    CMaterialPtr        mOverrideShader;

    CMaterialPtr        mWhiteShader;
    CTexturePtr         mWhiteTexture;
    CTexturePtr         mBlackTexture;

    CRenderTexturePtr   mCurrentRender;
    CRenderTexturePtr   mCurrentRenderSmall;

    CRenderTexturePtr   mBufferA;
    CRenderTexturePtr   mBufferB;

    CVector2            mCurrentRenderNormalize;

    typedef std::map<std::string, CTexturePtr> StringTextureMap;
	StringTextureMap    mAttenuationTextures;

	bool				mShadows;

    CMatrix4            mModelView;
    CMatrix4            mProjection;
    CMatrix4            mMVP;
    CMatrix4            mLastMVPMatrix;
    CMatrix4            mLastModelView;

private:
    bool                mNeedSpeedRender;
	bool                mAddLights;

    list<CLightPtr>     mLights;
    CRenderQueue*       mRenderQueue;

    CCamera*            camera;

    F32 mNearDepth;

    vector<COverlayQuad> overlayQuads;

	CMaterial*          mSkyShader;
	S32	                mSkyFlags;

    CVertexProgram      mShadowVP;
    bool                mUseShadowVP;

    bool                mUseHDR;
	bool				mCapHDR;
	F32					mMaxLuminance;
	F32					mMinLuminance;
    F32                 mMotionBlurAmount;

	S32                 mSciRect[4];
	S32                 mLastSciRect[4];

    bool mEnabledFog;

#define NUM_OF_LAST_EPS 64
	F32 mLastEPSs[NUM_OF_LAST_EPS];
	S32 mFrameCount;

	CTimerAdvanced mFrameCommitTimer;
	CTimerAdvanced mTimer1;
	CTimerAdvanced mTimer2;
};

F32 WrapToOne(F32 x);

inline void CRenderer::pushVertex(const CVector3 &v){
    arrays.verts[arrays.numVerts++] = v;
}

inline void CRenderer::pushVertex(const CVector4 &v){
    CVector4 &av = arrays.verts[arrays.numVerts++];
    av.x = v.x;
    av.y = v.y;
    av.z = v.z;
	av.w = v.w;
}

inline void CRenderer::pushVertex(const F32 x, const F32 y, const F32 z){
    CVector4 &av = arrays.verts[arrays.numVerts++];
    av.x = x;
    av.y = y;
    av.z = z;
}

inline void CRenderer::pushVertex(const F32 x, const F32 y, const F32 z, const F32 w){
    CVector4 &av = arrays.verts[arrays.numVerts++];
    av.x = x;
    av.y = y;
    av.z = z;
	av.w = w;
}

/* A call to this must come before pushVertex() */
inline void CRenderer::pushElem(const S32 a, const S32 b, const S32 c){
    S32 numElems = arrays.numElems;
    U32 *elems = (U32 *)arrays.mElems->map();
    elems[numElems] = a;
    elems[numElems+1] = b;
    elems[numElems+2] = c;
    arrays.numElems = numElems + 3;
}

#endif
