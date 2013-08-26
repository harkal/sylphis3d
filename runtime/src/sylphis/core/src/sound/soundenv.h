/***************************************************************************
                          soundenv.h  -  description
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

#ifndef _SOUNDENV_H_
#define _SOUNDENV_H_


#include <map>
#include <al/al.h>
#include <al/alc.h>

#include <resourcemanager.h>
#include <sound/soundlistener.h>

class CSoundBuffer;

class CSoundEnv : public CResourceManager {
public:
	CSoundEnv();
	virtual ~CSoundEnv();

	virtual void startup();
	virtual void shutdown();

	void registerSoundObject(CSoundObject *s);
	void unregisterSoundObject(CSoundObject *s);

	void setActiveListener(CSoundListener *sl);
	CSoundListener *getActiveListener();

	virtual CSceneManager *getSceneManager();

	// Sound Manager
	virtual void registerSoundBuffer(CSoundBuffer *sb);
	virtual CSoundBuffer *registerSoundBuffer(const std::string &name);
	virtual CSoundBuffer *findSoundBuffer(const std::string &name);

protected:
	typedef std::map<CSoundObject *, CSoundObjectPtr> SoundObjectMap;
	CSoundListenerPtr mSoundListener;
	SoundObjectMap mSoundObjects;

	CSoundBuffer *bufferFactory(const std::string &name);
    ALCcontext *mContext;
	ALCdevice *mDevice;
    S32 mEAXLevel;
};

#endif
