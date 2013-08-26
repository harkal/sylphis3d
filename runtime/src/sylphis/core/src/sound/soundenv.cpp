/***************************************************************************
                         soundenv.cpp  -  description
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

#include <al/al.h>
#include <al/alut.h>
#include <sound/soundenv.h>
#include <sound/soundbuffer.h>
#include <sound/soundwave.h>
#include <sound/soundogg.h>
#include <engine.h>
#include <console.h>
#include <exception.h>
#include <configfile.h>

ALvoid alError(char *szText){
	ALint error;
    if(error = alGetError() != AL_NO_ERROR){
        //char *errstr = (char *)alGetString(-error);
        gcon.errorf("%s%s\n", szText, 0); //FIXME!
    }
}

CSoundEnv::CSoundEnv(){
}

CSoundEnv::~CSoundEnv(){

}

void CSoundEnv::startup(){
    gcon.printf("-- CSoundEnv::startup\n");
	//alutInit (0, 0); // init OpenAL
    mDevice = alcOpenDevice((ALubyte*)"DirectSound3D");
    mContext = alcCreateContext(mDevice, NULL);
    alcGetError(mDevice);
	alcMakeContextCurrent(mContext);
    alError("Coundn't not init OpenAL");

    mEAXLevel = 0;
    if (alIsExtensionPresent((ALubyte *)"EAX2.0") == AL_TRUE){
		mEAXLevel = 2;
        gcon.printf("... EAX 2.0 detected\n");
	} else {
		if (alIsExtensionPresent((ALubyte *)"EAX") == AL_TRUE) {
			mEAXLevel = 1;
            gcon.printf("... EAX 1.0 detected\n");
		}
	}
    if(!mEAXLevel){
        gcon.printf("... EAX not detected\n");
    }

    // global settings
	F32 gain = gCfgFile.getAsFloat("s_globalgain");
	if(gain < 0.0)gain = 0.0;
	alListenerf(AL_GAIN, 1.0);
    alError("Failed to setup the listener");
	alDopplerFactor(1.0);
    alError("Coundn't set doppler factor");
	S32 speed = gCfgFile.getAsInt("s_soundspeed");
	if(speed < 10)speed = 10;
    gcon.printf("... Setting speed of sound at %d units/sec\n", speed);
	alDopplerVelocity(speed);
	alError("Coundn't set doppler velocity");
	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);

}

void CSoundEnv::shutdown(){
	mSoundListener = 0;
	alutExit();
}

void CSoundEnv::registerSoundObject(CSoundObject *s){
	assert(0 && "Not implemented");
	//mSoundObjects[s] = s;
}

void CSoundEnv::unregisterSoundObject(CSoundObject *s){
	assert(0 && "Not implemented");
	//SoundObjectMap::iterator i;
	//i = mSoundObjects.find(s);
	//mSoundObjects.erase(i);
}

void CSoundEnv::setActiveListener(CSoundListener *sl){
	mSoundListener = sl;
}

CSoundListener *CSoundEnv::getActiveListener(){
	return mSoundListener;
}

CSceneManager *CSoundEnv::getSceneManager(){
	return CEngine::getSingleton()->getClient()->getSceneManager();
}

CSoundBuffer *CSoundEnv::bufferFactory(const std::string &name){
	if(CSoundWave::isWaveFile(name)){
		CSoundWave *wave = new CSoundWave();
		wave->setName(name);
		wave->load();
		return wave;
	}
	else if(CSoundOgg::isOggFile(name)){
		CSoundOgg *ogg = new CSoundOgg();
		ogg->setName(name);
		ogg->load();
		return ogg;
	}
	else {
		return 0;
	}
}

void CSoundEnv::registerSoundBuffer(CSoundBuffer *sb){
	resources.push_back(sb);
}

CSoundBuffer *CSoundEnv::registerSoundBuffer(const std::string &name){
	CSoundBuffer *buffer;

	buffer = findSoundBuffer(name);
	if(buffer)
		return buffer;

	buffer = bufferFactory(name);
    if(!buffer)
        throw CException("Sound file not found : " + name);
	registerSoundBuffer(buffer);
	
	return buffer;
}

CSoundBuffer *CSoundEnv::findSoundBuffer(const std::string &name){
	return static_cast<CSoundBuffer *>(find(name));
}
