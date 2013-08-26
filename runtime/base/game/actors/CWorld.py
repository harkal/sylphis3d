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

