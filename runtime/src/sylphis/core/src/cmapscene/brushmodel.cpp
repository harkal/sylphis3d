/***************************************************************************
                          brushmodel.cpp  -  description
                             -------------------
    begin                : ??? ??? 4 2003
    copyright            : (C) 2003 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/
#include <map>
#include <vector.h>
#include <gutil.h>
#include "brushmodel.h"
#include "cmapfile.h"
#include "cmap.h"

CBrushModel::CBrushModel(){
}

CBrushModel::~CBrushModel(){
}

typedef struct {
	std::vector<CVector3> vertices;
	std::vector<CVector3> normals;
	std::vector<CTexCoord> texcoords;
	std::vector<U32> elems;
	std::vector<CEdge> elemEdges;
} tMesh;

void CBrushModel::loadBrushEntity(CMAP &cmap, S32 i){
#if 1
    tBrushModel &bmu = cmap.BrushModels[i];

	std::map<S32, tMesh> meshes;

	radius = 1000.0;
	for(S32 i = bmu.startFace ; i < bmu.startFace + bmu.facesSize ; i++){
		dface_t &face = cmap.Faces[i];
		tMesh &mesh = meshes[face.shader];
/*
		if(meshes.find(face.shader) != meshes.end()){
			r = meshes[face.shader];
		} else {
			r = new CRenderableMesh();
			meshes[face.shader] = r;
		}
*/		
		const S32 firstVert = mesh.vertices.size();
		for(S32 j = face.firstvert ; j < face.firstvert + face.numverts ; ++j){ 
			dvertex_t &vert = cmap.Verts[j];
			mesh.vertices.push_back(vert.point);
			mesh.normals.push_back(-vert.v_norm);
			mesh.texcoords.push_back(vert.tex_st);
		}
		for(S32 j = 0 ; j < face.numverts - 2 ; j++){ 
			const S32 l = firstVert + j;
			mesh.elems.push_back(firstVert);
			mesh.elems.push_back(l + 2);
			mesh.elems.push_back(l + 1);

			mesh.elemEdges.push_back(CEdge(firstVert, l + 2));
			mesh.elemEdges.push_back(CEdge(l + 2, l + 1));
			mesh.elemEdges.push_back(CEdge(l + 1, firstVert));
		}
	}

	std::map<S32, tMesh>::iterator iter;
	for(iter = meshes.begin() ; iter != meshes.end() ; iter++){
		tMesh &bmesh = iter->second;

		CRenderableMesh *mesh = new CRenderableMesh();
		mRMeshes.push_back(mesh);

		mesh->setDefaultShader(cmap.shaderrefs[iter->first].shader);

		mesh->numVertices = bmesh.vertices.size();
		mesh->numFrames = 1;
		mesh->numElems = bmesh.elems.size() / 3;
	
        mesh->mVertexBuffer->allocate(mesh->numVertices * (sizeof(CVector4) + sizeof(CVector3) * 2 + sizeof(CTexCoord)) , CVertexBuffer::STATIC);
        
        mesh->vertices.setupBuffer(mesh->mVertexBuffer, CVertexBuffer::POSITION);
        mesh->normals.setupBuffer(mesh->mVertexBuffer, CVertexBuffer::NORMAL);
        mesh->tex_st.setupBuffer(mesh->mVertexBuffer, CVertexBuffer::TEX_COORD);
        mesh->tangent.setupBuffer(mesh->mVertexBuffer, CVertexBuffer::TANGENT);

        mesh->mElems->allocate(bmesh.elems.size() * sizeof(U32), CIndexBuffer::STATIC, false);
		mesh->elemEdges = new U32[bmesh.elems.size()];

		for(S32 i = 0 ; i < bmesh.vertices.size() ; ++i){
			mesh->vertices[i] = bmesh.vertices[i];
			mesh->normals[i] = bmesh.normals[i];
			mesh->tex_st[i] = bmesh.texcoords[i];
		}

        // Upload data to the buffer
        mesh->mElems->uploadData(&bmesh.elems[0], 0, bmesh.elems.size() * sizeof(U32));
		
		mesh->makeNeighbors();
		mesh->makeElemNormals();

        U32 *elems = (U32 *)mesh->mElems->map(CIndexBuffer::READ_ONLY);
		for(S32 i = 0 ; i < mesh->numElems; i++){
			S32 j = i * 3;
            
			S32 e0 = elems[j];
			S32 e1 = elems[j + 1];
			S32 e2 = elems[j + 2];

			CVector3 tang;
			CVector3 &normal = mesh->elemPlanes[i].normal;
			calcTangentVector(bmesh.vertices[e0], bmesh.vertices[e1], bmesh.vertices[e2],
				bmesh.texcoords[e0], bmesh.texcoords[e1], bmesh.texcoords[e2],
				normal, mesh->tangent[e0]);
			calcTangentVector(bmesh.vertices[e1], bmesh.vertices[e2], bmesh.vertices[e0],
				bmesh.texcoords[e1], bmesh.texcoords[e2], bmesh.texcoords[e0],
				normal, mesh->tangent[e1]);
			calcTangentVector(bmesh.vertices[e2], bmesh.vertices[e0], bmesh.vertices[e1],
				bmesh.texcoords[e2], bmesh.texcoords[e0], bmesh.texcoords[e1],
				normal, mesh->tangent[e2]);

			//mesh->normals[e0] -= mesh->elemPlanes[i].normal;
			//mesh->normals[e1] -= mesh->elemPlanes[i].normal;
			//mesh->normals[e2] -= mesh->elemPlanes[i].normal;

		}

        mesh->mElems->unmap();

		for(S32 i = 0 ; i < mesh->numVertices ; i++ ){
			F32 l = mesh->tangent[i].length();
			if(l == 0.0)
				continue;
			mesh->tangent[i] /= l;
			//normalize(mesh->normals[i]);
		}
	}
#endif
}

void CBrushModel::load(const std::string &n, CVirtualFS *vfs){

}