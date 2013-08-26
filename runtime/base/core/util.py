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

import sylphis
import copy
from string import split
from weakref import ref

def manage_del(self):
    #print "Dereferencing", self, self.getReferences(), "Total", sylphis.CRefObject.getTotalRefObjects()
    self.decReferences()

def manage(m, *args):
    class cl(m):
        pass
    cl.__del__ = manage_del
    n = cl(*args)
    del cl
    #print "Referencing", n, n.getReferences()
    n.incReferences()
    return n

def updateKeepType(object, n):
    for i in n.keys():
        try:
            o = getattr(object, i)
            if o is None:
                object.__dict__[i] = n[i]
                continue
            class_type = o.__class__
            if class_type is sylphis.CVector3:
                o = split(n[i])
                try:
                    setattr(object, i, sylphis.CVector3(float(o[0]), float(o[1]), float(o[2])))
                except IndexError:
                    raise Exception, 'Expected 3 valued vector for specific "' + i + '" got : "' + n[i] + '"' 
            else:
                setattr(object, i, class_type(n[i]))
        except AttributeError:
            object.__dict__[i] = n[i]


    
    
