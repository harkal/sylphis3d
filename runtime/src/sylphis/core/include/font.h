/***************************************************************************
                          font.h  -  description
                             -------------------
    begin                : Tue Oct 1 2002
    copyright            : (C) 2002 by Harry Kalogirou
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

#ifndef FONT_H
#define FONT_H

#include <resourcemanager.h>
#include <vector.h>
#include <vector>
#include <string>
#include <ftlibrary.h>

SmartPointer(CFont);
class CFont : public CResource {
public: 
	CFont();
	CFont(const string &n);
	virtual ~CFont();

	virtual void makeFont();

	virtual void renderChar(U32 ch, U32 nextCh = 0);

	virtual void stringSize(const std::string &str, F32 *w, F32 *h);
	virtual void renderString(const std::string &str, F32 x = -9999.0, F32 y = -9999.0, F32 size = 0.0);

	virtual F32 getFontSize();
	virtual void setFontSize(F32 size);

	CVector2 pen;
protected:
	F32 mFontSize;
};

class CBitmapGlyph {
public:
	CBitmapGlyph(U32 textureId, const CTexCoord &start, const CTexCoord &end, const CVector2 &pos, const CVector2 &size, const CVector2 &advance)
		: mTextureId(textureId), mStart(start), mEnd(end), mPos(pos), mSize(size), mAdvance(advance) {};
	~CBitmapGlyph(){};

	U32 mTextureId;
	CVector2 mPos;
	CVector2 mSize;
	CVector2 mAdvance;
	CTexCoord mStart;
	CTexCoord mEnd;
};

SmartPointer(CBitmapFont);
class CBitmapFont : public CFont {
public:
	CBitmapFont();
	CBitmapFont(const string &n, F32 size = 16.0); 
	virtual ~CBitmapFont();

	virtual void renderChar(U32 ch, U32 nextCh = 0);
	virtual void stringSize(const std::string &str, F32 *w, F32 *h);
	virtual void renderString(const std::string &str, F32 x = -9999.0, F32 y = -9999.0, F32 size = 0.0);
    virtual void makeFont();

protected:
	U32 newTexture();
	void generateGlyph(U32 ch);

	typedef std::vector<CBitmapGlyph*> GlyphVector;
	GlyphVector mGlyphs;
	std::vector<U32> mTextures;
	S32 mTextureSize;
	FT_Face mFace;
	void *file;

	S32 curTex;
};

class CFonts : public CResourceManager {
public:
	CFonts();
	~CFonts();

	void registerFont(CFont *f);
	CFont* registerFont(const string &fontName);
	CFont* registerBitmapFont(const string &fontName, S32 size = 17);
    CFont* registerTextureFont(const string &fontName);

	CFont* findFont(const string &name);
};

extern CFont gDefaultFont;

#endif
