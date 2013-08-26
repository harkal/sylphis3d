/***************************************************************************
                         rendertexture.h  -  description
                             -------------------
    begin                : Sat Sep 17 2004
    copyright            : (C) 2004 by Harry Kalogirou
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

