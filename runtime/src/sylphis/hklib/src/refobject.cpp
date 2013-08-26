/***************************************************************************
                          refobject.cpp  -  description
                             -------------------
    begin                : Sun Jun 2 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#include <iostream>
#include <assert.h>
#include <refobject.h>

#include <assert.h>

#include <serializer.h>

using namespace std;

IMPLEMENT_SERIALIZABLE(CRefObject);

U32 CRefObject::totalRefObjects = 0;

CRefObject::CRefObject(){
    mTimeStamp = 0.0;
    mReferences = 0;
    totalRefObjects++;
}

CRefObject::~CRefObject(){
    assert(totalRefObjects);
    totalRefObjects--;
}

void CRefObject::incReferences(){
	mReferences++;
//    cout << "refs " << references << "\n";
}

void CRefObject::decReferences(){
    mReferences--;
//    cout << "refs " << references << "\n";
    if(mReferences == 0){
//        cout << "deleting " << this << "\n";
        delete this;
    }
}

F64 CRefObject::getCreationTime(){
    return mTimeStamp;
}

F64 CRefObject::getLiveTime(){
    return 0.0;
}

U32 CRefObject::getReferences(){
     return mReferences;
}

U32 CRefObject::getTotalRefObjects(){
    return totalRefObjects;
}

void CRefObject::save(CSerializer *ser) const {
	ser->writeUnsignedInt(mReferences);
	ser->writeDouble(mTimeStamp);
}

void CRefObject::load(CSerializer *ser){
	mReferences = ser->readUnsignedInt();
	mTimeStamp = ser->readDouble();
}

