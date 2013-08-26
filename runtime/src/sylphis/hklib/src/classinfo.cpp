/***************************************************************************
                        classinfo.cpp  -  description
                             -------------------
    begin                : Wed Jun 15 2005
    copyright            : (C) 2005 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#include <classinfo.h>


CClassInfo::CClassInfo(const std::string classname, CSerializable*  ( *fact )(void)){
    setClassName(classname);
    factory = fact;

    CClassManager::getSingleton()->registerClass(this);
}

CClassInfo::~CClassInfo(){

}

void CClassInfo::setClassName(const std::string classname){
    mClassName = classname;
}

std::string CClassInfo::getClassName(){
    return mClassName;
}


CClassManager::CClassManager(){

}

CClassManager::~CClassManager(){

}

void CClassManager::registerClass(CClassInfo *ci){
    mClassInfos[ci->getClassName()] = ci;
}

CClassInfo* CClassManager::getClassType(const std::string &classname){
    return mClassInfos[classname];
}

CSerializable* CClassManager::factory(const std::string &classname) const {
    const CClassInfo* ci = (*mClassInfos.find(classname)).second;
    return ci->factory();
}

