/***************************************************************************
                          renderer.cpp  -  description
                             -------------------
    begin                : Sun Mar 10 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#include <matrix.h>
#include <engine.h>
#include <renderer.h>
#include <renderqueue.h>
#include <renderable.h>

#include <silhouette.h>

#include <openglext.h>
#include <myglu.h>

#include <physicsenvironment.h>

#include <configfile.h>

#include <rendersystem.h>
#include <material.h>

#define ARRAY_MAX_VERTS (16384 * 4)
#define ARRAY_MAX_ELEMS (49152 * 4)

const F32 TURB_SCALE = 0.2;

static char shadowVP[] = 
"!!ARBvp1.0\n"
"TEMP RES;\n"

"SUB	RES, vertex.position, program.env[4];\n"
"MAD	RES, RES.wwww, program.env[4], RES;\n"

"DP4	result.position.x, RES, state.matrix.mvp.row[0];\n"
"DP4	result.position.y, RES, state.matrix.mvp.row[1];\n"
"DP4	result.position.z, RES, state.matrix.mvp.row[2];\n"
"DP4	result.position.w, RES, state.matrix.mvp.row[3];\n"
"END";

const S32 renderSmallSize = 256;

CRenderer::CRenderer() : mFrameCommitTimer(20) {
	mFrameCount = 0;
	mCapHDR = false;
}

CRenderer::~CRenderer(){
}

void CRenderer::setOverrideShader(CMaterial *s){
    mOverrideShader = s;
}

CMaterial* CRenderer::getOverrideShader(){
    return mOverrideShader;
}


void CRenderer::setLastMVPMatrix(const CMatrix4 &m){
    mLastMVPMatrix = m;
}

CMatrix4 &CRenderer::getLastMVPMatrix() {
    return mLastMVPMatrix;
}

CMatrix4 &CRenderer::getModelViewMatrix() {
    return mModelView;
}

CMatrix4 &CRenderer::getProjectionMatrix() {
    return mProjection;
}

CMatrix4 &CRenderer::getMVPMatrix() {
    return mMVP;
}

F32 CRenderer::getEPS(){
	F32 sum = 0;
	for(S32 i=0; i<NUM_OF_LAST_EPS; i++){
		sum += mLastEPSs[i];
	}
	return sum / NUM_OF_LAST_EPS;
}

CMaterialPtr logo;
bool wireframe;

CRenderTexturePtr mSpeedBuffer;
CRenderTexturePtr   mCurrentRender256;
CRenderTexturePtr   mCurrentRender128;
CRenderTexturePtr   mCurrentRender64;
CRenderTexturePtr   mCurrentRender32;
CRenderTexturePtr   mCurrentRender16;
CRenderTexturePtr   mCurrentRender8;
CRenderTexturePtr   mCurrentRender4;
CRenderTexturePtr   mCurrentRender2;
CRenderTexturePtr   mCurrentRender1;
CRenderTexturePtr   mExposureTexture;
CRenderTexturePtr   mVariableTexture;

CRenderTexturePtr   mBloom256;
CRenderTexturePtr   mBloom128;
CRenderTexturePtr   mBloom64;
CRenderTexturePtr   mBloom32;
CRenderTexturePtr   mBloom16;

CGLShaderPtr    mDetoneShader;
CGLShaderPtr    mDownsampleShader;
CGLShaderPtr    mDownsampleRectShader;
CGLShaderPtr    mBlurH;
CGLShaderPtr    mBlurV;
CGLShaderPtr    mMotionBlurShader;
CGLShaderPtr	mExposureControl;

CGLShaderPtr	mLumaInit;

F32 mFrameExposure;
F32 mTargetExposure;

void CRenderer::startup(){
    // Clean up the garbage in the buffer
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    eng = CEngine::getSingleton();

	// Check HDR capability
	mCapHDR = isFloatTexturesSupported;

    mFrameExposure = mTargetExposure = 1.00;
	setMinLuminance(10.1);
	setMaxLuminance(15.0);
    setMotionBlurAmount(0.4);
    setNearDepth(4.0);

    setUseShadowVP(gCfgFile.getAsInt("r_useShadowVP") && GLEW_ARB_vertex_program);
    setUseHDR(gCfgFile.getAsInt("r_useHDR"));

    int colorBits = isUseHDR() ? 16 : 8;

    mVertexBuffer = new CVertexBuffer();
    mVertexBuffer->allocate(ARRAY_MAX_VERTS * ( sizeof(CVector4) + sizeof(CVector3) + sizeof(CVector3) + sizeof(CTexCoord) ),
                            CVertexBuffer::DYNAMIC, 
                            true);

    mVertexBuffer->addComponent(CVertexBuffer::POSITION);
    mVertexBuffer->addComponent(CVertexBuffer::NORMAL);
    mVertexBuffer->addComponent(CVertexBuffer::TANGENT);
    mVertexBuffer->addComponent(CVertexBuffer::TEX_COORD);

    mElemBuffer = new CIndexBuffer();
    mElemBuffer->allocate(ARRAY_MAX_ELEMS * sizeof(U32), CIndexBuffer::DYNAMIC, true);

    useDefaultArrays();

	arrays.env_st = new CVector3[ARRAY_MAX_VERTS];
	arrays.spec_st = new CVector3[ARRAY_MAX_VERTS];
	arrays.colors = new CColor[ARRAY_MAX_VERTS];
	arrays.numElems = 0;
	arrays.numVerts = 0;

	S32 maxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

    logo = eng->mMaterials.registerMaterial("textures/sylphis");

	mSciRect[0] = -200000;
	mSciRect[1] = -200000;
	mSciRect[0] = 200000;
	mSciRect[1] = 200000;

	camera = 0;
	mSkyShader = 0;

#ifdef _WIN32 //Warning: VSYNC is handles differently in *nix, it should be off by default though
    if(WGLEW_EXT_swap_control)
        wglSwapIntervalEXT(0);
#endif

    mWhiteTexture = CEngine::getSingleton()->textures.registerTexture("_white");
    mWhiteShader = CEngine::getSingleton()->mMaterials.registerMaterial("_white");
    mBlackTexture = CEngine::getSingleton()->textures.registerTexture("_black");

	if(isUseShadowVP())
		mShadowVP.makeProgram(shadowVP);

    setFogColor(CVector3(1.0, 1.0, 0.1));
    setFogBounds(100, 2048);
    setEnableFog(false);

    S32 rw = 2048;
    S32 rh = 2048;

    pair<S32, S32> d = renderingView->getDimentions();

    while(rw >= d.first)rw = rw >> 1; rw = rw << 1;
    while(rh >= d.second)rh = rh >> 1; rh = rh << 1;

    mCurrentRenderNormalize = CVector2((F32)d.first / (F32)rw, (F32)d.second / (F32)rh);
    mCurrentRender = gRSystem->createRenderTexture();

    //if(isUseHDR()){
    //    mCurrentRender->create(rw, rh, false, false, 16, 8, 8, 8, 24);
    //} else {
        CImagePtr image = new CImageBase();
        image->setWidth(d.first);
        image->setHeight(d.second);
        image->create();
        mCurrentRender->makeTexture(image, 100000, 0, SHADER_NOMIPMAPS);
    //}

    int mipBits = colorBits;

    mCurrentRender->setName("_currentRender");
    CEngine::getSingleton()->textures.registerTexture(mCurrentRender);

    mCurrentRenderSmall = gRSystem->createRenderTexture();
    mCurrentRenderSmall->create(renderSmallSize, renderSmallSize);
    mCurrentRenderSmall->setName("_currentRenderSmall");
    CEngine::getSingleton()->textures.registerTexture(mCurrentRenderSmall);

    mCurrentRender256 = gRSystem->createRenderTexture();
    mCurrentRender256->create(renderSmallSize, renderSmallSize, false, false, mipBits);
    mCurrentRender256->setName("_currentRender256");
    CEngine::getSingleton()->textures.registerTexture(mCurrentRender256);

    mBloom256 = gRSystem->createRenderTexture();
    mBloom256->create(renderSmallSize, renderSmallSize, false, false, 8);
    mBloom256->setName("_bloom256");
    CEngine::getSingleton()->textures.registerTexture(mBloom256);

    mCurrentRender128 = gRSystem->createRenderTexture();
    mCurrentRender128->create(renderSmallSize >> 1, renderSmallSize >> 1, false, false, mipBits);
    mCurrentRender128->setName("_currentRender128");
    CEngine::getSingleton()->textures.registerTexture(mCurrentRender128);

    mBloom128 = gRSystem->createRenderTexture();
    mBloom128->create(renderSmallSize >> 1, renderSmallSize >> 1, false, false, 8);
    mBloom128->setName("_bloom128");
    CEngine::getSingleton()->textures.registerTexture(mBloom128);

    mCurrentRender64 = gRSystem->createRenderTexture();
    mCurrentRender64->create(renderSmallSize >> 2, renderSmallSize >> 2, false, false, mipBits);
    mCurrentRender64->setName("_currentRender64");
    CEngine::getSingleton()->textures.registerTexture(mCurrentRender64);

    mBloom64 = gRSystem->createRenderTexture();
    mBloom64->create(renderSmallSize >> 2, renderSmallSize >> 2, false, false, 8);
    mBloom64->setName("_bloom64");
    CEngine::getSingleton()->textures.registerTexture(mBloom64);

    mCurrentRender32 = gRSystem->createRenderTexture();
    mCurrentRender32->create(renderSmallSize >> 3, renderSmallSize >> 3, false, false, mipBits);
    mCurrentRender32->setName("_currentRender32");
    CEngine::getSingleton()->textures.registerTexture(mCurrentRender32);

    mBloom32 = gRSystem->createRenderTexture();
    mBloom32->create(renderSmallSize >> 3, renderSmallSize >> 3, false, false, 8);
    mBloom32->setName("_bloom32");
    CEngine::getSingleton()->textures.registerTexture(mBloom32);

    mCurrentRender16 = gRSystem->createRenderTexture();
    mCurrentRender16->create(renderSmallSize >> 4, renderSmallSize >> 4, false, false, mipBits);
    mCurrentRender16->setName("_currentRender16");
    CEngine::getSingleton()->textures.registerTexture(mCurrentRender16);

    mBloom16 = gRSystem->createRenderTexture();
    mBloom16->create(renderSmallSize >> 4, renderSmallSize >> 4, false, false, 8);
    mBloom16->setName("_bloom16");
    CEngine::getSingleton()->textures.registerTexture(mBloom16);

    mCurrentRender8 = gRSystem->createRenderTexture();
    mCurrentRender8->create(renderSmallSize >> 5, renderSmallSize >> 5, false, false, mipBits);
    mCurrentRender8->setName("_currentRender8");
    CEngine::getSingleton()->textures.registerTexture(mCurrentRender8);

    mCurrentRender4 = gRSystem->createRenderTexture();
    mCurrentRender4->create(renderSmallSize >> 6, renderSmallSize >> 6, false, false, mipBits);
    mCurrentRender4->setName("_currentRender4");
    CEngine::getSingleton()->textures.registerTexture(mCurrentRender4);

    mCurrentRender2 = gRSystem->createRenderTexture();
    mCurrentRender2->create(renderSmallSize >> 7, renderSmallSize >> 7, false, false, mipBits);
    mCurrentRender2->setName("_currentRender2");
    CEngine::getSingleton()->textures.registerTexture(mCurrentRender2);

    mCurrentRender1 = gRSystem->createRenderTexture();
    mCurrentRender1->create(renderSmallSize >> 8, renderSmallSize >> 8, false, false, mipBits);
    mCurrentRender1->setName("_currentRender1");
    CEngine::getSingleton()->textures.registerTexture(mCurrentRender1);

	mExposureTexture = gRSystem->createRenderTexture();
    mExposureTexture->create(1, 1, false, false, mipBits);
    mExposureTexture->setName("_exposure");
    CEngine::getSingleton()->textures.registerTexture(mExposureTexture);

	mVariableTexture = gRSystem->createRenderTexture();
    mVariableTexture->create(1, 1, false, false, mipBits);
    mVariableTexture->setName("_variabletexture");
    CEngine::getSingleton()->textures.registerTexture(mVariableTexture);

    mBufferA = gRSystem->createRenderTexture();
    mBufferB = gRSystem->createRenderTexture();

    mBufferA->create(renderSmallSize, renderSmallSize, false, false, mipBits);
    mBufferB->create(renderSmallSize, renderSmallSize, false, false, mipBits);

    mBufferA->setName("_bufferA");
    mBufferB->setName("_bufferB");
    CEngine::getSingleton()->textures.registerTexture(mBufferA);
    CEngine::getSingleton()->textures.registerTexture(mBufferB);

	if(isFloatTexturesSupported){
		mSpeedBuffer = gRSystem->createRenderTexture();
		mSpeedBuffer->create(d.first, d.second, false, false, 16, 16, 16, 16, 24);
		mSpeedBuffer->setName("_speedbuffer");
		CEngine::getSingleton()->textures.registerTexture(mSpeedBuffer);
	} else {
		setMotionBlurAmount(0.0);
	}

    CEngine::getSingleton()->mMaterials.registerMaterial("rtex");

#if 1
    //
    // Load the downsampling shader
    //
    {
    CFileText vert("shaders/downsample.vp");
    vert.loadData();
    CFileText frag("shaders/downsample.fp");
    frag.loadData();

    mDownsampleShader = new CGLShader();
    mDownsampleShader->setName("GLSL_downsample");
    mDownsampleShader->makeShader((char *)vert.getData(), (char *)frag.getData());
    }

	//
    // Load the downsamplingrect shader
    //
    {
    CFileText vert("shaders/downsample.vp");
    vert.loadData();
    CFileText frag("shaders/downsamplerect.fp");
    frag.loadData();

    mDownsampleRectShader = new CGLShader();
    mDownsampleRectShader->setName("GLSL_downsamplerect");
    mDownsampleRectShader->makeShader((char *)vert.getData(), (char *)frag.getData());
    }

    //
    // Load the detone shader
    //
    {
    CFileText vert("shaders/ambient.vp");
    vert.loadData();
    CFileText frag("shaders/convtofp.fp");
    frag.loadData();

    mDetoneShader = new CGLShader();
    mDetoneShader->setName("GLSL_detone");
    mDetoneShader->makeShader((char *)vert.getData(), (char *)frag.getData());
    }

    //
    // Load the blur H shader
    //
    {
    CFileText vert("shaders/blur.vp");
    vert.loadData();
    CFileText frag("shaders/blur_h.fp");
    frag.loadData();

    mBlurH = new CGLShader();
    mBlurH->setName("GLSL_blur_h");
    mBlurH->makeShader((char *)vert.getData(), (char *)frag.getData());
    }
    //
    // Load the blur V shader
    //
    {
    CFileText vert("shaders/blur.vp");
    vert.loadData();
    CFileText frag("shaders/blur_v.fp");
    frag.loadData();

    mBlurV = new CGLShader();
    mBlurV->setName("GLSL_blur_v");
    mBlurV->makeShader((char *)vert.getData(), (char *)frag.getData());
    }
    //
    // Load the blur motion blur shader
    //
    {
    CFileText vert("shaders/motionblur.vp");
    vert.loadData();
    CFileText frag("shaders/motionblur.fp");
    frag.loadData();

    mMotionBlurShader = new CGLShader();
    mMotionBlurShader->setName("GLSL_motion_blur");
    mMotionBlurShader->makeShader((char *)vert.getData(), (char *)frag.getData());
    }
	//
    // Load the exposure control shader
    //
    {
    CFileText vert("shaders/downsample.vp");
    vert.loadData();
    CFileText frag("shaders/exposurecontrol.fp");
    frag.loadData();

    mExposureControl = new CGLShader();
    mExposureControl->setName("GLSL_exposure_control");
    mExposureControl->makeShader((char *)vert.getData(), (char *)frag.getData());
    }
	//
    // Load the luma init
    //
    {
    CFileText vert("shaders/ambient.vp");
    vert.loadData();
    CFileText frag("shaders/luma_init.fp");
    frag.loadData();

    mLumaInit = new CGLShader();
    mLumaInit->setName("GLSL_luma_init");
    mLumaInit->makeShader((char *)vert.getData(), (char *)frag.getData());
    }
#endif
    setOverrideShader(0);

    glEnableClientState(GL_VERTEX_ARRAY); checkGLError();

	mCurrentRender1->beginCapture();
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	mCurrentRender1->endCapture();

    glClearColor(0.0,0.0,0.0,1.0);
}

void CRenderer::flushShadowVolume(CVector4 lightPos){
    if(wireframe)
        return;
    if(arrays.numVerts == 0)
        return;

    if(mUseShadowVP){
        glEnable(GL_VERTEX_PROGRAM_ARB); checkGLError();
        mShadowVP.activate(); checkGLError();
	    glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, 4, &lightPos.x); checkGLError();
    }

    flushShadowVolume();

    if(mUseShadowVP){
        glDisable(GL_VERTEX_PROGRAM_ARB); checkGLError();
    }
}

void CRenderer::flushShadowVolume(){
    arrays.verts.setupBuffer(arrays.mVertexBuffer, CVertexBuffer::POSITION, 0);
    arrays.verts.bindAsVertexBuffer();
    arrays.verts.deactivate();
    ////glEnableClientState(GL_VERTEX_ARRAY); checkGLError();    

    if(GLEW_ATI_separate_stencil){
        glStencilOpSeparateATI(GL_BACK,  GL_KEEP, GL_INCR_WRAP, GL_KEEP);
        glStencilOpSeparateATI(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
        //glStencilOpSeparateATI(GL_FRONT,GL_KEEP,GL_KEEP,GL_INCR_WRAP);
        //glStencilOpSeparateATI(GL_BACK,GL_KEEP,GL_KEEP,GL_DECR_WRAP);
        glDisable(GL_CULL_FACE);
        drawArrays();
        glEnable(GL_CULL_FACE);
    } else {
        glCullFace(GL_FRONT); checkGLError();
        glStencilOp(GL_KEEP, glIncrStencil, GL_KEEP ); checkGLError();
        drawArrays();

        glCullFace(GL_BACK); checkGLError();
        glStencilOp(GL_KEEP, glDecrStencil, GL_KEEP); checkGLError();
        drawArrays();
    }

	////glDisableClientState(GL_VERTEX_ARRAY); checkGLError();
    arrays.verts.setupBuffer(arrays.mVertexBuffer, CVertexBuffer::POSITION);
}



void CRenderer::useDefaultArrays(){
    arrays.mVertexBuffer = mVertexBuffer;
    arrays.verts.setupBuffer(mVertexBuffer, CVertexBuffer::POSITION);
    arrays.normals.setupBuffer(mVertexBuffer, CVertexBuffer::NORMAL);
    arrays.tex_st.setupBuffer(mVertexBuffer, CVertexBuffer::TEX_COORD);
    arrays.tangent.setupBuffer(mVertexBuffer, CVertexBuffer::TANGENT);
    arrays.mElems = mElemBuffer;
}

void CRenderer::shutdown(){
	delete[] arrays.env_st;
	delete[] arrays.spec_st;
	delete[] arrays.colors;
}

void CRenderer::setUseShadowVP(bool b){
    mUseShadowVP = b;
}

bool CRenderer::isUseShadowVP(){
    return mUseShadowVP;
}

void CRenderer::setUseHDR(bool b){
    mUseHDR = b && mCapHDR;
}

bool CRenderer::isUseHDR(){
    return mUseHDR;
}

void CRenderer::setShadows(bool b){
	mShadows = b;
}

bool CRenderer::isShadows(){
	return mShadows;
}

bool CRenderer::needSpeedRender(){
    return mNeedSpeedRender;
}

void CRenderer::setRenderingView(CRenderingTarget &v){
	renderingView = &v;
}

void CRenderer::setRenderingView(CTexture &v){
	renderingView = dynamic_cast <CRenderingTarget*>(&v);
}

const CRenderingTarget &CRenderer::getRenderingView() const {
	return *renderingView;
}

void CRenderer::useCamera(CCamera *c){
    if(camera)
        camera->setActive(false);
	camera = c;
    if(c)
        c->setActive(true);
}

CCamera *CRenderer::getCamera(){
	return camera;
}

void CRenderer::setSkyShader(CMaterial *s){
	mSkyShader = s;
}

void CRenderer::addLight(CLight *l){
	if(mAddLights)
		mLights.push_back(l);
}

void CRenderer::ignoreNewLights(bool i){
	mAddLights = !i;
}

void CRenderer::clearLights(){
	mLights.clear();
}

list<CLightPtr>* CRenderer::getLights(){
    return &mLights;
}
S32 saved;
void CRenderer::clearStencil(){
	glClear(GL_STENCIL_BUFFER_BIT);
	return;
	if(mSciRect[0] > mLastSciRect[2] || mSciRect[2] < mLastSciRect[0])goto ret;
	if(mSciRect[1] > mLastSciRect[3] || mSciRect[3] < mLastSciRect[1])goto ret;

	//glScissor(mLastSciRect[0], mLastSciRect[1], mLastSciRect[2] - mLastSciRect[0], mLastSciRect[3] - mLastSciRect[1]);
	glScissor(0,0,800,600);
	glClear(GL_STENCIL_BUFFER_BIT);
	glScissor(mSciRect[0], mSciRect[1], mSciRect[2] - mSciRect[0], mSciRect[3] - mSciRect[1]);
	
	mLastSciRect[0] = mSciRect[0];
	mLastSciRect[1] = mSciRect[1];
	mLastSciRect[2] = mSciRect[2];
	mLastSciRect[3] = mSciRect[3];
	return;
ret:
	mLastSciRect[0] = mSciRect[0];
	mLastSciRect[1] = mSciRect[1];
	mLastSciRect[2] = mSciRect[2];
	mLastSciRect[3] = mSciRect[3];
	//glClear(GL_STENCIL_BUFFER_BIT);
	saved++;
}

void CRenderer::projectPoint(const CVector3 &c, CVector3 &p){
	CVector4 o(c.x, c.y, c.z, 1.0);
	CVector4 w = mMVP * o;
    pair<S32, S32> d = renderingView->getDimentions();
    
	w.x /= w.w;
	w.y /= w.w;
    p.x = d.first*w.x/2 + d.first/2;
    p.y = d.second*w.y/2 + d.second/2;
	if(p.x < 0.0)p.x = 0.0;
	if(p.y < 0.0)p.y = 0.0;
	if(p.x > d.first)p.x = d.first;
	if(p.y > d.second)p.y = d.second;
}

void CRenderer::addEdge(const CVector3 &v1, const CVector3 &v2){
	CPlane &plane = camera->getFrustum().getPlane(CFrustum::PNEAR);

	
	CVector3 vp[2];
	vp[0]=v1;
	vp[1]=v2;

	F32 d1 = -plane.distance(v1);
	F32 d2 = -plane.distance(v2);

	if(d1 < 0.0 && d2 < 0.0)
		return;

	CVector3 v;
	if(d1 < 0.0 || d2 < 0.0)
		plane.rayIntersection(v1, v2, v);

	if(d1 < 0.0){
		vp[0] = v;
		vp[1] = v2;
	} else if(d2 < 0.0) {
		vp[0] = v1;
		vp[1] = v;
	}

	for(S32 i = 0 ; i < 2 ; i++){
		projectPoint(vp[i], vp[i]);
		if(vp[i].x < mSciRect[0])mSciRect[0] = (S32)vp[i].x;
		if(vp[i].y < mSciRect[1])mSciRect[1] = (S32)vp[i].y;
		if(vp[i].x > mSciRect[2])mSciRect[2] = (S32)vp[i].x;
		if(vp[i].y > mSciRect[3])mSciRect[3] = (S32)vp[i].y;
	}
}

void CRenderer::setupScissor(){
	if(!mLight)
		return;

	mSciRect[0] = 10000000;
	mSciRect[1] = 10000000;
	mSciRect[2] = -10000000;
	mSciRect[3] = -10000000;
	CAABoundBox b = mLight->getBound();
	b.translate(mLight->getDerivedPosition());

	CVector3 v1, v2;

	CVector3 &max = b.getMax();
	CVector3 &min = b.getMin();

	v1 = CVector3(max.x,max.y,max.z);
	v2 = CVector3(min.x,max.y,max.z);
	addEdge(v1, v2);

	v1 = CVector3(max.x,max.y,max.z);
	v2 = CVector3(max.x,min.y,max.z);
	addEdge(v1, v2);

	v1 = CVector3(min.x,min.y,max.z);
	v2 = CVector3(min.x,max.y,max.z);
	addEdge(v1, v2);

	v1 = CVector3(min.x,min.y,max.z);
	v2 = CVector3(max.x,min.y,max.z);
	addEdge(v1, v2);

	//Bottom plane

	v1 = CVector3(max.x,max.y,min.z);
	v2 = CVector3(min.x,max.y,min.z);
	addEdge(v1, v2);

	v1 = CVector3(max.x,max.y,min.z);
	v2 = CVector3(max.x,min.y,min.z);
	addEdge(v1, v2);

	v1 = CVector3(min.x,min.y,min.z);
	v2 = CVector3(min.x,max.y,min.z);
	addEdge(v1, v2);

	v1 = CVector3(min.x,min.y,min.z);
	v2 = CVector3(max.x,min.y,min.z);
	addEdge(v1, v2);

	//Sides

	v1 = CVector3(min.x,max.y,min.z);
	v2 = CVector3(min.x,max.y,max.z);
	addEdge(v1, v2);

	v1 = CVector3(max.x,max.y,min.z);
	v2 = CVector3(max.x,max.y,max.z);
	addEdge(v1, v2);

	v1 = CVector3(min.x,min.y,min.z);
	v2 = CVector3(min.x,min.y,max.z);
	addEdge(v1, v2);

	v1 = CVector3(max.x,min.y,min.z);
	v2 = CVector3(max.x,min.y,max.z);
	addEdge(v1, v2);

	//CVector3 o;
	//projectPoint(max, o);

	glScissor(mSciRect[0], mSciRect[1], mSciRect[2] - mSciRect[0], mSciRect[3] - mSciRect[1]);
	//gcon.printf(" %d %d %d %d\n", mSciRect[0], mSciRect[1], mSciRect[2], mSciRect[3]);
	glEnable(GL_SCISSOR_TEST);

}

void CRenderer::setScissor(U32 x, U32 y, U32 w, U32 h){
    glScissor(x,y,w,h);
    glEnable(GL_SCISSOR_TEST);
}

void CRenderer::setLight(CLight *light){
	mLight = light;
	//setupScissor();
}

CLight *CRenderer::getLight(){
	return mLight;
}

void CRenderer::setAmbientLight(const CColorf &a){
    mAmbientLight = a;
}

S32 mRElems;

void CRenderer::beginFrame(){
    //gcon.printf("CRenderer::beginFrame\n");

    
	renderingView->beginCapture();


    if(!camera)
        return;

    if(camera){
	    glClear(GL_DEPTH_BUFFER_BIT);//|GL_STENCIL_BUFFER_BIT);
    } else {
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    }

    if(wireframe){
		glClear(GL_COLOR_BUFFER_BIT);
    }

	setup3DMode(renderingView);

    gRSystem->pushWorldMatrix();
    gRSystem->resetWorldMatrix();
    
    //Did this have to be a reference for some reason? I can't see why, removed it as gcc wouldn't accept it.
	CVector3 o = camera->getDerivedPosition();
	camera->getDerivedOrientation().toRotationMatrix(mModelView);
	mModelView.transpose(); // Invert the rotation matrix
	CMatrix4 transl;
	transl.identity();
	transl.setTransform(CVector3(-o.x,-o.y,-o.z));	
	mModelView = mModelView * transl;
	glMultMatrixf(&mModelView.m[0][0]);
    
    mMVP = mProjection * mModelView;
	
    camera->updateFrustum(mMVP);

    mRElems = 0;
	mFrameCount++;

	setSkyShader(0);
	mSkyFlags = 0;
	saved = 0;

    if(mSpeedBuffer && mSpeedBuffer->wasUsed(1)){
        mSpeedBuffer->beginCapture();
        glClear(GL_DEPTH_BUFFER_BIT);// | GL_COLOR_BUFFER_BIT);
        pair<S32, S32> d = renderingView->getDimentions();
        setup3DMode(renderingView, (F32)d.first / (F32)d.second);

        gRSystem->pushWorldMatrix();
        gRSystem->resetWorldMatrix();
        glMultMatrixf(&mModelView.m[0][0]);

        mSpeedBuffer->endCapture();
        mNeedSpeedRender = true;
    } else {
        mNeedSpeedRender = false;
    }
}

void CRenderer::setSkyFlags(S32 flags){
	mSkyFlags |= flags;
}

void drawQuad(float sx, float sy, float ex, float ey){
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0); glVertex2f(sx, sy); 
    glTexCoord2f(0.0, 0.0); glVertex2f(sx, ey); 
    glTexCoord2f(1.0, 0.0); glVertex2f(ex, ey); 
    glTexCoord2f(1.0, 1.0); glVertex2f(ex, sy); 
    glEnd();
}

void CRenderer::endFrame(){
#if 0
    // Debug rendering
    const CPhysicsEnv::RigidBodyList &bodies = CPhysicsEnv::getSingleton()->getRigidBodies();

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glDisable(GL_CULL_FACE);
    glDepthMask(0);
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_TEXTURE_2D );

    for(CPhysicsEnv::RigidBodyList::const_iterator i = bodies.begin() ; i != bodies.end() ; ++i){
        (*i)->draw();
    }

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				glEnable( GL_TEXTURE_2D );
                glDepthMask(1);
				glEnable(GL_CULL_FACE);
				glEnable( GL_DEPTH_TEST );
#endif 
    //gcon.printf("endFrame()\n");

    mLastModelView = mModelView;
    drawPic(630, 520, 128, 64, logo, 0.0, 0.0, 1.0, 1.0, 0);

	//drawPic(40, 200, 256, 256, CEngine::getSingleton()->mMaterials.registerMaterial("viewtex"), 0.0, 0.0, 1.0, 1.0, 0);
	//drawPic(40 + 270, 200, 256, 256, CEngine::getSingleton()->mMaterials.registerMaterial("viewtex2"), 0.0, 0.0, 1.0, 1.0, 0);
	if(isUseHDR()){
		drawPic(0, 0, 800, 600, CEngine::getSingleton()->mMaterials.registerMaterial("glare"), 0.0, 0.0, 1.0, 1.0, 0);
	}

    enter2DMode();
    gRSystem->resetProjMatrix();
	glOrtho(0, 800.0, 600.0,0,-999.0,999.0); checkGLError();
    renderOverlay2D();
    exit2DMode();

    if(camera){
        gRSystem->popWorldMatrix();
    }

	mFrameCommitTimer.start();
    //glFinish();

#if 0
    if(isUseHDR()){
        mCurrentRender->endCapture();
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        
        mCurrentRenderSmall->activate();
        mCurrentRenderSmall->markUsed();
        //glBindTexture(GL_TEXTURE_2D, 14);
        enter2DMode();
        glEnable(GL_TEXTURE_2D);
        glMatrixMode(GL_TEXTURE);
        glPushMatrix();
        glScalef(mCurrentRenderNormalize.x, mCurrentRenderNormalize.y, 1.0);
        drawQuad(0.0, 0.0, 800.0, 600.0);
        glPopMatrix();
        exit2DMode();
    }
#endif

#if 0
        mCurrentRenderSmall->activate();
        mCurrentRenderSmall->markUsed();

        //glBindTexture(GL_TEXTURE_2D, 14);
        enter2DMode();
        glEnable(GL_TEXTURE_2D);
        glMatrixMode(GL_TEXTURE);
        glPushMatrix();
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0); glVertex2f(100.0, 100.0); 
        glTexCoord2f(0.0, 0.0); glVertex2f(100.0, 400.0); 
        glTexCoord2f(1.0, 0.0); glVertex2f(400.0, 400.0); 
        glTexCoord2f(1.0, 1.0); glVertex2f(400.0,  100.0); 
        glEnd();
        glPopMatrix();
        exit2DMode();

#endif

    renderingView->swapBuffers(); 
    
	mFrameCommitTimer.stop();
	mFrameCommitTimer.reset();

    renderingView->endCapture();

//	eng->con.printf("Saved %d stencil clears (lights %d)\n", saved, mLights.size());
    clearLights();

	mLastEPSs[mFrameCount & (NUM_OF_LAST_EPS - 1)] = eng->getFPS() * mRElems;
#if 0
	eng->con.printf("RElems : %d  EPS : %f\n", mRElems, getEPS());
#endif
}

void CRenderer::beginObjectTransformations(){
    gRSystem->pushWorldMatrix();
    //glLoadIdentity();
}

void CRenderer::endObjectTransformations(){
    myglPopMatrix(); checkGLError();
}

void CRenderer::clearTransformations(){
    glLoadIdentity();
}

void CRenderer::translate(const CVector3 &o){
	glTranslatef(o.x, o.y, o.z);
}

void CRenderer::rotate(const CVector3 &r){
	glRotatef(r.x, 1, 0, 0);
	glRotatef(r.y, 0, 1, 0);
	glRotatef(r.z, 0, 0, 1);
}

void CRenderer::rotate(const CQuaternion &q){
	CMatrix4 m;
    q.toRotationMatrix(m);
    rotate(m);
}

void CRenderer::rotate(const CMatrix4 &m){
    glMultMatrixf(&m.m[0][0]);
}

void CRenderer::scale(CVector3 s){
	glScalef(s.x, s.y, s.z);
}

void CRenderer::GLBeginQuad(){
	glBegin(GL_QUADS);
}

void CRenderer::GLBeginLine(){
	glBegin(GL_LINES);
}

void CRenderer::GLEnableTexture2D(){
	glEnable(GL_TEXTURE_2D);
}

void CRenderer::GLDisableTexture2D(){
	glDisable(GL_TEXTURE_2D);
}

void CRenderer::GLEnd(){
	glEnd();
}

void CRenderer::GLColor4(F32 r, F32 g, F32 b, F32 a ){
	glColor4f(r, g, b, a);
}

void CRenderer::GLVertex3(F32 x, F32 y, F32 z){
	glVertex3f(x, y, z);
}

void CRenderer::GLVertex2(F32 x, F32 y){
	glVertex2f(x, y);
}

void CRenderer::GLTexCoord2(F32 s, F32 t){
	glTexCoord2f(s, t);
}

void CRenderer::setNearDepth(F32 d){
    mNearDepth = d;
}

F32 CRenderer::getNearDepth(){
    return mNearDepth;
}

void createProjectionMatrix(F64 fov, F64 aspectr, F64 zNear, CMatrix4 &m){
	const F32 nudge = 1.0 - 1.0 / (1<<23);

	fov = fov * M_PI / 360.0;

	m.m[1][0] = m.m[2][0] = m.m[3][0] = m.m[0][1] =
		m.m[2][1] = m.m[3][1] = m.m[0][2] = m.m[1][2] =
		m.m[0][3] = m.m[1][3] = m.m[2][2] = 0;

	m.m[0][0] = 1.0 / tan(fov);
	m.m[1][1] = m.m[0][0] * aspectr;

	m.m[3][2] = -2.0 * zNear * nudge;
	m.m[3][3] = -1.0 * nudge;
	m.m[2][3] = -1.0;
}

void CRenderer::setup3DMode(CRenderingTarget *rt, F32 ratio){
	pair<S32, S32> d = rt->getDimentions();
    
    if(ratio == 0.0)
        ratio = (GLfloat)d.first / (GLfloat)d.second;

	createProjectionMatrix(camera->getFov(), ratio, mNearDepth, mProjection);

    gRSystem->resetProjMatrix();
	glMultMatrixf(&mProjection.m[0][0]);
    glViewport(0,0, d.first, d.second);
    glDepthRange(0.0, 1.0);
}

void CRenderer::setup2DMode(CRenderingTarget *rt){

}

void CRenderer::setRenderQueue(CRenderQueue *rq){
    mRenderQueue = rq;
}

CRenderQueue *CRenderer::getRenderQueue(){
    return mRenderQueue;
}

CArrays *CRenderer::getArrays(){
    return &arrays;
}

void CRenderer::clearArrays(){
    arrays.numVerts = arrays.numElems = 0;    
}

void CRenderer::drawArrays(){
    glDrawRangeElements(GL_TRIANGLES,  0, arrays.numVerts, arrays.numElems, GL_UNSIGNED_INT, arrays.mElems->getPointer()); checkGLError();
    arrays.mElems->deactivate();
    //glDrawElements(GL_TRIANGLES, arrays.numElems, GL_UNSIGNED_INT, arrays.mElems->map());
}



void CRenderer::renderShadowVolumes(CLight *light, S32 back){
    CRenderQueue::RenderableQueueMap::const_iterator i;
    CRenderQueue::RenderableQueueMap &mQueue = getRenderQueue()->mQueue;

	for (i = mQueue.begin(); i != mQueue.end(); ++i){
        CMaterial *shader = i->first;
		//if(shader->flags & SHADER_TRANSPARENT)
		//	continue;

        vector<CRenderableQueueMemberPtr>::const_iterator ri;
        for(ri = i->second.begin(); ri != i->second.end(); ++ri){
            if((*ri)->getNoShadows())
                continue;

            CRenderable *r = (*ri)->getRenderable();

            if(!light->getDerivedBound().collide(&(*ri)->getBounds()))
                continue;
            
            CMatrix4 m = (*ri)->getTransformMatrix();

            gRSystem->pushWorldMatrix();
            glMultMatrixf(&m.m[0][0]);

            m.invert();

            CVector3 objLight = m * light->getEmissionPosition();

            r->renderShadowVolume(*this, objLight, back);

            myglPopMatrix(); checkGLError();
        }
    }
}

void CRenderer::flushRenderQueue(CLight *light){
#if 1
    CRenderQueue::RenderableQueueMap::iterator i;
    CRenderQueue::RenderableQueueMap &mQueue = getRenderQueue()->mQueue;

	for (i = mQueue.begin(); i != mQueue.end(); i++){
        CMaterial *shader = i->first;
		
        vector<CRenderableQueueMemberPtr>::iterator ri, rend;
        rend = i->second.end();
        for(ri = i->second.begin(); ri != rend; ++ri){
			CRenderable *r = (*ri)->getRenderable();

			//(*ri)->getBounds().render(this);
            if(light){
                if(!light->getDerivedBound().collide(&(*ri)->getBounds()))
                    continue;
            }

			CMatrix4 m = (*ri)->getTransformMatrix();

			gRSystem->pushWorldMatrix();
			gRSystem->resetWorldMatrix();
			CMatrix4 mvp = getModelViewMatrix() * m;
			glMultMatrixf(&mvp.m[0][0]);
		
			setLastMVPMatrix(mLastModelView * (*ri)->getLastTransformMatrix());
            r->render(*this, (*ri)->getTransformMatrix());
			m.invert();
			flushWithMaterial(shader, &(*ri)->getTransformMatrix(), &m);
			
            useDefaultArrays();
			arrays.numVerts = arrays.numElems = 0;

			gRSystem->popWorldMatrix();
        }
    }
#else
    F64 time = eng->getFrameTime();
    CRenderQueue::RenderableQueueMap::iterator i;
    CRenderQueue::RenderableQueueMap &mQueue = getRenderQueue()->mQueue;
    glEnableClientState(GL_VERTEX_ARRAY); checkGLError();
    for (i = mQueue.begin(); i != mQueue.end(); i++){
        CMaterial *shader = i->first;
        if(mOverrideShader){
            shader = mOverrideShader;
        }

        vector<CRenderableQueueMemberPtr>::iterator ri, rend;
        rend = i->second.end();
        beginFlush(shader);
        for(S32 p = 0 ; p < shader->getPassesCount() ; ++p){
            CShaderPass &pass = shader->getPass(p);
            if(enableShaderPass(pass, 0.0)){
                //gcon.printf("queue %d\n", shader->getPassesCount());
                for(ri = i->second.begin(); ri != rend; ++ri){
                    CRenderable *r = (*ri)->getRenderable();

                    //(*ri)->getBounds().render(this);
                    if(light){
                        if(!light->getDerivedBound().collide(&(*ri)->getBounds()))
                            continue;
                    }

                    CMatrix4 m = (*ri)->getTransformMatrix();

                    gRSystem->pushWorldMatrix();
                    glMultMatrixf(&m.m[0][0]);

                    //            useDefaultArrays();
                    setLastMVPMatrix( (*ri)->getLastTransformMatrix() * mLastModelView );
                    r->render(*this);
                    m.invert();

                    passFlush(&pass, time, &(*ri)->getTransformMatrix(), &m);

                    useDefaultArrays();
                    arrays.numVerts = arrays.numElems = 0;

                    myglPopMatrix(); checkGLError();
                }
                disableShaderPass(pass);
            }
        }

        //glDisableClientState(GL_VERTEX_ARRAY); checkGLError();
    }


#endif
}

#if 1
void CRenderer::pushRaw(CVector3 *points, CVector3 *normals, CVector3 *tangent, CTexCoord *tm_st, U32 *elems, S32 numVerts, S32 numElems){
	S32 i;
	
    U32 *aelems = ((U32 *)arrays.mElems->map()) + arrays.numElems;
	arrays.numElems += numElems * 3;

    for( i = 0; i < numElems * 3; i++ )
		*aelems++ = arrays.numVerts + *elems++;

    CVector4 *averts = &arrays.verts[arrays.numVerts];
    CVector3 *anormals = &arrays.normals[arrays.numVerts];
    CTexCoord *atex_st = &arrays.tex_st[arrays.numVerts];
    CVector3 *atangent = &arrays.tangent[arrays.numVerts];
    arrays.numVerts += numVerts;
	for( i = 0; i < numVerts; i++) {
		*averts++ = *points++;
		*anormals++ = *normals++;
		*atex_st++ = *tm_st++;
		*atangent++ = *tangent++;
	}
}
#else
void CRenderer::pushRaw(CVector3 *points, CVector3 *normals, CVector3 *tangent, CTexCoord *tm_st, U32 *elems, S32 numVerts, S32 numElems){
	S32 i;
	
	for( i = 0; i < numElems * 3; i++ )
		arrays.elems[arrays.numElems++] = arrays.numVerts + *elems++;

	for( i = 0; i < numVerts; i++, arrays.numVerts++ ) {
		arrays.verts[arrays.numVerts]  = points[i];
		arrays.normals[arrays.numVerts] = normals[i];
		arrays.tex_st[arrays.numVerts] = tm_st[i];
		arrays.tangent[arrays.numVerts] = tangent[i];
	}
}
#endif

void CRenderer::renderChunk(CRenderChunk *rc){
    //for (S32 p = 0; p < shader->getPassesCount(); p++){
	//		pass = &shader->getPass(p);

    //}
}

GLenum getGLBlendType(EBlendType t){
	switch(t){
	case BLEND_NONE:
		return 0;
	case BLEND_ZERO:
		return GL_ZERO;
	case BLEND_ONE:
		return GL_ONE;
	case BLEND_DST_COLOR:
		return GL_DST_COLOR;
	case BLEND_ONE_MINUS_SRC_ALPHA:
		return GL_ONE_MINUS_SRC_ALPHA;
	case BLEND_SRC_ALPHA:
		return GL_SRC_ALPHA;
	case BLEND_SRC_COLOR:
		return GL_SRC_COLOR;
	case BLEND_ONE_MINUS_DST_COLOR:
		return GL_ONE_MINUS_DST_COLOR;
	case BLEND_ONE_MINUS_SRC_COLOR:
		return GL_ONE_MINUS_SRC_COLOR;
	case BLEND_DST_ALPHA:
		return GL_DST_ALPHA;
	case BLEND_ONE_MINUS_DST_ALPHA:
		return GL_ONE_MINUS_DST_ALPHA;
	};
	return 0;
}

void CRenderer::renderMaterialPass(CMaterialPass *pass, F64 time, CMatrix4 *WorldSpaceMat, CMatrix4 *invWorldSpaceMat){
    CVector3 lightSize;
	CVector3 lightPos;
    CVector4 color;
	CTexture *attMap3D;
    bool ambient = !getLight();

    if(pass->isAmbient() != ambient)
        return;

    if(!ambient){
        color = getLight()->getColor();
		if(invWorldSpaceMat && getLight()->getType() != CLight::sun){
			lightPos = *invWorldSpaceMat * getLight()->getEmissionPosition();
		} else {
			lightPos = getLight()->getEmissionPosition();
			color = mAmbientLight * (1.0/0.8);
		}

		const CVector3 &l = getLight()->getRadius();
		lightSize.x = 1.0f / l.x;
		lightSize.y = 1.0f / l.y;
		lightSize.z = 1.0f / l.z;
		
		attMap3D = getLight()->getAttenuation();
		//if(!attMap3D)
		//	attMap3D = attenuationMap3D;
		//getLight()->getDerivedBound().render(this);
    } else {
		color = mAmbientLight;
    }
    
    glColorMask(1, 1, 1, 1);

    switch (pass->getDepthWrite()){
    case DEPTHWRITE_NONE:
        glDepthMask(0);
        break;
    case DEPTHWRITE_EQUAL:
        glDepthFunc(GL_EQUAL);
        break;
    case DEPTHWRITE_LEQUAL:
        glDepthFunc(GL_LEQUAL);
        break;
    case DEPTHWRITE_LESS:
        glDepthFunc(GL_LESS);
        break;
    case DEPTHWRITE_GREATER:
        glDepthFunc(GL_GREATER);
        break;
    case DEPTHWRITE_GEQUAL:
        glDepthFunc(GL_GEQUAL);
        break;
    }

    switch (pass->getCullType()){
    case CULL_BACK:
        glEnable(GL_CULL_FACE);
        break;
    case CULL_FRONT:
        glEnable(GL_CULL_FACE);
        break;
    case CULL_NONE:
        glDisable(GL_CULL_FACE);
        break;
    }

    if(pass->getDepthTest()){
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    U32 bindArrays = pass->getBindArrays();

    if(bindArrays & BIND_NORMAL){
        glEnableClientState(GL_NORMAL_ARRAY); checkGLError();
        arrays.normals.bindAsNormalBuffer();
        arrays.normals.deactivate();
    }
    if(bindArrays & BIND_TANGENT){
        glClientActiveTextureARB(GL_TEXTURE2_ARB);
        glActiveTextureARB(GL_TEXTURE2_ARB);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        arrays.tangent.bindAsTexCoordBuffer(CVertexBuffer::TANGENT, 3);
        arrays.tangent.deactivate();
    }
    if(bindArrays & BIND_TEXCOORD0){
        glClientActiveTextureARB(GL_TEXTURE0_ARB);
        glActiveTextureARB(GL_TEXTURE0_ARB);checkGLError();
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        arrays.tex_st.bindAsTexCoordBuffer();
        arrays.tex_st.deactivate();
    }

	if(!pass->getShader()){
		 //FIXME : some shader remains enabled. Create some checking system.
		pass->getTexture()->activate();
		glEnable(GL_TEXTURE_2D);
	} else {
		pass->getShader()->activate();checkGLError();

		for(CMaterialPass::AttribVector::iterator i = pass->getAttribs().begin() ; i != pass->getAttribs().end() ; ++i){
			switch ((*i).mType) {
			case ATTRIB_TANGENT:
				arrays.tangent.bindAsAttribBuffer((*i).location, CVertexBuffer::TANGENT, 3);
				arrays.tangent.deactivate();
				glEnableVertexAttribArrayARB((*i).location);checkGLError();
			}
		}

		S32 count = 0;

		int expLocation = pass->getShader()->getUniformLocation("exposure");
		if(expLocation != -1){
			//pass->getShader()->setUniform(expLocation, mFrameExposure);
			glActiveTexture(GL_TEXTURE0);
			mCurrentRender1->activate();
			glUniform1iARB(expLocation, count);checkGLError();
			++count;
		}
		for(CMaterialPass::SamplerVector::iterator i = pass->getSamplers().begin() ; i != pass->getSamplers().end() ; ++i, ++count){
			CGLShader *shader = pass->getShader();
			glActiveTexture(GL_TEXTURE0 + count);
			if((*i).texture){
				(*i).texture->activate();checkGLError();
				glUniform1iARB((*i).location, count);checkGLError();
			} else {
				switch((*i).mType){
				case SAMPLER_ATTENUATION:
					attMap3D->activate();checkGLError();
					glUniform1iARB((*i).location, count);checkGLError();
					break;
				case SAMPLER_FILTER:
					if(getLight()->getFilter()){
						getLight()->getFilter()->activate();
						glUniform1iARB((*i).location, count);checkGLError();
					}
					break;
				}
			}
		}

		glActiveTexture(GL_TEXTURE0);
		PyObject *pyResult;
		for(CMaterialPass::UniformVector::iterator i = pass->getUniforms().begin() ; i != pass->getUniforms().end() ; ++i){
			CGLShader *shader = pass->getShader();
			switch ((*i).mType) {
			case UNIFORM_1F:
				shader->setUniform((*i).location, (*i).data.x);
				break;
			case UNIFORM_PYTHON_1F:
				if(PyCallable_Check((*i).func)){
					pyResult = PyObject_CallFunction((*i).func, 0);
				} else {
					pyResult = (*i).func;
				}
				if(PyTuple_Check(pyResult)){
					CVector3 v;
					v.x = PyFloat_AsDouble(PyTuple_GetItem(pyResult, 0));
					v.y = PyFloat_AsDouble(PyTuple_GetItem(pyResult, 1));
					v.z = PyFloat_AsDouble(PyTuple_GetItem(pyResult, 2));
					shader->setUniform((*i).location, v);
				} else {
					shader->setUniform((*i).location, PyFloat_AsDouble(pyResult));
				}
				break;
			case UNIFORM_2F:
				shader->setUniform((*i).location, (*i).data.x, (*i).data.y);
				break;
			case UNIFORM_3F:
				shader->setUniform((*i).location, (*i).data.x, (*i).data.y, (*i).data.z);
				break;
			case UNIFORM_4F:
				shader->setUniform((*i).location, (*i).data.x, (*i).data.y, (*i).data.z, (*i).data.w);
				break;
			case UNIFORM_LIGHTPOS:
				shader->setUniform((*i).location, CVector4(lightPos));
				break;
			case UNIFORM_EXPOSURE:
				shader->setUniform((*i).location, mFrameExposure);
				break;
			case UNIFORM_LIGHTSIZE:
				shader->setUniform((*i).location, CVector3(lightSize));
				break;
			case UNIFORM_LIGHTCOLOR:
				shader->setUniform((*i).location, color);
				break;
			case UNIFORM_CAMERAPOS:
				shader->setUniform((*i).location, CVector4(objectSpaceEye));
				break;
			case UNIFORM_CURRENTRENDERNORMALIZE:
				{
					pair<S32,S32> d = renderingView->getDimentions();
					shader->setUniform((*i).location, mCurrentRenderNormalize.x / d.first, mCurrentRenderNormalize.y / d.second);
				}
				break;
			case UNIFORM_INVVIEWSIZE:
				shader->setUniform((*i).location, mCurrentRenderNormalize.x, mCurrentRenderNormalize.y);                
				break;
			case UNIFORM_FRAMETIME:
				shader->setUniform((*i).location, CEngine::getSingleton()->getFrameTime());                
				break;
			case UNIFORM_LASTMVPMATRIX:
				shader->setUniformMatrix((*i).location, mLastMVPMatrix);                
				break;
			}
		}
	}

	

    EBlendType s,d;
    s = pass->getBlendSrc();
    d = pass->getBlendDst();
    if(s != BLEND_NONE){
        glEnable(GL_BLEND);
        glBlendFunc(getGLBlendType(s), getGLBlendType(d));
    }

    if(pass->getTarget()){
        gRSystem->pushWorldMatrix();
        gRSystem->resetWorldMatrix();
        gRSystem->pushProjMatrix();
        gRSystem->resetProjMatrix();
        pair<S32, S32> d = pass->getTarget()->getDimentions();
        glOrtho(0,1.0, 0 ,1.0,-999.0,999.0);
        glViewport(0,0, d.first, d.second);
        glDisable(GL_CULL_FACE);
        pass->getTarget()->beginCapture();
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex2f(0.0, 0.0); 
        glTexCoord2f(0.0, 1.0); glVertex2f(0.0, 1.0); 
        glTexCoord2f(1.0, 1.0); glVertex2f(1.0, 1.0); 
        glTexCoord2f(1.0, 0.0); glVertex2f(1.0,  0.0); 
        glEnd();
        pass->getTarget()->endCapture();
        d = renderingView->getDimentions();
        glViewport(0,0, d.first, d.second);
        gRSystem->popProjMatrix();
        gRSystem->popWorldMatrix();
		glEnable(GL_CULL_FACE);
    } else {
        drawArrays();
    }

    if(s != BLEND_NONE){
        glDisable(GL_BLEND);
    }

	if(pass->getShader()){
		pass->getShader()->deactivate();
	} else {
		pass->getTexture()->deactivate();
		glDisable(GL_TEXTURE_2D);
	}

    if(bindArrays & BIND_NORMAL){
        glDisableClientState(GL_NORMAL_ARRAY); checkGLError();
    }
    if(bindArrays & BIND_TANGENT){
        glClientActiveTextureARB(GL_TEXTURE2_ARB);
        glActiveTextureARB(GL_TEXTURE2_ARB);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    if(bindArrays & BIND_TEXCOORD0){
        glActiveTextureARB(GL_TEXTURE0_ARB);checkGLError();
        glClientActiveTextureARB(GL_TEXTURE0_ARB);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }      

    if(pass->getDepthWrite() == DEPTHWRITE_NONE)
        glDepthMask(1);

    glEnable(GL_CULL_FACE);
}

void CRenderer::flushWithMaterial(CMaterial *material, CMatrix4 *WorldSpaceMat, CMatrix4 *invWorldSpaceMat){
    if (arrays.numVerts == 0) return;

    if(camera){
        if(invWorldSpaceMat)
            objectSpaceEye = *invWorldSpaceMat * camera->getDerivedPosition();
        else
            objectSpaceEye = camera->getDerivedPosition();
    }

    // An override shader is used.
    if(mOverrideShader)
         material = mOverrideShader;

    arrays.verts.bindAsVertexBuffer();
    arrays.verts.deactivate();

    F32 time = eng->getFrameTime();

    CMaterialTechnique* technique = material->getActiveTechnique();

    MaterialPassVector::const_iterator pass, lastPass;
    lastPass = technique->getPasses().end();
    for(pass = technique->getPasses().begin() ; pass != lastPass ; ++pass){
        renderMaterialPass(*pass, time, WorldSpaceMat, invWorldSpaceMat);
    }

    

}

void CRenderer::setEnableFog(bool b){
    if(b){
        glFogi(GL_FOG_MODE, GL_LINEAR); checkGLError();
        mEnabledFog = true;
    } else {
        mEnabledFog = false;
    }
}

void CRenderer::setFogColor(const CVector3 &color){
    glFogfv(GL_FOG_COLOR, &color.x);
}

void CRenderer::setFogBounds(F32 start, F32 end){
    glFogf(GL_FOG_START, start);
    glFogf(GL_FOG_END, end);
}

void CRenderer::drawPic(S32 x, S32 y, S32 w, S32 h, CMaterial *shader, F32 s1, F32 t1, F32 s2, F32 t2, CColor *c){
    COverlayQuad q;
	bool b = false;

    shader->markUsed();

	if(!c){
		c = new CColor(1.0, 1.0, 1.0,1.0);
		b = true;
	}

    q.points[0][0] = (F32)x;
    q.points[0][1] = (F32)y;
    q.tex_st[0][0] = (F32)s1;
    q.tex_st[0][1] = (F32)t1;
    q.colors[0] = *c;

    q.points[1][0] = (F32)(x + w);
    q.points[1][1] = (F32)y;
    q.tex_st[1][0] = (F32)s2;
    q.tex_st[1][1] = (F32)t1;
    q.colors[1] = *c;

    q.points[2][0] = (F32)(x + w);
    q.points[2][1] = (F32)(y + h);
    q.tex_st[2][0] = (F32)s2;
    q.tex_st[2][1] = (F32)t2;
    q.colors[2] = *c;

    q.points[3][0] = (F32)x;
    q.points[3][1] = (F32)(y + h);
    q.tex_st[3][0] = (F32)s1;
    q.tex_st[3][1] = (F32)t2;
    q.colors[3] = *c;

    q.shader = shader;

    overlayQuads.push_back(q);

    if(b)
		delete c;
}

#include <font.h>

void CRenderer::renderOverlay2D(){
    COverlayQuad *q;
    vector<COverlayQuad>::iterator iter;

//    CVector3 *normals = (CVector3 *)arrays.normals.map();
    U32 *elems = (U32 *)arrays.mElems->map();
    for( iter = overlayQuads.begin() ; iter != overlayQuads.end() ; iter++ ){
        q = &*iter;

        elems[arrays.numElems++] = arrays.numVerts + 2;
        elems[arrays.numElems++] = arrays.numVerts + 1;
        elems[arrays.numElems++] = arrays.numVerts + 0;

        elems[arrays.numElems++] = arrays.numVerts + 3;
        elems[arrays.numElems++] = arrays.numVerts + 2;
        elems[arrays.numElems++] = arrays.numVerts + 0;

        arrays.verts[arrays.numVerts][0] = q->points[0][0];
        arrays.verts[arrays.numVerts][1] = q->points[0][1];
        arrays.verts[arrays.numVerts][2] = 0.0f;
        arrays.verts[arrays.numVerts][3] = 1.0f;
//        normals[arrays.numVerts] = CVector3::ZERO;
        arrays.tex_st[arrays.numVerts] = q->tex_st[0];
        
        arrays.colors[arrays.numVerts] = q->colors[0];
        arrays.numVerts++;

        arrays.verts[arrays.numVerts][0] = q->points[1][0];
        arrays.verts[arrays.numVerts][1] = q->points[1][1];
        arrays.verts[arrays.numVerts][2] = 0.0f;
        arrays.verts[arrays.numVerts][3] = 1.0f;
//        normals[arrays.numVerts] = CVector3::ZERO;
        arrays.tex_st[arrays.numVerts] = q->tex_st[1];
        arrays.colors[arrays.numVerts] = q->colors[1];
        arrays.numVerts++;

        arrays.verts[arrays.numVerts][0] = q->points[2][0];
        arrays.verts[arrays.numVerts][1] = q->points[2][1];
        arrays.verts[arrays.numVerts][2] = 0.0f;
        arrays.verts[arrays.numVerts][3] = 1.0f;
//        normals[arrays.numVerts] = CVector3::ZERO;
        arrays.tex_st[arrays.numVerts] = q->tex_st[2];
        arrays.colors[arrays.numVerts] = q->colors[2];
        arrays.numVerts++;

        arrays.verts[arrays.numVerts][0] = q->points[3][0];
        arrays.verts[arrays.numVerts][1] = q->points[3][1];
        arrays.verts[arrays.numVerts][2] = 0.0f;
        arrays.verts[arrays.numVerts][3] = 1.0f;
//        normals[arrays.numVerts] = CVector3::ZERO;
        arrays.tex_st[arrays.numVerts] = q->tex_st[3];
        arrays.colors[arrays.numVerts] = q->colors[3];
        arrays.numVerts++;

        //flush(q->shader, 0, 0);
        flushWithMaterial(q->shader, 0, 0);
        clearArrays();
    }
    
    overlayQuads.clear();
}

void DrawQuad(F32 sw = 1.0, F32 sh = 1.0, F32 w = 1.0, F32 h = 1.0){
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex2f(0.0, 0.0);
        glTexCoord2f(0.0, sh); glVertex2f(0.0, h);
        glTexCoord2f(sw, sh); glVertex2f(w, h);
        glTexCoord2f(sw, 0.0); glVertex2f(w, 0.0);
        glEnd();checkGLError();
}

void DrawQuadInv(F32 sw = 1.0, F32 sh = 1.0, F32 w = 1.0, F32 h = 1.0){
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, sh); glVertex2f(0.0, 0.0);
        glTexCoord2f(0.0, 0.0); glVertex2f(0.0, h);
        glTexCoord2f(sw, 0.0); glVertex2f(w, h);
        glTexCoord2f(sw, sh); glVertex2f(w, 0.0);
        glEnd();checkGLError();
}

float CRenderer::calcAverageLuma(CTexture2D *tex){
        tex = mCurrentRenderSmall;
        S32 texSize = mDownsampleShader->getUniformLocation("texSize");
        S32 RT = mDownsampleShader->getUniformLocation("RT");

		S32 expTexSize = mExposureControl->getUniformLocation("texSize");
		S32 expRT = mExposureControl->getUniformLocation("RT");
		S32 expLimits = mExposureControl->getUniformLocation("limits");

		S32 lumaRT = mLumaInit->getUniformLocation("RT");
        S32 lumaExp = mLumaInit->getUniformLocation("exposure");

        S32 detoneRT = mDetoneShader->getUniformLocation("RT");
        S32 detoneExp = mDetoneShader->getUniformLocation("exposure");

        S32 blurHtexSize = mBlurH->getUniformLocation("texSize");
        S32 blurHRT = mBlurH->getUniformLocation("RT");

        S32 blurVtexSize = mBlurV->getUniformLocation("texSize");
        S32 blurVRT = mBlurV->getUniformLocation("RT");
        S32 blurVexp = mBlurV->getUniformLocation("exposure");

        gRSystem->pushWorldMatrix();
        gRSystem->resetWorldMatrix();
        gRSystem->pushProjMatrix();
        gRSystem->resetProjMatrix();

        glOrtho(0,1.0, 0 ,1.0,-999.0,999.0);

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

		//////////////////////////////////////////

#if 1
		mLumaInit->activate();
        glActiveTexture(GL_TEXTURE0);
        tex->activate();checkGLError();
        glUniform1iARB(lumaRT, 0);checkGLError();
		glActiveTexture(GL_TEXTURE1);
		mCurrentRender1->activate();
		glUniform1iARB(lumaExp, 1);checkGLError();
		glActiveTexture(GL_TEXTURE0);

        mCurrentRender256->beginCapture();
        glViewport(0,0, 256.0, 256.0);
        DrawQuad();
        mCurrentRender256->endCapture();

		mCurrentRender1->deactivate();
		glActiveTexture(GL_TEXTURE0);

        mDownsampleShader->activate();
        mCurrentRender256->activate();checkGLError();
        glUniform1iARB(RT, 0);checkGLError();
        mDownsampleShader->setUniform(texSize, 1.0 / 256.0);
        mCurrentRender128->beginCapture();
        glViewport(0,0, 128.0, 128.0);
        DrawQuad();
        mCurrentRender128->endCapture();

        mCurrentRender128->activate();checkGLError();
        glUniform1iARB(RT, 0);checkGLError();
        mDownsampleShader->setUniform(texSize, 1.0 / 128.0);
        mCurrentRender64->beginCapture();
        glViewport(0,0, 64.0, 64.0);
        DrawQuad();
        mCurrentRender64->endCapture();

        mCurrentRender64->activate();checkGLError();
        glUniform1iARB(RT, 0);checkGLError();
        mDownsampleShader->setUniform(texSize, 1.0 / 64.0);
        mCurrentRender32->beginCapture();
        glViewport(0,0, 32.0, 32.0);
        DrawQuad();
        mCurrentRender32->endCapture();

        mCurrentRender32->activate();checkGLError();
        glUniform1iARB(RT, 0);checkGLError();
        mDownsampleShader->setUniform(texSize, 1.0 / 32.0);
        mCurrentRender16->beginCapture();
        glViewport(0,0, 16.0, 16.0);
        DrawQuad();
        mCurrentRender16->endCapture();

        mCurrentRender16->activate();checkGLError();
        glUniform1iARB(RT, 0);checkGLError();
        mDownsampleShader->setUniform(texSize, 1.0 / 16.0);
        mCurrentRender8->beginCapture();
        glViewport(0,0, 8.0, 8.0);
        DrawQuad();
        mCurrentRender8->endCapture();

        mCurrentRender8->activate();checkGLError();
        glUniform1iARB(RT, 0);checkGLError();
        mDownsampleShader->setUniform(texSize, 1.0 / 8.0);
        mCurrentRender4->beginCapture();
        glViewport(0,0, 4.0, 4.0);
        DrawQuad();
        mCurrentRender4->endCapture();

        mCurrentRender4->activate();checkGLError();
        glUniform1iARB(RT, 0);checkGLError();
        mDownsampleShader->setUniform(texSize, 1.0 / 4.0);
        mCurrentRender2->beginCapture();
        glViewport(0,0, 2.0, 2.0);
        DrawQuad();
        mCurrentRender2->endCapture();
		mDownsampleShader->deactivate();

		mExposureControl->activate();
        mCurrentRender2->activate();checkGLError();
        glUniform1iARB(expRT, 0);checkGLError();
        mExposureControl->setUniform(expTexSize, 1.0 / 2.0);
		mExposureControl->setUniform(expLimits, getMinLuminance(), getMaxLuminance(), 0.0);
		mCurrentRender1->beginCapture();
        glViewport(0,0, 1.0, 1.0);
        DrawQuad();
		mExposureControl->deactivate();
        mCurrentRender1->endCapture();

#endif
		//////////////////////////////////////////
#if 1
        mDetoneShader->activate();
        glActiveTexture(GL_TEXTURE0);
        tex->activate();checkGLError();
        glUniform1iARB(detoneRT, 0);checkGLError();
        //mDetoneShader->setUniform(detoneExp, mFrameExposure);
		glActiveTexture(GL_TEXTURE1);
		mCurrentRender1->activate();
		glUniform1iARB(detoneExp, 1);checkGLError();
		glActiveTexture(GL_TEXTURE0);

        mCurrentRender256->beginCapture();
        glViewport(0,0, 256.0, 256.0);
        DrawQuad();
        mCurrentRender256->endCapture();

		mCurrentRender1->deactivate();
		glActiveTexture(GL_TEXTURE0);

        mDownsampleShader->activate();
        mCurrentRender256->activate();checkGLError();
        glUniform1iARB(RT, 0);checkGLError();
        mDownsampleShader->setUniform(texSize, 1.0 / 256.0);
        mCurrentRender128->beginCapture();
        glViewport(0,0, 128.0, 128.0);
        DrawQuad();
        mCurrentRender128->endCapture();

        mCurrentRender128->activate();checkGLError();
        glUniform1iARB(RT, 0);checkGLError();
        mDownsampleShader->setUniform(texSize, 1.0 / 128.0);
        mCurrentRender64->beginCapture();
        glViewport(0,0, 64.0, 64.0);
        DrawQuad();
        mCurrentRender64->endCapture();

        mCurrentRender64->activate();checkGLError();
        glUniform1iARB(RT, 0);checkGLError();
        mDownsampleShader->setUniform(texSize, 1.0 / 64.0);
        mCurrentRender32->beginCapture();
        glViewport(0,0, 32.0, 32.0);
        DrawQuad();
        mCurrentRender32->endCapture();

        mCurrentRender32->activate();checkGLError();
        glUniform1iARB(RT, 0);checkGLError();
        mDownsampleShader->setUniform(texSize, 1.0 / 32.0);
        mCurrentRender16->beginCapture();
        glViewport(0,0, 16.0, 16.0);
        DrawQuad();
        mCurrentRender16->endCapture();
#if 0
        mCurrentRender16->activate();checkGLError();
        glUniform1iARB(RT, 0);checkGLError();
        mDownsampleShader->setUniform(texSize, 1.0 / 16.0);
        mCurrentRender8->beginCapture();
        glViewport(0,0, 8.0, 8.0);
        DrawQuad();
        mCurrentRender8->endCapture();

        mCurrentRender8->activate();checkGLError();
        glUniform1iARB(RT, 0);checkGLError();
        mDownsampleShader->setUniform(texSize, 1.0 / 8.0);
        mCurrentRender4->beginCapture();
        glViewport(0,0, 4.0, 4.0);
        DrawQuad();
        mCurrentRender4->endCapture();

        mCurrentRender4->activate();checkGLError();
        glUniform1iARB(RT, 0);checkGLError();
        mDownsampleShader->setUniform(texSize, 1.0 / 4.0);
        mCurrentRender2->beginCapture();
        glViewport(0,0, 2.0, 2.0);
        DrawQuad();
        mCurrentRender2->endCapture();
		mDownsampleShader->deactivate();

		mExposureControl->activate();
        mCurrentRender2->activate();checkGLError();
        glUniform1iARB(RT, 0);checkGLError();
        mDownsampleShader->setUniform(texSize, 1.0 / 2.0);
        mCurrentRender1->beginCapture();
        glViewport(0,0, 1.0, 1.0);
        DrawQuad();
		mExposureControl->deactivate();
        mCurrentRender1->endCapture();
#endif
#endif
#if 1
        //
        // MAKE BLOOM
        //
// 16
        mBlurH->activate();
        glActiveTexture(GL_TEXTURE0);
        mCurrentRender16->activate();checkGLError();
        glUniform1iARB(blurHRT, 0);checkGLError();
        mBlurH->setUniform(blurHtexSize, 1.0 / 16.0);
        mBufferA->beginCapture();
        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0,0, 16.0, 16.0);
        DrawQuad();
        mBufferA->endCapture();

        mBlurV->activate();
        glActiveTexture(GL_TEXTURE0);
        mBufferA->activate();checkGLError();
        glUniform1iARB(blurVRT, 0);checkGLError();
        mBlurV->setUniform(blurVtexSize, 1.0 / 256.0);
		glActiveTexture(GL_TEXTURE1);
        mCurrentRender1->activate();checkGLError();
        glUniform1iARB(blurVexp, 1);checkGLError();
        //mBlurV->setUniform(blurVexp, mFrameExposure);
        mBloom16->beginCapture();
        DrawQuad(0.125/2.0, 0.125/2.0);
        mBloom16->endCapture();

        // 32
        mBlurH->activate();
        glActiveTexture(GL_TEXTURE0);
        mCurrentRender32->activate();checkGLError();
        glUniform1iARB(blurHRT, 0);checkGLError();
        mBlurH->setUniform(blurHtexSize, 1.0 / 32.0);
        mBufferA->beginCapture();
        glViewport(0,0, 32.0, 32.0);
        DrawQuad();
        mBufferA->endCapture();

        mBlurV->activate();
        glActiveTexture(GL_TEXTURE0);
        mBufferA->activate();checkGLError();
        glUniform1iARB(blurVRT, 0);checkGLError();
        mBlurV->setUniform(blurVtexSize, 1.0 / 256.0);
        glActiveTexture(GL_TEXTURE1);
        mCurrentRender1->activate();checkGLError();
        glUniform1iARB(blurVexp, 1);checkGLError();
        //mBlurV->setUniform(blurVexp, mFrameExposure);
        mBloom32->beginCapture();
        DrawQuad(0.125, 0.125);
        mBloom32->endCapture();

        // 64
        mBlurH->activate();
        glActiveTexture(GL_TEXTURE0);
        mCurrentRender64->activate();checkGLError();
        glUniform1iARB(blurHRT, 0);checkGLError();
        mBlurH->setUniform(blurHtexSize, 1.0 / 64.0);
        mBufferA->beginCapture();
        glViewport(0,0, 64.0, 64.0);
        DrawQuad();
        mBufferA->endCapture();

        mBlurV->activate();
        glActiveTexture(GL_TEXTURE0);
        mBufferA->activate();checkGLError();
        glUniform1iARB(blurVRT, 0);checkGLError();
        mBlurV->setUniform(blurVtexSize, 1.0 / 256.0);
        glActiveTexture(GL_TEXTURE1);
        mCurrentRender1->activate();checkGLError();
        glUniform1iARB(blurVexp, 1);checkGLError();
        //mBlurV->setUniform(blurVexp, mFrameExposure);
        mBloom64->beginCapture();
        DrawQuad(0.25, 0.25);
        mBloom64->endCapture();
        
        // 128
        mBlurH->activate();
        glActiveTexture(GL_TEXTURE0);
        mCurrentRender128->activate();checkGLError();
        glUniform1iARB(blurHRT, 0);checkGLError();
        mBlurH->setUniform(blurHtexSize, 1.0 / 128.0);
        mBufferA->beginCapture();
        glViewport(0,0, 128.0, 128.0);
        DrawQuad();
        mBufferA->endCapture();

        mBlurV->activate();
        glActiveTexture(GL_TEXTURE0);
        mBufferA->activate();checkGLError();
        glUniform1iARB(blurVRT, 0);checkGLError();
        mBlurV->setUniform(blurVtexSize, 1.0 / 256.0);
        glActiveTexture(GL_TEXTURE1);
        mCurrentRender1->activate();checkGLError();
        glUniform1iARB(blurVexp, 1);checkGLError();
        //mBlurV->setUniform(blurVexp, mFrameExposure);
        mBloom128->beginCapture();
        DrawQuad(0.5, 0.5);
        mBloom128->endCapture();

        //256
        mBlurH->activate();
        glActiveTexture(GL_TEXTURE0);
        mCurrentRender256->activate();checkGLError();
        glUniform1iARB(blurHRT, 0);checkGLError();
        mBlurH->setUniform(blurHtexSize, 1.0 / 256.0);
        mBufferA->beginCapture();
        glViewport(0,0, 256.0, 256.0);
        DrawQuad();
        mBufferA->endCapture();

        mBlurV->activate();
        glActiveTexture(GL_TEXTURE0);
        mBufferA->activate();checkGLError();
        glUniform1iARB(blurVRT, 0);checkGLError();
        mBlurV->setUniform(blurVtexSize, 1.0 / 256.0);
        glActiveTexture(GL_TEXTURE1);
        mCurrentRender1->activate();checkGLError();
        glUniform1iARB(blurVexp, 1);checkGLError();
        //mBlurV->setUniform(blurVexp, mFrameExposure);
        mBloom256->beginCapture();
        DrawQuad();
        mBloom256->endCapture();

		mBlurV->deactivate();
#endif
        pair<S32,S32> d = renderingView->getDimentions();
        glViewport(0,0, d.first, d.second);
        gRSystem->popProjMatrix();
        gRSystem->popWorldMatrix();
		//glEnable(GL_CULL_FACE);
        //glEnable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);

		mDownsampleShader->deactivate();

        return 0.0;
}

void CRenderer::makeBloomTexture(){
    
}

void CRenderer::setMotionBlurAmount(F32 amount){
    mMotionBlurAmount = amount;
}

F32 CRenderer::getMotionBlurAmount(){
    return mMotionBlurAmount;
}

void CRenderer::setMaxLuminance(F32 l){
	mMaxLuminance = l;
}

void CRenderer::setMinLuminance(F32 l){
	mMinLuminance = l;
}

F32  CRenderer::getMaxLuminance() const {
	return mMaxLuminance;
}

F32  CRenderer::getMinLuminance() const {
	return mMinLuminance;
}

void CRenderer::grabCurrentRender(){
    glDepthMask(0);
//    if(!isUseHDR()){
        if(mCurrentRender->wasUsed(1) || mCurrentRenderSmall->wasUsed(1) ||1){
            glReadBuffer(GL_BACK);
            pair<S32,S32> d = renderingView->getDimentions();
            mCurrentRender->activate();
			glCopyTexImage2D(GL_TEXTURE_RECTANGLE_ARB,0,GL_RGBA,0,0,d.first,d.second,0); checkGLError();

            F32 motionBlurAmount = getMotionBlurAmount();
            if(motionBlurAmount > 0.0){
                mSpeedBuffer->markUsed();
                S32 amount = mMotionBlurShader->getUniformLocation("amount");
                S32 currentRender = mMotionBlurShader->getUniformLocation("currentRender");
                S32 speedBuffer = mMotionBlurShader->getUniformLocation("speedBuffer");
                
                mMotionBlurShader->activate();
				mMotionBlurShader->setUniform(amount, motionBlurAmount * d.first);
                
                glActiveTexture(GL_TEXTURE1);
                mSpeedBuffer->activate();checkGLError();
                glUniform1iARB(speedBuffer, 1);checkGLError();
                glActiveTexture(GL_TEXTURE0);
                mCurrentRender->activate();checkGLError();
                glUniform1iARB(currentRender, 0);checkGLError();

                gRSystem->pushWorldMatrix();
                gRSystem->resetWorldMatrix();

                gRSystem->pushProjMatrix();
                gRSystem->resetProjMatrix();
	    
                pair<S32,S32> d = renderingView->getDimentions();
	            glOrtho(0,d.first,d.second,0,-999.0,999.0);
                glDisable(GL_DEPTH_TEST);
                glDisable(GL_CULL_FACE);

				DrawQuadInv(d.first, d.second, d.first, d.second);

				//mMotionBlurShader->setUniform(amount, motionBlurAmount * d.first);
				//DrawQuadInv(d.first, d.second, d.first, d.second);
                
                mMotionBlurShader->disable();
                glEnable(GL_CULL_FACE);
                gRSystem->popProjMatrix();
                gRSystem->popWorldMatrix();   
                glEnable(GL_DEPTH_TEST);

                // Regrab the rendering
                mCurrentRender->activate();
                glCopyTexImage2D(GL_TEXTURE_RECTANGLE_ARB,0,GL_RGBA,0,0,d.first,d.second,0); checkGLError();
            }
        }
  //  }

    if(mCurrentRenderSmall->wasUsed(1)||1){
        //if(isUseHDR()){
        //    mCurrentRender->endCapture();
        //}
        glColor3f(1.0,1.0,1.0);

        //S32 texSize = mDownsampleRectShader->getUniformLocation("texSize");
        S32 RT = mDownsampleRectShader->getUniformLocation("RT");
        
        gRSystem->pushWorldMatrix();
        gRSystem->resetWorldMatrix();

        gRSystem->pushProjMatrix();
        gRSystem->resetProjMatrix();
	    
        pair<S32,S32> d = renderingView->getDimentions();
	    glOrtho(0,d.first,d.second,0,-999.0,999.0);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        mDownsampleRectShader->activate();
        glActiveTexture(GL_TEXTURE0);
        mCurrentRender->activate();checkGLError();
        glUniform1iARB(RT, 0);checkGLError();

        mCurrentRenderSmall->beginCapture();
        glBegin(GL_QUADS);
		glTexCoord2f(0.0, d.second); glVertex2f(0.0, d.second - 0.0); 
        glTexCoord2f(0.0, 0.0); glVertex2f(0.0, d.second - renderSmallSize); 
		glTexCoord2f(d.first, 0.0); glVertex2f(renderSmallSize, d.second - renderSmallSize); 
		glTexCoord2f(d.first, d.second); glVertex2f(renderSmallSize, d.second - 0.0); 
        glEnd();
        mCurrentRenderSmall->endCapture();
        mDownsampleRectShader->disable();

        glEnable(GL_CULL_FACE);
 
        gRSystem->popProjMatrix();
        gRSystem->popWorldMatrix();   
        glEnable(GL_DEPTH_TEST);

        if(isUseHDR()){
			calcAverageLuma(mCurrentRenderSmall);
        }
    }
    glDepthMask(1);
}

void CRenderer::enter2DMode(){
    gRSystem->pushWorldMatrix();
    gRSystem->resetWorldMatrix();
    gRSystem->pushProjMatrix();
    gRSystem->resetProjMatrix();
	//glOrtho(0,d.first,d.second,0,-999.0,999.0);
	glOrtho(0,800,600,0,-999.0,999.0);
    glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setAmbientLight(CColorf(1.0, 1.0, 1.0));
    setLight(0);
}

void CRenderer::exit2DMode(){
        glEnable(GL_DEPTH_TEST);
        gRSystem->popProjMatrix();
        gRSystem->popWorldMatrix();
		glEnable(GL_CULL_FACE);
}

CTexture *CRenderer::registerAttenuation(const std::string &name){
	return 0;
}


void CRenderer::renderPerformaceInfo(){
	char text[256];
	F32 avrFrameLength = 1.0f / eng->getFPS();
	F32 perC = mFrameCommitTimer.getAvrTime() / avrFrameLength * 100.0f;
	sprintf(text, "Frame commit : %f (%4.2f%%)\n", mFrameCommitTimer.getAvrTime(), perC);
	eng->getDefaultFont()->renderString(text);
	sprintf(text, "Texture data size : %4.1f MB\n", (F32)getTextureDataSize() / (1024.0 * 1024.0));
	eng->getDefaultFont()->renderString(text);
}

U32 CRenderer::getTextureDataSize(){
	CTextures &textures = CEngine::getSingleton()->textures;
	
	U32 total = 0;

	CResourceManager::ResourceList::iterator i, end;
	end = textures.getResources()->end();
	for(i = textures.getResources()->begin() ; i != end ; ++i){
		CResource *res = *i;
		CTexture *tex = dynamic_cast<CTexture *>(res);
		if(tex->wasUsed())
			total += tex->getMemoryUsage();
	}

	return total;
}

void CRenderer::debugDrawLine(const CVector3 &start, const CVector3 &end, CColor *c){
    
}

void CRenderer::downSample(CTexture2D* in, CRenderTexture* out, U32 inw, U32 inh, U32 outw, U32 outh) const {
	int inwidth, inheight;
	int outwidth, outheight;
	int RT = mDownsampleShader->getUniformLocation("RT");
	int texSize = mDownsampleShader->getUniformLocation("texSize");
	
	in->getDimentions(&inwidth, &inheight);
	pair<S32, S32> dim = out->getDimentions();
	outwidth = dim.first;
	outheight = dim.second;

	F32 maxu = (F32)inw / inwidth;
	F32 maxv = (F32)inh / inheight;

	glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

	mDownsampleShader->activate();checkGLError();
	glActiveTexture(GL_TEXTURE0);
	in->activate();checkGLError();
	glUniform1iARB(RT, 0);checkGLError();
	mDownsampleShader->setUniform(texSize, 1.0 / inwidth);

	out->beginCapture();
	glViewport(0,0, outw, outh);
	DrawQuad(maxu,maxv);
	out->endCapture();
	in->deactivate();
	mDownsampleShader->deactivate();checkGLError();

	dim = getRenderingView().getDimentions();
	glViewport(0,0, dim.first, dim.second);
}

void CRenderer::scaleDown(CTexture2D* in, CRenderTexture* out, int outWidth, CRenderTexture* tempTex) const {
	int inwidth, outwidth, inheight;
	CTexture2D *from = in;
	int RT = mDownsampleShader->getUniformLocation("RT");
	int texSize = mDownsampleShader->getUniformLocation("texSize");

	glActiveTexture(GL_TEXTURE0);

	from->getDimentions(&inwidth, &inheight);

	while(true){
		from->activate();
		outwidth = inwidth >> 1;

		mDownsampleShader->activate();checkGLError();
		glUniform1iARB(RT, 0);checkGLError();
		mDownsampleShader->setUniform(texSize, 1.0 / inwidth);

		tempTex->beginCapture();
		glViewport(0,0, outwidth, outwidth);
		DrawQuad();
		tempTex->endCapture();
		mDownsampleShader->deactivate();checkGLError();

		if(outwidth == outWidth)
			break;
	}
}


