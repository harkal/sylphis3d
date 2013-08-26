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
import pyui

class CSpeaker(CSimpleActor):
    __classname__ = 'speaker'
    __desc__ = 'A sound speaker'
    __actortype__ = 'e'
    __specs__ = [
        {'name':'sound', 'doc':'The sound file to play'},
        {'name':'loop', 'select':{0:'NO', 1:'YES'}, 'doc':'Will the sound loop?'},
        {'name':'play', 'select':{0:'NO', 1:'YES'}, 'doc':'Start playing on startup'}
    ]
    sound = ''
    loop = 1
    play = 1
    
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)

    def join(self, ent):
        CSimpleActor.join(self, ent)
        if self.sound != '':
            sb = CEngine.mSoundEnv.registerSoundBuffer(self.sound)
            self.node = manage(sylphis.CSoundSource, CEngine.mSoundEnv)
            self.node.setBuffer(sb)
            self.node.setPosition(self.origin)
            self.node.setLooping(self.loop)

    def start(self):
        CSimpleActor.start(self)
        if self.play == 1:
            self.Action_play()
        
    def Action_play(self, other):
        self.node.play()

    def Action_pause(self, other):
        self.node.stop()

    def Action_stop(self, other):
        self.node.stop()

    def Action_rewind(self, other):
        self.node.rewind()
        
