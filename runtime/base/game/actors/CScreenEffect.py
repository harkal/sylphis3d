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

import game.client

class CScreenEffect(CSimpleActor):
    """
    
    """
    __classname__ = 'screen_effect'
    __desc__ = 'An actor to control screen effects, like fade in/out'
    __actortype__ = 'e'
    __specs__ = [
        {'name':'fadein_time', 'doc':'Time in sec to fade in'},
        {'name':'fadeout_time', 'doc':'Time in sec to fade out'}
    ]
    fadein_time = 5.0
    fadeout_time = 5.0
      
    def Action_fadeIn(self, other):
        game.client.fadeInScreen(self.fadein_time)

    def Action_fadeOut(self, other):
        game.client.fadeOutScreen(self.fadeout_time)

    def Action_glareOn(self, other):
        CClient.useGlare(True)

    def Action_glareOff(self, other):
        CClient.useGlare(False)

    def Action_oldFilmOn(self, other):
        CClient.useOldFilm(True)

    def Action_oldFilmOff(self, other):
        CClient.useOldFilm(False)

    def Action_oldFilmGlareOn(self, other):
        CClient.useOldFilmGlare(True)

    def Action_oldFilmOffGlareOff(self, other):
        CClient.useOldFilmGlare(False)

    def Action_motionBlurOn(self, other):
        CClient.useMotionBlur(True)

    def Action_motionBlurOff(self, other):
        CClient.useMotionBlur(False)

        
