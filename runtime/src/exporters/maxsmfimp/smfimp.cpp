/***************************************************************************
                         smfimp.cpp  -  description
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

#include "Max.h"
#include <stdio.h>
#include <direct.h>
#include <commdlg.h>
#include "smfimp.h"
#include "smfires.h"
#include "SMFileIO.h"

HINSTANCE hInstance;

TCHAR *GetString(int id){
	static TCHAR buf[256];
	if (hInstance)
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	return NULL;
}

// Jaguar interface code

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) {
	hInstance = hinstDLL;
	switch(fdwReason) {
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
		}
	return(TRUE);
	}


//------------------------------------------------------

class SMFClassDesc:public ClassDesc {
	public:
	int 			IsPublic() { return 1; }
	void *			Create(BOOL loading = FALSE) { return new SMFImport; }
	const TCHAR *	ClassName() { return "SMF importer"; }
	SClass_ID		SuperClassID() { return SCENE_IMPORT_CLASS_ID; }
	Class_ID		ClassID() { return Class_ID(0x2822570b, 0x16d14531); }
	const TCHAR* 	Category() { return "SMF importer";  }
};

static SMFClassDesc SMFDesc;

__declspec( dllexport ) const TCHAR *
LibDescription() { return _T("SMF import plugin"); }

__declspec( dllexport ) int
LibNumberClasses() { return 1; }

__declspec( dllexport ) ClassDesc *
LibClassDesc(int i) {
	switch(i) {
		case 0: return &SMFDesc; break;
		default: return 0; break;
	}
}

__declspec( dllexport ) ULONG 
LibVersion() { return VERSION_3DSMAX; }

__declspec( dllexport ) ULONG CanAutoDefer(){
	return 1;
}

SMFImport::SMFImport() {
}

SMFImport::~SMFImport() {
}

int SMFImport::ExtCount() {
	return 1;
}

const TCHAR * SMFImport::Ext(int n) {
	switch(n) {
	case 0:
		return _T("SMF");
	}
	return _T("");
}

const TCHAR *SMFImport::LongDesc() {
	return _T("SMF Simple Mesh File");
}
	
const TCHAR *SMFImport::ShortDesc() {
	return _T("SMF");
}

const TCHAR *SMFImport::AuthorName() {
	return _T("Harry Kalogirou");
}

const TCHAR *SMFImport::CopyrightMessage() {
	return _T("Copywrite (c) 2004");
}

const TCHAR *SMFImport::OtherMessage1() {
	return _T("");
}

const TCHAR *SMFImport::OtherMessage2() {
	return _T("");
}

unsigned int SMFImport::Version() {
	return 100;
}

void SMFImport::ShowAbout(HWND hWnd) {
}

BOOL objFileRead(const TCHAR *filename, ImpInterface *imp){

	SMFile smf, out;
	smf.loadData(filename);

	TriObject *object = CreateNewTriObject();
	if(!object)
		return 0;
	
	Mesh *msh = &object->GetMesh();

	msh->setNumVerts(smf.getVerticesCount());

	for(unsigned int i = 0 ; i < smf.getVerticesCount() ; ++i){
		SMFile::TVertex v = smf.getVertex(i);
		msh->setVert(i, v.x, v.y, v.z);
	}

	msh->setNumFaces(smf.getTrianglesCount(), TRUE);
	for(unsigned int i = 0 ; i < smf.getTrianglesCount() ; ++i){
		SMFile::TTriangle t = smf.getTriangle(i);
		msh->faces[i].setVerts(t.a, t.b, t.c);
		msh->faces[i].setEdgeVisFlags(1, 1, 1);
	}

	msh->buildNormals();
	msh->buildBoundingBox();
	msh->InvalidateEdgeList();

	ImpNode *node = imp->CreateNode();
	if(!node) {
		delete object;
		return 0;
	}
	Matrix3 tm;
	tm.IdentityMatrix();
	node->Reference(object);
	node->SetTransform(0,tm);
	imp->AddNodeToScene(node);
	node->SetName(_T("simple mesh file"));


    return TRUE;
}

int SMFImport::DoImport(const TCHAR *filename,ImpInterface *i,Interface *gi, BOOL suppressPrompts) {
	
	if(!objFileRead(filename, i))
		return 0;

	i->RedrawViews();
	return 1;
}

