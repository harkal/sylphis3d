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

#ifndef _SHADER_H_
#define _SHADER_H_
#include <string>
#include <map>
#include <vector>
using namespace std;


class CShaderFlags {
public:
	CShaderFlags();
	CShaderFlags(bool opaque, bool structur, bool toKeep, bool pr, int por);

	void setOpaque(bool s);
	void setStructural(bool t);
	void setKeep(bool d);
	void setPortal(const int& pNo);

	bool isStructural();
	bool isOpaque();
	bool isPrior();
	bool isKeep();

	int isPortal();

	void setSmoothNormals(bool s);
	bool isSmoothNormals();

	void setTessSize(float t);
	float getTessSize();

	void setPrior(bool pr);

	bool isLeak();
	void setLeak(bool a);

	void printFlags();

	int getShaderId(){ return shaderId; }
	void setShaderId(int sId){ shaderId = sId; }
private :
	bool structural; //triggers to split at brushPlane
	bool opaque; //collision

	bool prior;          //if split first with it's polygon
	bool keep;           // keep it's polygons after all the things(placed to last structs         
	bool smoothNormals;
	float tessSize;		// Tessatelation of surfaces 
	int portal;          //if 0 no portal else keeps which portal it is , its id.

	//this is used for leaking check. always should be false
	bool leak;

	//this is for writting    
	int shaderId;
};

class CShaderLoader{
public:
	CShaderLoader(bool load=false);
	~CShaderLoader(){ if(whichWasRead)delete whichWasRead;};

	void setShadersToRead(map<string, CShaderFlags *> *sh);

	//this fuction load all the shaders of directory
	void loadShaders(bool readAll);

	//this one loads only the the ones that exist in shadersToLoad
	void loadShaders(map<string, CShaderFlags *> &shadersToLoad);

	void freeShaders();
private:
	string directory;
	map<string, CShaderFlags*> *shaders;

	char *extension;
	int extcharno;

	//to read them only once, keep the file it was read first
	map<string, string> *whichWasRead;

	void setUpShadersInfo();
	int selector(char *d);
	void CShaderLoader::readShaderFile(const char *fileName, bool readAll);    
};

#endif


