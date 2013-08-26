
#ifndef _GLOBALDEFS_H_
#define _GLOBALDEFS_H_

#include <platform/types.h>

#ifdef _WIN32
#define strcasecmp stricmp
#define strncasecmp strnicmp
#define usleep Sleep
#endif

#ifdef _WIN32

#pragma warning(disable : 4018)     // signed/unsigned mismatch
#pragma warning(disable : 4032)
#pragma warning(disable : 4051)
#pragma warning(disable : 4057)		// slightly different base types
#pragma warning(disable : 4100)		// unreferenced formal parameter
#pragma warning(disable : 4115)
#pragma warning(disable : 4125)		// decimal digit terminates octal escape sequence
#pragma warning(disable : 4127)		// conditional expression is constant
#pragma warning(disable : 4136)
#pragma warning(disable : 4201)
#pragma warning(disable : 4214)
#pragma warning(disable : 4220)		// varargs matches remaining parameters
#pragma warning(disable : 4244)
#pragma warning(disable : 4251)
#pragma warning(disable : 4275)
#pragma warning(disable : 4305)		// truncation from const F64 to F32
#pragma warning(disable : 4310)		// cast truncates constant value
#pragma warning(disable : 4514)
#pragma warning(disable : 4711)		// selected for automatic inline expansion
#pragma warning(disable : 4786)

#endif

#define PI              3.141592f

typedef unsigned char   byte; 
typedef U16  word; 
typedef unsigned long   dword; 


#endif

