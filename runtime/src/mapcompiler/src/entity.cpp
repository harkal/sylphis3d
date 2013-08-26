/***************************************************************************
entity.cpp  -  description
-------------------
begin                : Sun Jun 9 2002
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
#include <stdio.h>
#include <iostream>

#ifdef MEMPROF
#include "mmgr.h"
#endif

#include "entity.h"
//#include "epsilon.h"
#include "polygon.h"
using namespace std;

CEntity::CEntity(void)
{
};

string CEntity::getProperty(const string & prop)
{
	//cout << "props.size="<<props.size()<<endl;
	map < string, string >::iterator mFind = props.find(prop);
	map < string, string >::iterator mEnd = props.end();
	if(mFind!= mEnd)
		return props[prop];
	return "";
}

CVector3 CEntity::getAsVector(const string & prop)
{
	int k = 0;
	if(props.find(prop)==props.end()){
		return CVector3(0,0,0);
	}
	string origin = props[prop];
	int originSize = origin.size();
	string *xyz = new string[3];
	for (int i = 0; i < originSize; i++) {
		if (origin[i] == ' ') {
			k++;
			int j = i + 1;
			for (; j < originSize; j++)
				if (origin[j] != ' ')
					break;
			i = j - 1;
			continue;
		}
		xyz[k] += origin[i];
	}
	//CVector3 rv((float) atof(xyz[0].c_str()), (float) atof(xyz[2].c_str()),
	//            -(float) atof(xyz[1].c_str()));
	CVector3 rv;

	//for(int i=0; i<3;i++)
	sscanf(xyz[0].c_str(), "%f", &rv.x);
	sscanf(xyz[2].c_str(), "%f", &rv.y);    
	sscanf(xyz[1].c_str(), "%f", &rv.z);
	rv.z=-rv.z;
	delete[]xyz;
	return rv;
}


void CEntity::setProperty(const string & prop, const string & value)
{
	props[prop] = value;
}

int CEntity::getNoProperties()
{
	return props.size();
}

void CEntity::addBrush(CBrush & b)
{
	brushes.push_back(b);
	brushes.back().setBrushId(brushes.size()-1);
}

CBrush & CEntity::getBrush(const int &n)
{
	return brushes[n];
}

CBrush & CEntity::getLastBrush(void)
{
	return brushes[brushes.size() - 1];
}

vector < CBrush > &CEntity::getBrushes(void)
{
	return brushes;
}

int CEntity::getBrushesNo(void){
	return brushes.size();
}

void CEntity::debug(void){

}


void CEntity::callCreatePolygons(){
	vector < CBrush >::iterator bIter, bEnd;
	bEnd = brushes.end();

	for(bIter = brushes.begin() ; bIter != bEnd ; ++bIter){
		bIter->createPolygons();
	}
}


void CEntity::appendChoppedAndStructural( list<CPolygon *> *polygonList )
{
	vector < CBrush >::iterator bIter, bEnd, bIter2;
	bool             takeIt;
	CPolygon         *polygon;
	bIter          = brushes.begin();
	bEnd           = brushes.end();

	while(bIter != bEnd){
		if(!bIter->isStructural()){
			++bIter;
			continue;
		}
		bIter->polygonsInit();
		polygon = bIter->nextPolygon();

		while(polygon){
			bIter2 = brushes.begin();  
			takeIt=true;
			while(bIter2!=bEnd){
				if(bIter2==bIter || !bIter2->isOpaque()){
					bIter2++;
					continue;
				}
				if(bIter2->subPolygon(polygon)){
					takeIt=false;
					break;
				}
				bIter2++;
			}
			if(takeIt){
				polygonList->push_front(polygon);
			}
			else{
				polygon->getBrushPlane()->setVisible(false);
			}
			polygon=bIter->nextPolygon();
		}    
		++bIter;
	}

}

void CEntity::appendChoppedStructuralAndVisible( list<CPolygon *> *polygonList )
{
	vector < CBrush >::iterator bIter, bEnd, bIter2;
	bool             takeIt;
	CPolygon         *polygon;
	bIter          = brushes.begin();
	bEnd           = brushes.end();

	while(bIter != bEnd){
		if(!bIter->isStructural()){
			++bIter;
			continue;
		}
		bIter->polygonsInit();
		polygon = bIter->nextPolygon();

		while(polygon){
			if(!polygon->getBrushPlane()->isVisible()){
				polygon=bIter->nextPolygon();
				continue;
			}
			bIter2 = brushes.begin();  
			takeIt=true;
			while(bIter2!=bEnd){
				if(bIter2->getBrushId()==bIter->getBrushId() || !bIter2->isOpaque()){
					bIter2++;
					continue;
				}
				if(bIter2->subPolygon(polygon)){
					takeIt=false;
					break;
				}
				bIter2++;
			}
			if(takeIt)
				polygonList->push_front(polygon);
			polygon=bIter->nextPolygon();
		}    
		//bIter->reBoundBrush();
		++bIter;
	}

}

void CEntity::appendChopped( list<CPolygon *> *polygonList )
{
	vector < CBrush >::iterator bIter, bEnd, bIter2;
	bool             takeIt;
	CPolygon         *polygon;
	bIter          = brushes.begin();
	bEnd           = brushes.end();

	while(bIter != bEnd){
		bIter->polygonsInit();
		polygon = bIter->nextPolygon();

		while(polygon){
			bIter2 = brushes.begin();  
			takeIt=true;
			while(bIter2!=bEnd|| !bIter2->isOpaque()){
				if(bIter2==bIter){
					bIter2++;
					continue;
				}
				if(bIter2->subPolygon(polygon)){
					takeIt=false;
					break;
				}
				bIter2++;
			}
			if(takeIt)
				polygonList->push_front(polygon);
			polygon=bIter->nextPolygon();
		}    
		++bIter;
	}

}

void CEntity::appendStructural(list<CPolygon *> *polygonList){
	vector < CBrush >::iterator bIter, bEnd;
	bIter          = brushes.begin();
	bEnd           = brushes.end();

	while(bIter != bEnd){
		if(bIter->isStructural())
			bIter->appendYourPolygons(polygonList);
		//polygonList->push_back(bIter->getLargerPolygon());

		++bIter;
	}
}

void CEntity::appendStructuralAndVisible(list<CPolygon *> *polygonList){
	vector < CBrush >::iterator bIter, bEnd;
	bIter          = brushes.begin();
	bEnd           = brushes.end();

	while(bIter != bEnd){
		if(bIter->isStructural())
			bIter->appendYourVisiblePolygons(polygonList);
		//polygonList->push_back(bIter->getLargerPolygon());

		++bIter;
	}
}

CPolygon *CEntity::getLargerPolygon()
{
	vector<CPolygon *> largOnes;

	vector < CBrush >::iterator bIter, bEnd;
	bIter          = brushes.begin();
	bEnd           = brushes.end();

	int larger     = -1;
	float howLarge = -huge;
	int index      = 0;  
	float area;


	while(bIter != bEnd){
		largOnes.push_back(bIter->getLargerPolygon());
		area = largOnes.back()->area();    

		if( area > howLarge){
			howLarge = area;
			larger = index;
		}
		++bIter;
		++index;
	}

	if(larger == -1)
		return 0;
	return largOnes[larger];
}

string CEntity::asString(){
	string rp="{";
	for(map<string,string>::iterator i=props.begin();i!=props.end();i++){
		rp+=" \"";
		rp+=i->first;
		rp+="\" \"";
		rp+=i->second;
		rp+="\"";
	}
	rp+="}";
	return rp;
}


CEntity::~CEntity(void)
{
};

void CEntity::setLeakEntity(CShaderFlags *leakFlags)
{
	vector<CBrush>::iterator bIter, bend;
	bIter = bBegin();
	bend = bEnd();
	while(bIter!=bend){
		bIter->setLeakBrush(leakFlags);
		++bIter;
	}
}

bool CEntity::isMainEntity()
{
	return getProperty("classname") == "worldspawn";
}

bool CEntity::isPortalEntity()
{
	return getProperty("classname") == "portal";
}


bool CEntity::isLeakEntity(){
	return getProperty("classname") == "huge_leak_detect_entity";
}

bool CEntity::isModelEntity(){ 
	return (!isPortalEntity() && !isMainEntity() && getBrushesNo() && !isLeakEntity());
}

void CEntity::moveEntity(CVector3 &middle){
	vector < CBrush >::iterator brushIter,brushEnd;
	brushIter = bBegin();
	brushEnd = bEnd();
	while(brushIter!=brushEnd){      
		brushIter->moveBrush(middle);
		++brushIter;
	}
}


