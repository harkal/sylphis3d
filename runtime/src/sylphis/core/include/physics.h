/***************************************************************************
                         physics.h  -  description
                             -------------------
    begin                : Thu Jun 13 2004
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

#ifndef PHYSICS_H
#define PHYSICS_H

#define WORLD_TO_PHYSICS_RATIO 0.1
#define PHYSICS_TO_WORLD_RATIO (1.0 / WORLD_TO_PHYSICS_RATIO)
#define TO_PHYSICS(x) ((x)*WORLD_TO_PHYSICS_RATIO)
#define TO_WORLD(x) ((x)*PHYSICS_TO_WORLD_RATIO)

#endif


