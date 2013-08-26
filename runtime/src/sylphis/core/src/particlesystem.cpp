/***************************************************************************
                      particlesystem.cpp  -  description
                             -------------------
    begin                : Sat May 13 2006
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

#include <engine.h>
#include <particlesystem.h>
#include <material.h>
#include <physicsenvironment.h>
#include <platform/types.h>
#include <scenemanager.h>
#include <trace.h>

CParticleSystemDef::CParticleSystemDef(){
    mBudget = 1000;
    mAcceleration = CPhysicsEnv::getSingleton()->getGravity();
    mRotation = 0.0;

	mEmitterShape = PARTICLE_EMITTER_SPHERE;
	mEmitterDimensions = CVector3::ZERO;
	mEmitterDimensions.x = 50.0;

	mSpawnDirection = PARTICLE_SPAWN_DIRECTION_NATURAL;
	mSpawnDirectionVariation = 5.0;

	mSpawnVelocityMax = 20.0;
	mSpawnVelocityMin = 10.0;

    mEmitRate = 100.0;
    mEmitTime = -1.0; // Forever

    CInterpolatorVector3 *ci = new CInterpolatorVector3();
    ci->insert(CVector3(1.0,1.0,1.0), 0.0);
    ci->insert(CVector3(0.0,0.0,0.0), 1.0);
    mColorEnvelope = ci;

    CInterpolatorF32 *si = new CInterpolatorF32();
    //si->insert(1.1f, 0.0);
    //si->insert(1.1f, 1.0);
    mSizeEnvelope = si;

    si = new CInterpolatorF32();
    //si->insert(0.1f, 1.0);
    //si->insert(1.1f, 1.0);
    mAspectRatioEnvelope = si;

    mParticleType = PARTICLE_TRACER;
}

void CParticleSystemDef::setMaterial(const std::string &name){
	mMaterial = CEngine::getSingleton()->mMaterials.registerMaterial(name);
}

CMaterial* CParticleSystemDef::getMaterial(){
	return mMaterial;
}

CInterpolatorF32* CParticleSystemDef::getSizeEnvelope(){
	return mSizeEnvelope;
}
CInterpolatorF32* CParticleSystemDef::getAspectRatioEnvelope(){
	return mAspectRatioEnvelope;
}
CInterpolatorVector3* CParticleSystemDef::getColorEnvelope(){
	return mColorEnvelope;
}

CParticleSystem::CParticleSystem(){
    mEmitFrac = 0.0;
    mEmittingTime = 0.0;
    mBudget = 0;
	mDieAfterUse = true;

    mVertexBuffer = new CVertexBuffer();
    mVertexBuffer->addComponent(CVertexBuffer::POSITION);
    mVertexBuffer->addComponent(CVertexBuffer::NORMAL);     // This is used for the color. Smart?! :P
    mVertexBuffer->addComponent(CVertexBuffer::TEX_COORD);

    mElems = new CIndexBuffer();
    mSVertexBuffer = 0;

    mDef = 0;
}

CParticleSystem::~CParticleSystem(){
}

void CParticleSystem::setFrame(F32 f){

}
void CParticleSystem::render(CRenderer &r, const CMatrix4 &transform){
    CArrays *a = r.getArrays();
    
    a->mVertexBuffer = mVertexBuffer;
    a->verts.setupBuffer(mVertexBuffer, CVertexBuffer::POSITION);
    a->normals.setupBuffer(mVertexBuffer, CVertexBuffer::NORMAL);
	a->tex_st.setupBuffer(mVertexBuffer, CVertexBuffer::TEX_COORD);

	CMatrix4 invTrans = transform;
	invTrans.invert();

    CMatrix4 &mv = r.getModelViewMatrix();

    // And setup up up,right vectors
    CVector3 up, right, forward, campos, trans;
    up[0] = mv.m[0][0];
	up[1] = mv.m[1][0];
	up[2] = mv.m[2][0];

	right[0] = mv.m[0][1];
	right[1] = mv.m[1][1];
	right[2] = mv.m[2][1];

    forward[0] = mv.m[0][2];
	forward[1] = mv.m[1][2];
	forward[2] = mv.m[2][2];

	// Local camera position
    campos  = invTrans * r.getCamera()->getDerivedPosition();

	trans[0] = mv.m[3][0];
	trans[1] = mv.m[3][1];
	trans[2] = mv.m[3][2];

    U32 count = mParticles.size();
    ParticleList::const_iterator i, pend;
    pend = mParticles.end();

    TParticleVertex *v = mSVertexBuffer;
    for(i = mParticles.begin() ; i != pend ; ++i){
        F32 sizeX = (*i)->mSizeX;
        F32 sizeY = (*i)->mSizeY;
        if(mDef->mParticleType == PARTICLE_BILLBOARD){
            CQuaternion q;
            q.fromAngleAxis(forward, (*i)->mRotation);
            CMatrix3 m;
            q.toRotationMatrix(m);
            CVector3 upR = m * up;
            CVector3 rightR = m * right;

            const CVector3 &color = (*i)->mColor;
            const CVector3 corner = (*i)->mPosition - (upR * sizeY + rightR * sizeX)  * 0.5;
            v->position = corner;
            v->color = color;
            v->texCoord = CTexCoord(0.0,  0.0);
            ++v;
            v->position = corner + upR * sizeY;
            v->color = color;
            v->texCoord = CTexCoord(0.0,  1.0);
            ++v;
            v->position = corner + (upR * sizeY + rightR * sizeX);
            v->color = color;
            v->texCoord = CTexCoord(1.0,1.0);
            ++v;
            v->position = corner + rightR * sizeX;
            v->color = color;
            v->texCoord = CTexCoord(1.0,0.0);
            ++v;
        } else {

            CVector3 upR = (*i)->mVelocity;
			float l = upR.length();
			if(l < 1.0){
				upR /= l;
			}

			CVector3 pos = (*i)->mPosition;
			pos.y += 10.0;

			CVector3 eyevec = campos - pos;
			
            CVector3 rightR = ((*i)->mVelocity ^ eyevec).normalized();
			
            const CVector3 &color = (*i)->mColor;
            const CVector3 corner = pos - (upR * sizeY + rightR * sizeX)  * 0.5;
            v->position = corner;
            v->color = color;
            v->texCoord = CTexCoord(0.0,  0.0);
            ++v;
            v->position = corner + upR * sizeY;
            v->color = color;
            v->texCoord = CTexCoord(0.0,  1.0);
            ++v;
            v->position = corner + (upR * sizeY + rightR * sizeX);
            v->color = color;
            v->texCoord = CTexCoord(1.0,1.0);
            ++v;
            v->position = corner + rightR * sizeX;
            v->color = color;
            v->texCoord = CTexCoord(1.0,0.0);
            ++v;
        }
    }

    a->numVerts = count * 4;
    a->numElems = count * 6;

    mVertexBuffer->uploadData(mSVertexBuffer, 0, 4 * count * sizeof(TParticleVertex));
    
    a->mElems = mElems;
}

CSilhouette* CParticleSystem::getSilhoutte(CVector3 &o){
    return 0;
}

void CParticleSystem::setBudget(U32 count){
    if(mBudget != count){
        mVertexBuffer->allocate(4 * count * sizeof(TParticleVertex), CVertexBuffer::DYNAMIC, false, 0);
        mElems->allocate(6 * count * sizeof(U32), CVertexBuffer::STATIC, false, 0);

        U32 *elems = new U32[6 * count];
        for(int i = 0 ; i < count ; ++i){
            U32 si = i * 6;
            U32 vi = i * 4;
            elems[si + 0] = vi;
            elems[si + 1] = vi + 1;
            elems[si + 2] = vi + 2;
            elems[si + 3] = vi;
            elems[si + 4] = vi + 2;
            elems[si + 5] = vi + 3;
        }

        mElems->uploadData(elems, 0, 6 * count * sizeof(U32));
        delete [] elems;

        mBudget = count;

        if(mSVertexBuffer)
            delete [] mSVertexBuffer;
        mSVertexBuffer = new TParticleVertex[4 * count];
    }
}


U32 CParticleSystem::getBudget(){
    return mDef->mBudget;
}

void CParticleSystem::setDieAfterUse(bool b){
	mDieAfterUse = b;
}

bool CParticleSystem::isDieAfterUse() const {
	return mDieAfterUse;
}	

void CParticleSystem::fire() {
	mEmittingTime = 0.0;
}

void CParticleSystem::renderWithShader(CRenderer &r, CMaterial *shader){
    
}

void CParticleSystem::setEmitter(CSceneObject *o){
	mEmitter = o;
}

CSceneObject* CParticleSystem::getEmitter(){
	return mEmitter;
}

CParticle* CParticleSystem::createParticle(){
    if(mParticles.size() >= mDef->mBudget)
        return 0;
    CParticle *p = new CParticle();

    p->mPosition = CVector3::ZERO;
    p->mVelocity = CVector3(0.0, 0.0, 0.0);

	if(mDef->mEmitterShape == PARTICLE_EMITTER_SPHERE){
		CVector3 dir = CVector3(randomInRange(-1.0, 1.0), randomInRange(-1.0, 1.0), randomInRange(-1.0, 1.0)).normalized();
		p->mPosition = dir * mDef->mEmitterDimensions.x;
		if(mDef->mSpawnDirection == PARTICLE_SPAWN_DIRECTION_NATURAL){
			p->mVelocity = dir * randomInRange(mDef->mSpawnVelocityMin, mDef->mSpawnVelocityMax);
		} else {
			p->mVelocity = CVector3(0.0, 0.0, 1.0) * randomInRange(mDef->mSpawnVelocityMin, mDef->mSpawnVelocityMax);
		}
	}

	CVector3 var = CVector3(randomInRange(-1.0, 1.0), randomInRange(-1.0, 1.0), randomInRange(-1.0, 1.0));
	p->mVelocity += var * mDef->mSpawnDirectionVariation;

    if(mEmitter){
        p->mPosition += mEmitter->getDerivedPosition();
		p->mVelocity = mEmitter->getDerivedOrientation() * p->mVelocity;
    }
    
    if(mDef->mRotation != 0.0)
        p->mRotation = mDef->mRotation;
    else
        p->mRotation = 2 * PI * random();

    p->mRotationSpeed = mDef->mRotationSpeed;
    p->mTimeLived = 0.0;
	p->mLifetime = mDef->mLifeTime;
    p->mSizeX = 10.0;
    p->mSizeY = 10.0;
    mParticles.push_back(p);

    return p;
}

bool CParticleSystem::update(F64 time){
    if(mDef->mEmitTime > mEmittingTime || mDef->mEmitTime < 0.0){
#if 1
        F64 f = time * mDef->mEmitRate;
        mEmitFrac += f;

        if(mEmitFrac > 1.0){
            U32 times = floor(mEmitFrac);
            mEmitFrac -= times;
            for(int i = 0 ; i < times ; ++i)
                createParticle();
        }
#else
        if(mNoEmitTime >= mEmitPeriod){
            U32 times = floor(mNoEmitTime / mEmitPeriod);
            mNoEmitTime -= mEmitPeriod * times;
            for(int i = 0 ; i < times ; ++i)
                createParticle();
        } else {
            mNoEmitTime += time;
        }
#endif
    }

    mEmittingTime += time;

    // Update particles
    ParticleList::iterator i, pend;
    pend = mParticles.end();
    for(i = mParticles.begin() ; i != pend ;){
        (*i)->mVelocity += mDef->mAcceleration * time;

        CVector3 end = (*i)->mPosition + (*i)->mVelocity * time;

		if(mDef->mCollitionsEnabled){
			CTraceResult tr;
			CAABoundBox bbox(CVector3(1.0, 1.0, 1.0));
			CEngine::getSingleton()->getClient()->getSceneManager()->trace(&tr, bbox, (*i)->mPosition, end);

			if(tr.frac < 1.0){
				(*i)->mVelocity -= tr.plane.normal * (tr.plane.normal * (*i)->mVelocity) * 2.0;
				(*i)->mVelocity *= mDef->mBounce;
				(*i)->mPosition = tr.mEnd;
			} else {
				(*i)->mPosition = end;
			}
		} else {
			(*i)->mPosition = end;
		}

        (*i)->mRotation += time * (*i)->mRotationSpeed;
        (*i)->mTimeLived += time;

        F32 t = (*i)->mTimeLived / (*i)->mLifetime;
        (*i)->mColor = mDef->mColorEnvelope->getAt(t);
        (*i)->mSizeX = mDef->mSizeEnvelope->getAt(t);
        (*i)->mSizeY = (*i)->mSizeX * mDef->mAspectRatioEnvelope->getAt(t);

        if((*i)->mTimeLived > (*i)->mLifetime){
            delete (*i);
            i = mParticles.erase(i);
		} else {
			++i;
		}
    }

    if(mDef->mEmitTime < mEmittingTime && mParticles.size() == 0){
		if(isDieAfterUse())
			return false;
		else
			return true;
    } else {
        return true;
    }
}

void CParticleSystem::addParticle(U32 pos, const CParticle &p){
    
}

