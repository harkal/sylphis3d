/***************************************************************************
                        soundogg.h  -  description
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

#ifndef _SOUNDOGG_H_
#define _SOUNDOGG_H_

#include <al/al.h>
#include <al/alut.h>

#include <vorbis/vorbisfile.h>

#include <string>
#include <sound/soundbuffer.h>
#include <vfs.h>
class CSoundEnv;

SmartPointer(CSoundOgg);
class CSoundOgg : public CSoundBuffer {
public:
	CSoundOgg(bool streaming = true);
	virtual ~CSoundOgg();

	virtual void load();

	virtual ALuint getBuffer();
	virtual bool fillBuffer(ALuint buffer);

	S32 mFilePos;
	char *mFileData;
	CFile mFile;

	static bool isOggFile(const std::string &name);
protected:
	OggVorbis_File mOggFile;
	S32 mSection;
	char *mPCMBuffer;
	S32 mPCMBufferSize;
};


#endif
