/***************************************************************************
                    indexbuffer.h  -  description
                            -------------------
                begin                : Sun Sep 4 2005
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

#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <openglext.h>
#include <assert.h>
#include <vector.h>
#include <pointer.h>
#include <buffer.h>
#include <gutil.h>

SmartPointer(CIndexBuffer);
class CIndexBuffer : public CBuffer {
public:
    CIndexBuffer();
    virtual ~CIndexBuffer();

    void allocate(U32 size, eUsageType usage = DYNAMIC, bool system = false, void *data = 0);

protected:
    virtual void *_map(eOperation op);
    virtual void _unmap();
    virtual void _uploadData(void *data, U32 offset, U32 size);
    virtual void _activate();
    virtual void _deactivate();
};


#endif

