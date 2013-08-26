/***************************************************************************
                          pointer.cpp  -  description
                             -------------------
    begin                : Sun Jun 2 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#include "pointer.h"
#include <serializer.h>
    
template<class T>
void TPointer<T>::save(CSerializer *ser) const {
    ser->writeSerializable(mObject);
}

template<class T>
void TPointer<T>::load(CSerializer *ser) {
    mObject = ser->readSerializable();
}

