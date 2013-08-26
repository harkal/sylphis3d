/***************************************************************************
                        soundsource.cpp  -  description
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

#include <Python.h>

#include <sound/soundenv.h>
#include <sound/soundsource.h>
#include <sound/soundbuffer.h>
#include <console.h>
#include <al/al.h>
#include <al/alut.h>
#include <actor.h>
#include <python/mthread.h>

static ALvoid alError(char *szText){
	ALint error;
	if(error = alGetError() != AL_NO_ERROR)
		gcon.printf("%s%s", szText, 0);
}

IMPLEMENT_SERIALIZABLE(CSoundSource);

CSoundSource::CSoundSource(){
}

CSoundSource::CSoundSource(CSoundEnv *se) : CSoundObject(se) {
	alGenSources(1, &mSource);
	alError("genSources ");
	setFalloff(64.0);
	setSpatial(true);
}

CSoundSource::~CSoundSource(){
	//gcon.printf("Deleting sound source : %p\n", this);
	alDeleteSources(1, &mSource);
}

void CSoundSource::setBuffer(CSoundBuffer *buffer){
	mBuffer = buffer;

    if(!mBuffer)
        return;

	if(buffer->isStreaming()){
		ALuint b[2];
		b[0] = mBuffer->getBuffer();
		mBuffer->fillBuffer(b[0]);
		b[1] = mBuffer->getBuffer();
		mBuffer->fillBuffer(b[1]);
		alSourceQueueBuffers(mSource, 2, b);
		alError("setBuffer ");
	} else {
		alSourcei(mSource, AL_BUFFER, mBuffer->getBuffer());
	}
}

void CSoundSource::play(CSoundBuffer *b){
    if(getState() == SOURCE_PLAYING)
        return;

	if(b)
		setBuffer(b);

	if(!mBuffer->isStreaming()){
		alSourcei(mSource, AL_LOOPING, mLooping ? AL_TRUE : AL_FALSE);
	}
	alSourcePlay(mSource);
	setState(SOURCE_PLAYING);
}

void CSoundSource::pause(){
	alSourcePause(mSource);
	setState(SOURCE_PAUSED);
}

void CSoundSource::stop(){
	alSourceStop(mSource);
	setState(SOURCE_STOPPED);
}

void CSoundSource::rewind(){
	alSourceRewind(mSource);
}

void CSoundSource::setSpatial(bool s){
	mSpatial = s;
	//alSourcei(mSource, AL_SOURCE_RELATIVE, s ? AL_FALSE : AL_TRUE);
	alSourcef(mSource, AL_ROLLOFF_FACTOR, s ? 1.0 : 0.0);
}

void CSoundSource::setLooping(bool l){
	mLooping = l;
}

void CSoundSource::setGain(F32 g){
	if(g < 0.0)
		g = 0.0;
	alSourcef(mSource, AL_GAIN, g);
}

void CSoundSource::setPitch(F32 p){
	if(p < 0.0)
		p = 0.0;
	alSourcef(mSource, AL_PITCH, p);
}

void CSoundSource::setFalloff(F32 d){
	if(d<1.0)
		d = 1.0;
	alSourcef(mSource, AL_REFERENCE_DISTANCE, d);
}

void CSoundSource::setState(CSoundSource::ePlayState s){
	mState = s;
}

CSoundSource::ePlayState CSoundSource::getState(){
	return mState;
}

void CSoundSource::updateSound(){
	if(!mBuffer)
		return;

	if(mSpatial){
		const CVector3 &pos = getDerivedPosition();
		alSourcefv(mSource, AL_POSITION, (ALfloat *)&pos.x);
        //gcon.printf("(%f %f %f)\n", pos.x, pos.y, pos.z);
		alSourcefv(mSource, AL_VELOCITY, &mVelocity.x);
	}

	ALint processed;
	alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed);

	if(!mBuffer->isStreaming()){
		ALint p;
		alGetSourcei(mSource, AL_SOURCE_STATE, &p);
		if(p == AL_STOPPED && getState() != SOURCE_STOPPED){
			// Send the event
			sendNewState(SOURCE_STOPPED);
			stop();
		}
		return;
	}

	if(processed == 0){
		return;
	} 

	ALuint b = mBuffer->getBuffer();
	alSourceUnqueueBuffers(mSource, 1, &b);

	if(!mBuffer->fillBuffer(b)){
		//stop();
	}

	alSourceQueueBuffers(mSource, 1, &b);
	
	if(processed == 2){
		play();	
	}
}

void CSoundSource::sendNewState(CSoundSource::ePlayState s){
	if(mListenerActor){
		//gcon.printf("sendNewState(%p, %d)\n", mListenerActor->self, s);
        // FIXME : events are now synced
		PyObject *o = PyEval_CallMethod(mListenerActor->self, "handleEvent", "ii", 12, (S32)s);
		Py_XDECREF(o);
	}
	//wake();
}

void CSoundSource::save(CSerializer *ser) const {
	CSoundObject::save(ser);
}

void CSoundSource::load(CSerializer *ser){
	CSoundObject::load(ser);
}

