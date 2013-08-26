/***************************************************************************
						light.h  -  description
                             -------------------
    begin                : Thu Apr 4 2002
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


#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "sceneobject.h"
#include "pointer.h"
#include "vector.h"

class CSceneManager;
SmartPointer(CTexture);

SmartPointer(CLight);
class CLight : public CSceneObject {
DEFINE_SERIALIZABLE(CLight);
public:
    enum EType {
        omni = 1,
        spot = 2,
		flickering = 4,
		strobo = 8,
		pulsating = 16,
		sun = 32
    };

	CLight();
	CLight(CSceneManager *sm);
	virtual ~CLight();

    void setType(EType t);
    EType getType() const;

    void setColor(CVector4 &color);
	void setColor(CVector3 &color);
    CVector4 &getColor();

	void setFilter(CTexture *f);
	CTexture *getFilter();

	void setAttenuation(CTexture *f);
	CTexture *getAttenuation();

    void setRadius(F32 r);
	void setRadius(F32 x, F32 y, F32 z);
	void setRadius(const CVector3 &v);

	void setCenter(F32 x, F32 y, F32 z);
	void setCenter(const CVector3 &v);

    const CVector3 &getRadius();
	const CVector3 &getCenter();

	void setIsShadowCaster(bool c);
	bool isShadowCaster();

	void setSpecular(bool c);
	bool givesSpecular();

//    void fix();
//    void unfix();
//    bool isFixed();

	bool isActive();
	void setActive(bool a);

	void setPhase(F32 f);
	void setFreq(F32 f);

    void updateData(F64 time);

	virtual CVector3 getEmissionPosition();

	virtual CVector3 &getPosition();
	virtual CVector3 getDerivedPosition() const;
	virtual CVector3 getDirection() const;

	virtual void setPosition(const CVector3 &p);
    virtual void render(CRenderer *renderer);

    virtual void save(CSerializer *ser) const;
    virtual void load(CSerializer *ser);

protected:
    EType mType;
	CVector4 mColor;
	CVector4 mColorScaled;
	F32 mColorScale;
	bool	mActive;
	CTexturePtr mFilter;
	CTexturePtr mAttenuation;
    CVector3 mRadius;
	CVector3 mCenter;
	bool	mCastShadows;
	bool	mSpecular;
    bool    mFixed;

	F32	mTime;
	F32	mPhase;
	F32	mFreq;
};


#endif

