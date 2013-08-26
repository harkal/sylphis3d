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
                        
