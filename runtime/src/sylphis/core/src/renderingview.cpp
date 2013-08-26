/***************************************************************************
                          renderingview.cpp  -  description
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

#include <engine.h>
#include <renderingview.h>
#include <configfile.h>


void CRenderingTarget::beginCapture(){

}

void CRenderingTarget::endCapture(){

}


void CRenderingTarget::swapBuffers(){

}

void CRenderingTarget::setDimentions(const pair<S32,S32> &d){
	
}

void CRenderingTarget::setDimentions(const S32 width, const S32 heigth){
	
}

pair<S32,S32> CRenderingTarget::getDimentions() const {
	return pair<S32,S32>(0,0);
}


CRenderingView::CRenderingView(){
}
CRenderingView::~CRenderingView(){
}

void CRenderingView::startup(){
    eng = CEngine::getSingleton();
	
	S32 mode = gCfgFile.getAsInt("r_videomode");
	switch(mode){
	case 1:setDimentions(make_pair(640, 480));break;
	case 2:setDimentions(make_pair(800, 600));break;
	case 3:setDimentions(make_pair(1024, 768));break;
    case 4:setDimentions(make_pair(1152, 864));break;
	case 5:setDimentions(make_pair(1280, 1024));break;
	case 12:setDimentions(make_pair(140, 100));break;
	case 123:setDimentions(make_pair(320, 240));break;
	default:
		setDimentions(make_pair(640, 480));break;
	}
}

void CRenderingView::shutdown(){

}

void CRenderingView::setDimentions(const pair<S32,S32> &d){
	dimentions = d;
}

void CRenderingView::setDimentions(const S32 width, const S32 heigth){
	setDimentions(pair<S32,S32>(width, heigth));
}

pair<S32,S32> CRenderingView::getDimentions() const {
	return dimentions;
}

void CRenderingView::handleEvents(){

}

