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

import sys, sylphis
from util import manage
from string import *
from CSimpleActor import *

class CExplosion(CSimpleActor):
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)

    def join(self, ent):
        CSimpleActor.join(self, ent)

        sm = self.mCont.getSceneManager()
        sl = manage(sylphis.CLight, sm)
        sl.setPosition(0, 0, 0)
        self.radius = 10.0
        sl.setRadius(self.radius)
        sl.setColor(sylphis.CVector3(1.0, 0.9, 0.6));
        sl.setPosition(self.origin)
        self.node = sl

        self.age = 0.0

    def update(self, timeDelta):
        self.age += timeDelta
        if self.age < 0.15:
            self.radius += 1000.0 * timeDelta
            if self.radius > 200.0:
                self.radius = 200.0
        if self.age > 0.3:
            self.radius -= 400.0 * timeDelta
        if self.radius < 0.0:
            self.remove()
            return

        self.node.setRadius(self.radius)
        
            
        
