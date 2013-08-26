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

            
