/***************************************************************************
                          shadowvolume.cpp  -  description
                             -------------------
    begin                : ??? ??? 6 2003
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

#include <shadowvolume.h>
#include <cmapscene/cmap.h>

CShadowVolume::CShadowVolume(){
}
CShadowVolume::~CShadowVolume(){
}

void CShadowVolume::makeVertexBuffers(CMAP *cm, const CVector4 lightPos, F32 radius){
    dface_t *face;
    vector<S32>::iterator f;
    CVector4 temp;
    S32 polynum = 0;

    S32 pad = 0;
    std::vector<CVector4> verts;
    for(f = mPolygons.begin() ; f != mPolygons.end() ; ++f, ++polynum){
        face = &cm->Faces[*f];

        S32 off = face->numverts;
        S32 i;

        for(i = face->numverts -1 ; i > 1 ; i--){
            mElements.push_back(i+ pad);
            mElements.push_back(i - 1 + pad);
            mElements.push_back(0 + pad);
        }
        for(i = face->firstvert; i < face->firstvert + face->numverts; i++){
            temp = cm->Verts[i].point;
            verts.push_back(temp);
        }

        for(i = face->numverts -1 ; i > 1 ; i--){
            mElements.push_back(off + 0 + pad);
            mElements.push_back(off+i-1 + pad);
            mElements.push_back(off+i + pad);
        }
        for(i = face->firstvert; i < face->firstvert + face->numverts; i++){
#if 1
            temp = - lightPos + cm->Verts[i].point;// - pos;
            normalize(temp);
            temp = temp * (radius * 20) + cm->Verts[i].point;
            temp.w = 1.0;
#else
            temp = cm->Verts[i].point;// - pos;
            temp.w = 0.0;
#endif
            verts.push_back(temp);
        }

        vector<S32>::iterator end = mUsedEdges[polynum].end();
        for(vector<S32>::iterator it = mUsedEdges[polynum].begin() ; it != end ; ++it){
            i = *it;
            S32 nexti = (i + 1) % face->numverts;
            mElements.push_back(off + i + pad);
            mElements.push_back(i + pad);
            mElements.push_back(off + nexti + pad);

            mElements.push_back(off + nexti + pad);
            mElements.push_back(i + pad);
            mElements.push_back(nexti + pad);
        }
        pad = verts.size();
    }

    if(verts.size() == 0)
        return;

    mVertices = new CVertexBuffer();
    mVertices->addComponent(CVertexBuffer::POSITION);
    mVertices->allocate(verts.size() * sizeof(CVector4), CVertexBuffer::STATIC, false, &verts[0].x);
}
