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
        
        
