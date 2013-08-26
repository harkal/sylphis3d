/***************************************************************************
    copyright            : (C) 2002-2005 Harry Kalogirou (harkal@gmail.com)
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

class CTimer {
    dword starttime;
    int	secbase;
public:
    CTimer();
    double getTime();
};

class CTimerStoppable : public CTimer {
public:

};

class CTimerAdvanced : public CTimer {
public:
    CTimerAdvanced(int historySize = 1);
    void start();
    double stop();
    void reset();
    double getAvrTime();
protected:
    typedef std::vector<double> TimeVector;
    TimeVector mHistory;
    unsigned int mHistorySize;
    unsigned int mUseCount;
    double mStartTime;
    double mAccTime;
};

#endif
