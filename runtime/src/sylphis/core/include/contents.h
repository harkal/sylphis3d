/***************************************************************************
                         contents.h  -  description
                             -------------------
    begin                : Sat Nov 9 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/


#ifndef _CONTENTS_H_
#define _CONTENTS_H_

const U32 CONTENTS_AIR         = 0;
const U32 CONTENTS_SOLID       = 1 << 0;
const U32 CONTENTS_LAVA        = 1 << 1;
const U32 CONTENTS_SLIME       = 1 << 2;
const U32 CONTENTS_WATER       = 1 << 3;
const U32 CONTENTS_FOG         = 1 << 4;
const U32 CONTENTS_BODY        = 1 << 5;
const U32 CONTENTS_TRIGGER     = 1 << 6;
const U32 CONTENTS_PLAYERCLIP  = 1 << 7;
const U32 CONTENTS_MONSTERCLIP = 1 << 8;
const U32 CONTENTS_CORPSE      = 1 << 9;

const U32 MASK_ALL             = (U32)-1;
const U32 MASK_SOLID           = CONTENTS_SOLID;
const U32 MASK_PLAYERSOLID     = CONTENTS_SOLID | CONTENTS_PLAYERCLIP | CONTENTS_BODY;
const U32 MASK_WATER           = CONTENTS_WATER | CONTENTS_LAVA | CONTENTS_SLIME;
const U32 MASK_OPAQUE          = CONTENTS_SOLID | CONTENTS_SLIME | CONTENTS_LAVA;
const U32 MASK_SHOT            = CONTENTS_SOLID | CONTENTS_BODY | CONTENTS_CORPSE;

enum eSurfaceType {
	
};

#endif
