/***************************************************************************
                          inputdevice.cpp  -  description
                             -------------------
    begin                : Fri Sep 6 2002
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

#include <inputdevice.h>
#include <exception.h>

CInputDevice::CInputDevice(){
}

CInputDevice::~CInputDevice(){
}

void CInputDevice::setInputHandler(CInput *i){
	mHandler = i;	
}

CInput *CInputDevice::getInputHandler(){
	return mHandler;
}

void CInputDevice::getPointerXY(S32 &x, S32 &y){
	x = getPointerX();
	y = getPointerY();
}

void CInputDevice::centerPointer(){
	S32 x, y;

	getPointerCenter(x, y);
	setPointer(x, y);
}

void CPyInputListener::onKeyPressed(S32 i){
	if(self){
		PyObject *rslt = PyObject_CallMethod(self, "onKeyPressed", "i", i);
		if(rslt){
			Py_DECREF(rslt);
		} else {
			PyErr_Print();
			throw CException("Python exception");
		}
	}
}

void CPyInputListener::onKeyReleased(S32 i){
	if(self){
		PyObject *rslt = PyObject_CallMethod(self, "onKeyReleased", "i", i);
		if(rslt){
			Py_DECREF(rslt);
		} else {
			PyErr_Print();
			throw CException("Python exception");
		}
	}
}

CInput::CInput(){
	for(S32 i = 0 ; i < 256 ; i++){
		mKeys[i] = false;
	}
	lastX = lastY = -1;
	mGrabPointer = false;
}

CInput::~CInput(){
}

bool CInput::getGrabPointer(){
	return mGrabPointer;
}

void CInput::setGrabPointer(bool g){
	mGrabPointer = g;
}

void CInput::addKeyDevice(CInputDevice *d){
	d->setInputHandler(this);
	mDevices.push_back(d);
}

void CInput::addPointingDevice(CInputDevice *d){
	mPointingDevice = d;
	d->centerPointer();
}

void CInput::clearDevices(){
	mDevices.clear();
	mPointingDevice = 0;
}

void CInput::addListener(CInputListener *l){
	mListeners.push_back(l);
}

S32 CInput::getPointerX(){
	return mPointingDevice->getPointerX();
}

S32 CInput::getPointerY(){
	return mPointingDevice->getPointerX();
}

void CInput::getPointerXY(S32 &x, S32 &y){
	mPointingDevice->getPointerXY(x, y);
}

void CInput::setPointer(S32 x, S32 y){
	mPointingDevice->setPointer(x, y);
}
#include <stdio.h>
void CInput::getPointerDelta(S32 *x, S32 *y){
	S32 centerX, centerY;
	S32 curX, curY;

	if(mGrabPointer){
		mPointingDevice->getPointerCenter(centerX, centerY);
		mPointingDevice->getPointerXY(curX, curY);
		mPointingDevice->centerPointer();
	} else {
		centerX = lastX;
		centerY = lastY;
		mPointingDevice->getPointerXY(curX, curY);
		lastX = curX;
		lastY = curY;
	}

	*x = (curX - centerX) / 2.0;
	*y = (curY - centerY) / 2.0;
}

S32 CInput::getPointerDeltaX(){
	S32 centerX, centerY;
	S32 curX, curY;

	getPointerDelta(&deltaX, &deltaY);
	return deltaX;
}

S32 CInput::getPointerDeltaY(){
	return deltaY;
}
