/***************************************************************************
                          shadowvolume.cpp  -  description
                             -------------------
    begin                : ??? ??? 6 2003
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
