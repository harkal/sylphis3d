/***************************************************************************
                          texture.h  -  description
                             -------------------
    begin                : Wed Mar 20 2002
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
