/***************************************************************************
                        soundwave.cpp  -  description
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

#include <vfs.h>
#include <sound/soundwave.h>
#include <exception.h>

CSoundWave::CSoundWave(bool streaming ) : CSoundBuffer(streaming) {

}

CSoundWave::~CSoundWave(){

}

void CSoundWave::load(){
	ALuint buffer = newBuffer();
	CFile f(getName());
	f.loadData();

	ALsizei size,freq;
	ALenum	format;
	ALvoid	*data;
	ALboolean loop;

	alutLoadWAVMemory((ALbyte *)f.getData(),&format,&data,&size,&freq,&loop);
	//alutLoadWAVFile((ALbyte *)name.c_str(),&format,&data,&size,&freq,&loop);
	alBufferData(buffer,format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);
}

bool CSoundWave::isWaveFile(const std::string &name){
	CFile f(name);

	try { 
		f.loadData();
	} catch (CException e) {
		return false;
	}

	if(*(S32*)f.getData() == *(S32*)"RIFF")
		return true;
	else
		return false;
}
