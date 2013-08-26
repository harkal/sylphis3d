/***************************************************************************
                          meshloader.cpp  -  description
                             -------------------
    begin                : Wed Mar 27 2002
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

#include "exception.h"
#include "tokenizer.h"
#include "meshloader3ds.h"

CMeshLoader3DS::CMeshLoader3DS(CVirtualFS &v){
	vfs = &v;
}
CMeshLoader3DS::~CMeshLoader3DS(){
}

void CMeshLoader3DS::load(CRenderableMesh &mesh, const string &name){
	bool mapped = false;
	pair<string, F32> p;
	mesh._free();
	
	CFileText f(name);
	vfs->readFile(f); // NOTE: throws
	
	string buff = (char *)f.getData();
	
	CTokenizer t(buff);
	t.setLowerCaseTokens(true);
	t.addSeparator(":");
	
	t.skipToToken("named");
	p = getValuePair(t);
	
	if(p.first != "object")
		throw CTokenizerException(t, "Expected 'object'");
		
	expect(t, "tri-mesh,");
	
	{for(S32 i = 0 ; i < 2 ; i++){
		p = getValuePair(t);
		if(p.first == "vertices") mesh.numVertices = (S32)p.second;
		else if(p.first == "faces") mesh.numElems = (S32)p.second;
		else
			throw CTokenizerException(t, "Unexpected token '" + p.first + "'");
	}}
	
    mesh.mVertexBuffer->allocate(mesh.numVertices);
    //mesh.vertices->allocate(mesh.numVertices);// = new CVector3[mesh.numVertices];
	//mesh.normals->allocate(mesh.numVertices); // = new CVector3[mesh.numVertices];
	//mesh.tex_st->allocate(mesh.numVertices); // = new CTexCoord[mesh.numVertices];

	t.nextToken(); if(t.getToken() == "mapped"){
		mapped = true;
		expect(t, "vertex");
	}
	
	expect(t, "list"); expect(t,":");
	
	{for(S32 i = 0 ; i < mesh.numVertices ; i++){
		t.skipToToken("vertex");
		t.nextToken();
		expect(t, ":");
		S32 components = mapped ? 5 : 3;
		for(S32 j = 0; j < components; j++){
			p = getValuePair(t);
			if(p.first == "x") mesh.vertices[i].x = p.second;
			else if(p.first == "y") mesh.vertices[i].y = p.second;
			else if(p.first == "z") mesh.vertices[i].z = p.second;
			else if(mapped && p.first == "u")mesh.tex_st[i].x = p.second;
			else if(mapped && p.first == "v")mesh.tex_st[i].y = 1.0-p.second;
			else
				throw CTokenizerException(t, "Unexpected token '" + p.first + "'");
		}
	}}
	
	expect(t, "face");
	expect(t, "list");
	expect(t,":");
	
    mesh.mElems->allocate(mesh.numElems * 3 * sizeof(U32), CIndexBuffer::STATIC, false);
	mesh.numFrames = 1;
	
    U32 *elems = (U32 *)mesh.mElems->map();
	{for(S32 i = 0 ; i < mesh.numElems ; i++){
		t.skipToToken("face");
		t.nextToken();
		expect(t, ":");
		for(S32 j = 0; j < 3; j++){
			p = getValuePair(t);
			U32 conv = (U32)p.second;
			if(p.first == "a") elems[i * 3] = conv;
			else if(p.first == "b") elems[i * 3 + 1] = conv;
			else if(p.first == "c") elems[i * 3 + 2] = conv;
			else
				throw CTokenizerException(t, "Unexpected token '" + p.first + "'");
		}
	}}
    mesh.mElems->unmap();
	
	mesh.makeRElems();
}

void CMeshLoader3DS::expect(CTokenizer &t, const string &token){
	t.nextToken();
	if(t.getToken() != token)
		throw CTokenizerException(t, "Expected '" + token + "'");
}

pair<string, F32> CMeshLoader3DS::getValuePair(CTokenizer &t){
	pair<string, F32> ret;
	
	t.nextToken();
	ret.first = t.getToken();
	
	expect(t, ":");
	
	t.nextToken();
	ret.second = atof(t.getToken().c_str());
	
	return ret;
}
