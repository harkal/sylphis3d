/***************************************************************************
                          material.h  -  description
                             -------------------
    begin                : Sat Apr 22 2006
    copyright            : (C) 2006 by Harry Kalogirou
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

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <vector>
#include <set>
#include <map>
#include <vector.h>
#include <pointer.h>
#include <resource.h>
#include <texture.h>

#include <Python.h>



enum EUniformType {
    UNIFORM_1F,
    UNIFORM_2F,
    UNIFORM_3F,
    UNIFORM_4F,
    UNIFORM_PYTHON_1F,

    UNIFORM_LIGHTPOS,
    UNIFORM_EXPOSURE,
    UNIFORM_LIGHTCOLOR,
    UNIFORM_LIGHTSIZE,
    UNIFORM_CAMERAPOS,
    UNIFORM_CURRENTRENDERNORMALIZE,
    UNIFORM_INVVIEWSIZE,
    UNIFORM_FRAMETIME,
    UNIFORM_LASTMVPMATRIX
} ;

typedef struct {
    EUniformType mType;
    S32 location;
    CVector4 data;
    PyObject *func;
} SUniformParameter;

enum ESamplerType {
    SAMPLER_ATTENUATION,
    SAMPLER_FILTER
};

typedef struct {
    S32 location;
    ESamplerType mType;
    CTexturePtr texture;
} SSamplerParameter;

enum EAttribType {
    ATTRIB_TANGENT,
    ATTRIB_BINORMAL
};

#define BIND_NORMAL     1
#define BIND_TANGENT    2
#define BIND_TEXCOORD0  4

typedef struct {
    S32 location;
    EAttribType mType;
} SAttribParameter;

enum EBlendType {
    BLEND_NONE,
    BLEND_ZERO,
    BLEND_ONE,
    BLEND_DST_COLOR,
    BLEND_ONE_MINUS_SRC_ALPHA,
    BLEND_SRC_ALPHA,
    BLEND_SRC_COLOR,
    BLEND_ONE_MINUS_DST_COLOR,
    BLEND_ONE_MINUS_SRC_COLOR,
    BLEND_DST_ALPHA,
    BLEND_ONE_MINUS_DST_ALPHA
};

enum EDepthWriteType {
    DEPTHWRITE_NONE,
    DEPTHWRITE_EQUAL,
    DEPTHWRITE_LEQUAL,
    DEPTHWRITE_LESS,
    DEPTHWRITE_GREATER,
    DEPTHWRITE_GEQUAL
};

enum ECullType {
    CULL_BACK,
    CULL_FRONT,
    CULL_NONE
};

SmartPointer(CRenderTexture);

SmartPointer(CMaterialPass);
class CMaterialPass : public CRefObject {
public:
    typedef std::vector<SUniformParameter>  UniformVector;
    typedef std::vector<SSamplerParameter>  SamplerVector;
    typedef std::vector<SAttribParameter>   AttribVector;

    CMaterialPass();

    void        setName(const std::string &name);
    std::string getName();

    void        setShader(CGLShader* shader);
    CGLShader*  getShader();

    UniformVector&  getUniforms();
    SamplerVector&  getSamplers();
    AttribVector&   getAttribs();

    void            setTarget(CRenderTexture *tex);
    CRenderTexture* getTarget();

    void            setBlend(const EBlendType& s, const EBlendType& d);
    EBlendType      getBlendSrc() const;
    EBlendType      getBlendDst() const;

    void            setDepthTest(bool dt);
    bool            getDepthTest() const;

    void            setDepthWrite(EDepthWriteType dw);
    EDepthWriteType& getDepthWrite();

    void            setCullType(ECullType ct);
    ECullType&      getCullType();

    void            setAmbient(bool ambient);
    bool            isAmbient() const;

    U32             getBindArrays() const;
    void            setBindArrays(const U32 &ba);

	CTexture*		getTexture();
	void			setTexture(CTexture *t);
protected:
    std::string     mName;
    CGLShaderPtr    mGLShader;
    UniformVector   mUniforms;
    SamplerVector   mSamplers;
    AttribVector    mAttribs;
    bool            mIsAmbient;
    CRenderTexturePtr mTarget;
    EBlendType      mBlendSrc;
    EBlendType      mBlendDst;
    EDepthWriteType mDepthWrite;
    bool            mDepthTest;
    ECullType       mCullType;
    U32             mBindArrays;

	CTexturePtr		mTexture;		// Simple texture to bind when no GLSL shader
};

typedef std::vector<CMaterialPassPtr> MaterialPassVector;

SmartPointer(CMaterialTechnique);
class CMaterialTechnique : public CRefObject {
public:
    void                setName(const std::string &name);
    std::string         getName();

    MaterialPassVector& getPasses();
    void                setAmbientPass(CMaterialPass *pass);
    CMaterialPass*      getAmbientPass();

    CMaterialPass*      getPass(const string &name);
    void                addPass(CMaterialPass *pass);
protected:
    std::string         mName;
    MaterialPassVector  mPasses;
    CMaterialPassPtr    mAmbientPass;
};

typedef std::vector<CMaterialTechniquePtr> MaterialTechniqueVector;

SmartPointer(CMaterial);
class CMaterial : public CResource {
public:
    CMaterial();
//    CMaterial(CMaterial &mat);
    virtual ~CMaterial();

    MaterialTechniqueVector&    getTechniques();
    void                        addTechnique(CMaterialTechnique* technique);
    CMaterialTechnique*         getActiveTechnique();
    void                        setActiveTechnique(CMaterialTechnique* technique);
    CMaterialTechnique*         getTechnique(const string &name);
	void                        setMotionBlurTechnique(CMaterialTechnique* technique);
    CMaterialTechnique*         getMotionBlurTechnique(const string &name);
    void                        getDimensions(U32 *w, U32 *h);
    void                        setDimensions(U32 w, U32 h);

    void                        markUsed();
    bool                        isTransparent();
    void                        setTransparent(bool t);

protected:
    MaterialTechniqueVector     mTechniques;
    CMaterialTechniquePtr       mActiveTechnique;
	CMaterialTechniquePtr       mMotionBlurTechnique;
    bool                        mTransparent;
    U32                         mWidth;
    U32                         mHeight;
};

class CMaterialManager {
public:
    CMaterialManager();
    ~CMaterialManager();
    CMaterial*  registerMaterial(const string &name);
    void        registerMaterial(CMaterial *mat);
    bool        isMaterialLoaded(const string &name);

	void		addFailedMaterial(const string &name);
	bool		isMaterialFailed(const string &name);

    typedef std::map<string, CMaterialPtr> MaterialMap;
protected:
	set<string> mMaterialsFailed;		
    MaterialMap mMaterials;
};

#endif

