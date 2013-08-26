/***************************************************************************
                         vector.cpp  -  description
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
#include <vector.h>
#include <serializer.h>

const CVector3 CVector3::ZERO(0,0,0);
const CVector3 CVector3::UNIT_X(1,0,0);
const CVector3 CVector3::UNIT_Y(0,1,0);
const CVector3 CVector3::UNIT_Z(0,0,1);

void CVector3::save(CSerializer *ser) const {
	ser->writeFloat(x);
	ser->writeFloat(y);
	ser->writeFloat(z);
}

void CVector3::load(CSerializer *ser){
	x = ser->readFloat();
	y = ser->readFloat();
	z = ser->readFloat();
}

template<>
void CVector4::save(CSerializer *ser) const {
	ser->writeFloat(x);
	ser->writeFloat(y);
	ser->writeFloat(z);
	ser->writeFloat(w);
}

template<>
void CVector4::load(CSerializer *ser){
	x = ser->readFloat();
	y = ser->readFloat();
	z = ser->readFloat();
	w = ser->readFloat();
}

void CVector2::save(CSerializer *ser) const {
	ser->writeFloat(x);
	ser->writeFloat(y);
}

void CVector2::load(CSerializer *ser){
	x = ser->readFloat();
	y = ser->readFloat();
}
