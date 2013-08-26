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

import sys, sylphis, CProjectile
from sylphis import CVector3
from string import *
from CSimpleActor import *
from util import manage
import pyui

thirdPerson = True

class CVehicle(CSimpleActor):
    __classname__ = 'base_vehicle'
    __desc__ = 'Base vehicle class'
    __actortype__ = 'e'
    weapons = []
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)
        
    def join(self, ent):
        CSimpleActor.join(self, ent)

        # Get engine and scene manager in handy variables
        sm = self.mScene

        self.precache()
        self.initBody()   

    def initBody(self):
        self.body = manage(sylphis.CRigidSphere, 0, self.node, 20)
        self.body.setDensity(0.000001)
        self.body.fitToModel(self.node)
        self.body.setListener(self)
        self.body.applyGravity(False)
        CPhysicsEnv.registerRigidBody(self.body)

    def precache(self):
        self.model = manage(sylphis.CSceneMD3, self.mScene)
        model = CEngine.models.registerMD3Model('models/bball.md3')
        self.model.setModel(model)
        
    def isOnGround(self):
        tr = sylphis.CTraceResult()
        start = self.body.getPosition() #+ sylphis.CVector3(0, -33, 0)
        end = start + CVector3(0, -33, 0)
        self.body.setDisabled(True)
        end.x = start.x + 10
        end.z = start.z + 10
        self.mScene.raytrace(tr, start, end, self.body)
        onG = tr.frac < 1.0
        end.x = start.x - 10
        end.z = start.z + 10
        self.mScene.raytrace(tr, start, end, self.body)
        onG = onG or tr.frac < 1.0
        end.x = start.x + 10
        end.z = start.z - 10
        self.mScene.raytrace(tr, start, end, self.body)
        onG = onG or tr.frac < 1.0
        end.x = start.x - 10
        end.z = start.z - 10
        self.mScene.raytrace(tr, start, end, self.body)
        onG = onG or tr.frac < 1.0
        self.body.setDisabled(False)
        
        return onG

    def updateCamera(self, onGround, timeDelta):
        v = self.body.getPosition()# - self.smoothing2
        v.y += 31
        self.camera.setPosition(v)
        g = self.gun.gun
        m = self.model
        if self.camera.isActive():
            g.setFlags(g.getFlags() & (~sylphis.CSceneModel.NORENDER))
            m.setFlags(m.getFlags() | sylphis.CSceneModel.NORENDER)
        else:
            g.setFlags(g.getFlags() | sylphis.CSceneModel.NORENDER)
            m.setFlags(m.getFlags() & (~sylphis.CSceneModel.NORENDER))

    def fire(self):
        self.gun.fire(self)

    def getUpdatePriority(self):
        return 0
            
    def update(self, timeDelta):
        i = CEngine.getInput()

        action = 0
        vel = CVector3(0,0,0)
        if i.isPressed(sylphis.kQ):
            action |= sylphis.ACTION_FORWARD
            vel.y = 550.0
            self.body.addLocalForce(vel)
            vel.y = 10.0
            self.body.addTorque(vel)
            
        
    def handleEvent(self, *args):
        if args[0] is self.EVENT_TRIGGER:
            CEngine.renderer.useCamera(self.camera)

            
