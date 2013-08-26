/***************************************************************************
                          material.h  -  description
                             -------------------
    begin                : Sat Apr 22 2006
    copyright            : (C) 2006 by Harry Kalogirou
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

