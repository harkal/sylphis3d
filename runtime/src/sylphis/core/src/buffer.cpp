/***************************************************************************
                         buffer.cpp  -  description
                            -------------------
                begin                : Sun Sep 4 2005
                copyright            : (C) 2005 by Harry Kalogirou
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

#include <buffer.h>
#include <gutil.h>

GLuint CBuffer::mActiveBuffer = 0;

CBuffer::CBuffer(){
    mSize = 0;
    mMapAddress = 0;
}

CBuffer::~CBuffer(){
    if(!mSize)
        return;
    if(!mSystemMemory){
        unmap();
        glDeleteBuffersARB(1, &mBufferId);
        checkGLError();
    } else {
        free(mSysAddr);
    }
}

void CBuffer::activate(){
    if(mActiveBuffer == mBufferId)
        return;
    if(mSystemMemory)
        return;
    _activate();
    checkGLError();
    mActiveBuffer = mBufferId;
};

void CBuffer::deactivate(){
    if(mSystemMemory)
        return;
    assert(mActiveBuffer == mBufferId);
    _deactivate();
    checkGLError();
    mActiveBuffer = 0;
};

void *CBuffer::map(eOperation op){
    if(mMapAddress)
        return mMapAddress;

    if(!mSystemMemory){
        activate();
        mMapAddress = _map(op);
        assert(mMapAddress);
        checkGLError();
        deactivate();
    } else {
        mMapAddress = mSysAddr;
    }

    return mMapAddress;
}

void CBuffer::unmap(){
    if(!mMapAddress)
        return;
    if(mSystemMemory)
        return;
    mMapAddress = 0;
    activate();
    _unmap();
    checkGLError();
    deactivate();
    if(mUsage == STATIC)
        mLocked = true;
}

void CBuffer::uploadData(void *data, U32 offset, U32 size){
    if(mSystemMemory)
        memcpy(mSysAddr, data, size);
    else {
        assert(mMapAddress == 0);
        activate();
        _uploadData(data, offset, size);
        deactivate();
    }
}

void *CBuffer::_map(eOperation op){
    return 0;
}

void CBuffer::_unmap(){
}

void CBuffer::_uploadData(void *data, U32 offset, U32 size){

}

void CBuffer::_activate(){

};

void CBuffer::_deactivate(){

};

