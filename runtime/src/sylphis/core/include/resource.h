/***************************************************************************
                          resource.h  -  description
                             -------------------
    begin                : Wed May 29 2002
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

#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <pointer.h>

SmartPointer(CResource);
class CResource : public CRefObject {
public: 
	CResource();
	virtual ~CResource();

	virtual void load();
	virtual void load(std::string &n);
	virtual void unload();

	std::string getName();
 	virtual void setName(const std::string &n);

	bool isLoaded();

	bool operator== (CResource &op1);
	bool operator== (const std::string &op1);
protected:
	std::string name;
	bool	IsLoaded;
};

#endif
