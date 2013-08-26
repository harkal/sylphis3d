/***************************************************************************
                          inputdevice.h  -  description
                             -------------------
    begin                : Fri Sep 6 2002
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

#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include <vector>
#include <pointer.h>

class CInput;

SmartPointer(CInputDevice);
class CInputDevice : public CRefObject {
public:
	CInputDevice();
	virtual ~CInputDevice();

	void setInputHandler(CInput *i);
	CInput *getInputHandler();

	virtual S32 getPointerX(){return 0;};
	virtual S32 getPointerY(){return 0;};
	virtual void getPointerXY(S32 &x, S32 &y);
	virtual void setPointer(S32 x, S32 y){};
	virtual void getPointerCenter(S32 &x, S32 &y){x = 0; y = 0;};
	virtual void centerPointer();
private:
	CInput *mHandler;
};

class CInputListener {
public:
	virtual void onKeyPressed(S32 i) = 0;
	virtual void onKeyReleased(S32 i) = 0;
};

#include <Python.h>

class CPyInputListener : public CInputListener {
public:
	S32 key;
	virtual void onKeyPressed(S32 i);
	virtual void onKeyReleased(S32 i);
	PyObject *self;
};

class CInput {
public:
	CInput();
	virtual ~CInput();

	void addKeyDevice(CInputDevice *d);
	void addPointingDevice(CInputDevice *d);
	void clearDevices();

	void addListener(CInputListener *l);

	inline bool isPressed(S32 i){
		return mKeys[i];
	}

	inline void keyPressed(S32 i){
		if(i>0)
			mKeys[i] = true;
		std::vector<CInputListener*>::iterator j;
		for(j = mListeners.begin();j!=mListeners.end();j++){
			(*j)->onKeyPressed(i);
		}
	}

	inline void keyReleased(S32 i){
		mKeys[i] = false;
		std::vector<CInputListener*>::iterator j;
		for(j = mListeners.begin();j!=mListeners.end();j++){
			(*j)->onKeyReleased(i);
		}
	}

	bool getGrabPointer();
	void setGrabPointer(bool g);

	S32 getPointerX();
	S32 getPointerY();
	void getPointerXY(S32 &x, S32 &y);
	void setPointer(S32 x, S32 y);

	void getPointerDelta(S32 *x, S32 *y);
	S32 getPointerDeltaX(); // Call this first
	S32 getPointerDeltaY();
protected:
	std::vector<CInputDevicePtr> mDevices;
	std::vector<CInputListener*> mListeners;
	CInputDevicePtr mPointingDevice;
	bool mKeys[256];
	S32 lastX;
	S32 lastY;
	S32 deltaX;
	S32 deltaY;
	bool mGrabPointer;
};

#ifdef _LINUX
#define VK_CAPITAL	1
#define VK_NUMLOCK	2
#define VK_SCROLL	3
#define VK_CONTROL	4
#define VK_SHIFT	5
#define VK_ESCAPE	6
#define VK_BACK		7
#define VK_RETURN	8
#define VK_SPACE	32
#define VK_TAB		10
#define VK_F1		11
#define VK_F2		12
#define VK_F3		13
#define VK_F4		14
#define VK_F5		15
#define VK_F6		16
#define VK_F7		17
#define VK_F8		18
#define VK_F9		19
#define VK_F10		20
#define VK_F11		21
#define VK_F12		22
#define VK_INSERT	23
#define VK_DELETE	24
#define VK_HOME		25
#define VK_END		26
#define VK_LEFT		27
#define VK_RIGHT	28
#define VK_UP		29
#define VK_DOWN		30
#define VK_NUMPAD0	31
#define VK_NUMPAD1	9 
#define VK_NUMPAD2	100
#define VK_NUMPAD3	101
#define VK_NUMPAD4	102
#define VK_NUMPAD5	103
#define VK_NUMPAD6	104
#define VK_NUMPAD7	105
#define VK_NUMPAD8	106
#define VK_NUMPAD9	107
#define VK_MULTIPLY	108
#define VK_SUBTRACT	109
#define VK_ADD		110
#define VK_ENTER	111
#define VK_DIVIDE	112
#endif

#ifdef _WIN32
#include <windows.h>
#endif
 
const S32 kSYSREQ	=	44;
const S32 kCAPSLOCK	=VK_CAPITAL;
const S32 kNUMLOCK	=VK_NUMLOCK; 
const S32 kSCROLLLOCK	=VK_SCROLL;
const S32 kLEFTCTRL	=VK_CONTROL; 
//const S32 kLEFTALT	=VK_LALT 
const S32 kLEFTSHIFT	=VK_SHIFT; 
const S32 kRIGHTCTRL	=VK_CONTROL; 
//const S32 kRIGHTALT	=0xB8 
const S32 kRIGHTSHIFT	=VK_SHIFT ;
const S32 kESC		=VK_ESCAPE ;
const S32 kBACKSPACE	=VK_BACK; 
const S32 kENTER		=VK_RETURN ;
const S32 kSPACE		=VK_SPACE ;
const S32 kTAB		=VK_TAB ;
const S32 kF1			=VK_F1 ;
const S32 kF2			=VK_F2;
const S32 kF3			=VK_F3;
const S32 kF4			=VK_F4;
const S32 kF5			=VK_F5;
const S32 kF6			=VK_F6 ;
const S32 kF7			=VK_F7 ;
const S32 kF8			=VK_F8;
const S32 kF9			=VK_F9 ;
const S32 kF10		=VK_F10 ;
const S32 kF11		=VK_F11 ;
const S32 kF12		=VK_F12 ;
const S32 kA		=65;
const S32 kB		=66 ;
const S32 kC		=67 ;
const S32 kD		=68 ;
const S32 kE		=69 ;
const S32 kF		=70 ;
const S32 kG		=71 ;
const S32 kH		=72 ;
const S32 kI		=73 ;
const S32 kJ		=74 ;
const S32 kK		=75 ;
const S32 kL		=76 ;
const S32 kM		=77 ;
const S32 kN		=78 ;
const S32 kO		=79;
const S32 kP		=80 ;
const S32 kQ		=81 ;
const S32 kR		=82 ;
const S32 kS		=83 ;
const S32 kT		=84 ;
const S32 kU		=85 ;
const S32 kV		=86 ;
const S32 kW		=87 ;
const S32 kX		=88 ;
const S32 kY		=89 ;
const S32 kZ		=90 ;
const S32 k1		=49 ;
const S32 k2		=50 ;
const S32 k3		=51 ;
const S32 k4		=52;
const S32 k5		=53 ;
const S32 k6		=54 ;
const S32 k7		=55 ;
const S32 k8		=56 ;
const S32 k9		=57 ;
const S32 k0		=48 ;
const S32 kMINUS		=189 ;
const S32 kEQUAL		=187 ;
const S32 kLBRACKET	=219 ;
const S32 kRBRACKET	=221 ;
const S32 kSEMICOLON	=186 ;
const S32 kTICK		=222 ;
const S32 kAPOSTROPHE	=192 ;
const S32 kBACKSLASH	=220 ;
const S32 kCOMMA		=188 ;
const S32 kPERIOD		=190 ;
const S32 kSLASH		=191 ;
const S32 kINS		=VK_INSERT; 
const S32 kDEL		=VK_DELETE ;
const S32 kHOME		=VK_HOME ;
const S32 kEND		=VK_END ;
const S32 kPGUP		=33 ;
const S32 kPGDN		=34 ;
const S32 kLARROW		=VK_LEFT ;
const S32 kRARROW		=VK_RIGHT;
const S32 kUARROW		=VK_UP;
const S32 kDARROW		=VK_DOWN;
const S32 kKEYPAD0	=VK_NUMPAD0 ;
const S32 kKEYPAD1	=VK_NUMPAD1 ;
const S32 kKEYPAD2	=VK_NUMPAD2;
const S32 kKEYPAD3	=VK_NUMPAD3;
const S32 kKEYPAD4	=VK_NUMPAD4;
const S32 kKEYPAD5	=VK_NUMPAD5;
const S32 kKEYPAD6	=VK_NUMPAD6;
const S32 kKEYPAD7	=VK_NUMPAD7;
const S32 kKEYPAD8	=VK_NUMPAD8;
const S32 kKEYPAD9	=VK_NUMPAD9;
const S32 kKEYPADDEL	=110;
const S32 kKEYPADSTAR		=VK_MULTIPLY;
const S32 kKEYPADMINUS	=VK_SUBTRACT;
const S32 kKEYPADPLUS		=VK_ADD;
const S32 kKEYPADENTER	=VK_RETURN ;
const S32 kKEYPADSLASH	=VK_DIVIDE ;

const S32 kMOUSERIGHT	=	254;
const S32 kMOUSELEFT	=	255;


#endif
