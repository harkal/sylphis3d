/***************************************************************************
                          renderingview.h  -  description
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

#ifndef RENDERINGVIEW_H
#define RENDERINGVIEW_H

#include <utility>
#include <platform/types.h>
#include <pointer.h>
#include <inputdevice.h>

class CEngine;

class CRenderingTarget {
public:
    virtual void setDimentions(const std::pair<S32,S32> &d);
	virtual void setDimentions(const S32 width, const S32 heigth);
	virtual std::pair<S32,S32> getDimentions() const;

	virtual void beginCapture();
    virtual void endCapture();
	virtual void swapBuffers();
};

SmartPointer(CRenderingView);
class CRenderingView : public CRenderingTarget, public CInputDevice {
public: 
	CRenderingView();
	virtual ~CRenderingView();
	
	virtual void startup();
	virtual void shutdown();

	virtual void setDimentions(const std::pair<S32,S32> &d);
	virtual void setDimentions(const S32 width, const S32 heigth);
	virtual std::pair<S32,S32> getDimentions() const;
		
	virtual void handleEvents();
private:
	CEngine *eng;

	std::pair<S32,S32> dimentions;
};

#endif
