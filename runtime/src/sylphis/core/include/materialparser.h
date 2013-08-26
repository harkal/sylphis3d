/***************************************************************************
                        materialparser.h  -  description
                             -------------------
    begin                : Sat Apr 22 2006
    copyright            : (C) 2006 by Harry Kalogirou
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

