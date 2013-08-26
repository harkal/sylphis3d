 /***************************************************************************
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

