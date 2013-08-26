/***************************************************************************
                         classinfo.h  -  description
                             -------------------
    begin                : Wed Jun 15 2005
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

#ifndef _CLASSINFO_H_
#define _CLASSINFO_H_

#include <singleton.h>
#include <map>
#include <string>

class CSerializable;

class CClassInfo {
public:
    CClassInfo(const std::string classname, CSerializable*  ( *fact )(void));
    ~CClassInfo();

    void setClassName(const std::string classname);
    std::string getClassName();

    CSerializable*  ( *factory )(void);

protected:
    std::string mClassName;
};

class CClassManager : public CSingleton<CClassManager> {
public:
    CClassManager();
    virtual ~CClassManager();

    static CClassManager* getSingleton() {
        static CClassManager singleton;
        return &singleton;
    }

    void                registerClass(CClassInfo *ci);
    CClassInfo*         getClassType(const std::string &classname);
    CSerializable*      factory(const std::string &classname) const ;

protected:
    typedef std::map<std::string, CClassInfo*> ClassInfoMap;
    ClassInfoMap mClassInfos;
};

#endif



