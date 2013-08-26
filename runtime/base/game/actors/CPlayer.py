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
#from string import *
from CSimpleActor import CSimpleActor
from util import manage
import pyui

thirdPerson = False

class CPlayer(CSimpleActor):
    __classname__ = 'info_player_start'
    __desc__ = 'A player'
    __actortype__ = 'e'
    weapons = []
    activeWeapon = None
    airControl = 10.0
    walkSpeed = 700.0
    runSpeed = 1050.0
    jumpSpeed = 500.0
    fov = 75.0
    height = 64
    camheight = 60
    width = 20

    score = 0
    
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)
        self.camera = manage(sylphis.CCamera, self.mScene)
        self.camera.setFov(self.fov)
        self.node = self.camera # To be registered on the scenemanager
        self.body = manage(sylphis.CRigidCapsule, 0, None, sylphis.CVector3(self.width, self.height, self.width))

        self.body.setMass(5.0)
        self.body.setDamping(0.1)
        self.body.setMaxAngularVelocity(0.001)
        self.body.setDamageFactor(0.000001)
        self.body.setListener(self)
        self.body.setContents(sylphis.CONTENTS_BODY)
        self.body.setCollisionMask(sylphis.MASK_PLAYERSOLID)
        CPhysicsEnv.registerRigidBody(self.body)

        self.omove = sylphis.CActorMover(self.mScene)
        self.omove.mAccel = 0.1

        self.slistener = manage(sylphis.CSoundListener, CEngine.mSoundEnv)
        self.camera.addChild(self.slistener)

        self.lastPos = sylphis.CVector3(0,0,0)
        self.onGround = 0
        self.jumping = False
        self.count = 0
        self.lastGround = None
        self.flyMode = False

        self.camheight -= self.height / 2.0

    def join(self, ent):
        CSimpleActor.join(self, ent)

        # Get engine and scene manager in handy variables
        sm = self.mScene

        if thirdPerson:
            # Register the player model
            pmodel = CEngine.models.registerSkelModel('models/paladin.model')

            # Create the player scene model
            self.model = sylphis.CSceneModel(sm, pmodel)
            self.model.setPosition(sylphis.CVector3(0.0, -self.height, 0.0))
            self.model.blendAnimCycle('idle', 0.1, 0.1)
            self.anim = 'idle'
            #self.model.setFlags(sylphis.CSceneModel.NOSHADOW)

            sm.getRootSceneNode().addChild(self.model)
            
        self.camera.setPosition(self.origin)
        self.body.setPosition(self.origin)

        self.weapons = []
        self.weapons.append(self.addActor({'classname':'CPistol'}))
        self.weapons.append(self.addActor({'classname':'CGrenadeLauncher'}))
        self.changeWeapon(self.weapons[0])

    def changeWeapon(self, weapon):
        if hasattr(self, 'gun'):
            # If the weapon is not ready don't change
            if not self.gun.isReady():
                return
            self.node.removeChild(self.gun.gun)
        self.gun = weapon
        self.gun.gun.setPosition(0,0,7)
        self.node.addChild(self.gun.gun)
        
    def updateCamera(self, onGround, timeDelta):
        v = self.body.getPosition()# - self.smoothing2
        v.y = v.y + self.camheight
        self.camera.setPosition(v)
        g = self.gun.gun
        if thirdPerson:
            m = self.model
        if self.camera.isActive():
            g.setFlags(g.getFlags() & (~sylphis.CSceneModel.NORENDER))
            if thirdPerson:
                m.setFlags(m.getFlags() | sylphis.CSceneModel.NORENDER)
        else:
            g.setFlags(g.getFlags() | sylphis.CSceneModel.NORENDER)
            if thirdPerson:
                m.setFlags(m.getFlags() & (~sylphis.CSceneModel.NORENDER))

        g.setFlags(sylphis.CSceneModel.NORENDER)

    def fire(self):
        self.gun.fire(self)

    def getUpdatePriority(self):
        return 0
            
    def update(self, timeDelta):
        i = sylphis.CEngine.getSingleton().getInput()

        if i.isPressed(sylphis.kF):
            self.body.applyGravity(False)
            self.flyMode = True
        if i.isPressed(sylphis.kG):
            self.body.applyGravity(True)
            self.flyMode = False

        action = 0
        vel = sylphis.CVector3(0,0,0)
        if i.isPressed(sylphis.kW):
            action = action | sylphis.ACTION_FORWARD
            vel.z = -1.0
        if i.isPressed(sylphis.kS):
            action = action | sylphis.ACTION_BACKWARD
            vel.z = 1.0
        if i.isPressed(sylphis.kA):
            action = action | sylphis.ACTION_MOVE_LEFT
            vel.x = -.8
        if i.isPressed(sylphis.kD):
            action = action | sylphis.ACTION_MOVE_RIGHT
            vel.x = .8

        if i.isPressed(sylphis.k1):
            self.changeWeapon(self.weapons[0])
        elif i.isPressed(sylphis.k2):
            self.changeWeapon(self.weapons[1])

        if thirdPerson:
            if action != 0:
                if self.anim == 'idle':
                    self.model.clearAnimCycle('idle', 0.1)
                    self.model.blendAnimCycle('walk', 0.1, 0.1)
                    self.anim = 'walk'
            else:
                if self.anim == 'walk':
                    self.model.clearAnimCycle('walk', 0.1)
                    self.model.blendAnimCycle('idle', 0.1, 0.1)
                    self.anim = 'idle'
            
        if not pyui.isActive():
            if i.isPressed(sylphis.kMOUSELEFT):
                self.fire()
            dx = i.getPointerDeltaX()
            dy = i.getPointerDeltaY()
            self.camera.yawFixed(-float(dx) / 300)
            self.camera.pitch(-float(dy) / 300)

        self.body.mOrientation = self.camera.getOrientation()
        self.body.setOrientation(1, 0, 0, 1)
        
        vel = self.body.mOrientation * vel
        
        onG = self.body.getGroundBody()
        if i.isPressed(sylphis.kSPACE):
            if self.jumping == False:
                action = action | sylphis.ACTION_JUMP
            self.jumping = True
        else:
            self.jumping = False

        if self.flyMode:
            if i.isPressed(sylphis.kSPACE):
                vel.y = 1.0
            vel = vel * 400.0
            self.omove.move(self.body, vel, None, timeDelta)
        else:
            vel.y = 0.0
            if vel.length() > 0.0:
                vel = vel.normalized()
            if onG != None:
                speed = self.walkSpeed
                if i.isPressed(sylphis.kLEFTSHIFT):
                    speed = self.runSpeed
                vel = vel * speed
                if action & sylphis.ACTION_JUMP:
                    f = sylphis.CVector3(0, self.jumpSpeed * self.body.getMass() / timeDelta, 0)
                    self.body.addForce(f)

                self.omove.move(self.body, vel, onG, timeDelta)
            else:
                vel = vel * self.airControl
                self.omove.move(self.body, vel, self.body, timeDelta)
                #self.body.addForce(vel * self.body.getMass() / CPhysicsEnv.getStepTime())
        
        self.updateCamera(onG, timeDelta)

        #l = self.body.getVelocity().length()
        #if l > 1.0:
        #    print l

        if thirdPerson:
            pos = sylphis.CVector3(self.body.getPosition())
            pos.y = pos.y - self.height / 2.0 + 3.0
            self.model.setPosition(pos)
            v = self.node.getOrientation() * sylphis.CVector3(0,0,1)
            v.y = 0.0
            self.model.lookAt(pos + (v * 100.0))
        
    def handleEvent(self, *args):
        if args[0] is self.EVENT_TRIGGER:
            CEngine.renderer.useCamera(self.camera)

            
