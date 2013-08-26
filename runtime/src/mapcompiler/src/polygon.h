/***************************************************************************
                          polygon.h  -  description
                             -------------------
    begin                : Fri Jun 7 2002
    copyright          : (C) 2002 by Vassili Tenente
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
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <cassert>
#include <list>
#include "plane.h"
#include "brushplane.h"
#include "epsilon.h"
#include "shader.h"
#include "boundbox.h"

using namespace std;

class CPolygon {
public:
	typedef vector<CVertex> VertsVector;
	
	CPolygon();
	CPolygon(const VertsVector &vertices);

	//CPolygon(const CPolygon & poly);
	~CPolygon(void){};

	vector < CVertex >::iterator vBegin(){return verts.begin();};
	vector < CVertex >::iterator vEnd(){return verts.end();};
	vector < CVertex >::reverse_iterator vrBegin(){return verts.rbegin();};
	vector < CVertex >::reverse_iterator vrEnd(){return verts.rend();};

	void setPolBounds(const CAABoundBox &box){ mBounds = box; };
	const CAABoundBox &getPolBounds() const { return mBounds; };

	void setBrushPlane(CBrushPlane *p){ 
		mBrushPlane = p;
		if(mBrushPlane)
			setPlane(mBrushPlane->getPlane());
	};
	CBrushPlane *getBrushPlane() const { return mBrushPlane;};

	CPlane::eSide planeSide(const CPlane &p, float eps=PLANES_ON_EPSILON) const;

	int getBrushId()const{return mBrushPlane->getFather()->getBrushId();}
	int getShaderId() const { 
		return mBrushPlane->getShader()->getShaderId();
	}

	void setStructural(bool s){
		mBrushPlane->getShader()->setStructural(s);
	}

	void setVisible(bool b){
		mBrushPlane->setVisible(b);
	}

	void setOpaque(bool s){
		mBrushPlane->getShader()->setOpaque(s);
	}

	void setKeep(bool k){
		mBrushPlane->getShader()->setKeep(k);
	}

	void setPortal(int portal){
		mBrushPlane->getShader()->setPortal(portal);
	}

	void setPrior(bool prior){
		mBrushPlane->getShader()->setPrior(prior);
	}

	bool isStructural() const {
		return mBrushPlane->getShader()->isStructural();
	}

	bool isOpaque() const {
		assert(mBrushPlane);
		return mBrushPlane->getShader()->isOpaque();
	}

	bool isPrior() const {
		assert(mBrushPlane);
		return mBrushPlane->getShader()->isPrior();
	}

	bool isKeep() const {
		return mBrushPlane->getShader()->isKeep();
	}

	int isPortal() const {
		return mBrushPlane->getShader()->isPortal();
	}

	int portalNo() const {
		return mBrushPlane->getShader()->isPortal();
	}

	bool isLeak() const { return mBrushPlane->getShader()->isLeak(); }

	void boundPolygon();
	void reBound();
	VertsVector& getVertices(void);
	int countVertices(void) const;
	CVertex & getVertex(const int &index);

	bool addVertex(const CVertex & v);
	void addVertices(const VertsVector &vertices);
	void addVertex(const CVector3 & v);
	void setVertices(const VertsVector &vertices);

	void appendTessPolygons( list<CPolygon *> *polyList );
    void getBoundBrokenPolygons(vector<CAABoundBox> &bounds, list<CPolygon *> *polyList );
	/**
	*  @param plane      the plane to split by
	*	@returns the two polygons that are genarated of splitting this Polygon by the plane
	*   first returns the polygon whiches points are at distanse <=0
	*/
	CPlane::eSide splitByPlane(CPlane *plane, CPolygon * f, CPolygon * b,
		float eps = PLANES_ON_EPSILON);
	float area() const;
	/*splits it making it equal to the negative side returning the positive */
	CPolygon splitIt(CPlane *plane, float eps = PLANES_ON_EPSILON);

	/*given a Plane if that plane splits the CPolygon
	it returns the vertices on the sides of the CPolygon
	where the planes is going through
	*/
	VertsVector whereCollidesWithPlane(CPlane & plane) const;

	bool isTooSmall(void) const;

	void moveOnY(float onY);
	void moveOnZ(float onZ);
	void moveOnX(float onX);

	void debug();
	void merge();

	//works only for Triangles
	static void smerge(CPolygon *p1, CPolygon *p2);
	static CPolygon hugePolygon(CPlane &p, float howHuge = huge);   
	static VertsVector removeCollinear(VertsVector &vertices);

    bool isInBound(const CAABoundBox &bound) const;

	CPlane* getPlane(){
		return mPlane;
	};
	const CPlane &getPlane() const {
		return *mPlane;
	}
	void setPlane(CPlane *p){ mPlane = p; };
	CPlane getNewPlane(){return CPlane(verts);}    
	void print();

	VertsVector::iterator rBegin(){ return verts.begin(); }
	VertsVector::iterator rEnd(){ return verts.end(); }
	void translate(const CVector3 &v);
	float outCycleR();
	CVector3 getMassCenter();
    void swapVertices();

protected:
	VertsVector verts;
	CAABoundBox mBounds;
	CBrushPlane *mBrushPlane;
	CPlane *mPlane;

};

#endif


