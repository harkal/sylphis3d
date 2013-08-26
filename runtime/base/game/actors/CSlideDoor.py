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

# The states the door can be in
CLOSE = 0
OPEN = 1
OPENING = 2
CLOSING = 3

class CSlideDoor(CMiscModel):
    ##
    ## Actor specification
    ##
    __classname__ = 'func_door'
    __desc__ = 'A simple sliding door'
    __actortype__ = 'b'
    __specs__ = [
        {'name':'speed', 'doc':'The movement speed of the door'},
        {'name':'wait', 'doc':'The time to wait before returning to rest state'},
        {'name':'useportal', 'select':{1:'YES', 0:'NO'}, 'doc':'Control the portal near the door?'},
        {'name':'movesize', 'doc':'Units that the door moves'},
        {'name':'movedir', 'doc':'Direction the door moves'},
        {'name':'initstate', 'select':{OPEN:'OPEN', CLOSE:'CLOSE'}, 'doc':'Initial state'},
        {'name':'reststate', 'select':{OPEN:'OPEN', CLOSE:'CLOSE'}, 'doc':'Rest state'}
    ]
    speed = 100.0
    wait = 4.0
    useportal = 1
    initstate = CLOSE
    reststate = CLOSE
    movesize = 32.0
    movedir = sylphis.CVector3(0, 1, 0)

    ##
    ## Implementation
    ##
    state = CLOSE

    def handleEvent(self, *args):
        if args[0] == 11:
            other = args[1]
            if(other.getContents() & sylphis.CONTENTS_BODY):
                self.Action_toggle(other)
      
    def join(self, ent):
        self.rigidbody = 1
        self.rigidflags = 1
        CMiscModel.join(self, ent)

        self.openPos = self.origin + self.movedir * self.movesize
        self.closePos = self.origin

        self.vel = self.movedir * self.speed

        if self.useportal:
            self.portalId = self.mScene.getAreaPortal(self.origin)
            self.node.setPassClosedPortals(True)

        self.state = self.initstate
        if self.state == OPEN:
            self.body.setPosition(self.openPos)
            self.setPortalOpen(True)
        elif self.state == CLOSE:
            self.body.setPosition(self.closePos)
            self.setPortalOpen(False)
        else:
            raise "Incorrect initial state"

    def Action_open(self, other):
        self.killSignalThreads('close')
        self.state = OPENING
        self.setPortalOpen(True)
        self.body.setVelocity(self.vel)
        time = self.calcMoveTime(self.openPos)
        self.sleep(time)
        self.body.setVelocity(CVector3.ZERO)
        self.body.setPosition(self.openPos)
        self.Action_openFinish()

    def Action_close(self, other):
        self.killSignalThreads('open')
        self.state = CLOSING
        self.body.setVelocity(-self.vel)
        time = self.calcMoveTime(self.closePos)
        self.sleep(time)
        self.body.setVelocity(CVector3.ZERO)
        self.body.setPosition(self.closePos)
        self.setPortalOpen(False)
        self.Action_closeFinish()

    def Action_openFinish(self, other):
        if other != None:
            return
        if self.reststate == OPEN:
            return
        self.sleep(self.wait)
        self.Action_close()

    def Action_closeFinish(self, other):
        if other != None:
            return
        if self.reststate == CLOSE:
            return
        self.sleep(self.wait)
        self.Action_open()

    def Action_toggle(self, other):
        if self.state in (OPENING, OPEN): self.Action_close(other)
        elif self.state in (CLOSING, CLOSE): self.Action_open(other)  

    def calcMoveTime(self, targetPos):
        """
        Calculate the time needed to move from the current
        position to the targetPos position   
        """
        return (self.body.getPosition() - targetPos).length() / self.speed
            
    def setPortalOpen(self, t):
        if self.useportal:
            self.mScene.setAreaPortalOpen(self.portalId, t)
            
