/***************************************************************************
                         texturefont.cpp  -  description
                             -------------------
    begin                : Tue Aug 10 2004
    copyright            : (C) 2004 by Harry Kalogirou
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

#include <engine.h>
#include <vfs.h>
#include <tokenizer.h>
#include <texturefont.h>

CTextureFont::CTextureFont(){
}

CTextureFont::~CTextureFont(){
	GlyphVector::iterator i;

	for(i = mGlyphs.begin() ; i != mGlyphs.end() ; i++){
		delete *i;
	}

	mGlyphs.clear();
}

CTextureFont::CTextureFont(const string &n) : CFont(n) {
    
}


void CTextureFont::makeFont(){
    CFileText f(name);
    f.loadData();

    string buffer = (char*)f.getData();
    CTokenizerNoComments t(buffer);

    mGlyphs.resize(255, 0);

    S32 code;
    S32 x,y,w,h;
    string shader;
    bool error = false;
	while(t.nextToken()){
		string token = t.getToken();
        sscanf(token.c_str(), "%d", &code);
        if(code < 0){
            error = true;
            break;
        }
        t.nextToken();
        token = t.getToken();
        sscanf(token.c_str(), "%d", &x);

        t.nextToken();
        token = t.getToken();
        sscanf(token.c_str(), "%d", &y);

        t.nextToken();
        token = t.getToken();
        sscanf(token.c_str(), "%d", &w);

        t.nextToken();
        token = t.getToken();
        sscanf(token.c_str(), "%d", &h);
        if(x < 0 || y < 0 || w < 0 || h < 0){
            error = true;
            break;
        }
        t.nextToken();
        token = t.getToken();
        shader = token.c_str();

        addGlyph(code, x, y, w, h, shader);
    }

    if(error){
        gcon.printf("Error loading font\n");
        return;
    }
}


void CTextureFont::addGlyph(S32 code, S32 x, S32 y, S32 w, S32 h, const string &shadername){
    if(code > 255)
        return;

    CTexCoord start((F32)x / 512.0, (F32)y / 512.0);
	CTexCoord end((F32)(x + w) / 512.0, (F32)(y + h) / 512.0);

    CTextureGlyph *g = new CTextureGlyph(CEngine::getSingleton()->mMaterials.registerMaterial(shadername), 
                        start, end, CVector2(0,0), CVector2(0,0), end - start);

    mGlyphs[code] = g;
}

void CTextureFont::renderChar(U32 ch, U32 nextCh){
	CTextureGlyph *g = mGlyphs[ch];

	if(!g){
		gcon.printf("No glyph for '%c' (%d)\n", ch, ch);
		return;
	}

    CVector2 delta((g->mEnd - g->mStart) * getFontSize());
    CEngine::getSingleton()->renderer->drawPic((pen.x) * getFontSize(), (pen.y) * getFontSize(), delta.x, delta.y, g->mShader,
		g->mStart.x, g->mStart.y, g->mEnd.x, g->mEnd.y);

	pen.x += g->mAdvance.x;
	//pen.y += g->mAdvance.y;
}

void CTextureFont::stringSize(const std::string &str, F32 *w, F32 *h){
	*w = *h = 0.0;
    F32 maxw = 0.0;
    S32 lines = 1;
	for(S32 i = 0 ; i < str.size() ; i++){
        if(str[i] == '\n'){
            *w = 0.0;
            ++lines;
            continue;
        }
        //gcon.printf("Scan code : %c %d\n", str[i], str[i]);
		CTextureGlyph *g = mGlyphs[str[i]];
		if(!g)
			continue;

		*w += g->mAdvance.x;
        if(*w > maxw)maxw = *w;
	}
    *w = maxw * getFontSize();
	*h = lines * 50 * getFontSize(); //convert points to pixels
}

void CTextureFont::renderString(const std::string &str, F32 x, F32 y, F32 size){
	F32 w,h;

	if(size == 0.0)size = 512.0;
	setFontSize(size);
	stringSize(str, &w, &h);
	if(x != -9999.0 && y != -9999.0){
		pen.x = x;
		pen.y = y + 16;
	}
    pen /= getFontSize();
	F32 startx = 0;
	
	for(S32 i = 0 ; i < str.size() ; i++){
        if(str[i] == '\n'){
            pen.x = startx;
            pen.y += 50.0;
        } else
            renderChar(str[i]);
	}
    CEngine::getSingleton()->renderer->renderOverlay2D();
}

