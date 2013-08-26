/***************************************************************************
                          camera.h  -  description
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

#ifndef CAMERA_H
#define CAMERA_H

#include <utility>
using namespace std;
#include "vector.h"
#include "matrix.h"
#include "scenenode.h"
#include "frustum.h"

class CSceneManager;

SmartPointer(CCamera);
class CCamera : public CSceneNode {
DEFINE_SERIALIZABLE(CCamera);
public:
	CCamera();
	CCamera(CSceneManager *sm);
	virtual ~CCamera();
	
	void setFov(F32 f);
	F32 getFov();

    void updateFrustum(CMatrix4 &clip);
    CFrustum &getFrustum();

	void yawFixed(F32 rad);
	void pitchFixed(F32 rad);

    void setActive(bool active);
    bool isActive();

    virtual void render(CRenderer *renderer);

    virtual void save(CSerializer *ser) const;
    virtual void load(CSerializer *ser);

//protected:
	F32 fov;
protected:
    CFrustum mFrustum;
    bool mActive;
};

#endif
