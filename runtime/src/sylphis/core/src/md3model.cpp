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
#include <md3model.h>
#include <math.h>
#include <gutil.h>
#include <configfile.h>
#include <engine.h>

typedef struct md3Frame_s {
	CVector3	bounds[2];
	CVector3	localOrigin;
	F32		radius;
	char		name[16];
} md3Frame_t;

typedef struct {
	char			name[64];
	S32				shaderIndex;
} md3Shader_t;

typedef struct {
	S32		ident;				// 

	char	name[64];	// polyset name

	S32		flags;
	S32		numFrames;			// all surfaces in a model should have the same

	S32		numShaders;			// all surfaces in a model should have the same
	S32		numVerts;

	S32		numTriangles;
	S32		ofsTriangles;

	S32		ofsShaders;			// offset from start of md3Surface_t
	S32		ofsSt;				// texture coords are common for all frames
	S32		ofsVertices;		// numVerts * numFrames

	S32		ofsEnd;				// next surface follows
} md3Surface_t;

typedef struct {
	S32			ident;
	S32			version;

	char		name[64];

	S32			flags;

	S32			numFrames;
	S32			numTags;			
	S32			numSurfaces;

	S32			numSkins;

	S32			ofsFrames;
	S32			ofsTags;
	S32			ofsSurfaces;
	S32			ofsEnd;
} md3Header_t;

typedef struct {
	S16		xyz[3];
	S16 normal;
} md3XyzNormal_t;

typedef struct {
	S32			indexes[3];
} md3Triangle_t;

typedef struct md3Vertex_s {
	S16 vec[3];
	S16 normal;
} md3Vertex_t;

typedef struct md3tag_s
{
	char		name[64];	// supported names : weapon
	CVector3	origin;			// pretty much self explanatory
	CVector3	axis[3];		// no ?

} md3tag_t;

CMD3Model::CMD3Model(){
}

CMD3Model::CMD3Model(CModelManager *m) : CModel(m) {
}

CMD3Model::~CMD3Model(){
}

F32 mabs(F32 n){
    return n>0?n:-n;
}

void CMD3Model::load(const std::string &n, CVirtualFS *vfs){
	setName(n);

    CFile f(n);
    vfs->readFile(f);

	md3Header_t *header = (md3Header_t *)f.getData();

	if(header->version != 15)
		throw CException("Not a MD3 file");

	md3Surface_t *surf = (md3Surface_t *)((char *)header + header->ofsSurfaces);

	radius = 0.0;
	for(S32 i = 0 ; i < header->numSurfaces ; i++){
		CRenderableMesh *mesh = new CRenderableMesh();
		mRMeshes.push_back(mesh);

		md3Shader_t *shader = (md3Shader_t *)((char*)surf + surf->ofsShaders);
        mesh->setDefaultShader(CEngine::getSingleton()->mMaterials.registerMaterial(shader->name));

		mesh->numVertices = surf->numVerts;
		mesh->numFrames = 1;
		mesh->numElems = surf->numTriangles;

        const bool mSystemMemory = !gCfgFile.getAsInt("r_useStaticModelVBO");
	
        mesh->mVertexBuffer->allocate(mesh->numVertices * 48, CVertexBuffer::STATIC, mSystemMemory);
        
        //mesh->vertices->allocate(mesh->numVertices, CVertexBuffer::STATIC, mSystemMemory);// = new CVector3[mesh->numVertices];
		//mesh->normals->allocate(mesh->numVertices, CVertexBuffer::STATIC, mSystemMemory);// = new CVector3[mesh->numVertices];
		//mesh->tangent->allocate(mesh->numVertices, CVertexBuffer::STATIC, mSystemMemory);// = new CVector3[mesh->numVertices];

		md3Vertex_t *vert = (md3Vertex_t *)((char *)surf + surf->ofsVertices);
		for(S32 j = 0 ; j < mesh->numVertices ; j++, vert++){
			mesh->vertices[j].x = (F32)vert->vec[0] / 64.0f;
            assert(mesh->vertices[j].x == (F32)vert->vec[0] / 64.0f);
			mesh->vertices[j].y = (F32)vert->vec[1] / 64.0f;
			mesh->vertices[j].z = (F32)vert->vec[2] / 64.0f;
            mesh->vertices[j].w = 1.0;
			mesh->vertices[j].swapHand();
			CVector4 &v = mesh->vertices[j];
			if(mabs(v.x) > radius)radius = mabs(v.x);
			if(mabs(v.y) > radius)radius = mabs(v.y);
			if(mabs(v.z) > radius)radius = mabs(v.z);
			//if(v.length() > radius)radius = v.length();

			CVector3 &normal = mesh->normals[j];
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
            mesh->tex_st[j] = *((CTexCoord *)((char*)surf + surf->ofsSt) + j);
		}

        //mesh->tex_st->allocate(mesh->numVertices, CVertexBuffer::STATIC, mSystemMemory);// = new CTexCoord[mesh->numVertices];
        //memcpy(&mesh->tex_st[0].x, (char*)surf + surf->ofsSt, mesh->numVertices * sizeof(CTexCoord));

        mesh->mElems->allocate(mesh->numElems * 3 * sizeof(U32), CIndexBuffer::STATIC, mSystemMemory);
		mesh->elemEdges = new U32[mesh->numElems * 3];
		
        U32 *elems = (U32 *)mesh->mElems->map();
		md3Triangle_t *tri = (md3Triangle_t *)((char*)surf + surf->ofsTriangles);
		{for(S32 i = 0 ; i < mesh->numElems; i++, tri++){
			S32 k = i * 3;

			elems[k + 2] = tri->indexes[1];
			elems[k + 1] = tri->indexes[2];
			elems[k + 0] = tri->indexes[0];
		}}

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

        mesh->mElems->unmap();

		for(S32 i = 0 ; i < mesh->numVertices ; i++ ){
			F32 l = mesh->tangent[i].length();
			if(l == 0.0)
				continue;
			mesh->tangent[i] /= l;
			normalize(mesh->normals[i]);
		}

		surf = (md3Surface_t *)((char*)surf + surf->ofsEnd);
	}
}

