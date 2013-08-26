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
from CMiscModel import *
from sylphis import CVector3
from util import manage

HERE = 0
THERE = 1

class CPlatform(CMiscModel):
    ##
    ## Actor specification
    ##
    __classname__ = 'func_platform'
    __desc__ = 'A moving platform'
    __actortype__ = 'b'
    __specs__ = [
        {'name':'speed', 'doc':'The movement speed of the plaform'},
        {'name':'movesize', 'doc':'Units that the platform moves'},
        {'name':'movedir', 'doc':'Direction the platform moves'},
        {'name':'initpos', 'select':{HERE:'HERE', THERE:'THERE'}, 'doc':'Initial position'},
    ]
    speed = 100.0
    initstate = HERE
    movesize = 32.0
    movedir = sylphis.CVector3(0, 1, 0)

    ##
    ## Implementation
    ##
    state = HERE
      
    def join(self, ent):
        self.rigidbody = 1
        self.rigidflags = 1
        CMiscModel.join(self, ent)

        self.herePos = self.origin + self.movedir * self.movesize
        self.therePos = self.origin

        self.vel = self.movedir * self.speed

        self.state = self.initstate
        if self.state == HERE:
            self.body.setPosition(self.herePos)
        elif self.state == THERE:
            self.body.setPosition(self.therePos)
        else:
            raise "Incorrect initial state"

    def Action_moveHere(self, other):
        self.killSignalThreads('moveThere')
        self.body.setVelocity(self.vel)
        time = self.calcMoveTime(self.herePos)
        self.sleep(time)
        self.body.setVelocity(CVector3.ZERO)
        self.body.setPosition(self.herePos)
        self.Action_here()

    def Action_moveThere(self, other):
        self.killSignalThreads('moveHere')
        self.body.setVelocity(-self.vel)
        time = self.calcMoveTime(self.therePos)
        self.sleep(time)
        self.body.setVelocity(CVector3.ZERO)
        self.body.setPosition(self.therePos)
        self.Action_there()

    def Action_here(self, other):
        return

    def Action_there(self, other):
        return

    def calcMoveTime(self, targetPos):
        """
        Calculate the time needed to move from the current
        position to the targetPos position   
        """
        return (self.body.getPosition() - targetPos).length() / self.speed
                        
