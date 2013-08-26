/***************************************************************************
                          loader.cpp  -  description
                             -------------------
    begin                : Sat Jun 8 2002
    copyright            : (C) 2002 by  Vassili Tenente
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
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <math.h>

#ifdef MEMPROF
#include "mmgr.h"
#endif

#include "loader.h"
#include "plane.h"
#include "entity.h"
#include "tokenizer.h"
#include "planemanager.h"
#include "epsilon.h"

//#define DEBUG 1

using namespace std;

CMapLoader::CMapLoader(){

}

CMapLoader::CMapLoader(string & filep, bool load){
    filepath = filep;
        
    if (load)
        reload();
}

CMapLoader::~CMapLoader(){
}

int CMapLoader::countEntities(void){
    return entities.size();
}

CEntity*  CMapLoader::getEntity(const int &index){
   int entitiesSize =entities.size();
   if(index>=entitiesSize || index<0)
     return 0;
   eIter = entities.begin();
   eEnd = entities.end();
   int i=0;
   while(eIter!=eEnd){
     if(i==index)
       return *eIter;
     ++i;
     ++eIter;
   }
   return 0;
}


CEntity* CMapLoader::getPlayerStart()
{
    int startInfoId=getEntityIdByPropAndVal("classname", "info_player_start");
    if(startInfoId==-1)
        startInfoId=getEntityIdByPropAndVal("classname", "info_player_deathmatch");
    if(startInfoId==-1){
        cout << "not info start found exiting"<<endl;
        return 0;
    }
    return getEntity(startInfoId);
}
list < CEntity *> &CMapLoader::getEntities(void)
{
    return entities;
}

int CMapLoader::getEntityIdByPropAndVal(string propName, string propVal)
{
    int i=0;
    eIter = entities.begin();
    eEnd = entities.end();
    while(eIter!=eEnd){
      if((*eIter)->getProperty(propName) == propVal)
        return i;
      ++eIter;
      i++;
    }
    return -1;
}

list<CEntity*>* CMapLoader::getListWithEntities(string propName, string propVal){
    list<CEntity*> *re=new list<CEntity*>();
    eIter = entities.begin();
    eEnd = entities.end();
    while(eIter!=eEnd){
      if((*eIter)->getProperty(propName) == propVal)
        re->push_back(*eIter);
      ++eIter;
    }
    
    if(re->size()!=0)
        return re;
    delete re;
    return 0;
}

CEntity * CMapLoader::getEntity(string propL, string propR)
{
    return (*this).getEntity(getEntityIdByPropAndVal(propL, propR));
}

bool CMapLoader::load(string & fileP)
{
  filepath = fileP;
  return reload();
}

void CMapLoader::setFilePath(string fileP)
{
  filepath = fileP;
}

string CMapLoader::hugeEntityText()
{
    string rs = "";
    rs+= "\n{\n";
    rs+= "\"classname\" \"huge_leak_detect_entity\"";
    rs+= "\n{\n";
    
    //plane 1
    rs+= " ( 1 -1 -";
    rs+=hugeText;
    rs+=" ) ( 1 -129 -";
    rs+=hugeText;
    rs+=" ) ( 129 -1 -";
    rs+=hugeText;
    rs+=" ) common/leak -1 1 0 1 -1 //TX1\n";
    
    //plane2
    rs+= " ( 1 1 ";
    rs+=hugeText;
    rs+=" ) ( 1 129 ";
    rs+=hugeText;
    rs+=" ) ( 129 1 ";
    rs+=hugeText;
    rs+=" ) common/leak -1 1 0 1 1 //TX1\n";
    
    //plane 3
    rs+= " ( -1 ";
    rs+=hugeText;
    rs+=" 1 ) ( -1 ";
    rs+=hugeText;
    rs+=" 129 ) ( -129 ";
    rs+=hugeText;
    rs+=" 1 ) common/leak -1 1 0 -1 1 //TX1\n";


    //plane 4
    rs+= " ( -";
    rs+=hugeText;
    rs+=" -1 1 ) ( -";
    rs+=hugeText;
    rs+=" -1 129 ) ( -";
    rs+=hugeText;
    rs+=" -129 1 ) common/leak -1 1 0 -1 1 //TX1\n";

    //plane 5
    rs+= " ( 1 -";
    rs+=hugeText;
    rs+=" 1 ) ( 1 -";
    rs+=hugeText;
    rs+=" 129 ) ( 129 -";
    rs+=hugeText;
    rs+=" 1 ) common/leak -1 1 0 1 1 //TX1\n";

    //plane 6
    rs+= " ( ";
    rs+=hugeText;
    rs+=" 1 1 ) ( ";
    rs+=hugeText;
    rs+=" 1 129 ) ( ";
    rs+=hugeText;
    rs+=" 129 1 ) common/leak -1 1 0 1 1 //TX1\n";


    rs+= "\n}\n";
    rs+= "\n}\n";    
    return rs;            
}

bool CMapLoader::load()
{
    string s;
    string all;
    int depth = 0;
    bool mustGet = true;
    bool as = true;
    string token;
    
    
    ifstream in(filepath.c_str());
    
    while (getline(in, s))
        all += "\n" + s;
    in.close();

    //this was for old leak detection
    //all += hugeEntityText();
    
    CTokenizerNoComments tokenizer(all);

    //Entity* ne=new Entity();
    CBrush nb(true);
    bool oldFormat;
    bool itIsBrush=true;
    while (as) {

        if (mustGet) {
            as = tokenizer.nextToken();
            token = tokenizer.getToken();
        } else
            mustGet = true;

        if (token == "{") {
            ++depth;

            if (depth == 1) {
                entities.push_back(new CEntity());
                //entities.back()->setEntityId(entities.size()-1);
#ifdef DEBUG
                cout << n << endl;
#endif
            } else if (depth == 2) {
                entities.back()->addBrush(nb);
            }
#ifdef DEBUG
            cout << "cc" << endl;
#endif

        } else if (token == "}") {
            --depth;
            //new
            if (depth == 0) {
                continue;       /*go to next entity */
            }
            //new
        } else if (token == "(") {
            //if (((depth == 2)&&oldFormat) || ((depth == 3)&&!oldFormat)) {
            if (depth >= 2) {
#ifdef DEBUG
                cout << "parsing at depth 2" << endl;
#endif
               //if(itIsBrush)
                if(!(*this).addPlaneToLastBrush(tokenizer, mustGet, token))
                {
                  exit(0);
                }
               /*else{
                 //it is patch
                 if(!(*this).addPatchSkata(tokenizer, mustGet, token))
                 {
                   exit(0);
                 }
               }*/
                //(*this).addPlaneToMain( tokenizer );
            }
        } else {
            if (depth == 1) {
                if (mustGet) {
                    as = tokenizer.nextToken();
                    if (!as) {
                        cout << " finished in undefined token..." << endl;
                        return false;
                    }
                } else {
                    mustGet = true;
                }
                entities.back()->setProperty(token,
                                                          tokenizer.
                                                          getToken());
#ifdef DEBUG
                cout << "setting at entity " << entities.size() -
                    1 << "\n" << token << "=" << tokenizer.getToken() << endl;
#endif
            }
            else if(depth==2){
              oldFormat=true;
              itIsBrush=true;
              if(token=="patchDef"){
                itIsBrush=false;  
                oldFormat=false;
              }
              else if(token=="brushDef"){
                itIsBrush=true;
                oldFormat=false;
              }
            }
        }
    }


    //now put the shaders parameters parameters
    shaderLoader.setShadersToRead(&shaders);
    shaderLoader.loadShaders(false);

    //eos edo
    return true;
}



bool CMapLoader::addPatchSkata(CTokenizerNoComments & tokenizer,
                                    bool & mustMoveOn, string & token)
{
  return true;
}

bool CMapLoader::readFloat(float &f, CTokenizerNoComments &tokenizer)
{
  if (!tokenizer.nextToken()) {
        return false;
  }
  sscanf(tokenizer.getToken().c_str(), "%f", &f);
  //f = atof(tokenizer.getToken().c_str());
  return true;
}

bool CMapLoader::readInt(int &i, CTokenizerNoComments &tokenizer)
{
  if (!tokenizer.nextToken()) {
        return false;
  }
  i = atoi(tokenizer.getToken().c_str());
  return true;
}

//this reads a vector3f x y z 
bool CMapLoader::readVector3( CVector3 &point, CTokenizerNoComments &tokenizer)
{
    if(!readFloat(point.x,tokenizer))
      return false;
    if(!readFloat(point.z,tokenizer))
      return false;
    point.z=-point.z;
    if(!readFloat(point.y,tokenizer))
      return false;
    return true;
}

void CMapLoader::addShader(const string &shaderName){
      if(shaders.find(shaderName)==shaders.end()){
        CShaderFlags *newShader = new CShaderFlags();
        newShader->setShaderId(shadersNames.size());
        shadersNames.push_back(shaderName);
        shaders[shaderName] = newShader;
      }
      
      entities.back()->getLastBrush().getLastBrushPlane().setShader(shaders[shaderName]);
}

bool CMapLoader::addPlaneToLastBrush(CTokenizerNoComments & tokenizer,
                                    bool & mustMoveOn, string & token)
{

//hear will be the three points
    /*CVector3 points[3];
    readVector3(points[1],tokenizer);
    tokenizer.nextToken();
    tokenizer.nextToken();
    readVector3(points[0],tokenizer);
    tokenizer.nextToken();
    tokenizer.nextToken();
    readVector3(points[2],tokenizer);
    tokenizer.nextToken();
    //tokenizer.nextToken();
cout <<"points[0].x="<<points[0].x<<endl;
cout <<"points[0].y="<<points[0].y<<endl;
cout <<"points[0].z="<<points[0].z<<endl;*/
    //hear will be the three points
    CVector3 points[3];

    readVector3(points[1],tokenizer);
    if (!tokenizer.nextToken()||!tokenizer.nextToken()) {
        return false;
    }
    readVector3(points[0],tokenizer);
    if (!tokenizer.nextToken()||!tokenizer.nextToken()) {
        return false;
    }
    readVector3(points[2],tokenizer);
    if (!tokenizer.nextToken()) {
        return false;
    }


    /*if(!||
       !readVector3(points[0],tokenizer)||
       !readVector3(points[2],tokenizer)){
      cout << "problem in three points waiting at parsing"<<endl;
      return false;
    }*/
    //this must be the close parenthesis ")" so we ignore it
    //tokenizer.getToken();

    CPlane *itsPlane;
    itsPlane = planeManager.getPlane(planeManager.addPlane(points));
    
    entities.back()->getLastBrush().addBrushPlane();    //add the new brush plane to last brush
    
    entities.back()->getLastBrush().getLastBrushPlane().setPlane(itsPlane);
    
#ifdef DEBUG
    cout << " going to add at entity " << (entities.size() -1) << " at brush " <<
        (entities[(entities.size() - 1)].getBrushesNo() - 1) << endl;

    for (int i = 0; i < 3; i++)
        cout << "point " << i << ": \n" << "x=" << points[i].
            x << endl << points[i].y << endl << points[i].z << endl;
#endif

    if (!tokenizer.nextToken()) {
        return false;
    }

    string nowToken = tokenizer.getToken();
    //new file
    if(nowToken=="("){
      //cout <<"Brush primitives file"<<endl;
      tokenizer.nextToken();
      CVector3 newTextP[2];
      readVector3(newTextP[0], tokenizer);
      if (!tokenizer.nextToken()||!tokenizer.nextToken())
        return false;
      readVector3(newTextP[1], tokenizer);
      if (!tokenizer.nextToken()||!tokenizer.nextToken()||!tokenizer.nextToken())
        return false;
      //cout << "shadername="<<tokenizer.getToken()<<endl;
      addShader(tokenizer.getToken());
    }
    //old File
    else{
      addShader(nowToken);
      float xoff;
      readFloat(xoff,tokenizer);
      entities.back()->getLastBrush().getLastBrushPlane().setXOff(xoff);

      float yoff;
      readFloat(yoff,tokenizer);
      entities.back()->getLastBrush().getLastBrushPlane().setYOff(yoff);
      float rot;
      readFloat(rot,tokenizer);
      entities.back()->getLastBrush().getLastBrushPlane().setRotation(rot);

      float xscale;
      readFloat(xscale,tokenizer);
      entities.back()->getLastBrush().getLastBrushPlane().setScaleX(xscale);

      float yscale;
      readFloat(yscale,tokenizer);
      entities.back()->getLastBrush().getLastBrushPlane().setScaleY(yscale);
    }

    //here to see if it must be splited
    if (!tokenizer.nextToken()) {
        cout << "bad format pall( ton ipia )" << endl;
        return false;
    }

    token = tokenizer.getToken();

#ifdef DEBUG
    cout << "TTHIS FUCKING TOKEN IS " << token << endl;
#endif
    if (token != "(" && token != "}") {

        //int isFinally = atoi(token.c_str());
        //int bit1 = 0x8000000;
        int bit1 = atoi(token.c_str());
        entities.back()->getLastBrush().getLastBrushPlane().setBitValue1(bit1);

        if (!tokenizer.nextToken()) {
            cout << "missing the BIT 2" << endl;
            return false;
        }
        int bit2 = atoi(tokenizer.getToken().c_str());;
        entities.back()->getLastBrush().getLastBrushPlane().setBitValue3(bit2);

        if (!tokenizer.nextToken()) {
            cout << "missing the BIT 3" << endl;
            return false;
        }
        int bit3 = atoi(tokenizer.getToken().c_str());;
        entities.back()->getLastBrush().getLastBrushPlane().setBitValue3(bit3);

        //to theoro se epipedo brush
#if 1
        if (bit1 != 0){
            entities.back()->getLastBrush().setStructural(false);
            //shaders[nowToken] is the pointer to the current brushplane shader flags
        }
        else{
            entities.back()->getLastBrush().setStructural(true);
        }
#endif

        mustMoveOn = true;
    } else
        mustMoveOn = false;

    //here the textMatrixe of the brushBlane we just add and is being calculated
    entities.back()->getLastBrush().getLastBrushPlane().
        calcTextureMatrix();
    return true;
}


CEntity * CMapLoader::getWorldEntity(void){
    int id = getEntityIdByPropAndVal("classname", "worldspawn");
    assert(id!=-1);
    
    if(id==-1){
        cout << "ERROR:failed to load world entity possible corrupted"<<endl;
        exit(0);
    }

    return getEntity(id);
}

bool CMapLoader::reload(){
    planeManager.freePlanes();
    shaderLoader.freeShaders();
    shaders.clear();
    freeEntities();
    return load();
}

void CMapLoader::freeEntities(){
  //vector < CEntity *>::iterator eIter, eEnd;
  eIter = entities.begin();
  eEnd = entities.end();
  CEntity *ent;
  while(eIter!=eEnd){
    ent = *eIter;
    if(ent)
      delete ent;      
    ++eIter;    
  }
  entities.clear();
}

string &CMapLoader::getFilePath()
{
  return filepath;
}

void CMapLoader::deleteEntity(CEntity *e){
    eIter = find(entities.begin(), entities.end(), e);
    delete *eIter;
    entities.erase(eIter);
}

