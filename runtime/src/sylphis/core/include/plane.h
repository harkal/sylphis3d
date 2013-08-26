/***************************************************************************
                           plane.h  -  description
                             -------------------
    begin                : Jun 13 2002
    copyright            : (C) 2002 by Harry Kalogirou
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


