/***************************************************************************
                    vertexbuffer.h  -  description
                            -------------------
                begin                : Mon Aug 23 2004
                copyright            : (C) 2004 by Harry Kalogirou
                email                : harkal@sylphis3d.com
 ***************************************************************************
    This file is part of "Sylphis3D Game Engine".

    "Sylphis3D Game Engine" is free software; you can redistribute it and/or 
    modify it under the terms of the GNU General Public License as 
    published by the Free Software Foundation; either version 2 of the 
    License, or (at your option) any later version, 
    plus a special exception:
      Linking "Sylphis3D Game Engine" statically or dynamically with other 
    modules is making a combined work based on this library. Thus, the 
    terms and conditions of the GNU General Public License cover the whole 
    combination. As a special exception, the copyright holders of this 
    library give you permission to link this library with independent 
    modules to produce an executable, regardless of the license terms of 
    these independent modules, and to copy and distribute the resulting 
    executable under terms of your choice, provided that you also meet, 
    for each linked independent module, the terms and conditions of the 
    license of that module. An independent module is a module which is 
    not derived from or based on this program. If you modify this program,
    you may extend this exception to your version of the library, but you 
    are not obligated to do so. If you do not wish to do so, delete this 
    exception statement from your version.

    "Sylphis3D Game Engine" is distributed in the hope that it will be 
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with "Sylphis3D Game Engine"; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 ***************************************************************************/

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <assert.h>
#include <vector.h>
#include <pointer.h>

#include <gutil.h>

#include <buffer.h>

SmartPointer(CVertexBuffer);
class CVertexBuffer : public CBuffer {
public:

    enum EComponent {
        POSITION = 0,
        NORMAL = 1,
        TANGENT = 2,
        TEX_COORD = 3,
        SIZE = 4
    };

    CVertexBuffer(){
        mStride = 0;
        mComponentNum = 0;
        memset(mOffsets, 0, SIZE * sizeof(S32));
    };

    virtual ~CVertexBuffer();

    virtual S32 getOffset(const EComponent &comp){
        return mOffsets[comp];
    }
    
    virtual S32 getStride(){
        return mStride;
    }

    void addComponent(const EComponent &comp);
    void addComponent(const EComponent &comp, S32 stride);

    void allocate(U32 size, eUsageType usage = DYNAMIC, bool system = false, void *data = 0);
protected:
    virtual void *_map(eOperation op);
    virtual void _unmap();
    virtual void _uploadData(void *data, U32 offset, U32 size);
    virtual void _activate();
    virtual void _deactivate();

    S32 mOffsets[SIZE];
    S32 mComponentNum;
    S32 mStride;
};

class CVertexBufferSegment {
public:
    U32 mOffset;
    U32 mSize;
};

template<class T> class CVertexBufferAccess {
public:
    CVertexBufferAccess(){
    }
    CVertexBufferAccess(CVertexBuffer *buf, CVertexBuffer::EComponent comp){
        setupBuffer(buf, comp);
    }

    void setupBuffer(CVertexBuffer *buf, CVertexBuffer::EComponent comp, S32 stride = -1){
        mBuffer = buf;
        mStride = stride == -1 ? buf->getStride() : stride;
        mComponent = comp;
    }

    T &operator[](U32 pos){
        char *data = (char *)mBuffer->map();
        S32 stride = mStride ? mStride : 1;
        return *(T*)(data + pos * stride + mBuffer->getOffset(mComponent));
    }

    S32 getOffset(const CVertexBuffer::EComponent &comp){
        return mBuffer->getOffset(comp);
    };

    S32 getStride(){
        return mStride;
    }

    void deactivate(){
        mBuffer->deactivate();
    }

    void bindAsVertexBuffer(S32 size = 4, S32 offset = 0){
        glVertexPointer(size, GL_FLOAT, getStride(), (char *)mBuffer->getPointer() + mBuffer->getOffset(CVertexBuffer::POSITION) + offset); checkGLError();
    }

    void bindAsNormalBuffer(S32 size = 3, S32 offset = 0){
        glNormalPointer(GL_FLOAT, getStride(), (char *)mBuffer->getPointer() + mBuffer->getOffset(CVertexBuffer::NORMAL) + offset); checkGLError();
    }

    void bindAsTexCoordBuffer(CVertexBuffer::EComponent comp = CVertexBuffer::TEX_COORD, S32 size = 2, S32 offset = 0){
        glTexCoordPointer(size, GL_FLOAT, getStride(), (char *)mBuffer->getPointer() + mBuffer->getOffset(comp) + offset); checkGLError();
    }

    void bindAsAttribBuffer(S32 location, CVertexBuffer::EComponent comp, S32 size = 3, S32 offset = 0){
        glVertexAttribPointerARB(location, size, GL_FLOAT, 0, getStride(), (char *)mBuffer->getPointer() + mBuffer->getOffset(comp) + offset); checkGLError();
    }

protected:
    CVertexBufferPtr mBuffer;
    CVertexBuffer::EComponent mComponent;
    S32 mStride;
};

#endif

