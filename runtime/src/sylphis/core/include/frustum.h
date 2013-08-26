/***************************************************************************
                          frustum.h  -  description
                             -------------------
    begin                : Fri Aug 30 2002
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

#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <deque>
#include <matrix.h>
#include <plane.h>
#include <bound.h>
#include <boundbox.h>

class CFrustum : public CBound {
DEFINE_SERIALIZABLE(CFrustum);
public: 
	CFrustum();
	virtual ~CFrustum();

    void makeFrustumPlanes(CMatrix4 &m);

	void pushBackPlane(const CPlane &p);
	void popBackPlanes(S32 i);

    bool isCulled(CAABoundBox &b);
	CPlane &getPlane(S32 t);
	virtual EType getType(){ return FRUSTUM; }

	virtual F32 distance(const CPlane &p) const;
    virtual F32 distance(const CVector3 &v) const;

    virtual bool collide(CBound *b);

    virtual void translate(const CVector3 &v);

    virtual void save(CSerializer *ser) const;
    virtual void load(CSerializer *ser);

    enum ePlaneType {
        PTOP = 0,
        PBOTTOM = 1,
        PLEFT = 2,
        PRIGHT = 3,
        PFAR = 4,
		PNEAR = 5
    };
private:
	typedef std::deque<CPlane> tPlaneList;
	tPlaneList mPlanes;
    //CPlane mPlanes[6];
};

#endif
