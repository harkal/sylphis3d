/***************************************************************************
                          brush.h  -  description
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
#ifndef _BRUSH_H
#define _BRUSH_H

#include <list>

#include "boundbox.h"
#include "brushplane.h"

using namespace std;

class CAABoundBox;

class CBrush {
public:
	CBrush(bool fb=false);
	CBrush(CBrush *fatherBrush, bool fb=false);
	~CBrush();

	void addBrushPlane(CBrushPlane *p);
	void addBrushPlane(CBrushPlane & p, CPolygon *pol);
	void CBrush::addBrushPlane(CBrushPlane *p, CPolygon *pol);

	CBrushPlane* CBrush::addBrushPlane();

	int getBrushPlanesNo(void) const;

	list<CBrushPlane>::iterator planesBegin(){return planes.begin();};
	list<CBrushPlane>::iterator planesEnd() {return planes.end();};

	list < CBrushPlane > &getBrushPlanes(void);
	CBrushPlane & getLastBrushPlane(void);

	void freeIt();
	void createPolygons();
	float volume();

	CPolygon * getLargerPolygon();
	void appendYourPolygons( list<CPolygon *> *polyList);
	void appendYourVisiblePolygons( list<CPolygon *> *polyList);
	bool isStructural(){return structural;}

	void setStructural(bool isS){structural = isS;}
	void setOpaque( bool op ){opaque = op;};
	bool isOpaque(){ return opaque; };
	void markVisibleSides();

	void boundBrush();
	void reBoundBrush();
	const CAABoundBox &getBounds() const {return mBounds;}
	CBrushPlane *getLastBPPointer();

	void setLeakBrush(CShaderFlags *leakFlag);

	void setReachLeaf(bool c){ rLeaf =c; };
	bool reachLeaf(){ return rLeaf; };
	bool isFromBegin(){ return fromBegin; }
	void setBrushId(const int bId){ brushId = bId; }
	int getBrushId(){ return brushId; }
	void moveBrush(CVector3 &middle);
	bool areAllVertsIn(CPolygon *p);
	bool isInOrOn(CVector3 &v);

	/// !!WARNING 
	/// This function changes the vertices of the polygon p
	/// with others less or equal to its depending which of them are in the brush
	/// @returns true if a subpolygon does not exist-at this point the p vertices are not changed-
	/// ans false when there is a subpolygon and p becomes the subpolygon
	bool subPolygon(CPolygon *p);
	float maximize(vector< float > &out, vector<float> &in);

	///these are to iterate the polygons
	///to work the createPolygons must have called first
	void polygonsInit();
	CPolygon *nextPolygon();
	list<CBrushPlane>::iterator planeIter;
private:
	list<CBrushPlane> planes;
	bool structural;
	bool opaque;

	CAABoundBox mBounds;

	//this lets you know if this brush was created by splitBrush
	bool rLeaf;

	//this is to know which brush is from the begining
	bool fromBegin;
	int brushId;
};

#endif


