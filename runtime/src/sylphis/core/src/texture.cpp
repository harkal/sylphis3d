/***************************************************************************
                          texture.cpp  -  description
                             -------------------
    begin                : Wed Mar 20 2002
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

#ifdef _LINUX
#define GL_GLEXT_PROTOTYPES
#endif

#include "texture.h"
#include "engine.h"
#include "vfs.h"
#include "imagefile.h"
#include "openglext.h"
#include "exception.h"
#include <configfile.h>
#include <rendersystem.h>

#include "myglu.h"

#include <math.h>

CTexture::CTexture(){
    mLastFrameUsed = -1;
	setMemoryUsage(0);
}
CTexture::CTexture(const string &n){
	name = n;
    mLastFrameUsed = -10;
	setMemoryUsage(0);
}
CTexture::~CTexture(){
}

void CTexture::copyFromFrameBuffer(){
    //activate();
    //glCopyTexSubImage2D(mGLType, 0, 0, 0, 0, 0, 640, 480);
}

void CTexture::markUsed(){
    mLastFrameUsed = CEngine::getSingleton()->getFrameCount();
}

bool CTexture::wasUsed(S32 offset){
    return mLastFrameUsed + offset >= CEngine::getSingleton()->getFrameCount();
}

void CTexture::setMemoryUsage(U32 b){
	mMemoryUsage = b;
}

U32 CTexture::getMemoryUsage(){
	return mMemoryUsage;
}


void CTexture::getDimentions(S32 *w, S32 *h){
    *w = mWidth;
    *h = mHeight;
}
//
// CTextureAnim
//

CTextureAnim::CTextureAnim(){
}

CTextureAnim::CTextureAnim(string &n) : CTexture(n) {
}

CTextureAnim::~CTextureAnim(){
}

void CTextureAnim::activate(F32 time){
	if(mFrames.size() == 0)
		return;

	S32 frame = (S32)(time) % mFrames.size();
	mFrames[frame]->activate();
}

void CTextureAnim::addTexture(CTexture *t){
	mFrames.push_back(t);
	t->getDimentions(&mWidth, &mHeight);
}

//
// CTextureCube
//

CTextureCube::CTextureCube(){
	mType = TEX_TYPE_CUBE;
}
CTextureCube::CTextureCube(string &n) : CTexture(n) {
	mType = TEX_TYPE_CUBE;
}
CTextureCube::~CTextureCube(){
}


//
// CTexture3D
//

CTexture3D::CTexture3D(){
	mType = TEX_TYPE_3D;
}

CTexture3D::CTexture3D(string &n) : CTexture(n) {
	mType = TEX_TYPE_3D;
}

CTexture3D::~CTexture3D(){
}

//
// CTextureRect
//

CTextureRect::CTextureRect(){
	mType = TEX_TYPE_RECT;
}

CTextureRect::CTextureRect(string &n) : CTexture(n) {
	mType = TEX_TYPE_RECT;
}

CTextureRect::~CTextureRect(){
}

//
// CVertexProgram
//

CVertexProgram::CVertexProgram(){
}
CVertexProgram::CVertexProgram(const string &n) : CTexture(n) {
}
CVertexProgram::~CVertexProgram(){
}

void CVertexProgram::makeProgram(const string &program){
#ifdef _WIN32
	if(!GLEW_ARB_vertex_program)
		return;

	GLuint error;
    glGenProgramsARB(1, &texNum); checkGLError();
    activate(); checkGLError();
    compile(program);

    error = glGetError();
    if( error != GL_NO_ERROR ){
        S32 line;
        const GLubyte *err;

        glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &line);
        err = glGetString(GL_PROGRAM_ERROR_STRING_ARB);
		char d[256];
		sprintf(d, "%d", line);
        string s = string("VP error at line ") + string(d);
        s += " : ";
        s += (char *)err;
        throw CException(s);
    }
#endif
}

void CVertexProgram::compile(const string &program){
	glProgramStringARB(GL_VERTEX_PROGRAM_ARB, 
        GL_PROGRAM_FORMAT_ASCII_ARB, program.size(), program.c_str()); 
}

void CVertexProgram::activate(F32 time){
#ifdef _WIN32
    glBindProgramARB(GL_VERTEX_PROGRAM_ARB, texNum); checkGLError();
#endif
}
void CVertexProgram::deactivate(){
}

//
// CFragmentProgram
//

CFragmentProgram::CFragmentProgram(){
}
CFragmentProgram::CFragmentProgram(const string &n) : CVertexProgram(n) {
}
CFragmentProgram::~CFragmentProgram(){
}
void CFragmentProgram::activate(F32 time){
#ifdef _WIN32
    glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, texNum); checkGLError();
#endif
}
void CFragmentProgram::compile(const string &program){
	glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB, 
        GL_PROGRAM_FORMAT_ASCII_ARB, program.size(), program.c_str());
}

//
// CGLShader
//

CGLShader::CGLShader(){
}
CGLShader::CGLShader(const string &n) : CTexture(n) {
}
CGLShader::~CGLShader(){
}

#if 1
char *sourceForHDR = 
"#extension GL_ARB_texture_rectangle : enable\n"
"uniform sampler2D exposure;\n"
"vec4 decompressHDR(vec4 color){\n"
"	float e = texture2D(exposure, vec2(0.5, 0.5)).x;\n"
"	return color / e;\n"
"}\n"
"vec4 compressHDR(vec4 color){\n"
"	float e = texture2D(exposure, vec2(0.5, 0.5)).x;\n"
"	return color * e;\n"
"}\n"
"vec3 compressHDR(vec3 color){\n"
"	float e = texture2D(exposure, vec2(0.5, 0.5)).x;\n"
"	return color * e;\n"
"}\n"
;
#else
char *sourceForHDR = 
"#version 110\n"
"#extension GL_ARB_texture_rectangle : enable\n"
"const float e = 0.1;\n"
"vec4 decompressHDR(vec4 color){\n"
"	return color / e;\n"
"}\n"
"vec4 compressHDR(vec4 color){\n"
"	return color * e;\n"
"}\n"
"vec3 compressHDR(vec3 color){\n"
"	return color * e;\n"
"}\n"
;
#endif

void CGLShader::makeShader(const string &vert, const string &frag, const vector<string> *defines){
    if(!isGLSLSupported)
        return;
    const char *source;
	string defsource;
	GLuint error;
    char info[1024];
    S32 s;

    defsource = "#version 110\n";
	if(defines){
		vector<string>::const_iterator i;
		for(i = defines->begin() ; i != defines->end() ; ++i){
			defsource = defsource + "#define " + *i + "\n";
		}
	}

    mShader = glCreateProgramObjectARB();
    mVertShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    mFragShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	const char *sources[3];
	source = vert.c_str();
	sources[0] = defsource.c_str();
    sources[1] = source;
    glShaderSourceARB(mVertShader, 2, sources, NULL);
    source = frag.c_str();

	sources[0] = defsource.c_str();
    sources[1] = sourceForHDR;
    sources[2] = source;
    glShaderSourceARB(mFragShader, 3, sources, NULL);

    glCompileShaderARB(mVertShader);
    glGetInfoLogARB(mVertShader, 1024, &s, info);
    gcon.errorf("Compiling vertex %s : %s\n", getName().c_str(), info);
    glCompileShaderARB(mFragShader);
    glGetInfoLogARB(mFragShader, 1024, &s, info);
    gcon.errorf("Compiling fragment %s : %s\n", getName().c_str(), info);

    glAttachObjectARB(mShader, mVertShader);
    glAttachObjectARB(mShader, mFragShader);

    glLinkProgramARB(mShader);

    glGetInfoLogARB(mShader, 1024, &s, info);
    gcon.errorf("Linking %s : %s\n", getName().c_str(), info);

    error = glGetError();
    if( error != GL_NO_ERROR ){
        S32 line;
        const GLubyte *err;

        glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &line);
        err = glGetString(GL_PROGRAM_ERROR_STRING_ARB);
		char d[256];
		sprintf(d, "%d", line);
        string s = string("GLSL error at line ") + string(d);
        s += " : ";
        s += (char *)err;
        throw CException(s);
    }
}

void CGLShader::compile(const string &vert, const string &frag){
}

void CGLShader::activate(F32 time){
    glUseProgramObjectARB(mShader);checkGLError();
}
void CGLShader::deactivate(){
    glUseProgramObjectARB(0);checkGLError();
}

S32 CGLShader::getUniformLocation(const string &name){
    S32 location = glGetUniformLocationARB(mShader, name.c_str());
    GLuint error;
    error = glGetError();
    if(error)
        return -1;
    return location;
}

void CGLShader::setUniform(S32 location, F32 x){
    glUniform1fARB(location, x);checkGLError();
}
void CGLShader::setUniform(S32 location, F32 x, F32 y){
    glUniform2fARB(location, x, y);checkGLError();
}
void CGLShader::setUniform(S32 location, F32 x, F32 y, F32 z){
    glUniform3fARB(location, x, y, z);checkGLError();
}
void CGLShader::setUniform(S32 location, F32 x, F32 y, F32 z, F32 w){
    glUniform4fARB(location, x, y, z, w);checkGLError();
}
void CGLShader::setUniform(S32 location, const CVector3 &data){
    glUniform3fARB(location, data.x, data.y, data.z);checkGLError();
}
void CGLShader::setUniform(S32 location, const CVector4 &data){
    glUniform4fARB(location, data.x, data.y, data.z, data.w);checkGLError();
}

void CGLShader::setUniformMatrix(S32 location, const CMatrix3 &mat){
    glUniformMatrix3fvARB(location, 1, 0, &mat.m[0][0]);checkGLError();
}

void CGLShader::setUniformMatrix(S32 location, const CMatrix4 &mat){
    glUniformMatrix4fvARB(location, 1, 0, &mat.m[0][0]);checkGLError();
}

S32 CGLShader::getAttributeLocation(const string &name){
    return glGetAttribLocationARB(mShader, name.c_str());
}

void CGLShader::setAttribute3v(S32 location, F32 *data){
    glVertexAttribPointerARB(location, 3, GL_FLOAT, 0, 0, data);checkGLError();
}

void CGLShader::setAttribute4v(S32 location, F32 *data){
    glVertexAttribPointerARB(location, 4, GL_FLOAT, 0, 0, data);checkGLError();
}

//
// CTextures
//

CTextures::CTextures(){
}
CTextures::~CTextures(){
	freeAll();
}

void CTextures::registerTexture(CTexture *t){
	resources.push_back(t);
}

CTexture* CTextures::registerTexture(const string &textureName, CImage *img){
	return registerTexture(textureName, 0, img);
}

CTexture* CTextures::registerTexture(const string &textureName, S32 flags, CImage *img){
	CImagePtr image = img;

	string chopedName = CFile::chopExtention(textureName);
		
	CTexture *found = findTexture(chopedName);
	if(found)return found;

    if(!image){
    	try {
    		if(textureName == "_white"){
    			image = new CImageDummy();
    		} else if(textureName == "_black"){
    			image = new CImageDummy();
                image->makeBlack();
    		} else {
    			CFile f(chopedName, *CEngine::getSingleton()->vfs);
    			image = CImageLoader::loadImage(f);
    		}
    	} catch (CException e) {
    		//image = new CImageDummy();
    		//getEngine().con.printString("Failed to load texture : " + textureName + "\n");
    		throw CException("Failed to load texture : " + textureName + "\n");
    	}
    }
	
    CTexture2D *newTexture = CEngine::getSingleton()->mRSystem->createTexture2D();
    newTexture->setName(chopedName);
	newTexture->makeTexture(image, 1024, 0, flags);
	
	registerTexture(newTexture);
	return newTexture;
}

CTexture* CTextures::registerNormalMap(const string &textureName, bool negGreen, CImage *img){
	CImagePtr image = img;

	string chopedName = CFile::chopExtention(textureName);
		
	CTexture *found = findTexture(chopedName);
	if(found)return found;

    if(!image){
	    try {
		    if(textureName == "_white"){
    			image = new CImageDummy();
	    	} else {
		    	CFile f(chopedName, *CEngine::getSingleton()->vfs);
			    image = CImageLoader::loadImage(f);
    		}
	    } catch (CException e) {
    		//image = new CImageDummy();
	    	//getEngine().con.printString("Failed to load texture : " + textureName + "\n");
		    throw CException("Failed to load texture : " + textureName + "\n");
	    }
    }

	CTexture2D *newTexture = CEngine::getSingleton()->mRSystem->createTexture2D();
    image->convertTo(CImage::NORMAL);
    newTexture->setName(chopedName);
	newTexture->makeTexture(image, 1024, 0, 0);
	
	registerTexture(newTexture);
	return newTexture;
}

CTexture* CTextures::registerHeightMap(const string &textureName, F32 hardness){
	CImagePtr image;

	string chopedName = CFile::chopExtention(textureName);
		
	CTexture *found = findTexture(chopedName);
	if(found)return found;

	try {	
		CFile f(chopedName, *CEngine::getSingleton()->vfs);
		image = CImageLoader::loadImage(f);	
	} catch (CException e) {
        image = new CImageDummy();
        gcon.printString("Failed to load heightmap : " + textureName + "\n");
	}

	image->convertTo(CImage::GRAYSCALE);
    image->setConvertParam(hardness);
    image->convertTo(CImage::NORMAL);

    CTexture2D *newTexture = gRSystem->createTexture2D();
    newTexture->setName(chopedName);
	newTexture->makeTexture(image, 1024, 0, 0);
	
	registerTexture(newTexture);
	return newTexture;
}

static char *cubeFaceNames[] = {
	"px",
	"nx",
	"py",
	"ny",
	"nz",
	"pz",
};

CTexture* CTextures::registerCubeMap(const string &textureName){
	if(!GLEW_ARB_texture_cube_map)
		return 0;
	CImagePtr image;
	CImagePtr lastGood;

	string chopedName = CFile::chopExtention(textureName);
		
	CTexture *found = findTexture(chopedName + "(cubemap)");
	if(found)return found;
	
	string decoName = chopedName + "(cubemap)";
	CTextureCube *newTexture = CEngine::getSingleton()->mRSystem->createTextureCube();

	for(S32 i = 0 ; i < 6 ; i++){
		try {
			try {
				CFile f(chopedName + cubeFaceNames[i], *CEngine::getSingleton()->vfs);
				image = CImageLoader::loadImage(f);
				lastGood = image;
			} catch (CException e){
				if (i == 0){
					// The name without decoration
					CFile f(chopedName, *CEngine::getSingleton()->vfs);
					image = CImageLoader::loadImage(f);
					lastGood = image;
				} else {
					image = lastGood;
				}
			}
		} catch (CException e) {
			//image = new CImageDummy();
			//getEngine().con.printString("Failed to load texture : " + textureName + "\n");
		}

		newTexture->makeTexture(image, i, 1024, 0, 0);	
	}

	registerTexture(newTexture);
	return newTexture;
}

CTexture* CTextures::findTexture(const string &name){
	return (CTexture *)find(name);
}


