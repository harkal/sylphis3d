/***************************************************************************
                          refobject.h  -  description
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

#ifndef REFOBJECT_H
#define REFOBJECT_H

#include <new>
#include <platform/types.h>
#include <serialize.h>

///
/// The base reference counted object class. All reference counted objects must 
/// inherit from this. Smart pointers use objects derived from this class.
///
class CRefObject : public CSerializable {
DEFINE_SERIALIZABLE(CRefObject);
public:
	CRefObject();
	virtual ~CRefObject();

	void incReferences();
	void decReferences();

    F64 getCreationTime();
    F64 getLiveTime();

    static U32 getTotalRefObjects();

	U32 getReferences();

    virtual void save(CSerializer *ser) const;
    virtual void load(CSerializer *ser);

private:
	U32 mReferences;
    F64 mTimeStamp;
    static U32 totalRefObjects;
};


#endif

