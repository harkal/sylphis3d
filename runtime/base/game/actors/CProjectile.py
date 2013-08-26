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
from util import manage
from string import *
from CSimpleActor import *
from CExplosion import *

class CProjectile(CSimpleActor):
    __classname__ = 'CProjectile'
#    model = 'models/bulletfire.md3'
#    model = 'models/box.md3'
    model = ''
    hit_sound = 'sounds/bullet_hit.wav'
    health = 5.0
    velocity = sylphis.CVector3(0, 0, -8000)
    angular_velocity = sylphis.CVector3(0, 0, 0)
    damping = 0.0
    radius = 3.0
    bounce = 0
    mass = 1.0
    fuse = 10.0
    detonate_on_fuse = 0
    detonate_on_death = 1
    detonate_on_world = 1
    detonate_on_actor = 1
    damage_factor = 1.0
        
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)

    def handleEvent(self, *args):
        eType = args[0]
        #print self.__class__, "Event", eType
        if eType is self.EVENT_COLLISION:
            other = args[1]
            if not self.bounce:
                self.explode()
                self.remove()

    def join(self, ent):
        CSimpleActor.join(self, ent)
        self.time = 0.0
        sm = self.mCont.getSceneManager()
        
        if self.model != '':
            model = CEngine.models.registerMD3Model(self.model)
            m = manage(sylphis.CSceneMD3, sm)
            m.setModel(model)
            m.setPosition(self.origin)
            self.node = m
            self.body = manage(sylphis.CRigidSphere, 0, self.node, self.radius)
        else:
            self.body = manage(sylphis.CRigidSphere, 0, None, self.radius)

        if self.hit_sound != '':
            shotsound = CEngine.mSoundEnv.registerSoundBuffer(self.hit_sound)
            self.s = manage(sylphis.CSoundSource, CEngine.mSoundEnv)
            self.s.setBuffer(shotsound)
            self.s.setLooping(0)
            self.s.setListener(self)
            sm.getRootSceneNode().addChild(self.s)
        else:
            pass

        self.body.setListener(self)
        self.body.setPosition(self.origin)
        v = self.angles * self.velocity
        self.body.setVelocity(v)
        self.body.setOrientation(self.angles)
        self.body.setDamping(self.damping)
        #self.body.setSuspendMultiCollitions(0)
        self.body.setDamageFactor(self.damage_factor)
        self.body.noCollitionWith(self.owner.body)
        self.body.mSR.softness = 0.01
        self.body.mSR.bounce = 0.1
        self.body.mSR.bounceCutoff = 0.1
        CPhysicsEnv.registerRigidBody(self.body)

    def explode(self):
        self.s.setPosition(self.body.getPosition())
        self.s.play()
        self.mScene.getRootSceneNode().removeChild(self.s)
        return
        e = CExplosion(self.mCont)
        self.mCont.actors.spawn(e, {'origin':self.body.getPosition()})

    def run(self):
        if not self.detonate_on_fuse:
            return
        # Sleep for 'fuse' seconds
        self.sleep(self.fuse)
        # Add the explosion actor
        self.explode()
        # Remove ourselfs
        self.remove()
