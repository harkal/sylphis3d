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

class CMiscCinematic(CSimpleActor):
    __classname__ = 'misc_cinematic'
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)
      
    def join(self, ent):
        CSimpleActor.join(self, ent)
        o = split(ent.get('origin', '0 0 0'))
        self.origin = sylphis.CVector3(float(o[0]), float(o[2]), -float(o[1]))
        pi = 3.14159 # FIXME
        o = split(ent.get('angles','0 0 0'))
        self.angles = sylphis.CVector3(-pi * float(o[0]) / 180, pi * float(o[1]) / 180, pi * float(o[2]) / 180)
        self.model = ent['model']
        self.startAnim = ent.get('start_anim', 'none')
        if self.startAnim is '':self.startAnim = 'none'
        self.triggerAnim = ent.get('trigger_anim', 'action')
        
##        try:
##            self.skin = ent['skin']
##        except KeyError, x:
##            self.skin = None

        model = CEngine.models.registerModel(self.model)

        sm = self.mContainer.getSceneManager()
        m = sylphis.CSceneModel(sm, model)
        m.setPosition(self.origin)
        if m.hasAnim(self.startAnim):
            m.blendAnimCycle(self.startAnim, 1.0, 0.01)
        
        m.yaw(self.angles.y)
        m.roll(self.angles.x)

        self.node = m

        self.cameraName = ent['camera']

    def start(self):
        CSimpleActor.start(self)
        self.camera = self.mCont.actors.getActorByName(self.cameraName)

    def handleEvent(self, *args):
        if args[0] is self.EVENT_TRIGGER:
            self.node.clearAllAnimCycle(0.01)
            if self.node.hasAnim(self.triggerAnim):
                self.node.blendAnimCycle(self.triggerAnim, 1.0, 0.01)
            self.camera.makeActiveCamera()
            return


