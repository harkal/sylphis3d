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

import sys, sylphis, math
from string import *
from CSimpleActor import *
from util import manage
import time, mthread

class CLight(CSimpleActor):
    __classname__ = 'light'
    __desc__ = 'A light'
    __actortype__ = 'e'
    __specs__ = [
        {'name':'radius', 'doc':'The radius of the light'},
        {'name':'light_center', 'doc':'The light emition point in the lights volume'},
        {'name':'color', 'type':'color', 'doc':'light color (not the intensity, only the color)'},
        {'name':'intensity', 'doc':'The lights intensity'},
        {'name':'type', 'select':{0:'NORMAL', 4:'Flickering', 8:'Strobo', 16:'Pulsating', 32:'Sun'}, 'doc':'The periodic effect to use'},
        {'name':'freq', 'doc':'The frequency (Hz) of the periodic effect the light might has'},
        {'name':'phase', 'doc':'The phase [0, 1.0] of the periodic effect the light might has'},
        {'name':'specular', 'type':'bool','doc':'Will the light create specular highlights on objects?'},
        {'name':'shadows', 'type':'bool', 'doc':'Will create shadows?'},
        {'name':'attenuation', 'doc':'The attenuation of the light'},
        {'name':'filter', 'doc':'The color filter to apply on the light'}
    ]
    radius = '300.0'
    color = sylphis.CVector3(1.0, 1.0, 1.0)
    intensity = 1.0
    light_center = sylphis.CVector3(0.0, 0.0, 0.0)
    type = 0
    freq = 1.0
    phase = 0.0
    bob = None
    specular = 1
    shadows = 1
    dynamic = 0
    filter = 'nofilter'
    attenuation = ''
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)

    def join(self, ent):
        CSimpleActor.join(self, ent)

        self.light_center.swapHand()

        o = self.radius.split()
        if len(o) == 1: o = (o[0], o[0], o[0])
        elif len(o) == 2: o = (o[0], o[1], (o[0] + o[1]) / 2)
        
        self.radius = (float(o[0]), float(o[2]), float(o[1]))

        sm = self.mScene
        sl = manage(sylphis.CLight, sm)
        sl.setCenter(self.light_center)
        sl.setPosition(self.origin)
        sl.setColor(self.color * self.intensity)
        sl.setIsShadowCaster(self.shadows)
        sl.setSpecular(self.specular)
        sl.setRadius(self.radius[0], self.radius[1], self.radius[2])
        sl.setType(int(self.type))
        sl.setFreq(float(self.freq))
        sl.setPhase(float(self.phase))
        
        if self.filter != 'nofilter':
            self.filterMap = CEngine.textures.registerCubeMap(self.filter)
            sl.setFilter(self.filterMap)

        sl.setAttenuation(CEngine.renderer.registerAttenuation(self.attenuation))
        self.node = sl
        self.time = 0
        self.timedelta = 0

    def start(self):
        CSimpleActor.start(self)
        
        # If the light is not dynamic inform the scene manager
        # This way it may optimize the shadowing and lighting
        if self.dynamic == 0:
            sm = self.mScene
            sm.makeLightStatic(self.node)

    def Action_makeStatic(self, other):
        """
        Signal to make the light static. The engine will try to optimize
        the the shadows and lighting casted from this light from the current
        position
        """
        sm = self.mScene
        sm.makeLightStatic(self.node)
        
    def Action_turnOn(self, other):
        """
        Signal for the light turn on (In/Out)
        """
        self.node.setActive(True)

    def Action_turnOff(self, other):
        """
        Signal for the light turn off (In/Out)
        """
        self.node.setActive(False)
        

