/***************************************************************************
                          engine.cpp  -  description
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
#include "engine.h"
#include "build.h"
#include "timer.h"
#include "renderingviewx11.h"

#ifdef _WIN32
//#define WIN32_LEAN_AND_MEAN
#include <windows.h> 
#include <conio.h>
#include "renderingviewwin32.h"
#include <direct.h>
#endif

#ifdef _LINUX
#include <unistd.h>
#endif

#include <inputdevice.h>

#include <sound/soundenv.h>

#include <rendererARB2.h>
#include <scenemanager.h>
#include <serializer.h>

#include <rendersystem.h>
#include <glrenderer/glrendersystem.h>

#include <configfile.h>

#include <exception.h>

#undef FPS_FRAMES
#define FPS_FRAMES 4

CEngine::CEngine(){
    gcon.printf("\n%s\n", getVersion().c_str());

	lastFrameTime = 0.0f;
	frameTime = 0.0f;
    setWorldTime(0.0);
	mFrameCount = 0;
    setFPS(0.0);
	setRunning(false);
	
	vfs = &gfs;
	self = 0;
	rView = 0;

	con.addConsole(&gcon);//gcon.addConsole(&con);

    input = new CInput();
}

CEngine::~CEngine(){

}

string CEngine::getVersion(){
	string s;
	s = "Sylphis 0.95 ";
#ifdef _WIN32
	s += "(Win32";
	s = s + " " + __DATE__ + " - " + __TIME__ + ")";
#endif
#ifdef _LINUX
	s += "(Linux-i386";
	s = s + " Build " + BUILD_NUMBER + " " + BUILD_DATE + ")";
#endif
	return s;
}

void CEngine::setClient(CClient *c){
	client = c;
}	

#include "python/initmodules.h"

void CEngine::startupPython(){
}

void CEngine::createRenderingView(){
	if(!rView){
        rView = mRSystem->createRenderingView();
        input->addKeyDevice(rView);
        input->addPointingDevice(rView);
	}
	rView->startup();
}

void CEngine::destroyRenderingView(){
	rView->shutdown();
	//delete rView;
	//rView = 0;
}

void CEngine::startup(){
    
	if(!client){
		con.printf("FATAL : No client to run\n");
		exit(1);
	}

	if(client){
        mRSystem = new CGLRenderSystem();
        createRenderingView();
        mRSystem->startup();
        gRSystem = mRSystem;

        con.printf("... Selecting rendering driver backend ... ");

#ifdef _WIN32		
#endif
		if(gCfgFile.getAsInt("r_useARB2")){
	        renderer = new CRendererARB2();
		    if(renderer->isSupported())
			    goto rendererok;
			delete renderer;
		}

		rView->shutdown();
        throw CException("Failed to find compatible video hardware.\n\n"
						 "Your video card might not be supported.");
        
rendererok:
        con.printf("\n    Using '%s'.\n", renderer->getName());
		renderer->setRenderingView(*rView);
        renderer->startup();
	}

	mDefaultFont = (CFont *)fonts.registerBitmapFont("fonts/times.ttf");

	mSoundEnv = new CSoundEnv();
	mSoundEnv->startup();

	startupPython();
	

	if(client)client->startup();

	//shaders.loadAll();
	
	setRunning(true);
    mNeedUpdate = true;
}

extern S32 frameCount;
extern F64 totalEngineTime;
void CEngine::shutdown(){
	con.printf("CEngine::shutdown()\n");
	if(client)
 		client->shutdown();
	
	input = 0;

	if(client){
		renderer->shutdown();
		delete renderer;
		destroyRenderingView();
        delete mRSystem;
	}

    mNeedUpdate = false;

	con.printf("  Instances %d\n", CRefObject::getTotalRefObjects());
    //gcon.printf("Average FPS : %5.2f\n", (F32)frameCount / totalEngineTime);

	// Revoved since if deletes pointers that doen't own
	//con.freeConsoles();
}

bool CEngine::needUpdate(){
    return mNeedUpdate;
}

F64 CEngine::getFrameLength(){
	return frameLength;
}

F64 CEngine::getFrameTime(){
	//return lastFrameTime;
    return totalEngineTime;
}

void CEngine::setRunning(bool r){
	running = r;
}

bool CEngine::isRunning(){
	return running;
}

F64 CEngine::getWorldTime(){
    return mWorldTime;
}

void CEngine::setWorldTime(F64 time){
    mWorldTime = time;
}

S32 CEngine::getFrameCount(){
    return mFrameCount;
}

void CEngine::setFPS(F64 fps){
    if(fps < 0.0)
        return;
    mConstantFPS = fps;
}

F64 lastTime = 0.0;
S32 lastFrame = 0;
S32 frameCount = 0;
F64 totalEngineTime = 0.0;
void CEngine::update(){
	mFrameCount++;
	if(lastFrameTime > 0.0f){
		frameTime = timer.getTime();
		frameLength = frameTime - lastFrameTime;
		lastFrameTime = frameTime;
	} else {
		frameTime = 0.0f;
		lastFrameTime = timer.getTime();
		frameLength = 0.0f;
	}
    if(mConstantFPS > 0.0)
        frameLength = 1 / mConstantFPS;
	totalEngineTime += frameLength;
	mLastFrameLengths[mFrameCount % FPS_FRAMES] = frameLength;

	if(client){
		rView->handleEvents();
		client->update();
	}
	//con.printf("  Instances %d\n", CRefObject::getTotalRefObjects());
	frameCount++;
}

CFont *CEngine::getDefaultFont(){
	return mDefaultFont;
}

CInput *CEngine::getInput(){
	return input;
}

CClient *CEngine::getClient(){
	return client;
}

F32 CEngine::getFPS(){
	F32 t = 0.0;

	for(S32 i=0;i<FPS_FRAMES;i++)
		t += mLastFrameLengths[i];

	if(t == 0.0)
		t = 0.01;

	mFPS = FPS_FRAMES / t;

	return mFPS;
}

void CEngine::setPyModule(PyObject *o){
	mPyModule = o;
}

PyObject *CEngine::getPyModule(){
	return mPyModule;
}

void CEngine::saveGame(const char *filename){
	CSerializer serializer;

	CFile *saveFile = vfs->createFile(filename);

	serializer.setFile(saveFile);
	serializer.writeSerializable(client->getSceneManager()->getRootSceneNode());

	delete saveFile;
}

void CEngine::loadGame(const char *filename){
	CSerializer serializer;

	CFile *loadFile = vfs->openFile(filename);
	serializer.setFile(loadFile);

	CSceneObjectPtr root = dynamic_cast<CSceneObject *>(serializer.readSerializable());

	delete loadFile;
}

