/***************************************************************************
                        DoExportSMF.cpp  -  description
                             -------------------
    begin                : 5 Jan 2004
    copyright            : (c) 2004 by Harry Kalogirou
    email                : harkal@gmx.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <list>
#include <vector>

#include "ExportSMF.h"
#include "SMFileIO.h"

class MySceneEnum : public ITreeEnumProc {
public:
	MySceneEnum(IScene *scene, bool exportSelected){
		mExportSelected = exportSelected;
		scene->EnumTree(this);
	}
	virtual int callback(INode *node){
		if(mExportSelected && node->Selected() == FALSE)
			return TREE_CONTINUE;

		Object *obj = node->GetObjectRef();

		if(obj->CanConvertToType(triObjectClassID)){
			mNodes.push_back(node);
		}
		return TREE_CONTINUE;
	}

	INode *operator[](int index){
		return mNodes[index];
	}

	int	getNodeCount() { 
		return mNodes.size(); 
	}

protected:
	std::vector<INode *> mNodes;
	bool mExportSelected;
};

int	DoExportSMF(const TCHAR *filename,ExpInterface *ei, Interface *gi,
				BOOL suppressPrompts, DWORD options){

	MySceneEnum checkScene(ei->theScene, (options & SCENE_EXPORT_SELECTED) ? true : false);
	if( !checkScene.getNodeCount() ){
		MessageBox( GetActiveWindow(), "No data to export", "No data error", MB_OK);
		return TRUE;
	}
	if( checkScene.getNodeCount() > 1){
		MessageBox( GetActiveWindow(), "Select only one object", "Error", MB_OK);
		return TRUE;
	}

	HWND progressDialog = CreateDialog( hInstance, MAKEINTRESOURCE(IDD_EXPORTING), GetActiveWindow(), 0 );
	//
	// FIXME : Handle multiple objects. Possibly by creating several files based on
	//         the main file name.
	//
	for(int i = 0; i < checkScene.getNodeCount(); ++i ){
		INode *node	= checkScene[i];
		bool remove;
		TriObject *tri;
		Object *obj = node->GetObjectRef();
		if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) { 
			tri = (TriObject *) obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));
		} else {
			continue;
		}
		Mesh &mesh = tri->GetMesh();

		if( mesh.numFaces == 0 )
			continue;

		SMFile smf;
		for( i = 0; i < mesh.getNumVerts(); i++ ){
			Point3 n = mesh.getVert( i );
			SMFile::TVertex v;
			v.x = n.x;
			v.y = n.y;
			v.z = n.z;
			smf.addVertex(v);
		}
		for( i = 0; i < mesh.getNumFaces(); i++ ){
			SMFile::TTriangle t;
			t.a = mesh.faces[i].getVert( 0 );
			t.b = mesh.faces[i].getVert( 1 );
			t.c = mesh.faces[i].getVert( 2 );
			smf.addTriangle(t);
            if (mesh.getNumMaps() > 0){
                for( int j = 0; j < 3; j++ ){
					int _v	= mesh.faces[i].getVert( j );
					int _tv	= mesh.tvFace[i].t[j];

                    smf.mVertices[_v].u = (mesh.getTVert(_tv)).x;
                    smf.mVertices[_v].v = (mesh.getTVert(_tv)).y;
                }
            }
		}

		smf.writeData(filename);

		if( obj != tri )
			tri->DeleteThis();
	}

	if( progressDialog )
		DestroyWindow( progressDialog );

	return TRUE;
}


