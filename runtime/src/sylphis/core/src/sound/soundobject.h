/***************************************************************************
                        soundobject.h  -  description
                             -------------------
    begin                : Fri Jun 6 2003
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

#ifndef _SOUNDOBJECT_H_
#define _SOUNDOBJECT_H_

#include <sceneobject.h>
class CSoundEnv;
class CActor;

SmartPointer(CSoundObject);
class CSoundObject : public CSceneObject {
DEFINE_SERIALIZABLE(CSoundObject);
public:
    CSoundObject();
	CSoundObject(CSoundEnv *se);
	virtual ~CSoundObject();

	void setVelocity(const CVector3 &v);
	CVector3 &getVelocity();

	virtual void updateSound();

	virtual void updateData(F64 time);
	virtual void render(CRenderer *renderer);
	virtual void setListener(CActor *a);

    virtual void save(CSerializer *ser) const;
    virtual void load(CSerializer *ser);
protected:
	CVector3 mVelocity;
	CSoundEnv *mSoundEnv;

	CActor *mListenerActor;
};


#endif
