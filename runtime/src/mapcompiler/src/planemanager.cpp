/***************************************************************************
    copyright            : (C) 2002 by Vassili Tenente
                           (C) 2002-2005 Harry Kalogirou (harkal@gmail.com)
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
#include <cassert>

#include "planemanager.h"
#include "plane.h"

#ifdef MEMPROF
#include "mmgr.h"
#endif

CPlaneManager::CPlaneManager(){
}

CPlaneManager::~CPlaneManager(){
	freePlanes();
}

void CPlaneManager::reset(){
	planes.clear();
}

void CPlaneManager::freePlanes(){
	PlaneVector::iterator pEnd = planes.end();
	for(PlaneVector::iterator pIter = planes.begin() ; pIter!=pEnd ; ++pIter)
		delete (*pIter);
	reset();
}

int CPlaneManager::addPlane(const CPlane &plane){
	int newId=-1;
	int potition=0;
	CPlane::pRelation relate;

	PlaneVector::const_iterator pIter, pEnd;    

	pEnd = planes.end();
	for (pIter = planes.begin() ; pIter != pEnd; ++pIter, ++potition) {
		relate = CPlane::planesRelation(plane, **pIter);
		if (relate == CPlane::EQUAL) {
			newId = potition;
			break;
		}
	}
	if (newId!=-1){
		return newId;
	}
	newId = planes.size();        

	planes.push_back(new CPlane(plane));
	planes.back()->setPlaneId(newId);
	planes.push_back(plane.inversed());
	planes.back()->setPlaneId(newId+1);

	return newId;
}

/*
int CPlaneManager::getPlaneId(CPlane *plane)
{
int newId=-1;
int potition=0;
CPlane::pRelation relate;

pEnd = planes.end();
for (pIter = planes.begin() ; pIter != pEnd; ++pIter,++potition) {
relate = CPlane::planesRelation(plane,*pIter);

if (relate == CPlane::EQUAL) {
newId = potition;
break;
}
}
if (newId!=-1){
return newId;
}
assert(false);
return newId;
}
*/

int CPlaneManager::addPlane(CVector3 points[3]){
	CPlane p(points);
	return addPlane(p);
}

int CPlaneManager::addPlane(vector < CVector3 > &points){
	CPlane p(points);
	return addPlane(p);
}

int CPlaneManager::addPlane(vector < CVertex > &points){
	CPlane p(points);
	return addPlane(p);
}

int CPlaneManager::addPlane(float d, CVector3 norma){
	CPlane p(norma, d);
	return addPlane(p);
}

CPlane *CPlaneManager::getPlane(const int &id){
	return planes[id];
}

CPlane *CPlaneManager::inversed(CPlane *plane){
	return planes[plane->getPlaneId()^1];
}

CPlane *CPlaneManager::inversed(const int &id){
	return planes[id^1];
}

void CPlaneManager::initForWritting(){
	writePlanes.clear();
	isWritten.clear();
	int noPlanes = planes.size();
    isWritten.resize(noPlanes, -1);
}

int CPlaneManager::getWrittingPlaneId(const int &planeId){
    assert(planeId < planes.size());
	if(isWritten[planeId] != -1)
		return isWritten[planeId];
	isWritten[planeId] = writePlanes.size();  
	writePlanes.push_back(planes[planeId]);
	return isWritten[planeId];
}

