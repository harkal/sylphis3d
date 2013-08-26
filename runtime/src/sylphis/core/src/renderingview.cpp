/***************************************************************************
                          renderingview.cpp  -  description
                             -------------------
    begin                : Sun Mar 10 2002
    copyright            : (C) 2002 by Harry Kalogirou
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

