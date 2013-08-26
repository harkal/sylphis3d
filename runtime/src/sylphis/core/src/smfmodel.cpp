/***************************************************************************
md3model.cpp  -  description
-------------------
begin                : Wed Dec 11 2002
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

