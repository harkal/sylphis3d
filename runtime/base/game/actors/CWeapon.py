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

