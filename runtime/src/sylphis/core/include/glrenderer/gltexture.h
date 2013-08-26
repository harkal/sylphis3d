/***************************************************************************
                          gltexture.h  -  description
                             -------------------
    begin                : Thu Apr 5 2006
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
