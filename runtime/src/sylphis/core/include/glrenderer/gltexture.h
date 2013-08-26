/***************************************************************************
                          gltexture.h  -  description
                             -------------------
    begin                : Thu Apr 5 2006
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

#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#ifdef _WIN32
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <openglext.h>

#include <texture.h>

class CImage;

class CGLTexture {
protected:
	//U32 texNum;
	GLenum mGLType;
};

SmartPointer(CGLTexture1D);
class CGLTexture1D : public CTexture1D, public CGLTexture {
public: 
	CGLTexture1D();
	CGLTexture1D(const string &n);
	virtual ~CGLTexture1D();

	virtual void activate(F32 time = 0.0);
	virtual void deactivate();

	virtual void enable();
	virtual void disable();

    virtual void makeTexture(CImage *image, S32 maxDimention, S32 bias, S32 flags);
};

SmartPointer(CGLTexture2D);
class CGLTexture2D : public CTexture2D, public CGLTexture {
public: 
	CGLTexture2D();
	CGLTexture2D(const string &n);
	virtual ~CGLTexture2D();

	virtual void activate(F32 time = 0.0);
	virtual void deactivate();

	virtual void enable();
	virtual void disable();

    virtual void makeTexture(CImage *image, S32 maxDimention, S32 bias, S32 flags);
};

SmartPointer(CGLTextureCube);
class CGLTextureCube : public CTextureCube, public CGLTexture {
public: 
	CGLTextureCube();
	CGLTextureCube(const string &n);
	virtual ~CGLTextureCube();

	virtual void activate(F32 time = 0.0);
	virtual void deactivate();

	virtual void enable();
	virtual void disable();

    virtual void makeTexture(CImage *image, S32 texNo, S32 maxDimention, S32 bias, S32 flags);
};

SmartPointer(CGLTexture3D);
class CGLTexture3D : public CTexture3D, public CGLTexture {
public: 
	CGLTexture3D();
	CGLTexture3D(const string &n);
	virtual ~CGLTexture3D();

	virtual void activate(F32 time = 0.0);
	virtual void deactivate();

	virtual void enable();
	virtual void disable();

    virtual void makeAttenTexture(const std::string &attMap, S32 size);
};

SmartPointer(CGLTextureRect);
class CGLTextureRect : public CTextureRect, public CGLTexture {
public: 
	CGLTextureRect();
	CGLTextureRect(const string &n);
	virtual ~CGLTextureRect();

	virtual void activate(F32 time = 0.0);
	virtual void deactivate();

	virtual void enable();
	virtual void disable();
};

#endif
