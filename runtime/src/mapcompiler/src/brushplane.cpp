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

#include "plane.h"
#include "brushplane.h"
#include "shader.h"
#include "polygon.h"

#ifdef MEMPROF
#include "mmgr.h"
#endif

#ifdef _NOINLINE
#include "brushplane.inl"
#endif

#ifndef M_PI
#define M_PI 3.141592654
#endif


CBrushPlane::CBrushPlane()
{        
    //split         = true;
    //transparent   = false;
    //prior         = false;
    //keep          = true;
    plane   = 0;
    polygon = 0;
    visible = false;
    brakeCounter = 1;
    merged = 0;
    father = 0;
    brushId = -1;
}

/*CBrushPlane::CBrushPlane(CShaderFlags &flags)
{
  polygon     = 0;
  plane       = 0;
  //prior       = flags.isPrior();
  //keep        = flags.isKeep();
}*/

float &CBrushPlane::getXOff(void)
{
    return offset[0];
}

float &CBrushPlane::getYOff(void)
{
    return offset[1];
}
float &CBrushPlane::getRotation(void)
{
    return rotation;
}

float &CBrushPlane::getScaleX(void)
{
    return scale[0];
}

float &CBrushPlane::getScaleY(void)
{
    return scale[1];
}

/*string & BrushPlane::getTexture(void)
{
    return texture;
}*/

void CBrushPlane::setXOff(float &xo)
{
    offset[0] = xo;
}

void CBrushPlane::setYOff(float &yo)
{
    offset[1] = yo;
}

void CBrushPlane::setRotation(float &rot)
{
    rotation = rot;
}

void CBrushPlane::setScaleX(float &sx)
{
    scale[0] = sx;
}

void CBrushPlane::setScaleY(float &sy)
{
    scale[1] = sy;
}


bool CBrushPlane::isVisible(){ return visible; }
void CBrushPlane::setVisible( bool v ){ visible = v; }

/*string BrushPlane::lowerCase(const string & a)
{
    string s = a;
    for (int i = 0; i < s.length(); i++) {
        s[i] = s[i] <= 'Z' && s[i] >= 'A' ? s[i] + ('a' - 'A') : s[i];
    }
    return s;
}*/


void CBrushPlane::setTextureMatrix(const CVector3 matrice[2])
{
    textMatrix[0] = matrice[0];
    textMatrix[1] = matrice[1];
}

void swap(CVector3 *v){
	float t = v->y;
    v->y = v->z;
    v->z = -t;
}

void CBrushPlane::calcTextureMatrix()
{
    assert(plane);
    //vector<CVector3> axis = textureAxis();
    vector < CVector3 > axis = plane->textureAxis();

    if (!scale[0])
        scale[0] = 1;
    if (!scale[1])
        scale[1] = 1;


    float angle, sinv, cosv;

// rotate axis
    if (rotation == 0) {
        sinv = 0;
        cosv = 1;
    } else if (rotation == 90) {
        sinv = 1;
        cosv = 0;
    } else if (rotation == 180) {
        sinv = 0;
        cosv = -1;
    } else if (rotation == 270) {
        sinv = -1;
        cosv = 0;
    } else {
        angle = rotation / 180 * M_PI;
        sinv = sin(angle);
        cosv = cos(angle);
    }

    int sv, tv;
    if ((int) axis[0][0])
        sv = 0;
    else if ((int) axis[0][1])
        sv = 1;
    else
        sv = 2;

    if ((int) axis[1][0])
        tv = 0;
    else if ((int) axis[1][1])
        tv = 1;
    else
        tv = 2;

    float ns, nt;
    for (int i = 0; i < 2; i++) {
        ns = cosv * axis[i][sv] - sinv * axis[i][tv];
        nt = sinv * axis[i][sv] + cosv * axis[i][tv];
        axis[i][sv] = ns;
        axis[i][tv] = nt;
    }

    //textMatrix.clear();
    //textMatrix.push_back(CVector3());
    //textMatrix.push_back(CVector3());
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 3; j++)
            textMatrix[i][j] = axis[i][j] / scale[i];

	swap(&textMatrix[0]);
	swap(&textMatrix[1]);
//  textMatrix[0][3] = offset[0];
//  textMatrix[1][3] = offset[1];
}

CVector3 & CBrushPlane::getXTextureMatrix()
{
    return textMatrix[0];
}

CVector3 & CBrushPlane::getYTextureMatrix()
{
    return textMatrix[1];
}


void CBrushPlane::setBitValue1(const int &b)
{
    b1 = b;
}
void CBrushPlane::setBitValue2(const int &b)
{
    b2 = b;
}
void CBrushPlane::setBitValue3(const int &b)
{
    b3 = b;
}



/*bool BrushPlane::goesFirst(){
    if(sType!=SplitNode::COMMON)
        return true;
    return false;
}*/

/*void BrushPlane::boundPlane()
{
    min.clear();
    max.clear();
    for (int i = 0; i < 3; i++) {
        min.push_back(10000.0f);
        max.push_back(-10000.0f);
    }
    if (!polygon)
        return;

    for (int i = 0; i < polygon->countVertices(); i++) {
        CVector3 v = polygon->getVertex(i).getRealCoor();
        for (int j = 0; j < 3; j++) {
            if (v[j] > max[j])
                max[j] = v[j];
            if (v[j] < min[j])
                min[j] = v[j];
        }
    }
}*/

CPolygon * CBrushPlane::getPolygon()
{ 
  return polygon; 
}

void CBrushPlane::setPolygon(CPolygon *pol)
{ 
  pol->setBrushPlane(this);
  polygon = pol; 
}

void CBrushPlane::setPlane(CPlane *pl)
{
  plane = pl;
}

void CBrushPlane::translate(const CVector3 &v){
	getPolygon()->translate(v);
	//getPlane()->translate(v);
}

void CBrushPlane::setShader(CShaderFlags *shaderFlags)
{
  shader = shaderFlags;
}

CShaderFlags * CBrushPlane::getShader()
{
  return shader;
}

bool CBrushPlane::isLeak(){ return shader->isLeak(); };

CBrushPlane::~CBrushPlane(void)
{
}

//increase brakecounter
void CBrushPlane::brake()
{
  ++brakeCounter;
}
//decrease brake counter
void CBrushPlane::touch()
{
  --brakeCounter;
}
    
bool CBrushPlane::canBeDetail(){
  return visible && (!brakeCounter);    
}

void CBrushPlane::merge(CPolygon *p){
  if(merged)
    CPolygon::smerge(merged, p);
  else
    merged = p;
  if(merged->countVertices()<3){
    merged=0;
  }
}

CPolygon *CBrushPlane::getMerged(){
  if(merged)
    return merged;
  return 0;
}

CBrushPlane *CBrushPlane::getFather(){
  if(father)
    return father;
  return this;
}

void CBrushPlane::setFather(CBrushPlane *f){
    father=f;
}

bool CBrushPlane::isFather(){
  if(father)
    return false;
  return true;
}

