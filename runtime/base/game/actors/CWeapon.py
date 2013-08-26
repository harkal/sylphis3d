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

class CWeapon(CSimpleActor):
    __classname__ = 'CWeapon'
    model = 'models/pistol.model'
    fire_sound = 'sounds/shot.wav'
    dryfire_sound = 'sounds/dryfire.wav'
    reload_sound = 'sounds/reload.wav'
    auto_reload = 1
    fire_delay = 0.5
    ammo_class = 'CProjectile'
    clip_ammo = 5
    clips = 10
    ammo = 0
    
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)
        self.lastFire = 0.0
    
    def join(self, ent):
        CSimpleActor.join(self, ent)

        # Get engine and scene manager in handy variables
        sm = self.mScene

        # Register the weapon model
        gun = CEngine.models.registerSkelModel(self.model)

        # Create the weapon scene model
        self.gun = manage(sylphis.CSceneModel, sm, gun)
        self.gun.setPosition(sylphis.CVector3(0.0, 0.0, 0.0))
        self.gun.blendAnimCycle('idle', 0.1, 0.1)
        self.gun.setFlags(sylphis.CSceneModel.NOSHADOW | sylphis.CSceneModel.NOCULL)

        # Register the weapon sounds
        self.shotsound = CEngine.mSoundEnv.registerSoundBuffer(self.fire_sound)
        self.drysound = CEngine.mSoundEnv.registerSoundBuffer(self.dryfire_sound)
        self.reloadsound = CEngine.mSoundEnv.registerSoundBuffer(self.reload_sound)
        
        # Create the weapon sound source
        self.s = manage(sylphis.CSoundSource, CEngine.mSoundEnv)
        self.s.setLooping(0)

        # Attach the sound source to the weapon node
        self.gun.addChild(self.s)

    EVENT_RELOAD = 100
    EVENT_FIRE = 101
    def handleEvent(self, *args):
        if args[0] == self.EVENT_RELOAD:
            self._reload()
        elif eType == self.EVENT_FIRE:
            self.fire(args[1])

    mCanFire = 1
    def fire(self, owner):
        # If we can't fire return immediatly
        if not self.mCanFire:
            return
        
        # Check if enough time has passed since
        # last time we fired
        time = CEngine.getFrameTime()
        if self.lastFire + self.fire_delay > time:
            return 0
        self.lastFire = time

        # If we don't have ammo, dryfire
        if self.ammo <= 0:
            self._dryFire()
            return 0

        # We are clear to fire the bullet
        # Spawn the bullet in the world
        sp = {'classname':self.ammo_class}
        sp['origin'] = self.gun.getDerivedPosition()
        sp['angles'] = self.gun.getDerivedOrientation()
        sp['owner'] = owner
        self.spawn(sp)

        # Play the fire animation to the weapon node
        self.gun.executeAction('fire', 0.0, 1.0)
        # Play the shoting sound from the sound source
        self.s.play(self.shotsound)
        # Reduce the ammo counter
        self.ammo -= 1
        return 1

    def isReady(self):
        return self.mCanFire

    def _dryFire_p(self):
        # Play the dryfire animation on the weapon node
        # and the dryfire sound on the sound source
        self.gun.executeAction('dryfire', 0.0, 1.0)
        self.s.play(self.drysound)
        # Wait until the animation action is completed
        self.gun.waitAction()

        # If auto_reload is true reload the weapon
        if self.auto_reload:
            self._reload()

        # Enable firing
        self.mCanFire += 1

    def _dryFire(self):
        # Disable firing
        self.mCanFire -= 1
        # Run the dryfire method in parallel
        mthread.parallelize(self._dryFire_p)

    def _reload_p(self):
        # Play the reload animation on the weapon node
        # and the reload sound on the sound source
        self.gun.executeAction('reload', 0.0, 1.0)
        self.s.play(self.reloadsound)
        # Wait until the animation action is completed
        self.gun.waitAction()

        # Update the data
        self.ammo = self.clip_ammo
        self.clips -= 1

        # Remove the reloading state, and enable firing
        del self.reloading
        self.mCanFire += 1

    def _reload(self):
        # If no clips available return 
        if self.clips <= 0:
            return

        # If we are already reloading return
        if hasattr(self, 'reloading'):
            return

        # Mark that we are reloading, disable firing
        # and run _reload_p in parallel
        self.reloading = 1
        self.mCanFire -= 1
        mthread.parallelize(self._reload_p)

