/***************************************************************************
                          texture.h  -  description
                             -------------------
    begin                : Wed Mar 20 2002
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

#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef _WIN32
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <openglext.h>
#include <string>
#include <vector>
using namespace std;

#include <vector.h>
#include <matrix.h>
#include <matrix3.h>
#include "imagefile.h"
#include "resourcemanager.h"

#define SHADER_NOMIPMAPS	1 << 5
#define SHADER_NOPICMIP 	1 << 9
#define SHADER_CLAMP		1 << 10

SmartPointer(CTexture);
class CTexture : public CResource {
public:
    enum ETextureType {
        TEX_TYPE_1D,
        TEX_TYPE_2D,
		TEX_TYPE_RECT,
        TEX_TYPE_3D,
        TEX_TYPE_CUBE
    };
	CTexture();
	CTexture(const string &n);
	virtual ~CTexture();

    virtual void activate(F32 time = 0.0) = 0; // Astruct
	virtual void deactivate(){}; // Astruct
    virtual void enable(){}; // Astruct
	virtual void disable(){}; // Astruct

    void markUsed();
    bool wasUsed(S32 offset = 0);

	void setMemoryUsage(U32 b);
	U32 getMemoryUsage();

	ETextureType getType(){
		return mType;
	}

    void copyFromFrameBuffer();

    void getDimentions(S32 *w, S32 *h);
protected:
    S32 mWidth;
    S32 mHeight;

	ETextureType mType;

    S32 mLastFrameUsed;
	U32 mMemoryUsage;

   GLuint texNum; // FIXME REMOVE!
};

SmartPointer(CTexture1D);
class CTexture1D : public CTexture {
public:
    virtual void makeTexture(CImage *image, S32 maxDimention, S32 bias, S32 flags) = 0;
};

SmartPointer(CTexture2D);
class CTexture2D : public CTexture {
public:
    virtual void makeTexture(CImage *image, S32 maxDimention, S32 bias, S32 flags) = 0;
};

SmartPointer(CTextureAnim);
class CTextureAnim : public CTexture {
public:
	CTextureAnim();
	CTextureAnim(string &n);
	virtual ~CTextureAnim();

	virtual void activate(F32 time = 0.0);

	void addTexture(CTexture *t);
private:
	typedef std::vector<CTexturePtr> TexturesVector;
	TexturesVector mFrames;
	F32 mFPS;
};

SmartPointer(CTextureCube);
class CTextureCube : public CTexture {
public:
	CTextureCube();
	CTextureCube(string &n);
	virtual ~CTextureCube();

	virtual void makeTexture(CImage *image, S32 texNo, S32 maxDimention, S32 bias, S32 flags) = 0;
};

SmartPointer(CTexture3D);
class CTexture3D : public CTexture {
public:
	CTexture3D();
	CTexture3D(string &n);
	virtual ~CTexture3D();

    virtual void makeAttenTexture(const std::string &attMap, S32 size) = 0;
protected:
    S32 mLength;
};

SmartPointer(CTextureRect);
class CTextureRect : public CTexture {
public:
	CTextureRect();
	CTextureRect(string &n);
	virtual ~CTextureRect();
};

//
// Vertex Program
//

SmartPointer(CVertexProgram);
class CVertexProgram : public CTexture {
public:
    CVertexProgram();
    CVertexProgram(const string &n);
    virtual ~CVertexProgram();

    virtual void makeProgram(const string &program);

    virtual void activate(F32 time = 0.0);
	virtual void deactivate();
protected:
	virtual void compile(const string &program);
};

//
// Fragment Program
//

SmartPointer(CFragmentProgram);
class CFragmentProgram : public CVertexProgram {
public:
    CFragmentProgram();
    CFragmentProgram(const string &n);
    virtual ~CFragmentProgram();

    virtual void activate(F32 time = 0.0);
protected:
	virtual void compile(const string &program);
};

//
// GL Shader
//

SmartPointer(CGLShader);
class CGLShader : public CTexture {
public:
    CGLShader();
    CGLShader(const string &n);
    virtual ~CGLShader();

    virtual void makeShader(const string &vert, const string &frag, const vector<string> *defines = 0);

    virtual void activate(F32 time = 0.0);
	virtual void deactivate();

    virtual S32 getUniformLocation(const string &name);
    virtual void setUniform(S32 location, F32 x);
    virtual void setUniform(S32 location, F32 x, F32 y);
    virtual void setUniform(S32 location, F32 x, F32 y, F32 z);
    virtual void setUniform(S32 location, F32 x, F32 y, F32 z, F32 w);
    virtual void setUniform(S32 location, const CVector3 &data);
    virtual void setUniform(S32 location, const CVector4 &data);

    virtual S32 getAttributeLocation(const string &name);
    virtual void setAttribute3v(S32 location, F32 *data);
    virtual void setAttribute4v(S32 location, F32 *data);

    virtual void setUniformMatrix(S32 location, const CMatrix3 &mat);
    virtual void setUniformMatrix(S32 location, const CMatrix4 &mat);
protected:
	virtual void compile(const string &vert, const string &frag);
    GLuint mVertShader;
    GLuint mFragShader;
    GLuint mShader;
};

class CImage;
class CTextures : public CResourceManager {
public:
	CTextures();
	~CTextures();
	
	void registerTexture(CTexture *t);
	CTexture* registerTexture(const string &textureName, CImage *img = 0);
	CTexture* registerTexture(const string &textureName, S32 flags, CImage *img = 0);

	CTexture* registerNormalMap(const string &textureName, bool negGreen = true, CImage *img = 0);

	CTexture* registerHeightMap(const string &textureName, F32 hardness);

	CTexture* registerCubeMap(const string &textureName);

	CTexture* findTexture(const string &name);
	
protected:
};


#endif
