/***************************************************************************
                          client.cpp  -  description
                             -------------------
    begin                : Tue Mar 12 2002
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

#include <vector.h>
#include <quaternion.h>
#include <matrix.h>
#include <client.h>
#include <engine.h>
#include <renderer.h>
#include <meshloader3ds.h>
#include <renderable.h>
#include <renderablemultimesh.h>
#include <interpolator.h>
#include <inputdevice.h>

#include <scenemanager.h>
#include <trace.h>
#include <cmapscene/cmapscene.h>

#include <string>

#include <scenemodel.h>

extern std::string dir, file;

CClient::CClient() : self (0) {
}
CClient::~CClient(){
}

#include <sound/soundlistener.h>
#include <sound/soundsource.h>
#include <sound/soundbuffer.h>
#include <sound/soundogg.h>

CRenderableMultiMesh *mm;
CSceneModel *m;

CSoundListener *listener;
CSoundSource *so;
CSoundOgg *wave;
void CClient::startup_c(){
    CSceneManager *sm = new CCMapScene();
    setSceneManager(sm);
}

void CClient::startup(){
	if(self){
		PyObject *rslt = PyObject_CallMethod(self, "startup", 0);
		if(rslt){
			Py_DECREF(rslt);
		} else {
			PyErr_Print();
			throw CException("Python exception");
		}
	} else startup_c();
}

void CClient::updateActors(F64 timedelta){
	if(self){
		PyObject *rslt = PyObject_CallMethod(self, "updateActors", "d", timedelta);
		if(rslt){
			Py_DECREF(rslt);
		} else {
			PyErr_Print();
            throw CException("Python exception in client::updateActors()");
		}
	} else updateActors_c(timedelta);
}

void CClient::updateActors_c(F64 timedelta){

}

void CClient::shutdown(){
	//if(self)PyEval_CallMethod(self, "shutdown", 0);
}

void CClient::update(){
	if(self){
		PyObject *rslt = PyObject_CallMethod(self, "update", 0);
		if(rslt){
			Py_DECREF(rslt);
		} else {
			PyErr_Print();
			throw CException("Python exception");
		}
	}
	else update_c();
}

extern bool wireframe;
extern S32 frameCount;
extern F64 totalEngineTime;
void CClient::update_c(){

}

void CClient::setSceneManager(CSceneManager *sm){
    mSceneManager = sm;
}

CSceneManager *CClient::getSceneManager(){
    return mSceneManager;
}
