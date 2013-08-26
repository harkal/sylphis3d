/***************************************************************************
                         buffer.h  -  description
                            -------------------
                begin                : Sun Sep 4 2005
                copyright            : (C) 2005 by Harry Kalogirou
                email                : harkal@sylphis3d.com
***************************************************************************/

#ifndef BUFFER_H
#define BUFFER_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <openglext.h>
#include <pointer.h>


SmartPointer(CBuffer);
class CBuffer : public CRefObject {
public:
    enum eUsageType {
        STATIC,
        DYNAMIC
    };

    enum eOperation {
        READ_ONLY,
        WRITE_ONLY,
        READ_WRITE
    };

    CBuffer();
    virtual ~CBuffer();

    virtual void activate();
    virtual void deactivate();

    virtual void *map(eOperation op = READ_WRITE);
    virtual void unmap();

    virtual void uploadData(void *data, U32 offset, U32 size);

    void *getPointer(){
        if(mSystemMemory)
            return mSysAddr;
        else {
            unmap();
            activate();
            return NULL;
        }
    };

    U32 getSize(){
        return mSize;
    };


protected:
    virtual void *_map(eOperation op);
    virtual void _unmap();
    virtual void _uploadData(void *data, U32 offset, U32 size);
    virtual void _activate();
    virtual void _deactivate();

    GLuint mBufferId;
    U32 mSize;
    bool mSystemMemory;
    void *mMapAddress;
    void *mSysAddr;
    eUsageType mUsage;
    bool mLocked;    

    static GLuint mActiveBuffer;
};


#endif

