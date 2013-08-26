/***************************************************************************
                        gltexture.cpp  -  description
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

#include <glrenderer/gltexture.h>

#include <imagefile.h>
#include <configfile.h>
#include <gutil.h>
#include <myglu.h>
#include <exception.h>

CGLTexture1D::CGLTexture1D(){
    mGLType = GL_TEXTURE_1D;
}

CGLTexture1D::CGLTexture1D(const string &n){
    mGLType = GL_TEXTURE_1D;
}

CGLTexture1D::~CGLTexture1D(){

}

void CGLTexture1D::enable(){
	glEnable(mGLType);
}

void CGLTexture1D::disable(){
	glDisable(mGLType);
}

void  CGLTexture1D::activate(F32 time){
    glBindTexture(mGLType, texNum);
}

void CGLTexture1D::deactivate(){

}

void CGLTexture1D::makeTexture(CImage *image, S32 maxDimention, S32 bias, S32 flags){
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

    mGLType = GL_TEXTURE_1D;

    mWidth = w;
    mHeight = h;
	
	glGenTextures(1, &texNum); checkGLError();
	activate();
	
	glTexParameteri(mGLType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//if(flags & SHADER_NOMIPMAPS )
		glTexParameteri(mGLType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//else
	//	glTexParameteri(mGLType, GL_TEXTURE_MIN_FILTER, filter);

	
	if (flags & SHADER_CLAMP){
		GLenum clamp = GLEW_EXT_texture_edge_clamp ? GL_CLAMP_TO_EDGE_EXT : GL_CLAMP;
		glTexParameteri(mGLType, GL_TEXTURE_WRAP_S, clamp);
	} else {
		glTexParameteri(mGLType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}

	S32 numBytes;
	GLenum format;
	if(image->getColorFormat() == CImage::RGBA){ numBytes = 4;format = GL_RGBA;}
    else if(image->getColorFormat() == CImage::NORMAL){ numBytes = 4;format = GL_RGBA;}
	else if(image->getColorFormat() == CImage::RGB){ numBytes = 3;format = GL_RGB;}
	else if(image->getColorFormat() == CImage::GRAYSCALE){ numBytes = 1;format = GL_LUMINANCE;}

	//if (flags & SHADER_NOMIPMAPS ){
		glTexImage1D(mGLType, 0, numBytes, w, 0, 
            format, GL_UNSIGNED_BYTE, image->getImageData()); checkGLError();
	//} else {
	//	myBuild2DMipmaps(GL_TEXTURE_2D, numBytes, w, h, format, GL_UNSIGNED_BYTE, image.getImageData()); checkGLError();
	//}	

	setMemoryUsage(w * numBytes);
}

CGLTexture2D::CGLTexture2D(){
    mGLType = GL_TEXTURE_2D;
}

CGLTexture2D::CGLTexture2D(const string &n){
    mGLType = GL_TEXTURE_2D;
}

CGLTexture2D::~CGLTexture2D(){

}

void CGLTexture2D::enable(){
	glEnable(mGLType);
}

void CGLTexture2D::disable(){
	glDisable(mGLType);
}

void  CGLTexture2D::activate(F32 time){
    glBindTexture(mGLType, texNum);
}

void CGLTexture2D::deactivate(){

}

void CGLTexture2D::makeTexture(CImage *image, S32 maxDimention, S32 bias, S32 flags){
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
		mGLType = GL_TEXTURE_RECTANGLE_EXT;
	}
	
	glGenTextures(1, &texNum); checkGLError();
	activate();
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if(flags & SHADER_NOMIPMAPS )
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);

	
	if (flags & SHADER_CLAMP){
		GLenum clamp = GLEW_EXT_texture_edge_clamp ? GL_CLAMP_TO_EDGE_EXT : GL_CLAMP;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	S32 numBytes;
	GLenum format;
	if(image->getColorFormat() == CImage::RGBA){ numBytes = 4;format = GL_RGBA;}
    else if(image->getColorFormat() == CImage::NORMAL){ numBytes = 4;format = GL_RGBA;}
	else if(image->getColorFormat() == CImage::RGB){ numBytes = 3;format = GL_RGB;}
	else if(image->getColorFormat() == CImage::GRAYSCALE){ numBytes = 1;format = GL_LUMINANCE;}

	if (flags & SHADER_NOMIPMAPS ){
		glTexImage2D(GL_TEXTURE_2D, 0, numBytes, w, h, 0, 
            format, GL_UNSIGNED_BYTE, image->getImageData()); checkGLError();
		setMemoryUsage(w * h * numBytes);
	} else {
		myBuild2DMipmaps(GL_TEXTURE_2D, numBytes, w, h, format, GL_UNSIGNED_BYTE, image->getImageData()); checkGLError();
		setMemoryUsage(w * h * numBytes); // FIXME : Use the series...
	}	
}

CGLTextureCube::CGLTextureCube(){
    mGLType = GL_TEXTURE_CUBE_MAP_ARB;
}

CGLTextureCube::CGLTextureCube(const string &n){
    mGLType = GL_TEXTURE_CUBE_MAP_ARB;
}

CGLTextureCube::~CGLTextureCube(){

}

void CGLTextureCube::enable(){
	glEnable(mGLType);
}

void CGLTextureCube::disable(){
	glDisable(mGLType);
}

void  CGLTextureCube::activate(F32 time){
    glBindTexture(mGLType, texNum);
}

void CGLTextureCube::deactivate(){

}

void CGLTextureCube::makeTexture(CImage *image, S32 texNo, S32 maxDimention, S32 bias, S32 flags){
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

	if(texNum == 0){
		glGenTextures(1, &texNum); checkGLError();
		activate();
		GLenum clamp = GLEW_EXT_texture_edge_clamp ? GL_CLAMP_TO_EDGE_EXT : GL_CLAMP;
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, clamp);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, clamp);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	} else {
		activate();
	}

	S32 numBytes;
	GLenum format;
	if(image->getColorFormat() == CImage::RGBA){ numBytes = 4;format = GL_RGBA;}
	else if(image->getColorFormat() == CImage::RGB){ numBytes = 3;format = GL_RGB;}
	else if(image->getColorFormat() == CImage::GRAYSCALE){ numBytes = 1;format = GL_LUMINANCE;}

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB+texNo, 0, format
		, w, h, 0, format,
	     GL_UNSIGNED_BYTE, image->getImageData());

	setMemoryUsage(w * h * numBytes * 6);

}

CGLTexture3D::CGLTexture3D(){
    mGLType = GL_TEXTURE_3D;
}

CGLTexture3D::CGLTexture3D(const string &n){
    mGLType = GL_TEXTURE_3D;
}

CGLTexture3D::~CGLTexture3D(){

}

void CGLTexture3D::enable(){
	glEnable(mGLType);
}

void CGLTexture3D::disable(){
	glDisable(mGLType);
}

void  CGLTexture3D::activate(F32 time){
    glBindTexture(mGLType, texNum);
}

void CGLTexture3D::deactivate(){

}

void CGLTexture3D::makeAttenTexture(const std::string &attMap, S32 size){
	bool fromImage = false;
	CImagePtr image;
    CImagePtr image2D;
	string chopedName = CFile::chopExtention(attMap);
	unsigned char *imageData;
    unsigned char *imageData2D;
	S32 halfWidth;
	if(attMap != ""){
		try {
			CFile f(chopedName, gfs);
			image = CImageLoader::loadImage(f);
			image->convertTo(CImage::GRAYSCALE);
			halfWidth = image->getWidth() / 2;
			imageData = static_cast<unsigned char *>(image->getImageData());

            CFile f2(chopedName + "_2D", gfs);
            image2D = CImageLoader::loadImage(f2);
            image2D->convertTo(CImage::GRAYSCALE);
            imageData2D = static_cast<unsigned char *>(image2D->getImageData());
			fromImage = true;
		} catch (CException e) {
			throw CException("Failed to load attenuation map : " + attMap + "\n");
		}
	}
	
#define sqr(x) ((x) * (x))
    unsigned char *pixels = new unsigned char[size * size * size];
    unsigned char *dest;
    U32 i,j,k;
    F32 r, f;

    mWidth = mHeight = mLength = size;
    
    dest = pixels;
    for (k = 0; k < size; k++){
        for (j = 0; j < size; j++){
            for (i = 0; i < size; i++){
                F32 diff = (size - 1) / 2;
                r = sqr(i-diff) + sqr(j-diff) + sqr(k-diff);

				if(fromImage){
#if 0
					S32 dist = (S32)((sqrt(r) / diff) * halfWidth);
					if(dist > halfWidth)
						*dest = imageData[2*halfWidth];
					else
						*dest = imageData[halfWidth + dist];
#else
                    CPixel p;
                    image2D->getPixel(p, k, i);
                    F32 m = (F32)imageData[(S32)((F32)(j) / size * image->getWidth())] / 255.0f;
                    f = (F32)p.a / 255.0f * m;
                    //f /= 255.0;
                    if(f > 1.0)
                        f = 1.0f;
                    *dest = (unsigned char)(f * 255);
#endif
				} else {
					f = 1.0f - r / sqr(diff);
					if (f < 0.0) f = 0.0;
	                *dest = (unsigned char) (f * 255);
				}
                
                //if(k == 0 || j == 0 || i == 0 ||
                //   k == size-1 || j == size-1 || i == size-1)
                //    *dest = 0;

                dest++;
            }
        }
    }

	glGenTextures(1, &texNum); checkGLError();
	activate();

	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLenum clamp = GLEW_EXT_texture_edge_clamp ? GL_CLAMP_TO_EDGE_EXT : GL_CLAMP;
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_S, clamp);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_T, clamp);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_R_EXT, clamp);

    glTexImage3DEXT(GL_TEXTURE_3D_EXT, 0, GL_RGB8, size, size, size, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels); checkGLError();
	
	setMemoryUsage(size * size * size * 3);
	delete [] pixels;
}

CGLTextureRect::CGLTextureRect(){
    mGLType = GL_TEXTURE_RECTANGLE_EXT;
}

CGLTextureRect::CGLTextureRect(const string &n){
    mGLType = GL_TEXTURE_RECTANGLE_EXT;
}

CGLTextureRect::~CGLTextureRect(){

}

void CGLTextureRect::enable(){
	glEnable(mGLType);
}

void CGLTextureRect::disable(){
	glDisable(mGLType);
}

void  CGLTextureRect::activate(F32 time){
    glBindTexture(mGLType, texNum);
}

void CGLTextureRect::deactivate(){

}