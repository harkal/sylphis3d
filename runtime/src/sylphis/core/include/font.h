/***************************************************************************
                          font.h  -  description
                             -------------------
    begin                : Tue Oct 1 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
  ***************************************************************************
    This file is part of "Sylphis3D Game Engine".

    "Sylphis3D Game Engine" is free software; you can redistribute it and/or 
    modify it under the terms of the GNU General Public License as 
    published by the Free Software Foundation; either version 2 of the 
    License, or (at your option) any later version, 
    plus a special exception:
      Linking "Sylphis3D Game Engine" statically or dynamically with other 
    modules is making a combined work based on this library. Thus, the 
    terms and conditions of the GNU General Public License cover the whole 
    combination. As a special exception, the copyright holders of this 
    library give you permission to link this library with independent 
    modules to produce an executable, regardless of the license terms of 
    these independent modules, and to copy and distribute the resulting 
    executable under terms of your choice, provided that you also meet, 
    for each linked independent module, the terms and conditions of the 
    license of that module. An independent module is a module which is 
    not derived from or based on this program. If you modify this program,
    you may extend this exception to your version of the library, but you 
    are not obligated to do so. If you do not wish to do so, delete this 
    exception statement from your version.

    "Sylphis3D Game Engine" is distributed in the hope that it will be 
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with "Sylphis3D Game Engine"; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
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
