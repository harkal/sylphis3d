/***************************************************************************
                          epsilon.h  -  description
                             -------------------
    begin                : Mon Mar 3 2003
    copyright            : (C) 2003 by harkal
    email                : harkal@cool
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

 #ifndef _EPSILON_H_
 #define _EPSILON_H_


#define  INBOUNDS_EPSILON           1.0f
#define  AREA_EPSILON               1.0f
#define  MERGE_EPSILON              0.01f
#define  PLANES_ON_EPSILON          0.01f
//usally this must be POINT_ON_PLANE_EPSILON < PLANES_ON_EPSILON
#define  POINT_ON_PLANE_EPSILON     0.001f
#define  VOLUME_EPSILON             1.0f
#define  SMALL_SIDE_2               0.05f

//static const float PLANES_ON_EPSILON=0.1f;
const float huge = 100000;
//const char hugeText[9]= "10000000";
const char hugeText[7]= "100000";

#endif //_EPSILON_H_


