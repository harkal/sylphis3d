/***************************************************************************
                          beamtree.h  -  description
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
