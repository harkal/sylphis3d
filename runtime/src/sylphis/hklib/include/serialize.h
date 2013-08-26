/***************************************************************************
                          serialize.h  -  description
                             -------------------
    begin                : Sun Apr 25 2005
    copyright            : (C) 2005 by Harry Kalogirou
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


#ifndef _SERIALIZE_H_
#define _SERIALIZE_H_

#include <string>

#include <classinfo.h>

#define DEFINE_SERIALIZABLE(classname)              \
    public:                                         \
        static CClassInfo mClassInfo;               \
        static CSerializable *factory(void);        \
        static classname *cast(CSerializable *p);   \
        virtual std::string getClassName() const;   \
        virtual CClassInfo* getClassInfo() const;   \

#define IMPLEMENT_SERIALIZABLE(classname)           \
    CClassInfo classname::mClassInfo(#classname, classname::factory);  \
    CSerializable *classname::factory(void){                \
        classname *np = new classname;                      \
        return np;                                          \
    }                                                       \
    classname *classname::cast(CSerializable *p){           \
        return dynamic_cast<classname *>(p);                  \
    }                                                       \
    std::string classname::getClassName() const {           \
        return classname::mClassInfo.getClassName();        \
    }                                                       \
    CClassInfo *classname::getClassInfo() const {           \
        return &classname::mClassInfo;                      \
    }                                                       \

class CSerializer;

class CSerializable {
DEFINE_SERIALIZABLE(CSerializable);
public:
    CSerializable();
    virtual ~CSerializable();

    virtual void save(CSerializer *ser) const;
    virtual void load(CSerializer *ser);
};


#endif







