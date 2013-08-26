/***************************************************************************
                          beamtree.cpp  -  description
                             -------------------
    begin                : ??? ??? 7 2003
    copyright            : (C) 2003 by Harry Kalogirou
    email                : harkal@sylphis3d.com
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

#include <polygon.h>
#include <beamtree.h>
#include <assert.h>

CBeamTree::CBeamTree(){
}
CBeamTree::~CBeamTree(){
}

CBeamTree::CBeamTreeNode *CBeamTree::newNode(){
    CBeamTreeNode *node = new CBeamTreeNode();
	node->mChildren[0] = node->mChildren[1] = 0;
    node->mPolyIndex = -1;
    return node;
}

void CBeamTree::reset(const CVector3 &origin){
    CBeamTreeNode *node1 = newNode();
    CBeamTreeNode *node2 = newNode();

    CPlane &plane1 = node1->mSplitPlane;
    CPlane &plane2 = node2->mSplitPlane;
    plane1.normal[0] = 1;
    plane1.normal[1] = 0;
    plane1.normal[2] = 0;
    plane1.dist = origin * plane1.normal;

    plane2.normal[0] = -1;
    plane2.normal[1] = 0;
    plane2.normal[2] = 0;
    plane2.dist = origin * plane2.normal;

    node1->mChildren[0] = node2;
    node1->mChildren[1] = 0;

    node2->mChildren[0] = 0;
    node2->mChildren[1] = 0;

    mNumCutPolygons = 0;
    mNumKeptPolygons = 0;

    mOrigin = origin;
    mHead = node1;
}

S32 CBeamTree::getNumOfCutPolygons(){
    return mNumCutPolygons;
}

S32 CBeamTree::getNumOfKeptPolygons(){
    return mNumKeptPolygons;
}

bool CBeamTree::addPolygon(const CPolygon &p, S32 index){
	mKeep = false;
	mGrow = false;
    mCurIndex = index;
    mShadowingPolys[index] = 0;
    addShadowCastingPolygon_r(mHead, p);
	if(mKeep)
		mLitPolygons.push_back(index);
    return mKeep;
}

bool CBeamTree::addShadowCastingPolygon(const CPolygon &p, S32 index){
    mKeep = false;
	mGrow = true;
    mCurIndex = index;
    mShadowingPolys[index] = 0;
    addShadowCastingPolygon_r(mHead, p);
	if(mKeep)
		mLitPolygons.push_back(index);
    return mKeep;
}

void CBeamTree::addShadowCastingPolygon_r(CBeamTreeNode *n, const CPolygon &p){
    CPlane::eSide side;

    side = p.planeSide(n->mSplitPlane);
    if(side == CPlane::FRONT || side == CPlane::ON){
        if(n->mChildren[0] != 0)
            addShadowCastingPolygon_r(n->mChildren[0], p);
        else {
            mNumCutPolygons++;
            mShadowingPolys[n->mPolyIndex]++;
        }
    }
    else if (side == CPlane::BACK) {
        if(n->mChildren[1] != 0)
            addShadowCastingPolygon_r(n->mChildren[1], p);
        else {
            mNumKeptPolygons++;
			if(mGrow)
				n->mChildren[1] = growTree(p);
            mKeep = true;
        }
    }
    else if (side == CPlane::CROSS) {
        CPolygon front, back;
        p.splitByPlane(n->mSplitPlane, front, back);
        
        if(n->mChildren[0] != 0)
            addShadowCastingPolygon_r(n->mChildren[0], front);
        else {
            mNumCutPolygons++;
            mShadowingPolys[n->mPolyIndex]++;
        }
        
        if(n->mChildren[1] != 0)
            addShadowCastingPolygon_r(n->mChildren[1], back);
        else {
            mNumKeptPolygons++;
			if(mGrow)
				n->mChildren[1] = growTree(back);
            mKeep = true;
        }
    }
}

CBeamTree::CBeamTreeNode *CBeamTree::growTree(const CPolygon &p){
    assert(p.getVertices().size() > 2);
    CBeamTreeNode *cur, *start;

    start = cur = buildNodeByEdge(p, 0);
    for(S32 i = 1 ; i < p.getVertices().size() ; i++){
        cur->mChildren[0] = buildNodeByEdge(p, i);
        cur = cur->mChildren[0];
    }
    return start;
}

CBeamTree::CBeamTreeNode *CBeamTree::buildNodeByEdge(const CPolygon &p, S32 edge){
    const CPolygon::VertexVector &vertices = p.getVertices();

    const CVector3 &v1 = vertices[edge];
    const CVector3 &v2 = vertices[(edge + 1)%vertices.size()];

    CBeamTreeNode *n = newNode();
    n->mSplitPlane = CPlane(mOrigin, v2, v1);
    n->mPolyIndex = mCurIndex;
        
    return n;
}

void CBeamTree::getShadowCastingPolygons(std::vector<S32> &v){
    IndexPolysMap::iterator i;

    for(i = mShadowingPolys.begin() ; i != mShadowingPolys.end() ; i++){
        S32 index = i->first;
        if(index == -1)
            continue;
        S32 count = i->second;
        if(count)
            v.push_back(index);
    }
}

void CBeamTree::getLitPolygons(std::vector<S32> &v){
	IndexPolysMap::iterator i;

	v = mLitPolygons;
	return;
}
