/***************************************************************************
                       rendertexture.cpp  -  description
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

#include <gutil.h>
#include <console.h>
#include <configfile.h>
#include <myglu.h>
#include <glrenderer/glrendertexture.h>

#define CHECK_FRAMEBUFFER_STATUS()                            \
      {                                                           \
        GLenum status;                                            \
        status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); \
        switch(status) {                                          \
          case GL_FRAMEBUFFER_COMPLETE_EXT:                       \
            break;                                                \
          case GL_FRAMEBUFFER_UNSUPPORTED_EXT:                    \
            /* choose different formats */                        \
            break;                                                \
          default:                                                \
            /* programming error; will fail on all hardware */    \
            assert(0);                                            \
        }\
      }

CGLRenderTexture::CGLRenderTexture(){
    mGLType = GL_TEXTURE_2D;
}

CGLRenderTexture::CGLRenderTexture(const string &n){
    mGLType = GL_TEXTURE_2D;
}

void CGLRenderTexture::create(U32 width, U32 height, 
                            bool mipmaps, bool anisotropic,
                            U32 Rbits, U32 Gbits, U32 Bbits, U32 Abits,
                            U32 Dbits, U32 Sbits)
{
    if(!GLEW_EXT_framebuffer_object)
        return;

    mWidth = width;
    mHeight = height;

    mRedBits = Rbits;
    mGreenBits = Gbits;
    mBlueBits = Bbits;
    mAlphaBits = Abits;
    mDepthBits = Dbits;
    mStencilBits = Sbits;

    mHasMipmaps = mipmaps && GLEW_SGIS_generate_mipmap;
    mHasAnisotropic = anisotropic && GLEW_EXT_texture_filter_anisotropic;

	if(!(isPowerOfTwo(mWidth) && isPowerOfTwo(mHeight))){ 
		mGLType = GL_TEXTURE_RECTANGLE_EXT;
	}

    glGenTextures(1, &texNum); checkGLError();
    activate(); checkGLError();

    glTexParameteri(mGLType, GL_TEXTURE_WRAP_S, GL_CLAMP); checkGLError();
    glTexParameteri(mGLType, GL_TEXTURE_WRAP_T, GL_CLAMP);  checkGLError();

	if(mGLType != GL_TEXTURE_RECTANGLE_EXT){
		if(mRedBits > 8){
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); checkGLError();
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); checkGLError();
		}

		if (mHasMipmaps){
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); checkGLError();
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE); checkGLError();
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); checkGLError();
		}

		if (mHasAnisotropic){
			F32 maxAnisotropy;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy); checkGLError();
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy); checkGLError();
		}
	}

    S32 size;
    GLuint type;
    if(mRedBits > 8){
        size = 8;
        type = GL_RGB16F_ARB;
    } else {
        size = 4;
        type = GL_RGBA;
    }
    char *data = (char *)malloc(width * height * size);
    memset(data, 0, width * height * size);
    glCopyTexImage2D(mGLType, 0, type, 0, 0, width, height, 0);checkGLError();

	setMemoryUsage(width * height * size);
    free(data);

    glGenFramebuffersEXT(1, &mFBO);checkGLError();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO);checkGLError();

    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
                                  GL_COLOR_ATTACHMENT0_EXT,
                                  mGLType, texNum, 0);checkGLError();

    mDepthRenderBuffer = 0;
    if(mDepthBits > 0 && mStencilBits == 0){
        glGenRenderbuffersEXT(1, &mDepthRenderBuffer);checkGLError();
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, mDepthRenderBuffer);checkGLError();
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, mWidth, mHeight);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
                                  GL_DEPTH_ATTACHMENT_EXT,
                                  GL_RENDERBUFFER_EXT, mDepthRenderBuffer);checkGLError();
    } else {
        if(mStencilBits > 0){
            glGenRenderbuffersEXT(1, &mStencilRenderBuffer);checkGLError();
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, mStencilRenderBuffer);checkGLError();
            glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH24_STENCIL8_EXT, mWidth, mHeight);checkGLError();
            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_STENCIL_EXT, GL_RENDERBUFFER_EXT, mStencilRenderBuffer);checkGLError();
        } else {
            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);checkGLError();
        }
    }

#if 0
    if(mStencilBits > 0){
        glGenRenderbuffersEXT(1, &mStencilRenderBuffer);checkGLError();
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, mStencilRenderBuffer);checkGLError();
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_STENCIL_INDEX, mWidth, mHeight);checkGLError();
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, mStencilRenderBuffer);checkGLError();
    }
#endif

    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); 
        switch(status) {                                          
          case GL_FRAMEBUFFER_COMPLETE_EXT:                       
            break;                                                
          case GL_FRAMEBUFFER_UNSUPPORTED_EXT:                    
            /* choose different formats */                        
              gcon.printf("Framebuffer not complete\n");
            break;                                                
          default:                                                
            /* programming error; will fail on all hardware */    
            assert(0);                                            
        }

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);checkGLError();

    mCreated = true;
    glBindTexture(mGLType, 0); checkGLError();

    // Reinitialize the usage because activate() here marks as
    // used
    mLastFrameUsed = -10;
}

void CGLRenderTexture::beginCapture(){
    if(!mCreated)
        return;

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO);checkGLError();
}

void CGLRenderTexture::endCapture(){
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);checkGLError();
}

void CGLRenderTexture::setDimentions(const pair<S32,S32> &d){
	
}

void CGLRenderTexture::setDimentions(const S32 width, const S32 heigth){
	
}

pair<S32,S32> CGLRenderTexture::getDimentions(){
	return pair<S32,S32>(mWidth, mHeight);
}

void CGLRenderTexture::enable(){
	glEnable(mGLType);
}

void CGLRenderTexture::disable(){
	glDisable(mGLType);
}

void  CGLRenderTexture::activate(F32 time){
    glBindTexture(mGLType, texNum);checkGLError();
}

void CGLRenderTexture::deactivate(){

}

void CGLRenderTexture::makeTexture(CImage *image, S32 maxDimention, S32 bias, S32 flags){
	static S32 filter = gCfgFile.getAsInt("r_filter_trilinear")?GL_LINEAR_MIPMAP_LINEAR:GL_LINEAR_MIPMAP_NEAREST;
	S32 w = image->getWidth();
	S32 h = image->getHeight();

	if(w > maxDimention) w = maxDimention;
	if(h > maxDimention) h = maxDimention;

	if(!(flags & SHADER_NOMIPMAPS)){
		w = w >> bias;
		h = h >> bias;
	}
	
	image->scale(w, h);

    mWidth = w;
    mHeight = h;

	if(!(isPowerOfTwo(mWidth) && isPowerOfTwo(mHeight))){ 
		mGLType = GL_TEXTURE_RECTANGLE_ARB;
	}
	
	glGenTextures(1, &texNum); checkGLError();
	activate();
	
	if(mGLType != GL_TEXTURE_RECTANGLE_ARB){
		glTexParameteri(mGLType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if(flags & SHADER_NOMIPMAPS || 1)
			glTexParameteri(mGLType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		else
			glTexParameteri(mGLType, GL_TEXTURE_MIN_FILTER, filter);


		if (flags & SHADER_CLAMP || 1){
			GLenum clamp = GLEW_EXT_texture_edge_clamp ? GL_CLAMP_TO_EDGE_EXT : GL_CLAMP;
			glTexParameteri(mGLType, GL_TEXTURE_WRAP_S, clamp);
			glTexParameteri(mGLType, GL_TEXTURE_WRAP_T, clamp);
		} else {
			glTexParameteri(mGLType, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(mGLType, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
	} else {
		glTexParameteri(mGLType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(mGLType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(mGLType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(mGLType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	S32 numBytes;
	GLenum format;
	if(image->getColorFormat() == CImage::RGBA){ numBytes = 4;format = GL_RGBA;}
    else if(image->getColorFormat() == CImage::NORMAL){ numBytes = 4;format = GL_RGBA;}
	else if(image->getColorFormat() == CImage::RGB){ numBytes = 3;format = GL_RGB;}
	else if(image->getColorFormat() == CImage::GRAYSCALE){ numBytes = 1;format = GL_LUMINANCE;}

	if (flags & SHADER_NOMIPMAPS ){
		glTexImage2D(mGLType, 0, numBytes, w, h, 0, 
            format, GL_UNSIGNED_BYTE, image->getImageData()); checkGLError();
		setMemoryUsage(w * h * numBytes);
	} else {
		myBuild2DMipmaps(GL_TEXTURE_2D, numBytes, w, h, format, GL_UNSIGNED_BYTE, image->getImageData()); checkGLError();
		setMemoryUsage(w * h * numBytes); // FIXME : Use the series...
	}	
}
