
#ifndef __EXPORTSMF_H
#define __EXPORTSMF_H

#include "Max.h"
#include "resource.h"
#include "istdplug.h"

#include "iparamb2.h"
#include "iparamm2.h"

#include "stdmat.h"

#include <direct.h>
#include <commdlg.h>

#include <list>
#include <string>

extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;

int	DoExportSMF(const TCHAR *name,ExpInterface *ei, Interface *gi,
						BOOL suppressPrompts, DWORD options);

#endif
