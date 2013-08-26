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

        
