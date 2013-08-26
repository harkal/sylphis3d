/***************************************************************************
                      materialparser.cpp  -  description
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

#include <string>
#include <algorithm>
using namespace std;

#include <engine.h>
#include <materialparser.h>
#include <tokenizer.h>
#include <console.h>
#include <exception.h>
#include <python/pyhelper.h>

string lowerCase(const string &a){
	string s = a;
	for(S32 i = 0; i < s.length() ; i++){
		s[i] = s[i] <= 'Z' && s[i] >= 'A' ? s[i] + ('a'-'A') : s[i];
	}
	return s;
}

class CMaterialTokenizer : public CTokenizerNoComments {
public:
	CMaterialTokenizer(const string &b) : CTokenizerNoComments(b) {
		addSeparator("{");
		addSeparator("}");
        addSeparator("(");
        addSeparator(")");
        addSeparator("[");
        addSeparator("]");
        addSeparator(",");
        addSeparator("+");
	};
    virtual void expect(const string &token, bool nextAfter = false){
        CTokenizerNoComments::expect(token, true);
    };
	void skipBlock(){
		if(getToken() != "{"){
			throw CTokenizerException(*this, "Expected '{' found '" + getToken() + "' .");
		}
        if(!nextToken())return;
		skipOpenBlock();
	};
	void skipOpenBlock(){
		S32 open = 1;
		while(open != 0 ){
			if(getToken() == "{")open++;
			if(getToken() == "}")open--;
            if(!nextToken())return;
		}
	};
};

CMaterialParser::CMaterialParser(){
    mMaterialsToReadDirty = false;
}

CMaterialParser::~CMaterialParser(){

}

void CMaterialParser::addMaterialToRead(const std::string &name){
    mMaterialsToRead.insert(name);
    mMaterialsToReadDirty = true;
}

void CMaterialParser::removeMaterialToRead(const std::string &name){
	mMaterialsToRead.erase(name);
	mMaterialsToReadDirty = true;
}

void CMaterialParser::setMaterialsToReadDirty(bool b){
    mMaterialsToReadDirty = b;
}

bool CMaterialParser::isMaterialsToReadDirty(){
    return mMaterialsToReadDirty;
}

S32 CMaterialParser::getNumOfMaterialsToRead(){
    return mMaterialsToRead.size();
}

std::vector<CMaterialPtr>& CMaterialParser::getReadMaterials(){
    return mMaterials;
}

void CMaterialParser::setMaterialManager(CMaterialManager &matman){
    mMatManager = &matman;
}

void CMaterialParser::parseMaterials(const std::string &source){
    mTokenizer = new CMaterialTokenizer(source);
    try {
        mTokenizer->nextToken();
        while(parseMaterial()){

        }
    } catch (CTokenizerException e) {
		gcon.errorf("Material error (%s): %s\n", mMaterial->getName().c_str(), e.getExplanation().c_str());
		mMatManager->addFailedMaterial(mMaterial->getName());
	}
}

bool CMaterialParser::parseTransparent(){
    if(mTokenizer->getToken() != "transparent")
        return false;
    mTokenizer->nextToken();

    mMaterial->setTransparent(true);

    return true;
}

bool CMaterialParser::parseMaterial(){
    CMaterial* baseMat;
    string materialName = mTokenizer->getToken();
    string baseMatName;
    if(materialName == "")
        return false;
    mTokenizer->nextToken();

    if(mTokenizer->getToken() == ":"){
        mTokenizer->nextToken();
        baseMatName = mTokenizer->getToken();
        mTokenizer->nextToken();
    }

    // Check if we are interested in this material and if not skip it and return
    if(find(mMaterialsToRead.begin(), mMaterialsToRead.end(), materialName) == mMaterialsToRead.end()){
        mTokenizer->skipBlock();
        return true;
    }

    mMaterial = new CMaterial();

    if(baseMatName != ""){
		// If the base material failed to load... we fail too
		if(mMatManager->isMaterialFailed(baseMatName)){
			throw CTokenizerException(*mTokenizer, "Base material failed to load");
		}
        // If the base material is not loaded, add it to the "MaterialToRead" list and return
        if(!mMatManager->isMaterialLoaded(baseMatName)){
            addMaterialToRead(baseMatName);
            mTokenizer->skipBlock();
            return true;
        }

        baseMat = mMatManager->registerMaterial(baseMatName);
        *mMaterial = *baseMat;
    }

    gcon.printString("parsing " + materialName + "\n");

    // We got an material we can parse, so do it
    mTokenizer->expect("{");

    mMaterial->setName(materialName);
    
    while(parseTransparent() ||
          parseTechnique());

    mTokenizer->expect("}");

    mMaterials.push_back(mMaterial);
    mMatManager->registerMaterial(mMaterial);

    mMaterialsToRead.erase(materialName);
    mMaterialsToReadDirty = true;

    return true;
}

bool CMaterialParser::parseTechnique(){
    if(mTokenizer->getToken() != "technique")
        return false;

    mTokenizer->nextToken();
    string techName = mTokenizer->getToken();
    mTokenizer->nextToken();
    
    mTokenizer->expect("{");

    // Create a new technique
    mMatTech = new CMaterialTechnique();

    // Handle inheritance
    CMaterialTechnique* fatherTech = mMaterial->getTechnique(techName);
    if(fatherTech){
        *mMatTech = *fatherTech;
    }

    mMatTech->setName(techName);
    mMaterial->addTechnique(mMatTech);

    while(parsePass());
    mTokenizer->expect("}");

	if(mMatTech->getName() == "motionblur"){
		mMaterial->setMotionBlurTechnique(mMatTech);
	} else {
    //if(!mMaterial->getActiveTechnique())
        mMaterial->setActiveTechnique(mMatTech);
	}

    return true;
}



bool CMaterialParser::parsePass(){
    string token;
    string passName = "noname";

    if(mTokenizer->getToken() != "pass")
        return false;
    mTokenizer->nextToken();

    token = mTokenizer->getToken();
    mTokenizer->nextToken();

    if(token == "{"){
        
    } else {
        passName = token;
        mTokenizer->expect("{");
    }

    mMatPass = new CMaterialPass();

    CMaterialPass *fatherPass = mMatTech->getPass(passName);
    if(fatherPass){
        *mMatPass = *fatherPass;
    }

    mMatPass->setName(passName);

    if(passName == "ambient")
        mMatPass->setAmbient(true);

    while(1){
        if(!(
            parseDepthTest() ||
            parseDepthWrite() ||
            parseBind() ||
            parseCull() ||
            parseBlend() ||
            parseAmbient() ||
            parseTarget() ||
            parseShader() ||
            parseUniform() ||
            parseAttrib() ||
            parseSampler() ||
			parseBindTexture()
           ))
            break;
    }

    mTokenizer->expect("}");

    mMatTech->addPass(mMatPass);

    if(mMatPass->isAmbient()){
        mMatTech->setAmbientPass(mMatPass);
    }

    return true;
}

bool CMaterialParser::parseDepthWrite(){
    if(mTokenizer->getToken() != "depthwrite")
        return false;
    mTokenizer->nextToken();

    string type = mTokenizer->getToken();
    mTokenizer->nextToken();

    if(type == "equal"){
        mMatPass->setDepthWrite(DEPTHWRITE_EQUAL);
    } else if(type == "lequal"){
        mMatPass->setDepthWrite(DEPTHWRITE_LEQUAL);
    } else if(type == "less"){
        mMatPass->setDepthWrite(DEPTHWRITE_LESS);
    } else if(type == "greater"){
        mMatPass->setDepthWrite(DEPTHWRITE_GREATER);
    } else if(type == "gequal"){
        mMatPass->setDepthWrite(DEPTHWRITE_GEQUAL);
    } else if(type == "none"){
        mMatPass->setDepthWrite(DEPTHWRITE_NONE);
    } else {
        throw CTokenizerException(*mTokenizer, "Unknown depth write value");
    }

    return true;
}

bool CMaterialParser::parseDepthTest(){
    if(mTokenizer->getToken() != "depthtest")
        return false;
    mTokenizer->nextToken();

    string type = mTokenizer->getToken();
    mTokenizer->nextToken();

    if(type == "true"){
        mMatPass->setDepthTest(true);
    } else if(type == "false"){
        mMatPass->setDepthTest(false);
    } else {
        throw CTokenizerException(*mTokenizer, "Unknown depth test value");
    }

    return true;
}

bool CMaterialParser::parseCull(){
    if(mTokenizer->getToken() != "cull")
        return false;
    mTokenizer->nextToken();

    string type = mTokenizer->getToken();
    mTokenizer->nextToken();

    if(type == "back"){
        mMatPass->setCullType(CULL_BACK);
    } else if(type == "front"){
        mMatPass->setCullType(CULL_FRONT);
    } else if(type == "none"){
        mMatPass->setCullType(CULL_NONE);
    } else {
        throw CTokenizerException(*mTokenizer, "Unknown cull type");
    }

    return true;
}

bool CMaterialParser::parseBind(){
    if(mTokenizer->getToken() != "bind")
        return false;
    mTokenizer->nextToken();

    string type = mTokenizer->getToken();
    mTokenizer->nextToken();

    if(type == "normal"){
        mMatPass->setBindArrays(mMatPass->getBindArrays() | BIND_NORMAL);
    } else if(type == "tangent"){
        mMatPass->setBindArrays(mMatPass->getBindArrays() | BIND_TANGENT);
    } else if(type == "texcoord0"){
        mMatPass->setBindArrays(mMatPass->getBindArrays() | BIND_TEXCOORD0);
    } else {
        throw CTokenizerException(*mTokenizer, "Unknown attribute type to bind");
    }

    return true;
}

bool CMaterialParser::parseBlend(){
     if(mTokenizer->getToken() != "blend")
        return false;
    mTokenizer->nextToken();

    string type = mTokenizer->getToken();
    mTokenizer->nextToken();

    if(type == "add"){
        mMatPass->setBlend(BLEND_ONE, BLEND_ONE);
	} else if(type == "filter"){
		mMatPass->setBlend(BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA);
    } else {
        throw CTokenizerException(*mTokenizer, "Unknown blend type");
    }

    return true;
}

bool CMaterialParser::parseAmbient(){
    if(mTokenizer->getToken() != "ambient")
        return false;
    mTokenizer->nextToken();

    mMatPass->setAmbient(true);

    return true;
}

bool CMaterialParser::parseShader(){
	vector<string> defines;
    CGLShaderPtr glShader;
    if(mTokenizer->getToken() != "shader")
        return false;
    mTokenizer->nextToken();

    string vertexShader = mTokenizer->getToken();
    mTokenizer->nextToken();
    string fragmentShader = mTokenizer->getToken();
    mTokenizer->nextToken();

	if(mTokenizer->getToken() == "define"){
		mTokenizer->nextToken();
		mTokenizer->expect("(");
		while(true){
			defines.push_back(mTokenizer->getToken());
			mTokenizer->nextToken();
			if(mTokenizer->getToken() != ",")
				break;
		}
		mTokenizer->expect(")");
	}

    string resName = "GLSL_" + vertexShader + fragmentShader;
	vector<string>::iterator i;
	for(i = defines.begin() ; i != defines.end() ; ++i)
		resName = resName + "_" + *i;

    CTexture *found = CEngine::getSingleton()->textures.findTexture(resName);
    if(found){
        glShader = dynamic_cast<CGLShader *>(found);
        mMatPass->setShader(glShader);
        return true;
    }

    CFileText vert(vertexShader);
    vert.loadData();
    CFileText frag(fragmentShader);
    frag.loadData();

    glShader = new CGLShader();
    glShader->setName(resName);
    glShader->makeShader((char *)vert.getData(), (char *)frag.getData(), &defines);

    CEngine::getSingleton()->textures.registerTexture(glShader);
    mMatPass->setShader(glShader);

    return true;
}

bool CMaterialParser::parseTarget(){
    if(mTokenizer->getToken() != "target")
        return false;
    mTokenizer->nextToken();

    //string target = mTokenizer->getToken();
    //mTokenizer->nextToken();

    mMatPass->setTarget(dynamic_cast<CRenderTexture *>(parseTexture()));

    return true;
}

bool CMaterialParser::parseUniform(){
    if(mTokenizer->getToken() != "uniform")
        return false;
    mTokenizer->nextToken();

    CGLShader *shader = mMatPass->getShader();
	if(!shader)
		throw CTokenizerException(*mTokenizer, "Found uniform while no shader was defined");

    string location = mTokenizer->getToken();
    mTokenizer->nextToken();

    SUniformParameter u;
    u.location = shader->getUniformLocation(location);
    if(u.location < 0){
        gcon.printError("Warning: Unknown uniform " + location);
    }
    u.data.x = u.data.y = u.data.z = u.data.w = 0.0;

    string t = lowerCase(mTokenizer->getToken());

    vector<F32> vec = parseVector();
    if(vec.size()){
        switch (vec.size()) {
        case 1:
            u.mType = UNIFORM_1F;
            u.data.x = vec[0];
            break;
        case 2:
            u.mType = UNIFORM_2F;
            u.data.x = vec[0];
            u.data.y = vec[1];
            break;
        case 3:
            u.mType = UNIFORM_3F;
            u.data.x = vec[0];
            u.data.y = vec[1];
            u.data.z = vec[2];
            break;
        case 4:
            u.mType = UNIFORM_4F;
            u.data.x = vec[0];
            u.data.y = vec[1];
            u.data.z = vec[2];
            u.data.w = vec[3];
            break;
        }
    } else if(t == "_lightpos"){
        u.mType = UNIFORM_LIGHTPOS;
    } else if(t == "_exposure"){
        u.mType = UNIFORM_EXPOSURE;
    } else if(t == "_lightcolor"){
        u.mType = UNIFORM_LIGHTCOLOR;
    } else if(t == "_invlightsize"){
        u.mType = UNIFORM_LIGHTSIZE;
    } else if(t == "_camerapos"){
        u.mType = UNIFORM_CAMERAPOS;
    } else if(t == "_currentrendernormalize"){
        u.mType = UNIFORM_CURRENTRENDERNORMALIZE;
    } else if(t == "_invviewsize"){
        u.mType = UNIFORM_INVVIEWSIZE;
    } else if(t == "_frametime"){
        u.mType = UNIFORM_FRAMETIME;
    } else if(t == "_lastmvpmatrix"){
        u.mType = UNIFORM_LASTMVPMATRIX;
    } else if(t == "func"){
        parseFuncUniform(u);
    } else {
        u.mType = UNIFORM_1F;
        u.data.x = atof(t.c_str());
    }

    if(u.location >= 0){
        CMaterialPass::UniformVector &uniforms = mMatPass->getUniforms();
        CMaterialPass::UniformVector::iterator i;
        for(i = uniforms.begin() ; i != uniforms.end() ; ++i){
            if((*i).location == u.location)
                break;
        }
        if(i == uniforms.end())
            mMatPass->getUniforms().push_back(u);
        else
            *i = u;
    }

    if(t != "func")
        mTokenizer->nextToken();
    return true;
}

bool CMaterialParser::parseFuncUniform(SUniformParameter &u){
    if(mTokenizer->getToken() != "func")
        return false;

    mTokenizer->nextToken();
    string funcname = mTokenizer->getToken();

    mTokenizer->nextToken();
    
    mTokenizer->expect("(");
    mTokenizer->expect(")");

    PyObject *f = CPython::getSingleton()->getObject(funcname);
    if(!f)
        throw CException("Undefined python function");

	u.mType = UNIFORM_PYTHON_1F;
	u.func = f;
	
    return true;
}

bool CMaterialParser::parseSampler(){
    if(mTokenizer->getToken() != "sampler")
        return false;
    mTokenizer->nextToken();

    CGLShader *shader = mMatPass->getShader();
	if(!shader)
		throw CTokenizerException(*mTokenizer, "Found sampler while no shader was defined");
		
	string t = mTokenizer->getToken();

    SSamplerParameter u;
    u.location = shader->getUniformLocation(t);
    if(u.location < 0)
        throw CTokenizerException(*mTokenizer, "Unknown sampler " + t);
		
    mTokenizer->nextToken();
    t = mTokenizer->getToken();

    if(t == "_lightattenuation"){
        u.mType = SAMPLER_ATTENUATION;
        mTokenizer->nextToken();
    } else {
        u.texture = parseTexture();
        if(u.texture){
            S32 w,h;
            u.texture->getDimentions(&w,&h);
            mMaterial->setDimensions(w,h);
        }
    }

    CMaterialPass::SamplerVector &samplers = mMatPass->getSamplers();
    CMaterialPass::SamplerVector::iterator i;
    for(i = samplers.begin() ; i != samplers.end() ; ++i){
        if((*i).location == u.location)
            break;
    }
    if(i == samplers.end())
        mMatPass->getSamplers().push_back(u);
    else
        *i = u;

    return true;
}

bool CMaterialParser::parseAttrib(){
    if(mTokenizer->getToken() != "attrib")
        return false;
    mTokenizer->nextToken();

    CGLShader *shader = mMatPass->getShader();
	if(!shader)
		throw CTokenizerException(*mTokenizer, "Found attrib while no shader was defined");

    string t = mTokenizer->getToken();

    SAttribParameter u;
    u.location = shader->getAttributeLocation(t);
    if(u.location < 0)
        throw CTokenizerException(*mTokenizer, "Unknown attrib " + t);

    mTokenizer->nextToken();
    t = mTokenizer->getToken();

    if(t == "_tangent"){
        u.mType = ATTRIB_TANGENT;
    } else {
        //u.mType = 0;
    }

    CMaterialPass::AttribVector &attribs = mMatPass->getAttribs();
    CMaterialPass::AttribVector::iterator i;
    for(i = attribs.begin() ; i != attribs.end() ; ++i){
        if((*i).location == u.location)
            break;
    }
    if(i == attribs.end())
        mMatPass->getAttribs().push_back(u);
    else
        *i = u;

    mTokenizer->nextToken();
    return true;
}

bool CMaterialParser::parseBindTexture(){
	if(mTokenizer->getToken() != "texture")
        return false;
    mTokenizer->nextToken();

	mMatPass->setTexture(parseTexture());

	if(mMatPass->getTexture()){
		S32 w,h;
        mMatPass->getTexture()->getDimentions(&w,&h);
        mMaterial->setDimensions(w,h);
    }

	return true;
}

CTexture* CMaterialParser::parseTexture(){
    static int texcount = 0;
    if(mTokenizer->getToken() == "cubemap"){
        mTokenizer->nextToken();
        mTokenizer->expect("(");
        string filename = mTokenizer->getToken();
        mTokenizer->expect(")");
        return CEngine::getSingleton()->textures.registerCubeMap(filename);
    } else {
        CImagePtr img;
        string name = mTokenizer->getToken();
        img = parseImage();
        ++texcount;
        return CEngine::getSingleton()->textures.registerTexture(name , 0, img); //FIXME: SEND FLAGS
    }
}

CImage* CMaterialParser::parseImage(){
    string chopedName = CFile::chopExtention(mTokenizer->getToken());
    CFile f(chopedName);
    CImage* image;
    try {
        image = CImageLoader::loadImage(f);
    } catch (CException e) {
        image = new CImageDummy();
        gcon.printError("Failed to load texture : " + mTokenizer->getToken() + "\n");
    }

    mTokenizer->nextToken();
    return image;
}

vector<F32> CMaterialParser::parseVector(){
    vector<F32> vec;

    if(mTokenizer->getToken() != "[")
        return vec;
    mTokenizer->nextToken();

    while(1){
        string val = mTokenizer->getToken();
        vec.push_back(atof(val.c_str()));
        mTokenizer->nextToken();

        if(mTokenizer->getToken() != ","){
            break;
        }
        mTokenizer->nextToken();
    }

    if(mTokenizer->getToken() != "]")
        throw CTokenizerException(*mTokenizer, "Expected ']'");

    return vec;
}

void CMaterialParser::printError(const std::string &error){
	gcon.errorf("MaterialParser : Line %d : %s\n", mTokenizer->getLine(), error.c_str());
}
