/***************************************************************************
                         soundenv.cpp  -  description
                             -------------------
    begin                : Fri Jun 6 2003
    copyright            : (C) 2002 by Harry Kalogirou
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
