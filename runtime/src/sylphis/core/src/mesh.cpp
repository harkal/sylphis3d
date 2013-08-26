/***************************************************************************
                           mesh.cpp  -  description
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

#include <mesh.h>
#include <exception.h>
#include <vfs.h>

CSubMesh::CSubMesh(){
}

CSubMesh::~CSubMesh(){
}

S32 CSubMesh::addEdge(const CEdge &e){
    for(S32 i = 0 ; i < mEdges.size() ; i++){
        if(mEdges[i] == e)
            return i;
    }
    mEdges.push_back(e);
    return mEdges.size() - 1;
}

static CVector3 calcTangentVector__(const CVector3 &v0,
					   const CVector3 &v1,
					   const CVector3 &v2,
					   const CTexCoord &t0,
					   const CTexCoord &t1,
					   const CTexCoord &t2,
					   const CVector3 &n0)
{
		F64 du1 = t1.x - t0.x;
		F64 dv1 = t1.y - t0.y;
		F64 du2 = t2.x - t0.x;
		F64 dv2 = t2.y - t0.y;

		F64 prod1 = (du1*dv2-dv1*du2);
		F64 prod2 = (du2*dv1-dv2*du1);
		
		if ((fabs(prod1) < 0.000001)||(fabs(prod2) < 0.000001))
			return CVector3::ZERO;

		F64 x = du2 / prod1;
		F64 y = du1 / prod2;

		CVector3 vec1 = v1 - v0;
		CVector3 vec2 = v2 - v0;
		CVector3 tangent = (vec1 * x) + (vec2 * y);

		F64 component = (tangent * n0);
		tangent -= (n0 * component);
		normalize(tangent);
		return tangent;
}

static S32 scount = 0;

void CSubMesh::load(CFile *f){
    mShader = f->readInt();
	if(mShader < 0)
		mShader = scount++;
		//throw CException("No shader assigned on submesh");

    S32 numVertices = f->readInt();
    if(numVertices < 0)
        throw CException("File corrupted");
    S32 numFaces = f->readInt();
    S32 numLodSteps = f->readInt();
    S32 numSprings = f->readInt();
    if(numSprings > 0)
        throw CException("Springs not supported");
    S32 numMaps = f->readInt();
    
    mVertices.reserve(numVertices);
    mNormals.reserve(numVertices);
	mTangents.resize(numVertices);
    mTexCoords.reserve(numVertices);
    mInfluences.reserve(numVertices);

    {for(S32 i = 0 ; i < numVertices ; i++){
        F32 x,y,z;
        x = f->readFloat();
        y = f->readFloat();
        z = f->readFloat();
        CVector3 pos(x, y, z);
        pos.swapHand();
        x = f->readFloat();
        y = f->readFloat();
        z = f->readFloat();
        CVector3 normal(x, y, z);
        normal.swapHand();
        S32 collapseId = f->readInt();
        S32 collapseCount = f->readInt();
        
        CTexCoord texCoord;
        S32 maps = numMaps;
        if(numMaps > 0){
            texCoord.x = f->readFloat();
            texCoord.y = 1.0 - f->readFloat();
            while(--maps > 0){
                f->readFloat();
                f->readFloat();
            }
        }

        S32 numInfluences = f->readInt();
        if(numInfluences < 0)
            throw CException("File corrupted");

        VInfluences infs;
        infs.reserve(numInfluences);
        for(S32 j = 0 ; j < numInfluences ; j++){
            SInfluence inf;
            inf.boneId = f->readInt();
            inf.weight = f->readFloat();
            infs.push_back(inf);
        }

        // We don't support springs, so no physical property
        // F32 weight = f->readInt();

        mVertices.push_back(pos);
        mNormals.push_back(normal);
        mTexCoords.push_back(texCoord);
        mInfluences.push_back(infs);
    }}

    // No springs ... skip to faces
    mElems.reserve(numFaces * 3);
	CVector3 norm;
	S32 count=0;
    {for(S32 i = 0 ; i < numFaces ; i++){
        S32 a = f->readInt();
        S32 b = f->readInt();
        S32 c = f->readInt();

		norm = mNormals[a] + mNormals[b] + mNormals[c];
		normalize(norm);
		CPlane p(mVertices[a], mVertices[b], mVertices[c]);
		if(norm * p.normal > 0.0)
			count++;
		else
			count--;
		
		mElems.push_back(c);
		mElems.push_back(b);
		mElems.push_back(a);
    }}

#if 1
	if(count < 0){
		{for(S32 i = 0 ; i < numFaces ; i++){
			S32 k = i * 3;
			S32 t = mElems[k];
			mElems[k] = mElems[k + 2];
			mElems[k + 2] = t;
		}}
	}
#endif
    // Computed data
	// Tangents
	S32 e0;
	S32 e1;
	S32 e2;
	S32 k;
	{for(S32 j = 0 ; j < numFaces ; j++){
        k = j * 3;
		
		e0 = mElems[k];
		e1 = mElems[k + 1];
		e2 = mElems[k + 2];
        
		mTangents[e0] += calcTangentVector__(mVertices[e0], mVertices[e1], mVertices[e2],
						  mTexCoords[e0], mTexCoords[e1], mTexCoords[e2],
						  mNormals[e0]);
		mTangents[e1] += calcTangentVector__(mVertices[e1], mVertices[e2], mVertices[e0],
						  mTexCoords[e1], mTexCoords[e2], mTexCoords[e0],
						  mNormals[e1]);
		mTangents[e2] += calcTangentVector__(mVertices[e2], mVertices[e0], mVertices[e1],
						  mTexCoords[e2], mTexCoords[e0], mTexCoords[e1],
						  mNormals[e2]);
	}}

	for(S32 i = 0 ; i < numVertices ; i++){
		normalize(mTangents[i]);
	}

	// Face conectivity
    mElemPlanes.resize(numFaces);
    mElemEdges.resize(numFaces * 3);
    for(S32 i = 0 ; i < numFaces ; i++){
        S32 k = i * 3;
		CPlane p(mVertices[mElems[k]], mVertices[mElems[k + 1]], mVertices[mElems[k+2]]);
        mElemPlanes[i] = p;
        
        mElemEdges[k] = addEdge(CEdge(mElems[k], mElems[k + 1]));
        mElemEdges[k + 1] = addEdge(CEdge(mElems[k + 1], mElems[k + 2]));
        mElemEdges[k + 2] = addEdge(CEdge(mElems[k + 2], mElems[k]));
    }

    mNeighbors.resize(numFaces * 3);
    for(S32 k = 0 ; k < numFaces * 3 ; k++)
        mNeighbors[k] = -1;
#if 1
    {for(S32 i = 0 ; i < numFaces * 3 ; i++){
        for(S32 j = 0 ; j < numFaces * 3 ; j++){
            if(i == j)continue;
            if(mElemEdges[i] == mElemEdges[j])
                mNeighbors[j] = i / 3;
        }
    }}
#endif
}

CMesh::CMesh(){
}

CMesh::~CMesh(){
}

const S32 CMesh::EARLIEST_COMPATIBLE_FILE_VERSION = 699;
const S32 CMesh::CURRENT_FILE_VERSION = 910;

void CMesh::addSubMesh(CSubMesh *sm){
    mSubMeshes.push_back(sm);
}

void CMesh::addShader(CMaterial *s){
    mShaders.push_back(s);
}

CMaterial *CMesh::getShaderById(S32 i){
	assert(i >= 0);
    return mShaders[i];
}

void CMesh::load(const std::string &n, CVirtualFS *vfs){
    setName(n);

    CFile f(n);
    vfs->readFile(f);

    if(f.readChar() != 'C' || f.readChar() != 'M' || f.readChar() != 'F' || f.readChar() != 0)
        throw CException("Not skeleton file");

    S32 version = f.readInt();
    if(version < EARLIEST_COMPATIBLE_FILE_VERSION || version > CURRENT_FILE_VERSION)
        throw CException("File version not supported");

    S32 numSubMeshes = f.readInt();
    if(numSubMeshes <= 0)
        throw CException("File corrupted");

	scount = 0;
    for(S32 i = 0 ; i < numSubMeshes ; i++){
        CSubMeshPtr sm = new CSubMesh();
        sm->load(&f);
        addSubMesh(sm);
    }
}
