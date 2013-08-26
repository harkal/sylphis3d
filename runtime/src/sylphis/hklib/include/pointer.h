/***************************************************************************
                          pointer.h  -  description
                             -------------------
    begin                : Sun Jun 2 2002
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

#ifndef POINTER_H
#define POINTER_H

#include <refobject.h>

template <class T> class ScopePtr {
public:
    ScopePtr(T* object = 0){
        mObject = object;
    };

    ~ScopePtr(){
        delete mObject;
    };

    ScopePtr& operator= (T* object){
        if(mObject)
            delete mObject;
        mObject = object;
        return *this;
    };

    operator T* () const { return mObject; };
    T& operator* () const { return *mObject; };
    T* operator-> () const { return mObject; };

    bool operator== (T* object) const { return mObject == object; };
    bool operator!= (T* object) const { return mObject != object; };
    bool operator== (const ScopePtr& p) const {
        return mObject == p.mObject;
    };
    bool operator!= (const ScopePtr& p) const {
        return mObject != p.mObject;
    };

private:
    T* mObject;
};

#define SmartPointer(classname) \
    class classname; \
    typedef TPointer<classname> classname##Ptr

template <class T> class TPointer {
public:
    typedef T* pointer_type;
    TPointer(T* object = 0){
        mObject = object;
        if(mObject)
            mObject->incReferences();
    };

    TPointer(const TPointer& p){
        mObject = p.mObject;
        if(mObject)
            mObject->incReferences();
    };

    ~TPointer(){
        if(mObject)
            mObject->decReferences();
    };

    operator T* () const { return mObject; };
    T& operator* () const { return *mObject; };
    T* operator-> () const { return mObject; };

    TPointer& operator= (const TPointer& p){
        if(mObject != p.mObject){
            if(mObject)
                mObject->decReferences();
            mObject = p.mObject;
            if(mObject)
                mObject->incReferences();
        }
        return *this;
    };

    TPointer& operator= (T* object){
        if(mObject != object){
            if(mObject)
                mObject->decReferences();
            mObject = object;
            if(mObject)
                mObject->incReferences();
        }
        return *this;
    };

    bool operator== (T* object) const { return mObject == object; };
    bool operator!= (T* object) const { return mObject != object; };
    bool operator== (const TPointer& p) const {
        return mObject == p.mObject;
    };
    bool operator!= (const TPointer& p) const {
        return mObject != p.mObject;
    };

    void save(CSerializer *ser) const;
    void load(CSerializer *ser);
private:
    T* mObject;
};

#endif
