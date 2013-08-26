/***************************************************************************
                         DllEntry.cpp  -  description
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

extern ClassDesc2* GetExportSMFDesc();

HINSTANCE hInstance;
int controlsInit = FALSE;

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved){
	hInstance = hinstDLL;

	if (!controlsInit) {
		controlsInit = TRUE;
		InitCustomControls(hInstance);
		InitCommonControls();
	}
			
	return (TRUE);
}

__declspec( dllexport ) const TCHAR* LibDescription(){
	return GetString(IDS_LIBDESCRIPTION);
}

__declspec( dllexport ) int LibNumberClasses(){
	return 1;
}

__declspec( dllexport ) ClassDesc* LibClassDesc(int i){
	switch(i) {
		case 0: return GetExportSMFDesc();
		default: return 0;
	}
}

__declspec( dllexport ) ULONG LibVersion(){
	return VERSION_3DSMAX;
}

TCHAR *GetString(int id){
	static TCHAR buf[256];

	if (hInstance)
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	return NULL;
}

