/***************************************************************************
                          serialize.h  -  description
                             -------------------
    begin                : Sun Apr 25 2005
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


#ifndef _SERIALIZE_H_
#define _SERIALIZE_H_

#include <string>

#include <classinfo.h>

#define DEFINE_SERIALIZABLE(classname)              \
    public:                                         \
        static CClassInfo mClassInfo;               \
        static CSerializable *factory(void);        \
        static classname *cast(CSerializable *p);   \
        virtual std::string getClassName() const;   \
        virtual CClassInfo* getClassInfo() const;   \

#define IMPLEMENT_SERIALIZABLE(classname)           \
    CClassInfo classname::mClassInfo(#classname, classname::factory);  \
    CSerializable *classname::factory(void){                \
        classname *np = new classname;                      \
        return np;                                          \
    }                                                       \
    classname *classname::cast(CSerializable *p){           \
        return dynamic_cast<classname *>(p);                  \
    }                                                       \
    std::string classname::getClassName() const {           \
        return classname::mClassInfo.getClassName();        \
    }                                                       \
    CClassInfo *classname::getClassInfo() const {           \
        return &classname::mClassInfo;                      \
    }                                                       \

class CSerializer;

class CSerializable {
DEFINE_SERIALIZABLE(CSerializable);
public:
    CSerializable();
    virtual ~CSerializable();

    virtual void save(CSerializer *ser) const;
    virtual void load(CSerializer *ser);
};


#endif







