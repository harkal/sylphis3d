/***************************************************************************
                          filterer.h  -  description
                             -------------------
    begin                : Mon Jul 7 2003
    copyright            : (C) 2003 by Vassili Tenente
    email                :   p98009@rainbow.cs.unipi.gr
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
#ifndef _FILTERER_H_
#define _FILTERER_H_

#include <list>
#include "plane.h"
#include "tree.h"
#include "boundbox.h"
#include "shader.h"

using namespace std;

class CBrush;
class CBspNode;
class CBspLeaf;
class CPlaneManager;
class CEntity;
class CBrushPlane;
class CShaderFlags;

class CFilterer{
public:
	CFilterer();
	~CFilterer(){	};
	void setInitialValues(CPlaneManager *pM, 
		CBTree<CBspNode *>::Node *rNode, CEntity *we, vector<CPolygon *> *pPolygons, const int polNo);
	void doFilteringForFirstBsp();
	void doFilteringForSecondBsp();
	CBTree<CBspLeaf *>::Node *filterPointIntoTree(CVector3 &point);
	void filterPortalsIntoTree();
	void filterPortalIntoTree(CBTree<CBspNode *>::Node *node, CPolygon *portal);
	void forceDetailBrushes();
	void filterFor2OnlyDetail();
	void filterFor2OnlyStructural();
	void mergeDetails(CBTree<CBspNode *>::Node *node);
	void findDetailAndMerge();

private:
	const static bool STRUCTURAL  = true; 
	const static bool DETAIL      = false; 

	CEntity *worldEntity;
	CBTree<CBspNode *>::Node *rootNode;

	CPlaneManager *pManager;

	vector<CPolygon *> *portalPolygons;
	int portalPNo;
	int portalCurrentlyFilter;

	//these variables are used with the filterBrushIntoTree fuction
	bool        writeBounds;
	CBrush      *filteringBrush;
	int         opaqueNo;
	int         brushSplits;
	CAABoundBox worldBounds;
	int         tinyBrushes;
	CVector3    worldMin, worldMax;
	CShaderFlags defaultShaderFlags;

	int filterBrushIntoTree(CBrush * brush, CBTree<CBspNode *>::Node * node);
	void filterBrushes(bool which);

	CPlane::eSide splitBrush(CBrush *brushToSplit, CBrush ** front, CBrush ** back, CBrushPlane *splitBrushPlane);
	CPlane::eSide brushMostlyOnSide(CBrush * brush, CPlane *plane);

	list<CBrush *> brushesBuffer;

	void clearBrushesBuffer();

	void filterAllBrushesIntoTree();
	bool firstBsp;     
};

#endif

