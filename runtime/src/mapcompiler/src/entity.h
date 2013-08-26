/***************************************************************************
                          entity.h  -  description
                             -------------------
    begin                : Sat Jun 8 2002
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
#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <string>
#include <map>
#include <vector>

#include "brush.h"

using namespace std;

class CEntity {
public:
	CEntity();
	~CEntity();
	string getProperty(const string & prop);
	CVector3 getAsVector(const string & prop);
	void setProperty(const string & prop, const string & value);
	int getNoProperties();

	CPolygon *getLargerPolygon();
	void appendStructural(list<CPolygon *> *polygonList);
	void appendStructuralAndVisible(list<CPolygon *> *polygonList);

	void callCreatePolygons();
	void addBrush(CBrush & b);
	CBrush & getBrush(const int &id);
	vector < CBrush > &getBrushes(void);
	int getBrushesNo(void);
	CBrush & getLastBrush(void);
	string asString();
	bool isMainEntity();
	bool isPortalEntity();
	bool isModelEntity();
	bool isLeakEntity();

	CBrush &operator[] (const int index){
		return brushes[index];
	};
	string &operator[] (const string & key){
		return props[key];
	}

	vector<CBrush>::iterator bBegin(){ return brushes.begin(); }
	vector<CBrush>::iterator bEnd(){ return brushes.end(); }

	void setLeakEntity(CShaderFlags *leakFlags);
	void moveEntity(CVector3 &middle);
	void debug(void);
	void appendChopped( list<CPolygon *> *polygonList );
	void appendChoppedAndStructural( list<CPolygon *> *polygonList );
	void appendChoppedStructuralAndVisible( list<CPolygon *> *polygonList );
private:
	map < string, string > props;
	vector < CBrush > brushes;
};

#endif 
