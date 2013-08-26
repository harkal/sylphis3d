/***************************************************************************
                        soundogg.cpp  -  description
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

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <exception.h>
#include <configfile.h>

#include <vfs.h>
#include <sound/soundogg.h>

static size_t read_func(void *ptr, size_t size, size_t nmemb, void *datasource){
	CSoundOgg *self = static_cast<CSoundOgg *>(datasource);
	if(self->mFilePos >= self->mFile.getSize())
		return 0;
	S32 readsize = size * nmemb < self->mFile.getSize() - self->mFilePos ? size * nmemb : self->mFile.getSize() - self->mFilePos;
	memcpy(ptr, self->mFileData + self->mFilePos, readsize);
	self->mFilePos += readsize;
	return readsize;
}

#include <stdio.h>

static S32 seek_func(void *datasource, ogg_int64_t offset, S32 whence){
	CSoundOgg *self = static_cast<CSoundOgg *>(datasource);
	if(whence == SEEK_SET){
		self->mFilePos = offset;
	}
	else if(whence == SEEK_CUR){
		self->mFilePos += offset;
	}
	else if(whence == SEEK_END){
		self->mFilePos = self->mFile.getSize() - offset;
	}
	return self->mFilePos;
}

static S32 close_func(void *datasource){
	return 0;
}

static long tell_func(void *datasource){
	CSoundOgg *self = static_cast<CSoundOgg *>(datasource);
	return self->mFilePos;
}



CSoundOgg::CSoundOgg(bool streaming ) : CSoundBuffer(streaming) {
	newBuffer();
	newBuffer(); // Double buffering
	S32 size = gCfgFile.getAsInt("s_streambuffersize");
	if(size < 32768)size = 32768;
	if(size > 65536 * 4)size = 65536 * 4;
	mPCMBufferSize = size;
	mPCMBuffer = new char[mPCMBufferSize];
}

CSoundOgg::~CSoundOgg(){
	delete [] mPCMBuffer;
}

void CSoundOgg::load(){
	if(isLoaded())
		return;

	CSoundBuffer::load();
	mFile = CFile(getName());
	mFile.loadData();

	mFileData = (char *)mFile.getData();
	mFilePos = 0;

	ov_callbacks cb;
	cb.read_func = read_func;
	cb.seek_func = seek_func;
	cb.close_func = close_func;
	cb.tell_func = tell_func;

	S32 res = ov_open_callbacks(this, &mOggFile, 0, 0, cb);
}

ALuint CSoundOgg::getBuffer(){
	++mNextBuffer;
	mNextBuffer &= 1;
	ALuint buffer = CSoundBuffer::getBuffer();
	return buffer;
}

bool CSoundOgg::fillBuffer(ALuint buffer){
	S32 read = 0;
	bool ret = true;
	while(read < mPCMBufferSize){
		long ret = ov_read(&mOggFile, mPCMBuffer + read, mPCMBufferSize - read, 0, 2, 1, &mSection);
		if(ret == 0){
			ov_time_seek(&mOggFile, 0.0);
			ret = false;
		} 
		else if(ret < 0){
		} else {
			read += ret;
		}
	}

	vorbis_info *vi=ov_info(&mOggFile,-1);
	
	alBufferData(buffer, vi->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, mPCMBuffer, read, vi->rate);
	return ret;
}

bool CSoundOgg::isOggFile(const std::string &name){
	CSoundOgg ogg;

	ogg.mFile = CFile(name);

	try {
		ogg.mFile.loadData();
	} catch (CException e) {
		return false;
	}

	ogg.mFileData = (char *)ogg.mFile.getData();
	ogg.mFilePos = 0;

	ov_callbacks cb;
	cb.read_func = read_func;
	cb.seek_func = seek_func;
	cb.close_func = close_func;
	cb.tell_func = tell_func;

	S32 res = ov_test_callbacks(&ogg, &ogg.mOggFile, 0, 0, cb);

	return res == 0 ? true : false;
}