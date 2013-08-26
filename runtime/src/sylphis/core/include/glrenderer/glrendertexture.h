/***************************************************************************
                         rendertexture.h  -  description
                             -------------------
    begin                : Sat Sep 17 2004
    copyright            : (C) 2004 by Harry Kalogirou
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

#ifndef _GLRENDERTEXTURE_H_
#define _GLRENDERTEXTURE_H_

#include <texture.h>
#include <rendertexture.h>
#include <glrenderer/gltexture.h>
#include <renderingview.h>

SmartPointer(CGLRenderTexture);
class CGLRenderTexture : public CRenderTexture, public CGLTexture {
public:
    CGLRenderTexture();
	CGLRenderTexture(const string &n);
    virtual void create(U32 width, U32 height, 
                bool mipmaps = false, bool anisotropic = false,
                U32 Rbits = 8, U32 Gbits = 8, U32 Bbits = 8, U32 Abits = 8,
                U32 Dbits = 0, U32 Sbits = 0);

    virtual void beginCapture();
    virtual void endCapture();

    virtual void setDimentions(const pair<S32,S32> &d);
	virtual void setDimentions(const S32 width, const S32 heigth);
	virtual pair<S32,S32> getDimentions();

    virtual void activate(F32 time = 0.0);
	virtual void deactivate();

	virtual void enable();
	virtual void disable();
    // Dummy
    virtual void makeTexture(CImage *image, S32 maxDimention, S32 bias, S32 flags);

private:
    U32 mRedBits;
    U32 mGreenBits;
    U32 mBlueBits;
    U32 mAlphaBits;
    U32 mDepthBits;
    U32 mStencilBits;

    bool mHasMipmaps;
    bool mHasAnisotropic;

    bool mCreated;

    GLuint      mFBO;
    GLuint      mDepthRenderBuffer;
    GLuint      mStencilRenderBuffer;
};

#endif

