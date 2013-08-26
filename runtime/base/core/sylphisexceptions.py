"""
This file is part of the "Sylphis" 3D engine
Copyright (c) 2002 - 2007 by Harry Kalogirou

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
"""
import types
import exceptions

class CFatalError(Exception):
    """ A fatal engine error. Should not happen """

class CUserError(Exception):
    """ A recoverable user error """

#Add the custom Sylphis exceptions to the __buildin__
import __builtin__
__builtin__.CFatalError = CFatalError
__builtin__.CUserError = CUserError


def excepthook(type, value, traceback):
    print "My exception handler"
    traceback.print_exc()

import sys
#sys.excepthook = excepthook


