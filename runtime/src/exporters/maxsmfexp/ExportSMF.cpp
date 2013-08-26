/***************************************************************************
                        ExportSMF.cpp  -  description
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

#include "ExportSMF.h"

class ExportSMF : public SceneExport {
	public:
		static HWND hParams;

		int	ExtCount();
		const TCHAR *Ext(int n);
		const TCHAR *LongDesc();
		const TCHAR *ShortDesc();
		const TCHAR *AuthorName();
		const TCHAR *CopyrightMessage();
		const TCHAR *OtherMessage1();
		const TCHAR *OtherMessage2();
		unsigned int Version();
		void ShowAbout(HWND hWnd);
		int	DoExport(const TCHAR *name,ExpInterface *ei,Interface *gi, BOOL suppressPrompts=FALSE, DWORD options=0);	// Export file
		BOOL SupportsOptions(int ext, DWORD options);

		ExportSMF();
		~ExportSMF();		
};

class ExportSMFClassDesc:public ClassDesc2 {
public:
	int	IsPublic() {return 1;}
	void *Create(BOOL loading = FALSE) {return new ExportSMF();}
	const TCHAR *ClassName() {return GetString(IDS_CLASS_NAME);}
	SClass_ID SuperClassID() {return SCENE_EXPORT_CLASS_ID;}
	Class_ID ClassID() {return Class_ID(1,0);}
	const TCHAR* Category() {return GetString(IDS_CATEGORY);}
	const TCHAR* InternalName() { return _T("ExportSMF"); }	
	HINSTANCE HInstance() { return hInstance; }
};

static ExportSMFClassDesc ExportSMFDesc;
ClassDesc2* GetExportSMFDesc() {
	return &ExportSMFDesc;
}

ExportSMF::ExportSMF(){

}

ExportSMF::~ExportSMF(){

}

int ExportSMF::ExtCount(){
	return 1;
}

const TCHAR *ExportSMF::Ext(int n){		
	return _T("SMF");
}

const TCHAR *ExportSMF::LongDesc(){
	return _T("Simple Mesh File");
}
	
const TCHAR *ExportSMF::ShortDesc(){			
	return _T("SMF");
}

const TCHAR *ExportSMF::AuthorName(){			
	return _T("Harry Kalogirou");
}

const TCHAR *ExportSMF::CopyrightMessage(){	
	return _T("(c) 2002 Harry Kalogirou");
}

const TCHAR *ExportSMF::OtherMessage1(){		
	return _T("");
}

const TCHAR *ExportSMF::OtherMessage2(){		
	return _T("");
}

unsigned int ExportSMF::Version(){				
	return 010;
}

void ExportSMF::ShowAbout(HWND hWnd){			
	
}

int	ExportSMF::DoExport(const TCHAR *name,ExpInterface *ei, Interface *gi,
						BOOL suppressPrompts, DWORD options){

	return DoExportSMF( name, ei, gi, suppressPrompts, options );
}
	
BOOL ExportSMF::SupportsOptions(int ext, DWORD options){
	return(options == SCENE_EXPORT_SELECTED) ? TRUE : FALSE;
}


