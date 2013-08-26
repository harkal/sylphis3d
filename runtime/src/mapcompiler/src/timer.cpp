/***************************************************************************
    copyright            : (C) 2002 by Vassili Tenente
                           (C) 2002-2005 Harry Kalogirou (harkal@gmail.com)
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
#include <cassert>
#include <limits.h>

#include "globaldefs.h"
#include "timer.h"

#ifdef _WIN32
#include <windows.h>
#include <sys/timeb.h>
#include <mmsystem.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif 

#ifdef MEMPROF
#include "mmgr.h"
#endif

CTimer::CTimer(){
#ifdef _WIN32
	timeBeginPeriod(1);
	starttime = timeGetTime();
#else
    struct timeval tp;
    struct timezone tzp;

    gettimeofday(&tp, &tzp);
    secbase = tp.tv_sec;
#endif
}

#ifdef _WIN32
double CTimer::getTime() {
	dword now;

	now = timeGetTime();
	
	if (now < starttime)
		return (now / 1000.0) + (LONG_MAX - starttime / 1000.0);

	if (now - starttime == 0)
		return 0.0;

	return (now - starttime) / 1000.0;
}
#else
double CTimer::getTime() {
    struct timeval tp;
    struct timezone tzp;

    gettimeofday(&tp, &tzp);

    return (tp.tv_sec - secbase) + tp.tv_usec/1000000.0;
}
#endif

CTimerAdvanced::CTimerAdvanced(int historySize){
    assert(historySize != 0);
    mHistory = TimeVector(historySize, 0.0);
    mHistorySize = historySize;
	mUseCount = 0;
	mAccTime = 0.0;
}

void CTimerAdvanced::start(){
    mStartTime = getTime();
}


double CTimerAdvanced::stop(){
	double time = getTime() - mStartTime;
    mAccTime += time;
	return time;
}

void CTimerAdvanced::reset(){
	mHistory[mUseCount++ % mHistorySize] = mAccTime;
	mAccTime = 0.0;
}

double CTimerAdvanced::getAvrTime(){
    TimeVector::const_iterator i;
    double sum = 0.0;
    
    for(i = mHistory.begin() ; i != mHistory.end() ; ++i){
        sum += *i;
    }
    
    return sum / mHistorySize;
}

