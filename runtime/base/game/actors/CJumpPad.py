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
        
