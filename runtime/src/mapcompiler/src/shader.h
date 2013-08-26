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


