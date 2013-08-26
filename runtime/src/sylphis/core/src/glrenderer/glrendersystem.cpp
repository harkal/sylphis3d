/***************************************************************************
                      glrenderersystem.cpp  -  description
                             -------------------
    begin                : Mon Apr 4 2006
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

#include <glrenderer/glrendersystem.h>
#include <glrenderer/gltexture.h>
#include <glrenderer/glrendertexture.h>

#include <renderingview.h>

#ifdef _WIN32
#include <windows.h> 
#include <conio.h>
#include <renderingviewwin32.h>
#include <direct.h>
#else
#include <renderingviewx11.h>
#endif

#include <console.h>
#include <openglext.h>
#include <gutil.h>

CGLRenderSystem::CGLRenderSystem(){

}

CGLRenderSystem::~CGLRenderSystem(){

}

void CGLRenderSystem::startup(){
    gcon.printf("... Checking OpenGL implementation\n");
    gcon.printf("    VENDOR   : %s\n", glGetString(GL_VENDOR));
    gcon.printf("    RENDERER : %s\n", glGetString(GL_RENDERER));
    gcon.printf("    VERSION  : %s\n", glGetString(GL_VERSION));
    initOpenGlExtensions();
}

void CGLRenderSystem::shutdown(){

}

CRenderingView* CGLRenderSystem::createRenderingView(){
#ifndef _WIN32
	return new CRenderingViewX11();
#endif
#ifdef _WIN32
	return new CRenderingViewWin32();
#endif
}

CTexture1D* CGLRenderSystem::createTexture1D(){
    return new CGLTexture1D();
}

CTexture2D* CGLRenderSystem::createTexture2D(){
    return new CGLTexture2D();
}

CTexture3D* CGLRenderSystem::createTexture3D(){
    return new CGLTexture3D();
}

CTextureCube* CGLRenderSystem::createTextureCube(){
    return new CGLTextureCube();
}

CRenderTexture* CGLRenderSystem::createRenderTexture(){
    return new CGLRenderTexture();
}

void CGLRenderSystem::pushWorldMatrix(){
    glMatrixMode(GL_MODELVIEW); checkGLError();
    myglPushMatrix(); checkGLError();
}

void CGLRenderSystem::popWorldMatrix(){
    glMatrixMode(GL_MODELVIEW); checkGLError();
    myglPopMatrix(); checkGLError();
}

void CGLRenderSystem::resetWorldMatrix(){
    glMatrixMode(GL_MODELVIEW); checkGLError();
    glLoadIdentity();
}

void CGLRenderSystem::pushProjMatrix(){
    glMatrixMode(GL_PROJECTION); checkGLError();
    myglPushMatrix(); checkGLError();
}

void CGLRenderSystem::popProjMatrix(){
    glMatrixMode(GL_PROJECTION); checkGLError();
    myglPopMatrix(); checkGLError();
}

void CGLRenderSystem::resetProjMatrix(){
    glMatrixMode(GL_PROJECTION); checkGLError();
    glLoadIdentity();
}

