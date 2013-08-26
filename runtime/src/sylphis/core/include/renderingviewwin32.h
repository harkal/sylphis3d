/***************************************************************************
                          renderingviewwin32.h  -  description
                             -------------------
    begin                : Thu Mar 21 2002
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

#ifndef RENDERINGVIEWWIN32_H
#define RENDERINGVIEWWIN32_H

#include <renderingview.h>
#include <inputdevice.h>

class CRenderingViewWin32 : public CRenderingView {
public:
	CRenderingViewWin32();
	virtual ~CRenderingViewWin32();

	virtual void startup();
	virtual void shutdown();

	virtual void setDimentions(const std::pair<S32,S32> &d);
	
	virtual void swapBuffers();
	
	virtual void handleEvents();

	virtual S32 getPointerX();
	virtual S32 getPointerY();
	virtual void getPointerXY(S32 &x, S32 &y);
	virtual void setPointer(S32 x, S32 y);
	virtual void getPointerCenter(S32 &x, S32 &y);
	virtual void centerPointer();
protected:
	void changeResolution(S32 width, S32 height, S32 bpp);
	void createWindow();
	void destroyWindow();

	BOOL SetupPixelFormat(HDC hdc);
	
	HWND hwnd; 
	HDC   hdc; 
	HGLRC hrc; 
};


#endif
