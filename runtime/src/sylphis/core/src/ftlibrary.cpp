/***************************************************************************
                         ftlibrary.cpp -  description
                             -------------------
    begin                : Sat Mar 15 2003
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

#include <ftlibrary.h>
#include <exception.h>
#include <vfs.h>

//CFTLibrary ftlib;

CFTLibrary::CFTLibrary() {
	S32 error = FT_Init_FreeType(&mLibrary);
    if ( error ){
		throw CException("Failed to initialize freetype.");
	}
}

CFTLibrary::~CFTLibrary(){
	FT_Done_FreeType(mLibrary);
}

void *CFTLibrary::loadFontFace(const std::string &fname, FT_Face *face){
	CFile f(fname);
	f.loadData();

	FT_Byte *data = (FT_Byte *)f.getDataCopy();
	if(FT_New_Memory_Face(mLibrary, data, f.getSize(), 0, face)){
		free(data);
		throw CException("Failed to load font face.");
	}

	if(FT_Set_Char_Size(
              *face,    /* handle to face object           */
              0,       /* char_width in 1/64th of points  */
              16*64,   /* char_height in 1/64th of points */
              0,     /* horizontal device resolution    */
			  0 ))   /* vertical device resolution      */ {

		doneFontFace(face);
		free(data);
		throw CException("Failed to set char size.");
	}

    return (void *)data;
}

void CFTLibrary::doneFontFace(FT_Face *face){
	FT_Done_Face(*face);
}
