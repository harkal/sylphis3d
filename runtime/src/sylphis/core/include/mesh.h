/***************************************************************************
                           mesh.h  -  description
                             -------------------
    begin                : Fri Dec 13 2002
    copyright            : (C) 2002 by Harry Kalogirou
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
