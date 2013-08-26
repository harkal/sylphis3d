/***************************************************************************
                        soundsource.h  -  description
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

#ifndef _SOUNDSOURCE_H_
#define _SOUNDSOURCE_H_

#include <sound/soundobject.h>
#include <sound/soundbuffer.h>
#include <python/waitable.h>
#include <Python.h>
class CSoundEnv;


SmartPointer(CSoundSource);
class CSoundSource : public CSoundObject, public CWaitable {
DEFINE_SERIALIZABLE(CSoundSource);
public:
    CSoundSource();
	CSoundSource(CSoundEnv *se);
	virtual ~CSoundSource();

	virtual void setBuffer(CSoundBuffer *buffer);

	enum ePlayState {
		SOURCE_PLAYING,
		SOURCE_STOPPED,
		SOURCE_PAUSED
	};

	virtual void play(CSoundBuffer *b = 0);
	virtual void pause();
	virtual void stop();
	virtual void rewind();

	virtual void setState(ePlayState s);
	virtual ePlayState getState();

	virtual void setSpatial(bool s);
	virtual void setLooping(bool l);
	virtual void setGain(F32 g);
	virtual void setPitch(F32 p);
	virtual void setFalloff(F32 d);

	virtual void updateSound();
	virtual void sendNewState(ePlayState s);

    virtual void save(CSerializer *ser) const;
    virtual void load(CSerializer *ser);
protected:
	CSoundBufferPtr mBuffer;
	ALuint mSource;
	bool mSpatial;
	bool mLooping;
	ePlayState mState;
};


#endif
