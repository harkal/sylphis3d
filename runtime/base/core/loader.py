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
import string, marshal, sylphis

def getCode(filename):
    f = sylphis.CFileText(filename)
    try:
        f.loadData()
        source = string.replace(f.read(), "\r\n", "\n")
        return compile(source, filename, "exec")
    except IOError, e:
        pass

    f = sylphis.CFile(filename + 'm')
    f.loadData()
    return marshal.loads(f.read())
    
def execute(filename, g, l):
    exec getCode(filename) in g, l

    
    
