/***************************************************************************
                           mesh.h  -  description
                             -------------------
    begin                : Fri Dec 13 2002
    copyright            : (C) 2002 by Harry Kalogirou
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

#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include <vector.h>
#include <plane.h>
#include <resource.h>
#include <renderable.h> //CEDGE
#include <material.h>

using namespace std;

class CFile;
class CVirtualFS;

SmartPointer(CSubMesh);
class CSubMesh : public CRefObject {
public:
    CSubMesh();
    virtual ~CSubMesh();

    void load(CFile *f);

    typedef struct {
        S32 boneId;
        F32 weight;
    } SInfluence;

    typedef vector<SInfluence> VInfluences;

    vector<CVector3> &getVertices(){ return mVertices; }
    vector<CVector3> &getNormals(){ return mNormals; }
	vector<CVector3> &getTangents(){ return mTangents; }
    vector<CTexCoord> &getTexCoords(){ return mTexCoords; }
    vector<VInfluences> &getInfluences(){return mInfluences; }
    vector<S32> &getNeighbors(){ return mNeighbors; }
    vector<CPlane> &getElemPlanes(){ return mElemPlanes; }

    vector<S32> &getElems(){ return mElems; }
    S32 getShader(){ return mShader; }
protected:
    S32 addEdge(const CEdge &e);

    vector<CVector3> mVertices;
    vector<CVector3> mNormals;
	vector<CVector3> mTangents;
    vector<CTexCoord> mTexCoords;
    vector<VInfluences> mInfluences;

    vector<CPlane> mElemPlanes;
    vector<S32> mNeighbors;
    vector<S32> mElems;
    vector<S32> mElemEdges;
    vector<CEdge> mEdges;

    S32 mShader;
};

SmartPointer(CMesh);
class CMesh : public CResource {
public:
    CMesh();
    virtual ~CMesh();

    void addSubMesh(CSubMesh *sm);
    vector<CSubMeshPtr> &getSubMeshes(){ return mSubMeshes; }

	void        addShader(CMaterial *s);
    CMaterial*  getShaderById(S32 i);
    
    virtual void load(const std::string &n, CVirtualFS *vfs);
protected:
    static const S32 EARLIEST_COMPATIBLE_FILE_VERSION;
    static const S32 CURRENT_FILE_VERSION;
    vector<CSubMeshPtr> mSubMeshes;
	vector<CMaterialPtr> mShaders;
};

#endif
