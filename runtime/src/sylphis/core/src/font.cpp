/***************************************************************************
                          font.cpp  -  description
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

#include <string>
#include <font.h>
#include <texturefont.h>
#include <ftlibrary.h>
#include <exception.h>
#include <rendersystem.h>

CFont::CFont(){
}

CFont::CFont(const string &n){
	name = n;
}

CFont::~CFont(){
}

void CFont::makeFont(){

}

void CFont::renderChar(U32 ch, U32 nextCh){

}

void CFont::stringSize(const std::string &str, F32 *w, F32 *h){

}

void CFont::renderString(const std::string &str, F32 x, F32 y, F32 size){

}

F32 CFont::getFontSize(){
	return mFontSize;
}

void CFont::setFontSize(F32 size){
	mFontSize = size;
}

//
// CBitmapFont
//

#include <renderer.h>
#include FT_GLYPH_H

const F32 dpi = 60.0;

CBitmapFont::CBitmapFont(){
	file = 0;
}

CBitmapFont::~CBitmapFont(){
	if(file)
		free(file);

	GlyphVector::iterator i;

	for(i = mGlyphs.begin() ; i != mGlyphs.end() ; i++){
		delete *i;
	}

	mGlyphs.clear();

	std::vector<U32>::iterator j;
	
	glDeleteTextures(mTextures.size(), &mTextures[0]);
}

CBitmapFont::CBitmapFont(const string &n, F32 size) : CFont(n) {
    setFontSize(size);
}

void CBitmapFont::generateGlyph(U32 ch){
	S32 pad = 3;
	FT_GlyphSlot  slot = mFace->glyph;
	FT_Glyph glyph;
    FT_UInt glyph_index;

	glyph_index = FT_Get_Char_Index( mFace, (FT_ULong)ch );

	FT_Load_Glyph( mFace, glyph_index, FT_LOAD_DEFAULT);
	FT_Get_Glyph( slot, &glyph);

	FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0 ,1);

	FT_BitmapGlyph bitmap = (FT_BitmapGlyph)glyph;
	FT_Bitmap* source = &bitmap->bitmap;


	S32 srcWidth = source->width;
	S32 srcHeight = source->rows;
	S32 srcPitch = source->pitch;

	S32 dstWidth = srcWidth;
	S32 dstHeight = srcHeight;
	S32 dstPitch = srcPitch;

	FT_BBox bbox;
	FT_Glyph_Get_CBox( glyph, ft_glyph_bbox_pixels, &bbox);

	unsigned char *src = source->buffer;

	if(pen.x + srcWidth >= mTextureSize){
		pen.x = 0;
		pen.y += (getFontSize() * 64.0f / dpi) + pad;
	}

	glBindTexture(GL_TEXTURE_2D, curTex);checkGLError();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);checkGLError();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 
		pen.x, pen.y,
		srcWidth, srcHeight,
		GL_ALPHA, GL_UNSIGNED_BYTE, src); checkGLError();

	CTexCoord start(pen);
	start /= mTextureSize;
	CTexCoord end(pen.x + srcWidth, pen.y + srcHeight);
	end /= mTextureSize;
	//CVector2 pos(slot->bitmap_left, slot->bitmap_top);
	CVector2 pos(bitmap->left, srcHeight - bitmap->top);
	CVector2 size(srcWidth, srcHeight);
	CVector2 advance(slot->advance.x >> 6, slot->advance.y >> 6);
	
	mGlyphs[glyph_index] = new CBitmapGlyph(curTex, start, end, pos, size, advance);

	pen.x += srcWidth + pad;

	FT_Done_Glyph(glyph);
}

void CBitmapFont::makeFont(){
	//gcon.printf("makeFont() : making font %s_%f\n", name.c_str(), getFontSize());
	file = CFTLibrary::getSingleton()->loadFontFace(name, &mFace);
	//size = 17;
	if(FT_Set_Char_Size(
              mFace,    /* handle to face object           */
              0,       /* char_width in 1/64th of points  */
              (S32)getFontSize()*64,   /* char_height in 1/64th of points */
              0,     /* horizontal device resolution    */
			  0 ))   /* vertical device resolution      */ {

		throw CException("Failed to set char size.");
	}

	if(FT_Select_Charmap( mFace, ft_encoding_unicode ))
		throw CException("Non unicode font");

	mGlyphs.resize(mFace->num_glyphs, 0);

	mTextureSize = 256;

	pen.x = pen.y = 0;

	curTex = newTexture();

	for(S32 i = 0 ; i < 130 ; ++i){
		generateGlyph(i);	
	}

	//CFTLibrary::getSingleton()->doneFontFace(&mFace);
}

U32 CBitmapFont::newTexture(){
	char *data = new char[mTextureSize * mTextureSize];
	memset(data, 0, mTextureSize * mTextureSize);
	
	GLuint texId;
	glGenTextures(1, &texId);
	mTextures.push_back(texId);

	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 
		mTextureSize, mTextureSize, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);

	delete [] data;
	return texId;
}

void CBitmapFont::renderChar(U32 ch, U32 nextCh){
#if 0
	glBindTexture(GL_TEXTURE_2D, mTextures[0]);
	//glBindTexture(GL_TEXTURE_2D, 20);
	glEnable(GL_TEXTURE_2D);
	
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0);
	glVertex2f(50,50);
	
	glTexCoord2f(0.0,1.0);
	glVertex2f(50,400);
	
	glTexCoord2f(1.0,1.0);
	glVertex2f(400,400);
	
	glTexCoord2f(1.0,0.0);
	glVertex2f(400,50);
	
	glEnd();
#endif

	FT_UInt glyph_index;

	glyph_index = FT_Get_Char_Index( mFace, (FT_ULong)ch );

	CBitmapGlyph *g = mGlyphs[glyph_index];

	if(!g)
		return;

	glBindTexture(GL_TEXTURE_2D, g->mTextureId);
	glEnable(GL_TEXTURE_2D);

    gRSystem->pushWorldMatrix();
	glTranslatef(pen.x, pen.y, 0);
	//glScalef(2.0, 2.0, 0);
	glBegin(GL_QUADS);

	CVector2 t(/*pen +*/ g->mPos);
	glTexCoord2f(g->mStart.x, g->mEnd.y);
	glVertex2f(t.x, t.y);
	
	glTexCoord2f(g->mEnd.x, g->mEnd.y);
	glVertex2f(t.x + g->mSize.x, t.y);

	glTexCoord2f(g->mEnd.x, g->mStart.y);
	glVertex2f(t.x + g->mSize.x, t.y - g->mSize.y);
	
	glTexCoord2f(g->mStart.x, g->mStart.y);
	glVertex2f(t.x, t.y - g->mSize.y);
	
	glEnd();
	myglPopMatrix();
	pen.x += g->mAdvance.x;
	//pen.y += g->mAdvance.y;
}

void CBitmapFont::stringSize(const std::string &str, F32 *w, F32 *h){
	if(!mTextures.size())
		makeFont();
	*w = *h = 0.0;
    F32 maxw = 0.0;
    S32 lines = 1;
	for(S32 i = 0 ; i < str.size() ; i++){
		FT_UInt glyph_index;

        if(str[i] == '\n'){
            *w = 0.0;
            ++lines;
            continue;
        }
		glyph_index = FT_Get_Char_Index( mFace, (FT_ULong)str[i] );

		CBitmapGlyph *g = mGlyphs[glyph_index];

		if(!g)
			continue;

		*w += g->mAdvance.x;
        if(*w > maxw)maxw = *w;
	}
    *w = maxw;
	*h = lines * getFontSize() * 64.0 * 1.2 / dpi; //convert points to pixels
}


void CBitmapFont::renderString(const std::string &str, F32 x, F32 y, F32 size){
	if(!mTextures.size())
		makeFont();
	F32 w,h;
	stringSize(str, &w, &h);
	if(x != -9999.0 && y != -9999.0){
		pen.x = x;
		pen.y = y + 16;
	}
	F32 startx = 0;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for(S32 i = 0 ; i < str.size() ; i++){
        if(str[i] == '\n'){
            pen.x = startx;
            pen.y += getFontSize() * 64.0 * 1.4 / dpi;
        } else
            renderChar(str[i]);
	}
	glDisable(GL_BLEND);
}


CFonts::CFonts(){
}

CFonts::~CFonts(){
	freeAll();
}

void CFonts::registerFont(CFont *f){
	resources.push_back(f);
}

CFont* CFonts::registerFont(const string &fontName){
	CFont *found = findFont(fontName);
	if(found)return found;

	found = new CFont(fontName);
	found->makeFont();

	registerFont(found);
	return found;
}

CFont* CFonts::registerBitmapFont(const string &fontName, S32 size){
	char name[1024];
	//sprintf(name, "%s_%d", fontName.c_str(), size);
	//gcon.printString(name);
	CFont *found = findFont(fontName);
	if(found)return found;

	CBitmapFont *f = new CBitmapFont(fontName, size);
	// Bitmap fonts are automaked on usage...
	//f->makeFont();
	f->setName(fontName);
	registerFont(f);
	return f;
}

CFont* CFonts::registerTextureFont(const string &fontName){
	CFont *found = findFont(fontName);
	if(found)return found;

	CTextureFont *f = new CTextureFont(fontName);
	f->makeFont();

	registerFont(f);
	return f;
}

CFont* CFonts::findFont(const string &name){
	return (CFont *)find(name);
}

CFont gDefaultFont;
