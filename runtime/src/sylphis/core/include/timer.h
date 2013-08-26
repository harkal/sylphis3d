/***************************************************************************
                           timer.h  -  description
                             -------------------
    begin                : Aug 10 2002
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
#ifndef _TIMER_H_
#define _TIMER_H_


#include <vector>
#include "globaldefs.h"
#include <platform/types.h>

class CTimer {
    dword starttime;
    S32	secbase;
public:
    CTimer();
    F64 getTime();
};

class CTimerStoppable : public CTimer {
public:

};

class CTimerAdvanced : public CTimer {
public:
    CTimerAdvanced(S32 historySize = 1);
    void start();
    F64 stop();
    void reset();
    F64 getAvrTime();
protected:
    typedef std::vector<F64> TimeVector;
    TimeVector mHistory;
    U32 mHistorySize;
    U32 mUseCount;
    F64 mStartTime;
    F64 mAccTime;
};

#endif
