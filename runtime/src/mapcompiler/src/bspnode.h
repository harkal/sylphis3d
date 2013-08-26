/***************************************************************************
                          bspnode.h  -  description
                             -------------------
    begin                : Mon Jun 17 2002
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
#ifndef _BSPNODE_
#define _BSPNODE_

#include <vector>
#include <list>

#include "shader.h"
#include "brushplane.h"
#include "boundbox.h"

using namespace std;

class CPolygon;
class CPortal;
class CBrush;
class CEntity;

class CBspNode{
public:
	CBspNode();
	CBspNode(list<CPolygon *> *polys, CBrushPlane *sp);
	~CBspNode();

	void freePolygons();
	list<CPolygon *> *getPolygons();

	void addPolygon(CPolygon *polygon);
	void addPolygonWithOutBound(CPolygon *polygon);
	void addPolygonBound(CPolygon *polygon);
	void addBoundBox(const CAABoundBox &bbox);

	list<CPolygon *>::iterator pBegin();
	list<CPolygon *>::iterator pEnd();

	void addPolygons(list<CPolygon *> *pols);
	bool isBuggyBound();

	void setSplitBrushPlane(CBrushPlane *sp)
	{
		splitPlane=sp;
	};
	CPlane *getSplitPlane(){ return splitPlane->getPlane(); };
	CBrushPlane *getSplitBrushPlane(){ return splitPlane; }

	int getPortalNo(){return splitPlane->getShader()->isPortal(); };

	list<CPolygon *>::iterator polygonsBegin();
	list<CPolygon *>::iterator polygonsEnd();
	void erasePolygon(list<CPolygon *>::iterator &i){i = polygons.erase(i);--i;}
	CVector3 getMiddlePoint();

	void pushPolygon(CPolygon *p){ polygons.push_back(p);}
	CAABoundBox &getBounds(){return bounds;}

	void printBounds();
private:
	list<CPolygon *> polygons;

	CAABoundBox bounds;
	CBrushPlane *splitPlane;

	bool bounded;
};

class CBspLeaf : public CBspNode {
public:
	CBspLeaf():CBspNode(){
		opaque = false;
		leak   = false;
		visible= false;
		area   = 0;
		index  = -1;
		depth = 0;
		areaPortalsReach=0;
	}
	~CBspLeaf();

	bool isTrueLeaf();
	int getDepth(){ return depth; }
	void setDepth(const int d){depth = d;}
	int getArea(){ return area; }
	void setArea(const int &a){ area = a; }
	bool isOpaque(){ return opaque; }
	void setOpaque(bool op){ opaque = op; }
	bool isVisible(){ return visible; }
	void setVisible( bool v ){ visible = v; }

	void addDetailBrush(CBrush *brush,const int brushId, bool writeBounds);
	void addStructuralBrush(CBrush *brush, const int brushId);
	void addPortal(CPortal *portal);

	bool isLeak(){ return leak; }
	void setLeak(bool l){ leak = l; }

	//these one clears the portal polygons
	void addLeafPolygons(list<CPolygon *> *pols);
	void addLeafPolygonsKeepKeep(list<CPolygon *> *pols);

	list<CPortal *> &getPortals();

	void addLeaf(CBspLeaf *leaf);
	list<CPortal *>::iterator portalsBegin();
	list<CPortal *>::iterator portalsEnd();

	vector<int>::iterator sbBegin(){ return sBrushes.begin(); }
	vector<int>::iterator sbEnd(){ return sBrushes.end(); }    
	vector<CBrush *>::iterator bBegin(){ return brushes.begin(); }
	vector<CBrush *>::iterator bEnd(){ return brushes.end(); }
	vector<CBrush *>::iterator abBegin(){ return allBrushes.begin(); }
	vector<CBrush *>::iterator abEnd(){ return allBrushes.end(); }
	vector<CBspLeaf *>::iterator oLeafsBegin(){return otherLeafs.begin(); }
	vector<CBspLeaf *>::iterator oLeafsEnd(){return otherLeafs.end(); }
	void setIndex(const int in){index = in;}
	int getIndex(){ return index; }
	void printStructuralBrushes();
	void appendYourBrushes(vector<CBrush *> &abrushes, CEntity* entity);
private:
	vector<CBrush *> brushes;
	vector<int> sBrushes;
	list<CPortal *> portals;
	vector<CBrush *> allBrushes;
	bool opaque;
	//used at flooding fo leak check
	bool leak;
	//used for flooding
	bool visible;
	int area;
	int index;

	//used for finding less leak path
	int depth;
	bool *areaPortalsReach;  
	vector<CBspLeaf *> otherLeafs;
};

#endif


