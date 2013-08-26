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
