
#include "Max.h"
#include <stdio.h>
#include <direct.h>
#include <commdlg.h>
#include "cmapimp.h"
#include "cmapires.h"
#include "cmapfile.h"

// The file stream


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
			//MessageBox(NULL,L"OBJIMP.DLL: DllMain",L"OBJIMP",MB_OK);
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

class CMAPClassDesc:public ClassDesc {
	public:
	int 			IsPublic() { return 1; }
	void *			Create(BOOL loading = FALSE) { return new CMAPImport; }
	const TCHAR *	ClassName() { return "CMAP importer"; }
	SClass_ID		SuperClassID() { return SCENE_IMPORT_CLASS_ID; }
	Class_ID		ClassID() { return Class_ID(1,0); }
	const TCHAR* 	Category() { return "CMAP importer";  }
	};

static CMAPClassDesc CMAPDesc;

//------------------------------------------------------
// This is the interface to Jaguar:
//------------------------------------------------------

__declspec( dllexport ) const TCHAR *
LibDescription() { return _T("CMAP export plugin"); }

__declspec( dllexport ) int
LibNumberClasses() { return 1; }

__declspec( dllexport ) ClassDesc *
LibClassDesc(int i) {
	switch(i) {
		case 0: return &CMAPDesc; break;
		default: return 0; break;
	}
}

// Return version so can detect obsolete DLLs
__declspec( dllexport ) ULONG 
LibVersion() { return VERSION_3DSMAX; }

// Let the plug-in register itself for deferred loading
__declspec( dllexport ) ULONG CanAutoDefer(){
	return 1;
}

CMAPImport::CMAPImport() {
}

CMAPImport::~CMAPImport() {
}

int CMAPImport::ExtCount() {
	return 1;
}

const TCHAR * CMAPImport::Ext(int n) {		// Extensions supported for import/export modules
	switch(n) {
	case 0:
		return _T("CMAP");
	}
	return _T("");
}

const TCHAR *CMAPImport::LongDesc() {			// Long ASCII description (i.e. "Targa 2.0 Image File")
	return _T("CMAP Sylphis Map File");
}
	
const TCHAR *CMAPImport::ShortDesc() {			// Short ASCII description (i.e. "Targa")
	return _T("CMAP");
}

const TCHAR *CMAPImport::AuthorName() {			// ASCII Author name
	return _T("Harry Kalogirou");
}

const TCHAR *CMAPImport::CopyrightMessage() {	// ASCII Copyright message
	return _T("Copywrite (c) 2003");
}

const TCHAR *CMAPImport::OtherMessage1() {		// Other message #1
	return _T("");
}

const TCHAR *CMAPImport::OtherMessage2() {		// Other message #2
	return _T("");
}

unsigned int CMAPImport::Version() {				// Version number * 100 (i.e. v3.01 = 301)
	return 100;
}

void CMAPImport::ShowAbout(HWND hWnd) {			// Optional
}

BOOL objFileRead(const TCHAR *filename, ImpInterface *imp){
	FILE *fp;
    int i;
	tLump  *idxtable;

	if((fp = fopen(filename, "rb")) == NULL)
		return 0;

	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *data = new char[fileSize];
	fread(data, fileSize, 1, fp);

	tHeader *header = (tHeader *)data;
	if(*(int*)header->strID != CMAPHEADER_ID)
		return 0;

	if(header->version != CMAPHEADER_VERSION)
		return 0;

	idxtable = (tLump *)header + 1;

	tVertex *Verts = (tVertex *)(data + idxtable[kVertices].offset);	
	int NumVerts = idxtable[kVertices].length / sizeof(tVertex);

	tFace *faces = (tFace *)(data + idxtable[kLeafFaces].offset);	
	int NumFaces = idxtable[kLeafFaces].length / sizeof(tFace);

	int *elems = (int *)(data + idxtable[kElements].offset);	
	int NumElems = idxtable[kElements].length / sizeof(int);

	tArea *mAreas = (tArea *)(data + idxtable[kAreas].offset);
    int NumAreas = idxtable[kAreas].length / sizeof(tArea);

	Point3 p;

	TriObject *object = CreateNewTriObject();
	if(!object)
		return 0;
	
	Mesh *msh = &object->GetMesh();

	msh->setNumVerts(NumVerts);

	for(i = 0 ; i < NumVerts ; i++){
		msh->setVert(i, Verts[i].point[0], -Verts[i].point[2], Verts[i].point[1]);
		p.Set(Verts[i].norm[0], -Verts[i].norm[2], Verts[i].norm[1]);
		msh->setNormal(i, p);
	}

	int face = 0;
	for(int k = 1 ; k < NumAreas ; k++){
		for(i = mAreas[k].startFace ; i < mAreas[k].startFace + mAreas[k].numOfFaces ; i++){
			for(int j = faces[i].startElement ; j < faces[i].startElement + faces[i].elementsSize ; j++){
				int k = j * 3;
				if(elems[k] < 0){
					elems[k] = 0;
					elems[k+1] = 1;
					elems[k+2] = 2;
					faces[i].vertexIndex = 0;
				}
				msh->setNumFaces(face + 1, TRUE);
				msh->faces[face].setVerts(faces[i].vertexIndex + elems[k + 0], 
											faces[i].vertexIndex + elems[k + 1], 
											faces[i].vertexIndex + elems[k + 2]);
				msh->faces[face].setEdgeVisFlags(1, 1, 1);
				face++;
			}
		}
	}
	
	delete [] data;

	fclose(fp);
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
	node->SetName(_T("Sylphis map"));


    return TRUE;
}

int
CMAPImport::DoImport(const TCHAR *filename,ImpInterface *i,Interface *gi, BOOL suppressPrompts) {
	
	if(!objFileRead(filename, i))
		return 0;

/*	GenLight *light = i->CreateLightObject(OMNI_LIGHT);
	
	ImpNode *light_node = i->CreateNode();
	Matrix3 tm;
	tm.IdentityMatrix();
	light_node->Reference(light);
	light_node->SetTransform(0, tm);
	i->AddNodeToScene(light_node);
	light_node->SetName(_T("light"));
*/	
	i->RedrawViews();
	return 1;
}

