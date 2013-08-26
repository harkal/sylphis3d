/***************************************************************************
                          smfimp.h  -  description
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

class SMFImport : public SceneImport {
public:
					SMFImport();
					~SMFImport();
	int				ExtCount();					// Number of extensions supported
	const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
	const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	const TCHAR *	AuthorName();				// ASCII Author name
	const TCHAR *	CopyrightMessage();			// ASCII Copyright message
	const TCHAR *	OtherMessage1();			// Other message #1
	const TCHAR *	OtherMessage2();			// Other message #2
	unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
	void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box
	int				DoImport(const TCHAR *name,ImpInterface *i,Interface *gi, BOOL suppressPrompts=FALSE);	// Import file
};


// Handy file class

class WorkFile {
private:
	FILE *stream;
public:
	WorkFile(const TCHAR *filename,const TCHAR *mode) { stream = _tfopen(filename,mode); };
	~WorkFile() { if(stream) fclose(stream); stream = NULL; };
	FILE *Stream() { return stream; };
};
