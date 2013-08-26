/***************************************************************************
                         timer.cpp  -  description
                            -------------------
                begin                : Aug 23 2002
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
#include <cassert>
#include <limits.h>

#include "globaldefs.h"
#include "timer.h"

#ifdef _WIN32
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sys/timeb.h>
#include <mmsystem.h>
#else
#include <sys/time.h>
#include <unistd.h>
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
F64 CTimer::getTime() {
	dword now;

	now = timeGetTime();
	
	if (now < starttime)
		return (now / 1000.0) + (LONG_MAX - starttime / 1000.0);

	if (now - starttime == 0)
		return 0.0;

	return (now - starttime) / 1000.0;
}
#else
F64 CTimer::getTime() {
    struct timeval tp;
    struct timezone tzp;

    gettimeofday(&tp, &tzp);

    return (tp.tv_sec - secbase) + tp.tv_usec/1000000.0;
}
#endif

CTimerAdvanced::CTimerAdvanced(S32 historySize){
    assert(historySize != 0);
    mHistory = TimeVector(historySize, 0.0);
    mHistorySize = historySize;
	mUseCount = 0;
	mAccTime = 0.0;
}

void CTimerAdvanced::start(){
    mStartTime = getTime();
}


F64 CTimerAdvanced::stop(){
	F64 time = getTime() - mStartTime;
    mAccTime += time;
	return time;
}

void CTimerAdvanced::reset(){
	mHistory[mUseCount++ % mHistorySize] = mAccTime;
	mAccTime = 0.0;
}

F64 CTimerAdvanced::getAvrTime(){
    TimeVector::const_iterator i;
    F64 sum = 0.0;
    
    for(i = mHistory.begin() ; i != mHistory.end() ; ++i){
        sum += *i;
    }
    
    return sum / mHistorySize;
}

