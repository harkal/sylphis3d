/***************************************************************************
                      splineinterpolator.h  -  description
                             -------------------
    begin                : Thu Apr 15 2004
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

#ifndef SPLINEINTERPOLATOR_H
#define SPLINEINTERPOLATOR_H

#include <vector>
#include <pointer.h>
#include <vector.h>
#include <matrix.h>
#include <interpolator.h>

SmartPointer(CSplineInterpolatorVector3);
class CSplineInterpolatorVector3 : public CInterpolatorVector3 {
public:
	CSplineInterpolatorVector3();
	~CSplineInterpolatorVector3();
	
	virtual void insert(CVector3 &node, F32 nodeKey);
	virtual CVector3 getAt(F32 key);
    virtual CVector3 intepolate(S32 index, F32 t);
protected:
    void calcTangents();
    typedef std::vector<CVector3> PointVector;
    typedef std::vector<CVector3> TangentVector;
    CMatrix4 mCoeffs;
    TangentVector mTangents;
    bool mDirty;
};

#endif
