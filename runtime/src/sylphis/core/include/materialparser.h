/***************************************************************************
                        materialparser.h  -  description
                             -------------------
    begin                : Sat Apr 22 2006
    copyright            : (C) 2006 by Harry Kalogirou
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

#ifndef MATERIALPARSER_H_
#define MATERIALPARSER_H_

#include <vector>
#include <set>
#include <material.h>

SmartPointer(CMaterialTokenizer);

class CMaterialParser {
public:
    CMaterialParser();
    ~CMaterialParser();

    void addMaterialToRead(const std::string &name);
	void removeMaterialToRead(const std::string &name);
    void setMaterialsToReadDirty(bool b);
    bool isMaterialsToReadDirty();
    S32  getNumOfMaterialsToRead();
    std::vector<CMaterialPtr>& getReadMaterials();
    void parseMaterials(const std::string &source);
    void setMaterialManager(CMaterialManager &matman);

	void printError(const std::string &error);
protected:
    bool parseMaterial();
    bool parseTransparent();

    bool parseTechnique();
    bool parsePass();

    bool        parseBind();
    bool        parseDepthWrite();
    bool        parseDepthTest();
    bool        parseCull();
    bool        parseShader();
    bool        parseUniform();
    bool        parseTarget();
    bool        parseBlend();
    bool        parseFuncUniform(SUniformParameter &u);
    bool        parseAmbient();
    bool        parseSampler();
    bool        parseAttrib();
	bool		parseBindTexture();
    CTexture*   parseTexture();
    CImage*     parseImage();

    std::vector<F32>  parseVector();

    CMaterialPtr                mMaterial;
    CMaterialPassPtr            mMatPass;
    CMaterialTechniquePtr       mMatTech;
    std::vector<CMaterialPtr>   mMaterials;
    CMaterialTokenizerPtr       mTokenizer;
    std::set<string>            mMaterialsToRead;
    bool                        mMaterialsToReadDirty;
    CMaterialManager*           mMatManager;
};

#endif

