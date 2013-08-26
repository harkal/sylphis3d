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

import sys, sylphis, math
from string import *
from CSimpleActor import *
from util import manage

class CJoint(CSimpleActor):
    __actortype__ = 'e'
    __specs__ = [
        {'name':'body1', 'doc':'The one of the bodies to connect'},
        {'name':'body2', 'doc':'The other of the bodies to connect'}
    ]
    body1 = ''
    body2 = ''
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)
      
    def join(self, ent):
        CSimpleActor.join(self, ent)

    def start(self):
        CSimpleActor.start(self)

        body1 = self.mCont.actors.getActorByName(self.body1)
        body2 = self.mCont.actors.getActorByName(self.body2)

        try:
            r1 = body1.body
        except AttributeError:
            r1 = None

        try:
            r2 = body2.body
        except AttributeError:
            r2 = None

        if r1 is None and r2 is None:
            raise sylphis.CException("No bodies to attach to")

        self.joint.attach(r1, r2)
        self.joint.setAnchor(self.origin)

