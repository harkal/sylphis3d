/***************************************************************************
                           plane.h  -  description
                             -------------------
    begin                : Jun 13 2002
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

#ifndef _PLANE_H_
#define _PLANE_H_

#include "vector.h"
#include <math.h>

class CSerializer;

class CPlane {
public:
		inline CPlane() : normal(0.0, 0.0, 0.0), dist(0), onAxe(-1) {};
		inline CPlane(const F32 nA, const F32 nB, const F32 nC, const F32 nD) : normal(nA, nB, nC), dist(nD), onAxe(-1) {};
        inline CPlane(const CPlane &p) : normal(p.normal), dist(p.dist), onAxe(p.onAxe) {};
		inline CPlane(const CVector3 &v, const F32 nD) : normal(v), dist(nD), onAxe(-1) {};
		CPlane(const CVector3& a, const CVector3& b, const CVector3& c) {
			normal = ((c - a) ^ (b - a)).normalized();
			dist = a * normal;
            onAxe = -1;
		};

        enum eSide {
            FRONT,
            BACK,
            CROSS,
            ON
        };

		inline CPlane& operator=(const CPlane& p) {
			normal = p.normal;
			dist = p.dist;
            onAxe = p.onAxe;
            factor = p.factor;
			
			return *this;
		};
			
		inline CPlane& setNormal(CVector3 n) {
			normal = n;
			return *this;
		};

		F32 distance(const CVector3& point) const {
            if (onAxe != -1)
                return factor * point[onAxe] - dist;

			return (normal * point) - dist;
		};

		F32 lineIntersection(const CVector3& origin, const CVector3& direction) const {
			return -((normal * origin) - dist)/(normal * direction);
		};

		void rayIntersection(const CVector3& start, const CVector3& end, CVector3 &r) const {
			r = start - end;
			r *= lineIntersection(start, r);
			r += start;
		};

		bool valid() const {
			F32 l = normal.length();
			return ((l > 0.0f) && ((1.0f/l) > 0.0f));
		};

        void normalize() {
            F32 l = sqrt(normal.x * normal.x +
                      normal.y * normal.y +
                      normal.z * normal.z +
                      dist * dist);
            normal /= l;
            dist /= l;
        };

		void save(CSerializer *ser) const;
		void load(CSerializer *ser);

        F32 dist;
		CVector3 normal;
        S32 onAxe;                  // 0 means on X,  1 means on Y, 2 means on Z, -1 means unspecified      
        S32 factor;                 // if this is  1 means potitive, -1 negative
};

#endif


