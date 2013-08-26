/***************************************************************************
                         texturefont.h  -  description
                             -------------------
    begin                : Tue Aug 10 2004
    copyright            : (C) 2004 by Harry Kalogirou
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

#ifndef TEXTUREFONT_H
#define TEXTUREFONT_H

#include <font.h>

SmartPointer(CMaterial);

class CTextureGlyph {
public:
	CTextureGlyph(CMaterial *shader, const CTexCoord &start, const CTexCoord &end, const CVector2 &pos, const CVector2 &size, const CVector2 &advance)
		: mShader(shader), mStart(start), mEnd(end), mPos(pos), mSize(size), mAdvance(advance) {};
	~CTextureGlyph(){};

	CMaterialPtr mShader;
	CVector2 mPos;
	CVector2 mSize;
	CVector2 mAdvance;
	CTexCoord mStart;
	CTexCoord mEnd;
};

SmartPointer(CTextureFont);
class CTextureFont : public CFont {
public:
	CTextureFont();
	CTextureFont(const string &n); 
	virtual ~CTextureFont();

	virtual void renderChar(U32 ch, U32 nextCh = 0);
	virtual void stringSize(const std::string &str, F32 *w, F32 *h);
	virtual void renderString(const std::string &str, F32 x = -9999.0, F32 y = -9999.0, F32 size = 0.0);
    virtual void makeFont();
protected:
    void addGlyph(S32 code, S32 x, S32 y, S32 w, S32 h, const string &shadername);

	typedef std::vector<CTextureGlyph*> GlyphVector;
	GlyphVector mGlyphs;
	std::vector<CMaterialPtr> mShaders;
};

#endif
