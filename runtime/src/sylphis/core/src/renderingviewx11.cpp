/***************************************************************************
                          renderingviewx11.cpp  -  description
                             -------------------
    begin                : Sun Mar 10 2002
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
#ifndef _WIN32

#include <X11/keysym.h>

#include "engine.h"
#include "renderingviewx11.h"

#define XE_Mask (PointerMotionMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask | ExposureMask)

CRenderingViewX11::CRenderingViewX11() : dpy(0) {
}
CRenderingViewX11::~CRenderingViewX11(){
}

void CRenderingViewX11::startup(){
	S32 attrib[] = { GLX_RGBA,
                    GLX_RED_SIZE, 1,
                    GLX_GREEN_SIZE, 1,
                    GLX_BLUE_SIZE, 1,
                    GLX_ALPHA_SIZE, 1,
                    GLX_DOUBLEBUFFER,
                    GLX_DEPTH_SIZE, 2,
                    GLX_STENCIL_SIZE, 1,
                    None };
	S32 scrnum;
	XSetWindowAttributes attr;
	unsigned long mask;
	Window root;
	GLXContext ctx;
	XVisualInfo *visinfo;

	// Call father
	CRenderingView::startup();
		
	gcon.printf("-- CRenderingViewX11::startup\n");

	S32 width = getDimentions().first;
	S32 height = getDimentions().second;
	
	dpy = XOpenDisplay(0);
		
	scrnum = DefaultScreen( dpy );
	root = RootWindow( dpy, scrnum );

	visinfo = glXChooseVisual( dpy, scrnum, attrib );
	if (!visinfo) {
		gcon.printf("...Error: couldn't get an RGBA, Double-buffered visual\n");
		exit(1);
	}

	attr.background_pixel = 0;
	attr.border_pixel = 0;
	attr.colormap = XCreateColormap( dpy, root, visinfo->visual, AllocNone);
	attr.event_mask = XE_Mask;
	mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

	win = XCreateWindow( dpy, root, 0, 0, width, height,
                       0, visinfo->depth, InputOutput,
                        visinfo->visual, mask, &attr );

	ctx = glXCreateContext( dpy, visinfo, NULL, True );

	glXMakeCurrent( dpy, win, ctx );

	XMapWindow( dpy, win);
    S32 x,y;
    getPointerCenter(x, y);
    setPointer(x, y);
}

void CRenderingViewX11::shutdown(){
	
}

void CRenderingViewX11::swapBuffers(){
	glXSwapBuffers( dpy, win);
}

void CRenderingViewX11::handleEvents(){
	XEvent event;
	S32 keycode;

	while (XPending(dpy)){
    		XNextEvent( dpy, &event );
    		switch( event.type ){
    		case Expose:
        		glXSwapBuffers(dpy,event.xany.window);
			break;
    		case ConfigureNotify:
        		glViewport( 0, 0, event.xconfigure.width, event.xconfigure.height );
        		break;
		case KeyPress:
            keycode = XKeyCode(&event.xkey);
            getInputHandler()->keyPressed(keycode);
            break;
		case KeyRelease:
            keycode = XKeyCode(&event.xkey);
            getInputHandler()->keyReleased(keycode);
            break;
		case MotionNotify:
			mPointerX = (S32)event.xmotion.x;
			mPointerY = (S32)event.xmotion.y;
			break;
		case ButtonPress:
				if(event.xbutton.button == 1)
					getInputHandler()->keyPressed(kMOUSELEFT);
				//else if(event.xbutton.button == 2)
				//	MouPress |= 3;
				else if(event.xbutton.button == 3)
                    getInputHandler()->keyPressed(kMOUSERIGHT);
			break;
		case ButtonRelease:
				if(event.xbutton.button == 1)
                    getInputHandler()->keyReleased(kMOUSELEFT);
                //else if(event.xbutton.button == 2)
                //MouPress &= ~3;
                else if(event.xbutton.button == 3)
                    getInputHandler()->keyReleased(kMOUSERIGHT);
		break;
    	}
	}

}

S32 CRenderingViewX11::getPointerX(){
    return mPointerX;
}

S32 CRenderingViewX11::getPointerY(){
    return mPointerY;
}

void CRenderingViewX11::getPointerXY(S32 &x, S32 &y){
    x = mPointerX;
    y = mPointerY;
}

void CRenderingViewX11::setPointer(S32 x, S32 y){
    if(!dpy)
        return;
    XSelectInput(dpy, win, XE_Mask & ~PointerMotionMask);
    XWarpPointer(dpy,None,win,0,0,0,0, x , y );
    XSelectInput(dpy, win, XE_Mask);
    mPointerX = x;
    mPointerY = y;
}

void CRenderingViewX11::getPointerCenter(S32 &x, S32 &y){
    x = getDimentions().first >> 1;
    y = getDimentions().second >> 1;
}

void CRenderingViewX11::centerPointer(){
    setPointer(getDimentions().first>>1, getDimentions().second>>1);
}

S32 CRenderingViewX11::XKeyCode(XKeyEvent *ev){
	S32 key=0;
	char buff[64];
	KeySym keysym;

	XLookupString(ev, buff, sizeof(buff), &keysym, 0);

	switch(keysym){
	case 96:key = kAPOSTROPHE;break;
	case XK_KP_Page_Up:
        case XK_Page_Up: key = kPGUP; break;

        case XK_KP_Page_Down:
        case XK_Page_Down: key = kPGDN; break;

        case XK_KP_Home:
        case XK_Home: key = kHOME; break;

        case XK_KP_End:
        case XK_End: key = kEND; break;

        case XK_KP_Left:
        case XK_Left: key = kLARROW; break;

        case XK_KP_Right:
        case XK_Right: key = kRARROW; break;

        case XK_KP_Down:
        case XK_Down: key = kDARROW; break;

        case XK_KP_Up:
        case XK_Up: key = kUARROW; break;

        case XK_Escape: key = kESC; break;

	case XK_KP_Enter:
	case XK_Return: key = kENTER; break;

	case XK_BackSpace: key = kBACKSPACE;break;
	case XK_Tab: key = kTAB;break;
	case XK_F1: key = kF1;break;
	case XK_F2: key = kF2;break;
	case XK_F3: key = kF3;break;
	case XK_F4: key = kF4;break;
	case XK_F5: key = kF5;break;
        case XK_F6: key = kF6;break;
        case XK_F7: key = kF7;break;
        case XK_F8: key = kF8;break;
	case XK_F9: key = kF9;break;
        case XK_F10: key = kF10;break;
        case XK_F11: key = kF11;break;
        case XK_F12: key = kF12;break;
	case XK_KP_Delete:
        case XK_Delete: key = kDEL;break;
        case XK_Shift_L:
        case XK_Shift_R: key = kLEFTSHIFT;break;
        case XK_Execute:
        case XK_Control_L:
        case XK_Control_R: key = kLEFTCTRL;break;
        case XK_KP_Insert:
        case XK_Insert:key = kINS;break;
	case XK_period:key = kPERIOD;break;
	case 59: key = kSEMICOLON;break;
	case 34:
	case 39: key = kTICK;break;

	default:
		key = *(unsigned char*)buff;
		if(key >= 'a' && key <= 'z')key = key - 'a' + 'A';
		break;
	}

	return key;
}


#endif

