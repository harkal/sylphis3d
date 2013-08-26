/***************************************************************************
                         serializer.h  -  description
                             -------------------
    begin                : Sat Jun 11 2005
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

#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_

#include <string>
#include <map>

#include <platform/types.h>

class CFile;
class CSerializable;

class CSerializer {
public:
                    CSerializer();
    virtual         ~CSerializer();

    void            setFile(CFile *f);
    CFile*          getFile();
    S16             readShort();
    U16             readUnsignedShort();
    S8              readChar();
    U8              readUnsignedChar();
    S32             readInt();
    U32             readUnsignedInt();
    F32             readFloat();
    F64             readDouble();
    bool            readBool();
    std::string     readString();
    void*           readPointer();
    CSerializable*  readSerializable();

    void    writeShort(const S16 value);
    void    writeUnsignedShort(U16 value);
    void    writeChar(const S8 value);
    void    writeUnsignedChar(const U8 value);
    void    writeInt(const S32 value);
    void    writeUnsignedInt(const U32 value);
    void    writeFloat(const F32 value);
    void    writeDouble(const F64 value);
    void    writeBool(const bool value);
    void    writeString(const std::string &value);
    void    writePointer(const void *value);

    void    writeSerializable(const CSerializable *obj);

//#ifdef _WIN32
    template<class Cont>
    void writeContainer(const Cont &cont) {
        writeUnsignedInt(cont.size());
        typename Cont::const_iterator i;
        for(i = cont.begin() ; i != cont.end() ; ++i){
            writeSerializable(*i);
        }
    }

    template<class Cont>
    void writeContainerSimple(const Cont &cont) {
        writeUnsignedInt(cont.size());
        typename Cont::const_iterator i;
        for(i = cont.begin() ; i != cont.end() ; ++i){
            (*i).save(this);
        }
    }

    template<class Cont>
    void readContainer(Cont &cont) {
        U32 i, count;
        count = readUnsignedInt();
        for(i = 0 ; i < count ; ++i){
            cont.push_back(dynamic_cast<typename Cont::value_type>(readSerializable()));
        }
    }

    template<class Cont>
    void readContainerSimple(Cont &cont) {
        U32 i, count;
        count = readUnsignedInt();
        for(i = 0 ; i < count ; ++i){
            typename Cont::value_type obj;
            obj.load(this);
            cont.push_back(obj);
        }
    }

    template<class Cont>
    void readContainerPtr(Cont &cont) {
        U32 i, count;
        count = readUnsignedInt();
        for(i = 0 ; i < count ; ++i){
            typename Cont::value_type d = (typename Cont::value_type::pointer_type)(readSerializable());
            cont.push_back(d);
        }
    }
//#endif
protected:
    typedef std::map<void *, const CSerializable *> ConstObjectMap;
    typedef std::map<void *, CSerializable *> ObjectMap;

    CFile*              mFile;
    ConstObjectMap      mWriteObjects;
    ObjectMap           mReadObjects;
};



#endif
