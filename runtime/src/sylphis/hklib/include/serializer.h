/***************************************************************************
                         serializer.h  -  description
                             -------------------
    begin                : Sat Jun 11 2005
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
