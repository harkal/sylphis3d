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
