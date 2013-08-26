/***************************************************************************
                        rendererARB2.h  -  description
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

#ifndef RENDERER_ABR2_H
#define RENDERER_ABR2_H

#include <renderer.h>

class CRendererARB2 : public CRenderer {
public:
    virtual bool isSupported();
    virtual const char *getName();
	virtual void startup();

	virtual CTexture *registerAttenuation(const std::string &name);

    virtual void debugDrawLine(const CVector3 &start, const CVector3 &end, CColor *c = 0);
protected:
private:
	CTexture* attenuationMap3D;
    CVertexProgram mDiffuseVP;
	CVertexProgram mBumpFilterVP;
	CFragmentProgram mBumpFP;
    CFragmentProgram mBumpFilterFP;
};

#endif

