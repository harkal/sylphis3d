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
import sys, sylphis, mthread
from string import *
from CTriggerModel import *
from CMiscModel import *
import math

class CJumpPad(CTriggerModel):
    __classname__ = 'jumppad'
    __desc__ = 'A jump pad'
    __actortype__ = 'b'
    __specs__ = [
        {'name':'highpoint', 'doc':'The high point the actor should be push to'}
    ]
    highpoint = None

    def join(self, ent):
        CTriggerModel.join(self, ent)
        if self.highpoint is None:
            raise "ERROR : No high point for jumppad"
        
    def start(self):
        CTriggerModel.start(self)
        highpoint = self.mCont.actors.getActorByName(self.highpoint)
        if highpoint is None:
            raise "ERROR : No actor named %s" % (self.highpoint)

        self.highpoint = highpoint.origin

        delta = self.origin - self.highpoint
        vel = sylphis.CVector3(0, 0, 0)
        g = CPhysicsEnv.getGravity().y
        vel.y = math.sqrt(2 * delta.y * g)
        vel.x = delta.x / (vel.y / g)
        vel.z = delta.z / (vel.y / g)
        self.velocity = vel

    def getForce(self, body):
        return self.velocity * body.getMass() / CPhysicsEnv.getStepTime()
        
    def handleEvent(self, *args):
        if args[0] == 11:
            other = args[1]
            if(other.getContents()):# & sylphis.CONTENTS_BODY):
                other.setVelocity(sylphis.CVector3.ZERO)
                other.setPosition(other.getPosition() + sylphis.CVector3(0,1,0))
                other.addForce(self.getForce(other))
        
