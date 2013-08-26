/***************************************************************************
                          resourcemanager.cpp  -  description
                             -------------------
    begin                : Wed May 29 2002
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

#include <resourcemanager.h>
#include <console.h>

CResourceManager::CResourceManager() : dirty(false) {
}
CResourceManager::~CResourceManager(){
	freeAll();
}

CResource *CResourceManager::find(const string &name){
	ResourceList::iterator iter;
	
	for(iter = resources.begin() ; iter != resources.end() ; ++iter){
		CResource *r = *iter;
		if(*r == name)return *iter;
	}
	return 0;
}

void CResourceManager::loadAll(){

}

void CResourceManager::freeAll(){
	ResourceList::iterator iter;
	
	for(iter = resources.begin() ; iter != resources.end() ; ++iter){
		CResourcePtr r = *iter;
		r = 0;
	}

	resources.clear();
	dirty = false;
}

void CResourceManager::freeUnused(){
	ResourceList::iterator iter;
	S32 count = 0;
    bool removed = true;

    while(removed){
        removed = false;
	    for(iter = resources.begin() ; iter != resources.end() ; ++iter){
		    CResource *r = *iter;
            if(r->getReferences() == 1){
                iter = resources.erase(iter);
                ++count;
                removed = true;
            }
	    }
    }
    gcon.printf("Freed %d resources (used %d)\n", count, resources.size());
    for(iter = resources.begin() ; iter != resources.end() ; ++iter){
	    CResource *r = *iter;
        gcon.printf("%s", r->getName().c_str());
	}
}

CResourceManager::ResourceList *CResourceManager::getResources(){
	return &resources;
}

