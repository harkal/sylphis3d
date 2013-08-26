/***************************************************************************
    copyright            : (C) 2002 by Vassili Tenente
                           (C) 2002-2005 Harry Kalogirou (harkal@gmail.com)
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
#ifndef _GLOBALDEFS_H_
#define _GLOBALDEFS_H_

#if 0
#ifdef _WIN32
#define strcasecmp stricmp
#define strncasecmp strnicmp
#define usleep Sleep
#endif

#ifdef _WIN32

#pragma warning(disable : 4018) // signed/unsigned mismatch
#pragma warning(disable : 4032)
#pragma warning(disable : 4051)
#pragma warning(disable : 4057) // slightly different base types
#pragma warning(disable : 4100) // unreferenced formal parameter
#pragma warning(disable : 4115)
#pragma warning(disable : 4125) // decimal digit terminates octal escape sequence
#pragma warning(disable : 4127) // conditional expression is constant
#pragma warning(disable : 4136)
#pragma warning(disable : 4201)
#pragma warning(disable : 4214)
#pragma warning(disable : 4244)
#pragma warning(disable : 4305) // truncation from const double to float
#pragma warning(disable : 4310) // cast truncates constant value
#pragma warning(disable : 4514)
#pragma warning(disable : 4711) // selected for automatic inline expansion
#pragma warning(disable : 4220) // varargs matches remaining parameters
#endif

#define PI				3.141592f
#endif

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;


#ifdef _NOINLINE
#define INLINE
#else
#define INLINE inline
#endif

#endif


