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

#ifndef _FLOODGATE_H_
#define _FLOODGATE_H_

#include <string>
#include <stack>
#include <vector>
#include <list>

#include "vector.h"

using namespace std;

class CPolygon;
class CBspLeaf;
class CAreaPortalMother;
class CArea;

class CFloodgate {
public:
	CFloodgate();
	void setInitialValues(vector<CBspLeaf *> &sLeafs, 
		vector<CPolygon *> &portals,
		string &fileP, CVector3 &playerStart);

	bool firstBspFloodPlace();
	void secondBspFloodPlace();

	int getAreaNo(){ return areaNo; }
	void addAreaPortalsToAreas(vector<CArea *> &areas);
private:
	vector<CBspLeaf *>startLeafs;
	vector<CAreaPortalMother *> areaPortalMothers;
    vector<CPolygon *> mPortals;
	int portalPNo;

	CVector3 startPoint;

	string filePath;
	int areaNo;

	bool findBPPolygons();
	bool findBPPolygons2();  
	void alertForLeaking2(stack <CBspLeaf *> &leakPath);
	void alertForLeaking(CBspLeaf *leakLeaf);  
	void findAreas();
};

#endif 


