"""
This file is part of the "Sylphis" 3D engine
Copyright (c) 2002 - 2007 by Harry Kalogirou

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
            
