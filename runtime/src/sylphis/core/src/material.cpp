/***************************************************************************
                         material.cpp  -  description
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

#include <engine.h>
#include <material.h>
#include <materialparser.h>
#include <vfs.h>

CMaterialPass::CMaterialPass(){
    setAmbient(false);
    setBlend(BLEND_NONE, BLEND_NONE);
    setDepthWrite(DEPTHWRITE_LEQUAL);
    setDepthTest(true);
    setCullType(CULL_BACK);
    setBindArrays(0);
}

void CMaterialPass::setName(const std::string &name){
    mName = name;
}

std::string CMaterialPass::getName(){
    return mName;
}

void CMaterialPass::setShader(CGLShader* shader){
    mGLShader = shader;
}

CGLShader* CMaterialPass::getShader(){
    return mGLShader;
}

CMaterialPass::UniformVector& CMaterialPass::getUniforms(){
    return mUniforms;
}

CMaterialPass::SamplerVector& CMaterialPass::getSamplers(){
    return mSamplers;
}

CMaterialPass::AttribVector& CMaterialPass::getAttribs(){
    return mAttribs;
}

void CMaterialPass::setDepthTest(bool dt){
    mDepthTest = dt;
}

bool CMaterialPass::getDepthTest() const {
    return mDepthTest;
}

void CMaterialPass::setDepthWrite(EDepthWriteType dw){
    mDepthWrite = dw;
}

EDepthWriteType& CMaterialPass::getDepthWrite(){
    return mDepthWrite;
}

void CMaterialPass::setCullType(ECullType ct){
    mCullType = ct;
}

ECullType& CMaterialPass::getCullType(){
    return mCullType;
}

void CMaterialPass::setAmbient(bool ambient){
    mIsAmbient = ambient;
}

bool CMaterialPass::isAmbient() const {
    return mIsAmbient;
}

U32 CMaterialPass::getBindArrays() const {
    return mBindArrays;
}

void CMaterialPass::setBindArrays(const U32 &ba){
    mBindArrays = ba;
}

void CMaterialPass::setTarget(CRenderTexture *tex){
    mTarget = tex;
}

CRenderTexture* CMaterialPass::getTarget(){
    return mTarget;
}

void CMaterialPass::setBlend(const EBlendType& s, const EBlendType& d){
    mBlendSrc = s;
    mBlendDst = d;
}

EBlendType CMaterialPass::getBlendSrc() const {
    return mBlendSrc;
}

EBlendType CMaterialPass::getBlendDst() const {
    return mBlendDst;
}

CTexture* CMaterialPass::getTexture(){
	return mTexture;
}

void CMaterialPass::setTexture(CTexture *t){
	mTexture = t;
}

#if 0
CMaterialTechnique::CMaterialTechnique(CMaterialTechnique &tech){
    mName = tech.getName();

    MaterialPassVector &passes = tech.getPasses();
    MaterialPassVector::iterator i;
    for(i = passes.begin() ; i != passes.end() ; ++i){
        CMaterialPass *pass = new CMaterialPass(*(*i));
        mPasses.push_back(pass);
        if(pass->getName() == tech.getAmbientPass()->getName())
            setAmbientPass(pass);
    }
}
#endif

void CMaterialTechnique::setName(const std::string &name){
    mName = name;
}

std::string CMaterialTechnique::getName(){
    return mName;
}

MaterialPassVector& CMaterialTechnique::getPasses(){
    return mPasses;
}

void CMaterialTechnique::setAmbientPass(CMaterialPass *pass){
    mAmbientPass = pass;
}

CMaterialPass* CMaterialTechnique::getAmbientPass(){
    return mAmbientPass;
}

CMaterialPass* CMaterialTechnique::getPass(const string &name){
    MaterialPassVector::iterator i;
    for(i = mPasses.begin() ; i != mPasses.end() ; ++i){
        if((*i)->getName() == name)
            return *i;
    }
    return 0;
}

void CMaterialTechnique::addPass(CMaterialPass *pass){
    MaterialPassVector::iterator i;
    for(i = mPasses.begin() ; i != mPasses.end() ; ++i){
        if((*i)->getName() == pass->getName()){
            *i = pass;
            return;
        }
    }
    mPasses.push_back(pass);
}

CMaterial::CMaterial(){
    setTransparent(false);
}

#if 0
CMaterial::CMaterial(CMaterial &mat){
    mat.getDimensions(&mWidth, &mHeight);

    MaterialTechniqueVector &techs = mat.getTechniques();
    MaterialTechniqueVector::iterator i;
    for(i = techs.begin() ; i != techs.end() ; ++i){
        CMaterialTechnique *tech = new CMaterialTechnique(*(*i));
        mTechniques.push_back(tech);
        if(tech->getName() == mat.getActiveTechnique()->getName())
            setActiveTechnique(tech);
    }
}
#endif

CMaterial::~CMaterial(){

}

bool CMaterial::isTransparent(){
    return mTransparent;
}

void CMaterial::setTransparent(bool t){
    mTransparent = t;
}

MaterialTechniqueVector& CMaterial::getTechniques(){
    return mTechniques;
}

void CMaterial::addTechnique(CMaterialTechnique* technique){
    MaterialTechniqueVector::iterator i;
    for(i = mTechniques.begin() ; i != mTechniques.end() ; ++i){
        if((*i)->getName() == technique->getName()){
            *i = technique;
            return;
        }
    }
    mTechniques.push_back(technique);
}

CMaterialTechnique* CMaterial::getActiveTechnique(){
    return mActiveTechnique;
}

void CMaterial::setActiveTechnique(CMaterialTechnique* technique){
    mActiveTechnique = technique;
}

CMaterialTechnique* CMaterial::getTechnique(const string &name){
    MaterialTechniqueVector::iterator i;
    for(i = mTechniques.begin() ; i != mTechniques.end() ; ++i){
        if((*i)->getName() == name)
            return *i;
    }
    return 0;
}

void CMaterial::setMotionBlurTechnique(CMaterialTechnique* technique){
	mMotionBlurTechnique = technique;
}

CMaterialTechnique* CMaterial::getMotionBlurTechnique(const string &name){
	return mMotionBlurTechnique;
}

void CMaterial::getDimensions(U32 *w, U32 *h){
    *w = mWidth;
    *h = mHeight;
}

void CMaterial::setDimensions(U32 w, U32 h){
    mWidth = w;
    mHeight = h;
}

void CMaterial::markUsed(){
    CMaterialTechnique* tech = getActiveTechnique();

    MaterialPassVector &passes = tech->getPasses();
    MaterialPassVector::iterator p, end;
    end = passes.end();
    for(p = passes.begin() ; p != end ; ++p){
        CMaterialPass::SamplerVector &samplers = (*p)->getSamplers();
        CMaterialPass::SamplerVector::iterator s, send;
        send = samplers.end();
        for(s = samplers.begin() ; s != send ; ++s){ 
            (*s).texture->markUsed();
        }
    }
}

//
// CMaterialManager
//

CMaterialManager::CMaterialManager(){
    
}

CMaterialManager::~CMaterialManager(){

}

CMaterial* CMaterialManager::registerMaterial(const string &name){
    MaterialMap::iterator i;

    i = mMaterials.find(name);
    if(i != mMaterials.end()){
        return i->second;
    }

    gcon.printf("Loading material: %s\n", name.c_str());

    CMaterialParser materialParser;
    materialParser.setMaterialManager(*this);
    materialParser.addMaterialToRead(name);

    CVirtualFS::FindResult shaderFiles;
	CVirtualFS::FindResult::iterator file;
	CEngine &e = *CEngine::getSingleton();

	e.vfs->findFiles("materials/", "mat", shaderFiles);

    while(materialParser.getNumOfMaterialsToRead() && materialParser.isMaterialsToReadDirty()){
        materialParser.setMaterialsToReadDirty(false);
        for(file = shaderFiles.begin() ; file != shaderFiles.end() ; ++file){
            CFileText f(file->getName());
            //e.con.printf("parsing material file : %s\n", f.getName().c_str());

            e.vfs->readFile(f);
            if(!f.getData())continue;
            string buffer = (char*)f.getData();
            materialParser.parseMaterials(buffer);
        }
    }
	
    if(isMaterialLoaded(name)){
        // This call will find the material loaded
        return registerMaterial(name);
    } else {
		materialParser.removeMaterialToRead(name);
        return registerMaterial("failed_to_load");
    }
}

void CMaterialManager::registerMaterial(CMaterial *mat){
    if(isMaterialLoaded(mat->getName()))
        return;
    mMaterials[mat->getName()] = mat;
}

bool CMaterialManager::isMaterialLoaded(const string &name){
    MaterialMap::iterator i;

    i = mMaterials.find(name);
    if(i != mMaterials.end()){
        return true;
    }

    return false;
}

void CMaterialManager::addFailedMaterial(const string &name){
	mMaterialsFailed.insert(name);
}

bool CMaterialManager::isMaterialFailed(const string &name){
	return mMaterialsFailed.find(name) != mMaterialsFailed.end();
}

