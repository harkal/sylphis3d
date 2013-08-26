/***************************************************************************
                          beamtree.cpp  -  description
                             -------------------
    begin                : ??? ??? 7 2003
    copyright            : (C) 2003 by Harry Kalogirou
    email                : harkal@sylphis3d.com
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
