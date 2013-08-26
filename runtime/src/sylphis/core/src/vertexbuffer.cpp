/***************************************************************************
                       vertexbuffer.cpp  -  description
                             -------------------
    begin                : Mon Aug 23 2004
    copyright            : (C) 2004 by Harry Kalogirou
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

#include <vertexbuffer.h>

CVertexBuffer::~CVertexBuffer(){

}

void CVertexBuffer::allocate(U32 size, eUsageType usage, bool system, void *data) {
    mSize = size;
    mUsage = usage;
    mLocked = false;
    //GLEW_ARB_vertex_buffer_object = false;
    if(GLEW_ARB_vertex_buffer_object && !system){
        mSystemMemory = false;
        checkGLError();
        glGenBuffersARB(1, &mBufferId);
        checkGLError();
        GLenum use;
        switch (usage) {
            case STATIC:
                use = GL_STATIC_DRAW_ARB;
                break;
            case DYNAMIC:
                use = GL_STREAM_DRAW_ARB;
                break;
        }
        activate();
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, mSize, data, use);
        checkGLError();
        deactivate();
    } else {
        mSystemMemory = true;
        mSysAddr = malloc(mSize);
        if(data){
            memcpy(mSysAddr, data, mSize);
        }
    }
}

void *CVertexBuffer::_map(eOperation op){
    GLenum oper;
    switch (op) {
        case WRITE_ONLY:
            oper = GL_WRITE_ONLY_ARB;
            break;
        case READ_ONLY:
            oper = GL_READ_ONLY_ARB;
            break;
        case READ_WRITE:
            oper = GL_READ_WRITE_ARB;
            break;
    }
    return glMapBufferARB(GL_ARRAY_BUFFER_ARB, oper);
}

void CVertexBuffer::_unmap(){
    glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
}

void CVertexBuffer::_uploadData(void *data, U32 offset, U32 size){
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, size, data); checkGLError();
}

void CVertexBuffer::_activate(){
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferId); checkGLError();
};

void CVertexBuffer::_deactivate(){
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0); checkGLError();
};


void CVertexBuffer::addComponent(const EComponent &comp, S32 stride){
    mOffsets[comp] = stride;
}

void CVertexBuffer::addComponent(const EComponent &comp){
    switch (comp) {
    case POSITION:
        addComponent(comp, mStride);
        mStride += 4 * sizeof(F32);
        break;
    case NORMAL:
        addComponent(comp, mStride);
        mStride += 3 * sizeof(F32);
        break;
    case TANGENT:
        addComponent(comp, mStride);
        mStride += 3 * sizeof(F32);
        break;
    case TEX_COORD:
        addComponent(comp, mStride);
        mStride += 2 * sizeof(F32);
        break;
    }
}





