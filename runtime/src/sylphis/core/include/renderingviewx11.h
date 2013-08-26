/***************************************************************************
                          renderingviewx11.h  -  description
                             -------------------
    begin                : Sun Mar 10 2002
    copyright            : (C) 2002 by Harry Kalogirou
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

#ifndef RENDERINGVIEWX11_H
#define RENDERINGVIEWX11_H

#ifndef _WIN32

#include <GL/glx.h>
#include "renderingview.h"
#include <inputdevice.h>

class CEngine;

class CRenderingViewX11 : public CRenderingView {
public: 
	CRenderingViewX11();
	virtual ~CRenderingViewX11();
	
	virtual void startup();
	virtual void shutdown();
	
	virtual void swapBuffers();
	
	virtual void handleEvents();

	virtual S32 getPointerX();
	virtual S32 getPointerY();
	virtual void getPointerXY(S32 &x, S32 &y);
	virtual void setPointer(S32 x, S32 y);
	virtual void getPointerCenter(S32 &x, S32 &y);
	virtual void centerPointer();    
protected:
    S32 XKeyCode(XKeyEvent *ev);
    
	Display *dpy;
	Window win;
    S32 mPointerX;
    S32 mPointerY;
};

#endif

#endif
