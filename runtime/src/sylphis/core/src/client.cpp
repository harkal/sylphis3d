/***************************************************************************
                          client.cpp  -  description
                             -------------------
    begin                : Tue Mar 12 2002
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
