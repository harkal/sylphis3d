/***************************************************************************
                          polygon.cpp  -  description
                             -------------------
    begin                : Fri Jun 7 2002
    copyright            : (C) 2002 by  Vassili Tenente
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
#include <assert.h>
#include <iostream>

#ifdef MEMPROF
#include "mmgr.h"
#endif

#include "vertex.h"
#include "polygon.h"


using namespace std;

CPolygon::CPolygon(){
	mBrushPlane = 0;
    mPlane = 0;
}

#if 0
CPolygon::CPolygon(const CPolygon & poly){
    verts = poly.verts;
	mBrushPlane = poly.mBrushPlane;
    mPlane = poly.mPlane;
}
#endif

CPolygon::CPolygon(const VertsVector &vertices){
    verts = vertices;
	mBrushPlane = 0;
    mPlane = 0;
}

void CPolygon::boundPolygon(){
  mBounds.setBounds(verts[0].getRealCoor(),verts[0].getRealCoor());
  VertsVector::iterator v, vend;  
  v=verts.begin();
  vend = verts.end();
  ++v;
    while(v!=vend){
      mBounds.addPoint(v->getRealCoor());
      ++v;
    }
}

void CPolygon::reBound(){
	boundPolygon();
}

int CPolygon::countVertices(void) const {
    return verts.size();
}

CVertex & CPolygon::getVertex(const int &index)
{
    return verts[index];

}

CPolygon::VertsVector& CPolygon::getVertices(void)
{
    return verts;
}

/*BrushPlane& CPolygon::getPlane(){
    return *plane;
} */

bool CPolygon::isInBound(const CAABoundBox &bound) const {
    VertsVector::const_iterator i;
	for (i = verts.begin(); i != verts.end(); ++i){
        if(!bound.collide((*i).getRealCoor()))
            return false;
	}
    return true;
}

void CPolygon::swapVertices(){
    VertsVector::reverse_iterator vIter, vEnd;
    vIter = vrBegin();
    vEnd = vrEnd();
    VertsVector newVertices;
    while(vIter!=vEnd){
        newVertices.push_back(*vIter);
        ++vIter;
    }

    setVertices(newVertices);
}

/*
=============
Area
=============
*/
float CPolygon::area() const {
    int i;
    float total;

    total = 0;
    CVector3 d1, d2;
    int noVerts = verts.size();
    for (i = 2; i < noVerts; i++) {
        d1 = verts[i - 1].getRealCoor() - verts[0].getRealCoor();
        d2 = verts[i].getRealCoor() - verts[0].getRealCoor();

        total += 0.5 * (d1 ^ d2).length();
    }
    return total;
}


void CPolygon::setVertices(const VertsVector &vertices)
{
	verts = vertices;
}

CPlane::eSide CPolygon::planeSide(const CPlane &p, float eps) const {
	bool front = false;
	bool back = false;

	VertsVector::const_iterator i;
	for (i = verts.begin(); i != verts.end(); ++i){
		float d = p.distanseFromVertex(*i);
		if (d < -eps){
			if (front)
				return CPlane::CROSS;
			back = true;
		} else if (d > eps){
			if (back)
				return CPlane::CROSS;
			front = true;
		}
	}

	if (back){
		return CPlane::BACK;
	}
	if (front){
		return CPlane::FRONT;
	}
	return CPlane::ON;
}

CPlane::eSide CPolygon::splitByPlane(CPlane *plane, CPolygon * f, CPolygon * b,
                                   float eps)
{
    vector < float >dists(verts.size());
    vector < CPlane::eSide > sides( verts.size() );
    VertsVector::iterator vi, vend;

	f->setBrushPlane(mBrushPlane);
	b->setBrushPlane(mBrushPlane);
    f->setPlane(mPlane);
	b->setPlane(mPlane);

    int front = 0;
    int back = 0;
    int on = 0;

    int i;
    vend = verts.end();
    for (vi = verts.begin(), i = 0; vi != vend; vi++, i++) {
        float dist = plane->distanseFromVertex(*vi);
        dists[i] = dist;
        if (dist > eps) {
            sides[i] = CPlane::FRONT;
            front++;
        } else if (dist < -eps) {
            sides[i] = CPlane::BACK;
            back++;
        } else {
            sides[i] = CPlane::ON;
            on++;
        }
    }

    if (front == 0) {
        *b = *this;
        return CPlane::BACK;
    }
    if (back == 0) {
        *f = *this;
        return CPlane::FRONT;
    }

    dists.push_back(dists[0]);
    sides.push_back(sides[0]);
    vend = verts.end();
    
    for (vi = verts.begin(), i = 0; vi != vend; ++vi, ++i) {
        const CVector3 & v1 = vi->getRealCoor();
        if (sides[i] == CPlane::FRONT) {
            f->addVertex(v1);
        } else if (sides[i] == CPlane::BACK) {
            b->addVertex(v1);
        } else if (sides[i] == CPlane::ON) {
            f->addVertex(v1);
            b->addVertex(v1);
            continue;
        }

        if (sides[i + 1] == CPlane::ON || sides[i] == sides[i + 1])
            continue;

        const CVector3 & v2 = verts[(i + 1) % verts.size()].getRealCoor();
        float l = dists[i] / (dists[i] - dists[i + 1]);
        //NEW
        CVector3 m;
        CVector3 &normal = plane->getNorma();
        for (int j=0 ; j<3 ; j++)
		{	// avoid round off error when possible
			if (normal[j] == 1)
				m[j] = plane->getDistanse();
			else if (normal[j] == -1)
				m[j] = -plane->getDistanse();
			else
				m[j] = v1[j] + l*(v2[j]-v1[j]);
		}
        //NEW
        f->addVertex(m);
        b->addVertex(m);
    }
    return CPlane::CROSS;
}

//watch out bounds and this function together
//this fuction change the bounds of the polygon
CPolygon CPolygon::splitIt(CPlane *plane, float eps)
{
    CPolygon f, b;

    CPlane::eSide side = splitByPlane(plane, &f, &b, eps);

    if (side != CPlane::FRONT)
        verts = b.getVertices();
    
//    if(bounds)
//      reBound();
      
    return f;
}

CPolygon CPolygon::hugePolygon(CPlane & plane, float howHuge)
{
    CVector3 vector2, middlePoint;
    CPolygon p;


	const CVector3 &normal = plane.getNorma();
	float max = -10000000.0;
	int x = -1;
	for(int i = 0 ; i < 3; ++i) {
		float v = fabs(normal[i]);
		if (v > max){
			x = i;
			max = v;
		}
	}
	assert(x != -1);
		
	CVector3 vector1(0.0, 0.0, 0.0);
	switch (x){
	case 0:
	case 1:
		vector1[2] = 1;
		break;		
	case 2:
		vector1[0] = 1;
		break;		
	}

    //and project his vector on the plane
    vector1 = (plane.projectVector(vector1)).normalized();
    //vector2 is a vector on the plane vert to both norma and vector1( cross product )
    vector2 = (vector1 ^ plane.getNorma()).normalized();

    //this is where collide the norma of the plane with the plane
    middlePoint = plane.getNorma() * plane.getDistanse();

    CVector3 a1;
    a1 = (middlePoint + vector1 * howHuge);
    p.addVertex(a1);
    a1 = (middlePoint + vector2 * howHuge);
    p.addVertex(a1);
    a1 = (middlePoint - vector1 * howHuge);
    p.addVertex(a1);
    a1 = (middlePoint - vector2 * howHuge);
    p.addVertex(a1);

    p.setPlane(&plane);
    return p;
}


/*void CPolygon::snapIt()
{
  VertsVector::iterator vIter, vEnd;
  vIter = verts.begin();
  vEnd = verts.end();
  
  while(vIter!=vEnd){
     // round the xyz to a given precision
     for (int k = 0; k < 3; k++) {
        vIter->setXYZ(k, SNAP_INT_TO_FLOAT *floor(vIter->getXYZ(k)*SNAP_FLOAT_TO_INT + 0.5));
     }
    ++vIter;
  }
}*/

void CPolygon::moveOnX(float onX)
{
    int noVerts = verts.size();
    for (int i = 0; i < noVerts; i++)
        verts[i].setRealX(verts[i].getRealX() + onX);
}

void CPolygon::moveOnY(float onY)
{
    int noVerts = verts.size();
    for (int i = 0; i < noVerts; i++)
        verts[i].setRealY(verts[i].getRealY() + onY);
}


void CPolygon::moveOnZ(float onZ)
{
    int noVerts = verts.size();
    for (int i = 0; i < noVerts; i++)
        verts[i].setRealZ(verts[i].getRealZ() + onZ);
}



CPolygon::VertsVector CPolygon::whereCollidesWithPlane(CPlane & plane) const {
    //the vertices that may be found
    VertsVector rp;

    float previousDistanse = 0.0f;
    float currentDistanse = 0.0f;
    float distPlaneNew;
    CVector3 lineNormal;
    CVector3 newPoint;
    CVertex newVertex;
    int noVerts = verts.size();
    for (int i = 0; i < noVerts; i++) {
        //which size are we?
        currentDistanse = plane.distanseFromVertex(verts[i]);

        if (currentDistanse != 0.0f)    //if this (the i ) point is not on the Plane
        {
            //  if the previous was not on the Plane or it is not the first point  ( no previous side point added )
            if (i != 0 && previousDistanse != 0.0f) {
                // (furthermore means that i!=0 cause of the previous check)
                if (previousDistanse * currentDistanse < 0.0f)  // if the previous point was not at my side
                {
                    //now we must find the point on the plane that came the colision with the line of the two
                    // points and add it to both of the sides                       
                    lineNormal = (verts[i].getRealCoor() - verts[i - 1].getRealCoor()).normalized();    // this is the lines vector created by the two points
                    distPlaneNew = -previousDistanse / (plane.getNorma() * lineNormal); //this is the distanse of the previous point from the point we are looking for
                    //at this vertex came the collision                 
                    newPoint =
                        verts[i - 1].getRealCoor() +
                        (lineNormal) * (distPlaneNew);
                    newVertex.setRealCoor(newPoint);
                    // add it to the vertices
                    rp.push_back(newVertex);
                }
            }
        } else                  //the point is on the plane  (this distanse == 0 ) so we add it
            rp.push_back(verts[i]);

        previousDistanse = currentDistanse;
    }
    currentDistanse = plane.distanseFromVertex(verts[0]);
    if (currentDistanse * previousDistanse < 0) {
        lineNormal = (verts[0].getRealCoor() - verts[verts.size() - 1].getRealCoor()).normalized(); // this is the lines vector created by the two points
        distPlaneNew = -previousDistanse / (lineNormal * plane.getNorma()); //this is the distanse of the previous point from the point we are looking for
        newPoint =
            verts[verts.size() - 1].getRealCoor() +
            (lineNormal) * (distPlaneNew);
        newVertex.setRealCoor(newPoint);
        rp.push_back(newVertex);
    }


    return rp;
}

bool CPolygon::addVertex(const CVertex & v){
   /* for (int i = 0; i < verts.size(); i++)
        if (verts[i] == v)
            return false;*/

    verts.push_back(v);
    return true;
}

void CPolygon::addVertex(const CVector3 & v){
    verts.push_back(CVertex(v));
}

void CPolygon::debug()
{
    int noVerts = verts.size();
    for (int i = 0; i < noVerts; i++){
        cout << "vertex " << i << endl;
        cout << "x=" << verts[i].getRealX() << endl;
        cout << "y=" << verts[i].getRealY() << endl;
        cout << "z=" << verts[i].getRealZ() << endl;
    }
}

bool CPolygon::isTooSmall(void) const {
    assert(verts.size()>2);

    if (area() < AREA_EPSILON)
        return true;
    return false;
}



//float CPolygon::distanseFromVertex( CVector3& vertex )
//{
//  return (vertex*getPlane().getNorma() + getPlane().getDistanse());
//}

void CPolygon::addVertices(const VertsVector &vertices) { 
	VertsVector::const_iterator end = vertices.end();
    for (VertsVector::const_iterator i = vertices.begin(); i != end; ++i)
        verts.push_back(*i);
}

void CPolygon::smerge(CPolygon *p1, CPolygon *p2)
{                 
	CVector3 normal = p1->getPlane()->getNorma();
	CVector3 dir;
	VertsVector newVertices;
	vector<CVector3> directions;
	vector<bool> sides;

	VertsVector vertices, vertices2;
	
	//CVector3 normal(norm.x, -norm.z, norm.y);
	
	int v1No = p1->countVertices();

	vertices2 = p2->getVertices();	
	
	int v2No=p2->countVertices();
	sides.resize(v2No+v1No, false);
	directions.resize(v2No+v1No);

	int	i, j, k;		
	
	float d;

	bool outside;
	for ( i = 0 ; i < v2No; i++ ) {	
	    
		// calculate hull side vectors
		for ( j = 0 ; j < v1No ; j++ ) {
			k = ( j + 1 ) % v1No;
 		    dir=(p1->getVertices()[k].getRealCoor()-p1->getVertices()[j].getRealCoor()).normalized();
			directions[j] = (normal^dir);
		}
		
		outside = false;
		for ( j = 0 ; j < v1No; j++ ) {
		    dir=vertices2[i].getRealCoor()-p1->getVertices()[j].getRealCoor();
			d = dir*directions[j];
			if ( d >= MERGE_EPSILON ) {
				outside = true;
			}
			if ( d >= -MERGE_EPSILON ) {
				sides[j] = true;
			} else {
				sides[j] = false;
			}
		}
		   	    
		if ( !outside ) {
			continue;
		}

		// find the back side to front side transition
		for ( j = 0 ; j < v1No; j++ ) {
			if ( !sides[ j %v1No] && sides[ (j + 1) % v1No ] ) {
				break;
			}
		}
		if ( j == v1No ) {
			continue;
		}


		// insert the point here
		newVertices.clear();
		newVertices.push_back(vertices2[i]);
		// copy over all points that aren't double fronts
		j = (j+1)%v1No;
		for ( k = 0 ; k < v1No; k++ ) {
			if ( sides[ (j+k) % v1No] && sides[ (j+k+1) % v1No] ) {
				continue;
			}
			newVertices.push_back(p1->getVertices()[(j+k+1) % v1No]);
		}		
		//v1No = newVertices.size();
		//*p1=CPolygon::removeCollinear(newVertices);
		//p1->setVertices(newVertices);
		//v1No = newVertices.size();
		p1->setVertices(removeCollinear(newVertices));
	    v1No = p1->countVertices();
    }
    
	
	
	      /*CPolygon p;
          //int j;
          int l;
          CVector3 v1,v2;
          for (i=0 ; i<v1No; i++){
		    j = (i+1)%v1No;
		    l = (i+v1No-1)%v1No;
		    v1 = (newVertices[j].getRealCoor()-newVertices[i].getRealCoor()).normalized();
		    v2=(newVertices[i].getRealCoor()-newVertices[l].getRealCoor()).normalized();
		    if (v1*v2 < 0.999f)
	        {
			  p.addVertex(newVertices[i]);
		    }
	      }*/

}

CPolygon::VertsVector CPolygon::removeCollinear(VertsVector &newVertices)
{
      VertsVector rv;
      //int j;
      int l,j;
      CVector3 v1,v2;
      int v1No=newVertices.size();
      for (int i=0 ; i<v1No; i++){
        j = (i+1)%v1No;
	    l = (i+v1No-1)%v1No;
	    v1 = (newVertices[j].getRealCoor()-newVertices[i].getRealCoor()).normalized();
	    v2=(newVertices[i].getRealCoor()-newVertices[l].getRealCoor()).normalized();
	    if (v1*v2 < 1.0f - MERGE_EPSILON)
	    {
		  rv.push_back(newVertices[i]);
	    }
	   }
	   return rv;
}

void CPolygon::translate(const CVector3 &v)
{
  VertsVector::iterator rIter, vEnd;
  rIter = rBegin();
  vEnd = rEnd();
  while(rIter!=vEnd){
    rIter->setRealCoor(rIter->getRealCoor() + v);
    rIter++;
  }
}

CVector3 CPolygon::getMassCenter(){
  CVector3 mCenter(0,0,0);
  for(VertsVector::const_iterator i = rBegin() ; i != rEnd() ; ++i){
    mCenter += i->getRealCoor();
  }
  return mCenter / static_cast<float>(verts.size());
}

void CPolygon::merge()
{
    mBrushPlane->getFather()->merge(this);
}

float CPolygon::outCycleR()
{
  int noVerts = verts.size();
  float max=-1.0f;
  float temp;
  for( int i=0 ; i < noVerts ; i++ ){
    for( int j=(i+1) ; j < noVerts ; j++ ){
      if(i==j)
        continue;
      temp = (verts[i].getRealCoor()-verts[j].getRealCoor()).length();
      if(temp>max)
        max = temp;
    }
  }
  return max;
}

void subdivide_r(list<CPolygon *> *polyList, CPolygon *poly, float dist, bool delOriginal){
	poly->boundPolygon();
	const CAABoundBox &bound = poly->getPolBounds();
	for(int axis = 0 ; axis < 3 ; ++axis){
		CPlane p;
		CVector3 point(0,0,0);
		p.setNorma( CVector3(0,0,0) );
		
		float subFloor = floor( bound.getMin()[axis]  / dist ) * dist;
		float subCeil = ceil( bound.getMax()[axis] / dist ) * dist;

		// No further subdivide is needed
		if(subCeil - subFloor < dist)
			continue;

		point[axis] = subFloor + dist;
		p.getNorma()[axis] = -1;

		p.setDistanse(point *  p.getNorma());

		CPolygon *f = new CPolygon();
		CPolygon *b = new CPolygon();
		CPlane::eSide side = poly->splitByPlane(&p, f, b, 0.1f);
	
		if(side == CPlane::FRONT || side == CPlane::ON) {
			poly = f;
			delete b;
		} else if(side == CPlane::BACK) {
			poly = b;
			delete f;
		} else if(side == CPlane::CROSS) {
			subdivide_r(polyList, f, dist, true);
			subdivide_r(polyList, b, dist, true);
			
			if(delOriginal)
				delete poly;

			return;
		}
 	}

	polyList->push_back(poly);
}

void CPolygon::appendTessPolygons( list<CPolygon *> *polyList ){
	float tess = this->getBrushPlane()->getShader()->getTessSize();
	if(tess > 0.0){
		subdivide_r(polyList, this, tess, false);
	} else {
		polyList->push_back(this);
	}
}

void breakWithBounds_r(vector<CAABoundBox> &bounds, list<CPolygon *> *polyList, CPolygon *poly, bool delOriginal){
    poly->boundPolygon();
    const CAABoundBox &polybound = poly->getPolBounds();
    for(vector<CAABoundBox>::iterator bound = bounds.begin() ; bound != bounds.end() ; ++bound){
        if(!polybound.collide(*bound))
            continue;
        CVector3 max = (*bound).getMax();
        CVector3 min = (*bound).getMin();
        CVector3 origin = (max + min) / 2.0;
        if(poly->getPlane()->distanseFromVertex(origin) < 0.0)
            continue;
        CVector3 dim = ((*bound).getMax() - (*bound).getMin()) / 2.0;
        for(int axis = 0 ; axis < 6 ; ++axis){
            CPlane p;
            switch(axis){
            case 0:
                p.setNorma(CVector3(1.0, 0, 0));
                p.setDistanse(fabs(max.x));
                break;
            case 1:
                p.setNorma(CVector3(0.0, 1.0, 0));
                p.setDistanse(fabs(max.y));
                break;
            case 2:
                p.setNorma(CVector3(0.0, 0, 1.0));
                p.setDistanse(fabs(max.z));
                break;
            case 3:
                p.setNorma(CVector3(-1.0, 0, 0));
                p.setDistanse(fabs(min.x));
                break;
            case 4:
                p.setNorma(CVector3(0.0, -1.0, 0));
                p.setDistanse(fabs(min.y));
                break;
            case 5:
                p.setNorma(CVector3(0.0, 0, -1.0));
                p.setDistanse(fabs(min.z));
                break;
            }

            CPolygon *f = new CPolygon();
            CPolygon *b = new CPolygon();
            CPlane::eSide side = poly->splitByPlane(&p, f, b, 16.0f);

            if(side == CPlane::FRONT || side == CPlane::ON) {
                poly = f;
                delete b;
            } else if(side == CPlane::BACK) {
                poly = b;
                delete f;
            } else if(side == CPlane::CROSS) {
                breakWithBounds_r(bounds, polyList, f, true);
                breakWithBounds_r(bounds, polyList, b, true);

                if(delOriginal)
                    delete poly;

                return;
            }
        }
    }

    polyList->push_back(poly);
}

void CPolygon::getBoundBrokenPolygons( vector<CAABoundBox> &bounds, list<CPolygon *> *polyList ){
#if 1
    polyList->push_back(this);
#else
    breakWithBounds_r(bounds, polyList, this, false);
#endif
}

    /*   
void CPolygon::print(){
  for(int i=0;i<verts.size();i++){
     cout << "vertex "<<i<<endl;
     verts[i].print();
  }
};    */
