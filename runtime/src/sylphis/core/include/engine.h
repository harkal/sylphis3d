 /***************************************************************************
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
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <singleton.h>
#include <timer.h>
#include <vfs.h>
#include <console.h>

#include <renderable.h>
#include <renderingview.h>
#include <material.h>
#include <texture.h>
#include <modelmanager.h>
#include <font.h>

#include <client.h>

#include <Python.h>

class CRenderSystem;
class CRenderer;
class CInput;
class CSoundEnv;

class CEngine : public CSingleton<CEngine> {
public:
	CVirtualFS*     vfs;
	CConsoleMulti   con;

	CEngine();
	~CEngine();

    // Useing the original method from CSingleton fails in "Release" build
    // This is because of a bug in the MSVC 7.0 compiler.
    static CEngine* getSingleton(){
		static CEngine singleton;
		return &singleton;
	}

	virtual void setClient(CClient *c);	

	virtual void startup();
	virtual void shutdown();

    bool    needUpdate();
	
	void    setRunning(bool r);
	bool    isRunning();
	void    update();

	F64     getFrameLength();
	F64     getFrameTime();
    F64     getWorldTime();
    void    setWorldTime(F64 time);
    S32     getFrameCount();

	F32     getFPS();
    void    setFPS(F64 fps);
	
	string  getVersion();

	CFont*  getDefaultFont();

	CInput*     getInput();
	CClient*    getClient();

	void        setPyModule(PyObject *o);
	PyObject*   getPyModule();

	void createRenderingView();
	void destroyRenderingView();

	void saveGame(const char *filename);
	void loadGame(const char *filename);

protected:
	void startupPython();
    bool mNeedUpdate;
public:
	CRenderingViewPtr   rView;
    CRenderSystem*      mRSystem;
	CRenderer*          renderer;
	CSoundEnv*          mSoundEnv;
	CTimer              timer;
    CMaterialManager    mMaterials;
	CTextures           textures;
	CRenderableManager  renderables;
    CModelManager       models;
	CFonts              fonts;
protected:
    F64 mWorldTime;
	F64 frameLength;
	F64 frameTime;
	F64 lastFrameTime;
	S32 mFrameCount;
#define FPS_FRAMES 8

	F32 mLastFrameLengths[FPS_FRAMES];
	F32 mFPS;
    F32 mConstantFPS;
	bool running;

	CClient	*client;
	CInput *input;

	PyObject *mPyModule;
	PyObject *self;

	CFontPtr mDefaultFont;
};

#endif

