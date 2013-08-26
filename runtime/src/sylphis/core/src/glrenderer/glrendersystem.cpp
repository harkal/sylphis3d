/***************************************************************************
                      glrenderersystem.cpp  -  description
                             -------------------
    begin                : Mon Apr 4 2006
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

