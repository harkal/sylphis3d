/***************************************************************************
                         vector.h  -  description
                            -------------------
                begin                : Aug 23 2002
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
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "globaldefs.h"
#include <math.h>

class CSerializer;

class CVector3 {
	public:
		inline CVector3() : x(0), y(0), z(0) {};
		inline CVector3(const F32 nx, const F32 ny, const F32 nz) : x(nx), y(ny), z(nz) {};
		inline CVector3(const CVector3 &v) : x(v.x), y(v.y), z(v.z) {};

		inline CVector3 operator+(const CVector3 &v) const{
			return CVector3(x+v.x, y+v.y, z+v.z);
		}

		inline CVector3 operator-(const CVector3 &v) const{
			return CVector3(x-v.x, y-v.y, z-v.z);
		}

		//dot product
		inline F32 operator*(const CVector3 &v) const{
			return x*v.x + y*v.y + z*v.z;
		}

		//cross product
		inline CVector3 operator^(const CVector3 &v) const{
			return CVector3( (y*v.z-z*v.y), (z*v.x-x*v.z), (x*v.y-y*v.x) );
		}

		inline CVector3 operator+( const F32 f ) const{
			return CVector3(x+f, y+f, z+f);
		}

		inline CVector3 operator-( const F32 f ) const{
			return CVector3(x-f, y-f, z-f);
		}

		inline CVector3 operator-() const{
			return CVector3(-x, -y, -z);
		}

		inline CVector3 operator*(const F32 f) const{
			return CVector3(x*f, y*f, z*f);
		}

		inline CVector3 operator/(const F32 f) const{
			F32 inv = 1.0f/f;
			return CVector3(x*inv, y*inv, z*inv);
		}

		inline CVector3& operator=(const CVector3 &v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

   		inline CVector3& operator+=(const CVector3 &v){
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		inline CVector3& operator-=(const CVector3 &v){
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		inline CVector3& operator*=(const F32 f){
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}

		inline CVector3& operator/=(const F32 f){
			F32 inv = 1.0f / f;
			x *= inv;
			y *= inv;
			z *= inv;
			return *this;
		}

		inline bool operator==(const CVector3 &v) const {
			return !(x != v.x || y != v.y || z != v.z);
		}

		inline F32 length() const{
			return sqrtf(*this * *this);
		}

		inline CVector3 normalized() const{
			return (*this / this->length());
		}

        inline F32& operator[](const S32 i) {
            return *(&x + i);
        }

        inline F32 operator[](const S32 i) const {
            return *(&x + i);
        }

        inline void lerp(F32 f, const CVector3 &v){
            x += f * (v.x - x);
            y += f * (v.y - y);
            z += f * (v.z - z);
        }

        void swapHand(){
            F32 t = y;
            y = z;
            z = -t;
//            x = -x;
        }

		F32 x;
		F32 y;
		F32 z;

		static const CVector3 ZERO;
		static const CVector3 UNIT_X;
		static const CVector3 UNIT_Y;
		static const CVector3 UNIT_Z;

		void save(CSerializer *ser) const;
		void load(CSerializer *ser);
};

struct hash_CVector3 {
	S32 operator()(const CVector3 &v) const { return (S32)(v.x + v.y + v.z); }
};

template<class T>
class TVector4 {
	public:
		inline TVector4() : x(0), y(0), z(0), w(1) {};
		inline TVector4(const T nx, const T ny, const T nz) : x(nx), y(ny), z(nz), w(1) {};
		inline TVector4(const T nx, const T ny, const T nz, const T nw) : x(nx), y(ny), z(nz), w(nw) {};
		inline TVector4(const TVector4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) {};
        inline TVector4(const CVector3 &v) : x(v.x), y(v.y), z(v.z), w(1.0) {};

		inline TVector4 operator+(const TVector4 &v) const{
			return TVector4(x+v.x, y+v.y, z+v.z, w+v.w);
		}

		inline TVector4 operator-(const TVector4 &v) const{
			return TVector4(x-v.x, y-v.y, z-v.z, w-v.w);
		}

		//dot product
		inline T operator*(const TVector4 &v) const{
			return x*v.x + y*v.y + z*v.z + w*v.w;
		}

		//cross product
		inline TVector4 operator^(const TVector4 &v) const{
			return TVector4( (y*v.z-z*v.y), (z*v.x-x*v.z), (x*v.y-y*v.x) );
		}

		inline TVector4 operator+( const T f ) const{
			return TVector4(x+f, y+f, z+f, w+f);
		}

		inline TVector4 operator-( const T f ) const{
			return TVector4(x-f, y-f, z-f, w-f);
		}

		inline TVector4 operator-() const{
			return TVector4(-x, -y, -z, -w);
		}

		inline TVector4 operator*(const T f) const{
			return TVector4(x*f, y*f, z*f, w*f);
		}

		inline TVector4 operator/(const T f) const{
			T inv = 1.0f/f;
			return TVector4(x*inv, y*inv, z*inv, w*inv);
		}

		inline TVector4& operator=(const TVector4 &v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;
			return *this;
		}

		inline TVector4& operator=(const CVector3 &v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = 1.0f;
			return *this;
		}

        inline CVector3 toVec3() const {
            return CVector3(x, y, z);
        }

        inline TVector4& operator+=(const CVector3 &v){
			x += v.x;
			y += v.y;
			z += v.z;
            return *this;
		}

        inline TVector4& operator+=(const TVector4 &v){
			x += v.x;
			y += v.y;
			z += v.z;
            w += v.w;
			return *this;
		}

        inline TVector4& operator-=(const CVector3 &v){
			x -= v.x;
			y -= v.y;
			z -= v.z;
            return *this;
		}

		inline TVector4& operator-=(const TVector4 &v){
			x -= v.x;
			y -= v.y;
			z -= v.z;
            w -= v.w;
			return *this;
		}

		inline TVector4& operator*=(const T f){
			x *= f;
			y *= f;
			z *= f;
            w *= f;
			return *this;
		}

		inline TVector4& operator/=(const T f){
			T inv = 1.0f / f;
			x *= inv;
			y *= inv;
			z *= inv;
            w *= inv;
			return *this;
		}

        inline T& operator[](const S32 i) {
            return *(&x + i);
        }

        inline F32 operator[](const S32 i) const {
            return *(&x + i);
        }

		inline T length() const{
			return sqrtf(*this * *this);
		}

		inline TVector4 normalized() const{
			return (*this / this->length());
		}

        void swapHand(){
            F32 t = y;
            y = z;
            z = -t;
//            x = -x;
        }

		T x;
		T y;
		T z;
		T w;

		void save(CSerializer *ser) const;
		void load(CSerializer *ser);
};

typedef TVector4<F32> CVector4;

class CVector2 {
	public:
		inline CVector2() : x(0), y(0) {};
		inline CVector2(const F32 nx, const F32 ny) : x(nx), y(ny) {};
		inline CVector2(const CVector2 &v) : x(v.x), y(v.y) {};

		inline CVector2 operator+(const CVector2 &v) const{
			return CVector2(x+v.x, y+v.y);
		}

		inline CVector2 operator-(const CVector2 &v) const{
			return CVector2(x-v.x, y-v.y);
		}

		//dot product
		inline F32 operator*(const CVector2 &v) const{
			return x*v.x + y*v.y;
		}

		inline CVector2 operator+( const F32 f ) const{
			return CVector2(x+f, y+f);
		}

		inline CVector2 operator-( const F32 f ) const{
			return CVector2(x-f, y-f);
		}

		inline CVector2 operator-() const{
			return CVector2(-x, -y);
		}

		inline CVector2 operator*(const F32 f) const{
			return CVector2(x*f, y*f);
		}

		inline CVector2 operator/(const F32 f) const{
			F32 inv = 1.0f/f;
			return CVector2(x*inv, y*inv);
		}

		inline CVector2& operator=(const CVector2 &v)
		{
			x = v.x;
			y = v.y;
			return *this;
		}

		inline CVector2& operator+=(const CVector2 &v){
			x += v.x;
			y += v.y;
			return *this;
		}

		inline CVector2& operator-=(const CVector2 &v){
			x -= v.x;
			y -= v.y;
			return *this;
		}

		inline CVector2& operator*=(const F32 f){
			x *= f;
			y *= f;
			return *this;
		}

		inline CVector2& operator/=(const F32 f){
			F32 inv = 1.0f / f;
			x *= inv;
			y *= inv;
			return *this;
		}

		inline F32 length() const{
			return sqrtf(*this * *this);
		}

		inline CVector2 normalized() const{
			return (*this / this->length());
		}

        inline F32& operator[](const S32 i) {
            return *(&x + i);
        }

        inline F32 operator[](const S32 i) const {
            return *(&x + i);
        }

		F32 x;
		F32 y;

		void save(CSerializer *ser) const;
		void load(CSerializer *ser);
};

template<class vType>
inline void normalize(vType& v){
	v /= v.length();
}

typedef TVector4<byte> CColor;
typedef CVector4 CColorf;
typedef CVector2 CTexCoord;

#endif
