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
import time, mthread

class CCamera(CSimpleActor):
    __classname__ = 'camera'
    __desc__ = 'A general use camera'
    __actortype__ = 'e'
    __specs__ = [
        {'name':'angles', 'doc':'The orientation of the camera'},
        {'name':'fov', 'doc':'The field of view'},
        {'name':'target', 'doc':'The target entity to auto track'}
    ]
    fov = 90.0
    angles = ''
    target = ''

    def join(self, ent):
        CSimpleActor.join(self, ent)

        sm = self.mScene
        sl = sylphis.CCamera(sm)
        sl.setPosition(self.origin)
        sl.setFov(self.fov)

        try:
            o = split(ent['angles'])
            pi = 3.14159
            self.angles = sylphis.CVector3(-pi * float(o[0]) / 180, pi * (float(o[1]) - 90) / 180, pi * float(o[2]) / 180)

            sl.yaw(self.angles.y)
            sl.pitch(self.angles.x)
            sl.roll(self.angles.z)
 
        except KeyError, x:
            pass

        self.node = sl

    def start(self):
        CSimpleActor.start(self)
        actor = self.mCont.actors.getActorByName(self.target)
        if actor is None:
            self.targetNode = None
        else:
            self.targetNode = actor.node
            self.targetBody = actor.body

    def Action_makeActive(self, other):
        """
        Action for making the camera active (In/Out)
        """
        CEngine.renderer.useCamera(self.node)

    def handleEvent(self, *args):
        if args[0] is self.EVENT_TRIGGER:
            self.Action_makeActive()

    def update(self, timeDelta):
        if self.targetNode is None:
            return

        self.node.lookAt(self.targetNode.getPosition())
        
        
