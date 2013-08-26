/***************************************************************************
md3model.cpp  -  description
-------------------
begin                : Wed Dec 11 2002
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

#include <vector.h>
#include <vfs.h>
#include <exception.h>
#include <modelmanager.h>
#include <smfmodel.h>
#include <math.h>
#include <gutil.h>
#include <configfile.h>
#include <engine.h>
#include <tokenizer.h>

CSMFModel::CSMFModel(){
}

CSMFModel::CSMFModel(CModelManager *m) : CModel(m) {
}

CSMFModel::~CSMFModel(){
}

void CSMFModel::load(const std::string &n, CVirtualFS *vfs){
	setName(n);

	CFileText f(n);
	vfs->readFile(f);

	string buff = (char *)f.getData();

	CTokenizer t(buff);
	t.setLowerCaseTokens(true);
	t.addSeparator("#");

	string meshName;
	string materialName;
	vector<CVector3> vertices, normals;
	vector<U32> elems;
	vector<CTexCoord> texCoords;

	while(t.nextToken()){
		string token = t.getToken();

		if(token == "#"){
			t.skipToNextLine();
		}else if(token == "begin"){
			//t.nextToken();
			//string meshName = t.getToken();
			t.nextToken();
			materialName = t.getToken();
            elems.reserve(4096);
		}else if(token == "end"){
			CRenderableMesh *mesh = new CRenderableMesh();
			mRMeshes.push_back(mesh);

			mesh->setDefaultShader(CEngine::getSingleton()->mMaterials.registerMaterial(materialName));

			mesh->numVertices = vertices.size();
			mesh->numFrames = 1;
			mesh->numElems = elems.size() / 3;

			const bool mSystemMemory = !gCfgFile.getAsInt("r_useStaticModelVBO");

			mesh->mVertexBuffer->allocate(mesh->numVertices * 48, CVertexBuffer::STATIC, mSystemMemory);
			
			for(S32 j = 0 ; j < mesh->numVertices ; ++j){
				mesh->vertices[j] = vertices[j];
				mesh->vertices[j].swapHand();
				CVector4 &v = mesh->vertices[j];
				if(fabs(v.x) > radius)radius = fabs(v.x);
				if(fabs(v.y) > radius)radius = fabs(v.y);
				if(fabs(v.z) > radius)radius = fabs(v.z);
				//if(v.length() > radius)radius = v.length();

				CVector3 &normal = normals[j];
#if 0
				F32 lat = ( vert->normal >> 8 ) & 0xff;
				F32 lng = ( vert->normal & 0xff );
				lat *= PI/128;
				lng *= PI/128;
				normal.x = cos(lat) * sin(lng);
				normal.y = sin(lat) * sin(lng);
				normal.z = cos(lng);
				normal.swapHand();
#else
				normal = CVector3::ZERO;
#endif

				mesh->tangent[j] = CVector3::ZERO;
				mesh->tex_st[j] = texCoords[j];
			}

            mesh->mElems->allocate(mesh->numElems * 3 * sizeof(U32), CIndexBuffer::STATIC, mSystemMemory);
			mesh->elemEdges = new U32[mesh->numElems * 3];

			mesh->mElems->uploadData(&elems[0],0,elems.size() * sizeof(U32));

#if 0
			{for(S32 i = 0 ; i < mesh->numElems; ++i){
				S32 k = i * 3;

				mesh->elemEdges[k] = mesh->addEdge(CEdge(elems[k], elems[k + 1]));
				mesh->elemEdges[k + 1] = mesh->addEdge(CEdge(elems[k + 1], elems[k + 2]));
				mesh->elemEdges[k + 2] = mesh->addEdge(CEdge(elems[k + 2], elems[k]));
			}}
#endif
            for(S32 i = 0 ; i < mesh->numElems * 3 ; ++i){
                if(elems[i]>vertices.size()-1)gcon.printf("ERROR : Vertex index at %d (max = %d)\n", elems[i], vertices.size()-1);
            }

			mesh->makeNeighbors();
			mesh->makeElemNormals();
			
			for(S32 i = 0 ; i < mesh->numElems; i++){
				S32 j = i * 3;
				S32 e0 = elems[j];
				S32 e1 = elems[j + 1];
				S32 e2 = elems[j + 2];

				CVector3 tang;
				CVector3 &normal = mesh->elemPlanes[i].normal;
				calcTangentVector(mesh->vertices[e0], mesh->vertices[e1], mesh->vertices[e2],
					mesh->tex_st[e0], mesh->tex_st[e1], mesh->tex_st[e2],
					normal, mesh->tangent[e0]);
				calcTangentVector(mesh->vertices[e1], mesh->vertices[e2], mesh->vertices[e0],
					mesh->tex_st[e1], mesh->tex_st[e2], mesh->tex_st[e0],
					normal, mesh->tangent[e1]);
				calcTangentVector(mesh->vertices[e2], mesh->vertices[e0], mesh->vertices[e1],
					mesh->tex_st[e2], mesh->tex_st[e0], mesh->tex_st[e1],
					normal, mesh->tangent[e2]);

				mesh->normals[e0] -= mesh->elemPlanes[i].normal;
				mesh->normals[e1] -= mesh->elemPlanes[i].normal;
				mesh->normals[e2] -= mesh->elemPlanes[i].normal;

			}

			for(S32 i = 0 ; i < mesh->numVertices ; i++ ){
				F32 l = mesh->tangent[i].length();
				if(l == 0.0)
					continue;
				mesh->tangent[i] /= l;
				normalize(mesh->normals[i]);
			}

            gcon.printf("-- Loaded mesh with %d Vertices, %d facets\n", vertices.size(), elems.size() / 3);

			// Free temp buffers
			vertices.clear();
			normals.clear();
			texCoords.clear();
			elems.clear();
		}else if(token == "v"){
			CVector3 v;
			t.nextToken();v.x = atof(t.getToken().c_str());
			t.nextToken();v.y = atof(t.getToken().c_str());
			t.nextToken();v.z = atof(t.getToken().c_str());
			vertices.push_back(v);
		}else if(token == "n"){
			CVector3 v;
			t.nextToken();v.x = atof(t.getToken().c_str());
			t.nextToken();v.y = atof(t.getToken().c_str());
			t.nextToken();v.z = atof(t.getToken().c_str());
			normals.push_back(v);
		}else if(token == "t"){
			CTexCoord v;
			t.nextToken();v.x = atof(t.getToken().c_str());
			t.nextToken();v.y = 1.0 - atof(t.getToken().c_str());
			texCoords.push_back(v);
		}else if(token == "f"){
			t.nextToken(); elems.push_back(atof(t.getToken().c_str()) - 1);
			t.nextToken(); elems.push_back(atof(t.getToken().c_str()) - 1);
			t.nextToken(); elems.push_back(atof(t.getToken().c_str()) - 1);
		}
	}

}

