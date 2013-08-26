/***************************************************************************
                       rendererARB2.cpp  -  description
                             -------------------
    begin                : Tue Jan 24 2004
    copyright            : (C) 2004 by Harry Kalogirou
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

#ifdef _LINUX
#define GL_GLEXT_PROTOTYPES
#endif

#include <vector.h>
#include <matrix.h>
#include <engine.h>
#include <rendersystem.h>
#include <rendererARB2.h>
#include <openglext.h>
#include "myglu.h"
#include <exception.h>

bool CRendererARB2::isSupported(){
	return true;
}

static char *driverName = "OpenGL ARB [High quality]";
const char *CRendererARB2::getName(){
    return driverName;
}


void CRendererARB2::startup(){
    gcon.printString("-- CRendererARB2::startup()\n");
	CRenderer::startup();


	if(GLEW_ARB_vertex_program){
        try {
            //
        } catch (CException ex){
            gcon.printError("\n" + ex.getExplanation() + "\n");
        }
    }

#if 0
	try {
        gcon.printf("Loading fragment programs... ");
        mBumpFP.makeProgram(bumpFP);
        mBumpFilterFP.makeProgram(bumpFilterFP);
		gcon.printf("Done.\n");
	} catch (CException ex){
        gcon.printError("\n" + ex.getExplanation() + "\n");
    }
#endif 
	if(GLEW_EXT_texture3D){
		attenuationMap3D = registerAttenuation("");
	} else {
		//canBumpMap = false;
	}
}


CTexture *CRendererARB2::registerAttenuation(const std::string &name){
	StringTextureMap::iterator i;

	i = mAttenuationTextures.find(name);
	if(i != mAttenuationTextures.end())
		return static_cast<CTexture *>(&(*i->second));

    CTexture3D* attenuationMap3D = CEngine::getSingleton()->mRSystem->createTexture3D();
    attenuationMap3D->setName("3DAtt_" + name);
	attenuationMap3D->makeAttenTexture(name, 32);
	eng->textures.registerTexture(attenuationMap3D);

	mAttenuationTextures[name] = attenuationMap3D;

	return static_cast<CTexture *>(attenuationMap3D);
}

void CRendererARB2::debugDrawLine(const CVector3 &start, const CVector3 &end, CColor *c){
    glBegin(GL_LINES);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glVertex3fv(&start.x);
    glVertex3fv(&end.x);
    glEnd();
}

