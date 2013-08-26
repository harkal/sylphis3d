/***************************************************************************
                          renderablequeuemember.h  -  description
                             -------------------
    begin                : Thu Jun 6 2002
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

#ifndef RENDERABLEQUEUEMEMBER_H
#define RENDERABLEQUEUEMEMBER_H

#include "matrix.h"
#include <boundbox.h>
#include "quaternion.h"
#include <renderable.h>
#include "pointer.h"



SmartPointer(CRenderableQueueMember);
class CRenderableQueueMember : public CRefObject {
public: 
	CRenderableQueueMember();
	virtual ~CRenderableQueueMember();

	void setOrientation(const CQuaternion &q);
	CQuaternion &getOrientation();

	void setPosition(const CVector3 &p);
	CVector3 &getPosition();

    void setBounds(const CAABoundBox &bbox);
    CAABoundBox &getBounds();

	CMatrix4 getTransformMatrix() const;
	CMatrix4 getInverseTransformMatrix() const;

    void setLastTransform(const CQuaternion &q, const CVector3 &v);
    CMatrix4 getLastTransformMatrix() const;

    void setRenderable(CRenderable *r);
    CRenderable *getRenderable();

    void setNoShadows(bool b);
    bool getNoShadows() const;
private:
	CQuaternion mOrientation;
	CVector3 mPosition;
    CAABoundBox mBound;
    CRenderablePtr mRenderable;
    CQuaternion mLastOrientation;
	CVector3 mLastPosition;
    bool mNoShadows;
};

#endif
