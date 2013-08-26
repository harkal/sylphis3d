/***************************************************************************
                          beamtree.h  -  description
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

#ifndef BEAMTREE_H
#define BEAMTREE_H

#include <vector>
#include <map>
#include <plane.h>
#include <pointer.h>

class CPolygon;

class CBeamTree {
public: 
    CBeamTree();
    ~CBeamTree();

    SmartPointer(CBeamTreeNode);
    class CBeamTreeNode : public CRefObject {
    public:
        CPlane mSplitPlane;
        S32 mPolyIndex;
        CBeamTreeNodePtr mChildren[2];
    };

    void reset(const CVector3 &origin);
    CBeamTreeNode *newNode();

    bool addShadowCastingPolygon(const CPolygon &p, S32 index);
	bool addPolygon(const CPolygon &p, S32 index);

    S32 getNumOfCutPolygons();
    S32 getNumOfKeptPolygons();

    void getShadowCastingPolygons(std::vector<S32> &v);
	void getLitPolygons(std::vector<S32> &v);
    
    typedef std::map<S32, S32> IndexPolysMap;
protected:
    void addShadowCastingPolygon_r(CBeamTreeNode *n, const CPolygon &p);
    CBeamTreeNode *growTree(const CPolygon &p);
    CBeamTreeNode *buildNodeByEdge(const CPolygon &p, S32 edge);

    CVector3 mOrigin;
    CBeamTreeNodePtr mHead;
    IndexPolysMap mShadowingPolys;
	std::vector<S32> mLitPolygons;
    S32 mNumCutPolygons;
    S32 mNumKeptPolygons;
    S32 mCurIndex;
    bool mKeep;
	bool mGrow;
};

#endif
