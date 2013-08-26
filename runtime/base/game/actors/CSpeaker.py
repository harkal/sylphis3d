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
        
