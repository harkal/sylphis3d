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

#ifndef _LOADER_H_
#define _LOADER_H_

#include <string>
#include <vector>
#include <list>
#include "shader.h"
#include "vector.h"
#include "planemanager.h"

using namespace std;

class CEntity;
class CPlane;
class CTokenizerNoComments;
class CPlaneManager;


class CMapLoader {
	list<CEntity *> entities;

	CShaderLoader shaderLoader;
	CPlaneManager planeManager;
	map<string, CShaderFlags *> shaders;
	vector<string>  shadersNames;
	string filepath;

	list<CEntity *>::iterator eIter, eEnd;
public: 
	CMapLoader();
	CMapLoader(string & fileP, bool load);
	~CMapLoader();

	bool load(string & filePath);
	bool load();
	void setFilePath(string fileP);
	string &getFilePath();
	bool reload();
	int countEntities(void);

	CEntity* getEntity(const int &index);
	list < CEntity *> &getEntities(void);

	//Entity& getMainEntity();
	void freeEntities();
	//must be at entity map propL=propR eg "classname"="worldspawn"
	int getEntityIdByPropAndVal(string propL, string propR);
	CEntity* getEntity(string propL, string propR);
	//this is the entity with classname worldspawn
	CEntity* getWorldEntity(void);
	CEntity* getPlayerStart();
	//vector<Polygon> getPolygonsOfWorld();
	CPlaneManager *getPlaneManager(){ return &planeManager; };


	list<CEntity*>* getListWithEntities(string propName, string propVal);

	CShaderLoader &getShaderLoader(){ return shaderLoader; }

	vector<string>::iterator sBegin(){return shadersNames.begin();};
	vector<string>::iterator sEnd(){return shadersNames.end();};
	size_t countShaders(){ return shadersNames.size(); }
	list<CEntity *>::iterator entityBegin(){return entities.begin();}
	list<CEntity *>::iterator entityEnd(){return entities.end();}

	void deleteEntity(CEntity *e);
private:  
	bool addPlaneToLastBrush(CTokenizerNoComments & tokenizer,
		bool &mustMoveOn, string & token);
	bool addPatchSkata(CTokenizerNoComments & tokenizer,
		bool &mustMoveOn, string & token);

	string hugeEntityText();
	bool readVector3( CVector3 &point, CTokenizerNoComments &tokenizer);
	bool readFloat(float &f, CTokenizerNoComments &tokenizer);
	bool readInt(int &i, CTokenizerNoComments &tokenizer);
	void addShader(const string &shaderName);
};

#endif // END OF _LOADER_H  ///:~
