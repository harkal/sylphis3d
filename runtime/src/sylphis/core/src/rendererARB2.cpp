/***************************************************************************
                       rendererARB2.cpp  -  description
                             -------------------
    begin                : Tue Jan 24 2004
    copyright            : (C) 2004 by Harry Kalogirou
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

