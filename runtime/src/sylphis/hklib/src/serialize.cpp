/***************************************************************************
                        serialize.cpp  -  description
                             -------------------
    begin                : Sun Apr 25 2005
    copyright            : (C) 2005 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#include <serialize.h>

IMPLEMENT_SERIALIZABLE(CSerializable);

CSerializable::CSerializable(){
}

CSerializable::~CSerializable(){
}

void CSerializable::save(CSerializer *ser) const {
}

void CSerializable::load(CSerializer *ser) {
}
