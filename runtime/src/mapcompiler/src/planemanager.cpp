/***************************************************************************
    copyright            : (C) 2002 by Vassili Tenente
                           (C) 2002-2005 Harry Kalogirou (harkal@gmail.com)
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

