/***************************************************************************
                          inputdevice.cpp  -  description
                             -------------------
    begin                : Fri Sep 6 2002
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
