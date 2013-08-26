/***************************************************************************
                        particlesystem.h  -  description
                             -------------------
    begin                : Sat May 13 2006
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

#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include <renderable.h>
#include <list>
#include <interpolator.h>
#include <string>

SmartPointer(CMaterial);

class CParticle {
public:

    CVector3    mPosition;
    CVector3    mVelocity;
    CVector3    mColor;
    F32         mSizeX;
    F32         mSizeY;
    F32         mRotation;
    F32         mRotationSpeed;
    F32         mLifetime;
    F32         mTimeLived;
};

typedef struct {
    CVector4    position;
    CVector3    color;
    CTexCoord   texCoord;
} TParticleVertex;

enum EParticleType {
    PARTICLE_BILLBOARD,
    PARTICLE_TRACER
};

enum EParticleEmitterShape {
	PARTICLE_EMITTER_BOX,
	PARTICLE_EMITTER_SPHERE
};

enum EParticleSpawnDirection {
	PARTICLE_SPAWN_DIRECTION_NATURAL,
	PARTICLE_SPAWN_DIRECTION_AIMED
};

class CParticleSystemDef {
public:
    CParticleSystemDef();

	void					setMaterial(const std::string &name);
	CMaterial*				getMaterial();
	CInterpolatorF32*		getSizeEnvelope();
	CInterpolatorF32*		getAspectRatioEnvelope();
	CInterpolatorVector3*   getColorEnvelope();

    // Emmiter properties
	EParticleEmitterShape	mEmitterShape;
	CVector3				mEmitterDimensions;

	EParticleSpawnDirection	mSpawnDirection;
	F64						mSpawnDirectionVariation;
	F64						mSpawnVelocityMin;
	F64						mSpawnVelocityMax;

    F64						mEmitRate;
    F64		                mEmitTime;

    U32						mBudget;
    CVector3				mAcceleration;

    F32                     mRotation;
	F32						mRotationSpeed;

	F64						mLifeTime;

	bool					mCollitionsEnabled;
	F32						mBounce;

    CInterpolatorVector3Ptr mColorEnvelope;
    CInterpolatorF32Ptr     mSizeEnvelope;
    CInterpolatorF32Ptr     mAspectRatioEnvelope;
    
    EParticleType			mParticleType;

	CMaterialPtr			mMaterial;
};

SmartPointer(CParticleEmitter);

SmartPointer(CParticleSystem);
class CParticleSystem : public CRenderable {
public:
                            CParticleSystem();
    virtual                 ~CParticleSystem();

    virtual void            setFrame(F32 f);
    virtual void            render(CRenderer &r, const CMatrix4 &transform);
    virtual CSilhouette*    getSilhoutte(CVector3 &o);
    virtual void            renderWithShader(CRenderer &r, CMaterial *shader);

    virtual bool            update(F64 time);

    void                    setBudget(U32 count);
    U32                     getBudget();

	void                    setDieAfterUse(bool b);
    bool                    isDieAfterUse() const;

	void					setEmitter(CSceneObject *o);
	CSceneObject*			getEmitter();

	void					fire();

    void                    setDefinition(CParticleSystemDef *d){
                                mDef = d;
                                setBudget(mDef->mBudget);
                            };

    CParticleSystemDef      *getDefinition(){return mDef;};

    CParticle*              createParticle();

    typedef std::list<CParticle*> ParticleList;
protected:
    void                addParticle(U32 pos, const CParticle &p);

    // Hardware buffers
    CVertexBufferPtr    mVertexBuffer;
    CIndexBufferPtr     mElems;

    // Temp buffers
    TParticleVertex     *mSVertexBuffer;

    CSceneObjectPtr     mEmitter;
    F64                 mEmitFrac;
    F64                 mEmittingTime;

    ParticleList        mParticles;    

    U32                 mBudget;

	bool				mDieAfterUse;

    CParticleSystemDef  *mDef;
};

typedef std::list<CParticleSystemPtr> ParticleSystemList;

#endif

