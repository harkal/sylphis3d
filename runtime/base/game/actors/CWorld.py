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

import sys, sylphis
from string import *
from CSimpleActor import *
from util import manage

class CWorld(CSimpleActor):
    """
    This is the 'world' actor. It represents the world that everything
    takes place.
    """
    __classname__ = 'worldspawn'
    __desc__ = 'The ''world'' actor'
    __actortype__ = 'e'
    __specs__ = [
        {'name':'ambient_color', 'doc':'The ambient color'},
        {'name':'sound_track', 'doc':'The default audio file to play in this world'},
        {'name':'fog', 'type':'bool', 'doc':'Use global fog?'},
        {'name':'fog_color', 'type':'color', 'doc':'fog color'},
        {'name':'fog_bounds', 'doc':'fog bounds'}
    ]
    ambient_color = '0 0 0'
    sound_track = ''
    fog = 0
    fog_color = sylphis.CVector3(1.0, 1.0, 1.0)
    fog_bounds = sylphis.CVector3(256.0, 2048.0, 1.0)
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)
      
    def join(self, ent):
        """
        Loads the soundtrack for this world.
        """
        CSimpleActor.join(self, ent)
        o = split(self.ambient_color)
        self.ambientLight = sylphis.CVector3(float(o[0]), float(o[1]), float(o[2]))
        if self.sound_track != '':
            self.soundTrack = CEngine.mSoundEnv.registerSoundBuffer(self.sound_track)
            self.track = manage(sylphis.CSoundSource, CEngine.mSoundEnv)
            self.track.setBuffer(self.soundTrack)
            self.track.setPosition(0,0,0)
            self.track.setSpatial(0)
            self.mScene.getRootSceneNode().addChild(self.track)

    def start(self):
        """
        Starts the playback of the soundtrack
        """
        CSimpleActor.start(self)
        self.mScene.setAmbientLight(self.ambientLight)
        if hasattr(self, 'track'):
            self.track.play()

        if self.fog == 1:
            r = CEngine.renderer
            #r.setEnableFog(True)
            r.setFogColor(self.fog_color)
            r.setFogBounds(self.fog_bounds.x, self.fog_bounds.y)

