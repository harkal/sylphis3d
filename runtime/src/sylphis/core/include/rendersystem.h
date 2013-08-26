/***************************************************************************
                       renderersystem.h  -  description
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

#ifndef _RENDERERSYSTEM_H_
#define _RENDERERSYSTEM_H_

class CRenderingView;
class CTexture1D;
class CTexture2D;
class CTexture3D;
class CTextureCube;
class CRenderTexture;

class CRenderSystem {
public:
    CRenderSystem();
    virtual ~CRenderSystem();

    virtual void startup() = 0;
    virtual void shutdown() = 0;
    virtual CRenderingView* createRenderingView() = 0;
    virtual CTexture1D*     createTexture1D() = 0;
    virtual CTexture2D*     createTexture2D() = 0;
    virtual CTexture3D*     createTexture3D() = 0;
    virtual CTextureCube*   createTextureCube() = 0;
    virtual CRenderTexture* createRenderTexture() = 0;

    virtual void pushWorldMatrix() = 0;
    virtual void popWorldMatrix() = 0;
    virtual void resetWorldMatrix() = 0;

    virtual void pushProjMatrix() = 0;
    virtual void popProjMatrix() = 0;
    virtual void resetProjMatrix() = 0;

	
};

extern CRenderSystem*   gRSystem;

#endif
