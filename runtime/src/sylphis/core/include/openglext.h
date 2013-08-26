
#ifndef _OPENGLEXT_H_
#define _OPENGLEXT_H_

#include <platform/types.h>

#ifdef _WIN32
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stddef.h>
#endif

#include <glew.h>

#ifdef _WIN32
#include <wglew.h>
#endif

extern GLenum glIncrStencil;
extern GLenum glDecrStencil;

extern S32 maxMultiTextureUnits;
extern S32 maxGeneralCombiners;

extern bool isGLSLSupported;
extern bool isFloatTexturesSupported;

void initOpenGlExtensions();

#endif

