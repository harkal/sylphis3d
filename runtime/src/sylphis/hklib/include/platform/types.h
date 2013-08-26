/***************************************************************************
                           types.h  -  description
                             -------------------
    begin                : Sun 19 Mar 2006
    copyright            : (C) 2006 by Harry Kalogirou
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

#ifndef _TYPES_H_
#define _TYPES_H_

typedef float              F32;
typedef double             F64;

typedef char               S8;
typedef unsigned char      U8;

typedef signed short       S16;
typedef unsigned short     U16;

typedef signed int         S32;
typedef unsigned int       U32;

#if defined(__x86_64__) || defined(_M_X64)
    /** 64bit platform */
    typedef signed long int    S64;
    typedef unsigned long int  U64;
#else
    /** 32bit platform */
    typedef signed long long int    S64;
    typedef unsigned long long int  U64;
#endif

#endif
