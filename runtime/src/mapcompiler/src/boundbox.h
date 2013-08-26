/***************************************************************************
                          boundbox.h  -  description
                             -------------------
    begin                : Sun Jun 9 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@gmx.net
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
#ifndef BOUNDBOX_H
#define BOUNDBOX_H

#include "vector.h"

class Plane;

class CAABoundBox {
public: 
	CAABoundBox(const bool init=false);
	CAABoundBox(const CVector3 &v);
	~CAABoundBox();

    void setBounds(const CVector3 &min, const CVector3 &max);
    void setMin(const CVector3 &min);
    const CVector3 &getMin() const;
    void setMax(const CVector3 &max);
    const CVector3 &getMax() const;

	void addPoint(const CVector3 &v);

    float distance(const Plane &p);
    bool isBehind(const Plane &p);

    void translate(const CVector3 &v);

    bool collide(const CAABoundBox &b) const;
    bool collide(const CVector3 &v) const;
    bool isAllIn(const CAABoundBox &b) const;    

    void expand(float e);
protected:
    CVector3 mMax;
    CVector3 mMin;
};

#endif
