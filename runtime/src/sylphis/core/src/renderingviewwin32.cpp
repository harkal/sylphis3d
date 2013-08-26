/***************************************************************************
                          renderingviewwin32.cpp  -  description
                             -------------------
    begin                : Thu Mar 21 2002
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

#include <build.h>
#include <engine.h>
#include <renderingviewwin32.h>
#include <openglext.h>
#include <configfile.h>
#include <iostream>

const char szAppName[] = "Sylphis";
HINSTANCE gInstance;

CRenderingViewWin32::CRenderingViewWin32(){
	hrc = 0;
}
CRenderingViewWin32::~CRenderingViewWin32(){
}

BOOL CRenderingViewWin32::SetupPixelFormat(HDC hdc){ 
	PIXELFORMATDESCRIPTOR *ppfd, pfd = { 
    sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
    1,                     // version number 
    PFD_DRAW_TO_WINDOW |   // support window 
    PFD_SUPPORT_OPENGL |   // support OpenGL 
    PFD_DOUBLEBUFFER,      // F64 buffered 
    PFD_TYPE_RGBA,         // RGBA type 
    32,                    // 32-bit color depth 
    0, 0, 0, 0, 0, 0,      // color bits ignored 
    8,                     // 8-bit alpha buffer 
    0,                     // shift bit ignored 
    0,                     // no accumulation buffer 
    0, 0, 0, 0,            // accum bits ignored 
    24,                    // 32-bit z-buffer     
    8,                     // 8 bits stencil buffer 
    0,                     // no auxiliary buffer 
    PFD_MAIN_PLANE,        // main layer 
    0,                     // reserved 
    0, 0, 0                // layer masks ignored 
    }; 
	S32 pixelformat; 
 
	ppfd = &pfd; 
 
	if ((pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0) { 
		gcon.printf("FATAL ERROR : ChoosePixelFormat failed\n"); 
		return FALSE; 
	} 
  
	if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE) { 
		gcon.printf("FATAL ERROR : SetPixelFormat failed\n"); 
		return FALSE; 
	} 
 
	return TRUE; 
}

static CRenderingViewWin32 *self = 0;

LRESULT CALLBACK GLWndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam) {
	switch(message){
	case WM_ACTIVATE:
		if(self){
			if(wParam == WA_INACTIVE){
				ShowCursor(1);
				self->getInputHandler()->setGrabPointer(false);
			} else {
				ShowCursor(0);
				self->getInputHandler()->setGrabPointer(true);
			}
		}
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
} 

void CRenderingViewWin32::changeResolution(S32 width, S32 height, S32 bpp){
		DEVMODE dmScreenSettings;
		
		memset(&dmScreenSettings, 0, sizeof(DEVMODE));						// Clear Room To Store Settings 
		dmScreenSettings.dmSize			= sizeof(DEVMODE);					// Size Of The Devmode Structure 
		dmScreenSettings.dmBitsPerPel = bpp;
		dmScreenSettings.dmPelsWidth	= width;								// Screen Width 
		dmScreenSettings.dmPelsHeight	= height;								// Screen Height 
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;		// Pixel Mode 
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);			// Switch To Full Screen  
}

void CRenderingViewWin32::createWindow(){

}

void CRenderingViewWin32::destroyWindow(){
	DestroyWindow(hwnd);

	wglMakeCurrent(0, 0);
	//wglDeleteContext(hrc);
	ReleaseDC( hwnd, hdc );

	DestroyWindow(hwnd);
	UnregisterClass(szAppName, GetModuleHandle(0));
}

void CRenderingViewWin32::startup(){
	bool fullscreen = gCfgFile.getAsInt("r_fullscreen") == 1;
	// Call father
	self = this;
	CRenderingView::startup();
		
	gcon.printf("-- CRenderingViewWin32::startup\n");

	S32 width = getDimentions().first;
	S32 height = getDimentions().second;

	if (fullscreen) { 
		changeResolution(width, height, 32);
	} else {
		ChangeDisplaySettings(0, CDS_FULLSCREEN);
	}

	WNDCLASS wc; 	
	wc.style         = CS_HREDRAW | CS_VREDRAW; 
	wc.lpfnWndProc   = (WNDPROC)GLWndProc; 
	wc.cbClsExtra    = 0; 
	wc.cbWndExtra    = 0; 
	wc.hInstance     = gInstance;
	wc.hIcon         = LoadIcon (gInstance, szAppName); 
	wc.hCursor       = LoadCursor (NULL,IDC_ARROW); 
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); 
	wc.lpszMenuName  = szAppName; 
	wc.lpszClassName = szAppName; 
 
	if (!RegisterClass(&wc)) goto error; 
 
	if (!fullscreen) { 
	// Normal window 
        hwnd = CreateWindow(szAppName,CEngine::getSingleton()->getVersion().c_str(),  
			WS_CAPTION|WS_CLIPSIBLINGS, 
			CW_USEDEFAULT,CW_USEDEFAULT, 
			width, height, 
			NULL, NULL, gInstance, NULL); 
	} else { 
	// fullscreen window 
		hwnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_TOPMOST ,
			szAppName, CEngine::getSingleton()->getVersion().c_str(),  
			WS_POPUP, 
			0, 0, 
			width, height, 
			NULL, NULL, gInstance, NULL); 
	} 

	if (!hwnd) goto error; 
 
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	SetFocus(hwnd);

	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);
  
// Setup GL context 
	hdc = GetDC(hwnd); 
	
	if (!SetupPixelFormat(hdc)) goto error; 
 
	if(!hrc)
		hrc = wglCreateContext(hdc); 
	wglMakeCurrent(hdc,hrc); 

	RECT rect;
	GetClientRect(hwnd, &rect);
//	GLWinResize(rect.right-rect.left,rect.bottom-rect.top); 

	S32 alphaBits;
	glGetIntegerv(GL_ALPHA_BITS, &alphaBits);
	if (alphaBits == 0) {
		gcon.printf("No alpha buffer available\n");
	}

	SetFocus(hwnd);
	S32 x,y;
    getPointerCenter(x, y);
    setPointer(x, y);
	return;

error: 
	//printf("Error in GL init %d\n",ddrval); 
	DestroyWindow(hwnd); 
}

void CRenderingViewWin32::shutdown(){
	destroyWindow();
}

void CRenderingViewWin32::setDimentions(const pair<S32,S32> &d){
	CRenderingView::setDimentions(d);
}

void CRenderingViewWin32::handleEvents(){
	MSG msg;
 
	while(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) { 
		if (!(GetMessage(&msg, NULL, 0, 0))) return; 		
		PAINTSTRUCT ps;  
		
		switch (msg.message) { 
		case WM_CREATE: 
			break; 
 
		case WM_PAINT: 
			BeginPaint(msg.hwnd, &ps); 
			EndPaint(msg.hwnd, &ps); 
			break; 
 
		case WM_KEYDOWN: 
			getInputHandler()->keyPressed(msg.wParam);
	
			switch(msg.wParam) { 
				case VK_ESCAPE: 
					//PostMessage(msg.hwnd, WM_CLOSE, 0, 0); 
					break; 
			} 
			break; 
		case WM_KEYUP:
			getInputHandler()->keyReleased(msg.wParam);
			break;
		case WM_CHAR:
			getInputHandler()->keyPressed(-msg.wParam);
			break;
 
		case WM_LBUTTONDOWN:
			getInputHandler()->setGrabPointer(true);
			getInputHandler()->keyPressed(kMOUSELEFT);
			break;
		case WM_RBUTTONDOWN:
			getInputHandler()->setGrabPointer(true);
			getInputHandler()->keyPressed(kMOUSERIGHT);
			break;
		case WM_LBUTTONUP:
			getInputHandler()->keyReleased(kMOUSELEFT);
			break;
		case WM_RBUTTONUP:
			getInputHandler()->keyReleased(kMOUSERIGHT);
			break;

/*
		case WM_CLOSE: 
			Con_Command("quit");
			//Done = 1;
			break; 
 
		case WM_DESTROY: 
			Con_Command("quit");
			//Done = 1;
			PostQuitMessage (0); 
			break; 
*/
		case WM_SYSCOMMAND: 
			if (msg.wParam==SC_KEYMENU) continue; 
        }
  
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	}
}

void CRenderingViewWin32::swapBuffers(){
	SwapBuffers(hdc);
}

S32 CRenderingViewWin32::getPointerX(){
	POINT CurMousePos;
	GetCursorPos(&CurMousePos);
	return CurMousePos.x;
}

S32 CRenderingViewWin32::getPointerY(){
	POINT CurMousePos;
	GetCursorPos(&CurMousePos);
	return CurMousePos.y;
}

void CRenderingViewWin32::getPointerXY(S32 &x, S32 &y){
	POINT CurMousePos;
	GetCursorPos(&CurMousePos);
	x = CurMousePos.x;
	y = CurMousePos.y;
}

void CRenderingViewWin32::setPointer(S32 x, S32 y){
	SetCursorPos(x, y);
}

void CRenderingViewWin32::centerPointer(){
    pair<S32, S32> &dim = getDimentions();
	setPointer(dim.first>>1, dim.second>>1);
}

void CRenderingViewWin32::getPointerCenter(S32 &x, S32 &y){
    pair<S32, S32> &dim = getDimentions();
	x = dim.first >> 1;
	y = dim.second >> 1;
}
