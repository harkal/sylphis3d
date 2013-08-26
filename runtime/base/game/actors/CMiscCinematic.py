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

import sys, sylphis, math
from string import *
from CSimpleActor import *

class CMiscCinematic(CSimpleActor):
    __classname__ = 'misc_cinematic'
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)
      
    def join(self, ent):
        CSimpleActor.join(self, ent)
        o = split(ent.get('origin', '0 0 0'))
        self.origin = sylphis.CVector3(float(o[0]), float(o[2]), -float(o[1]))
        pi = 3.14159 # FIXME
        o = split(ent.get('angles','0 0 0'))
        self.angles = sylphis.CVector3(-pi * float(o[0]) / 180, pi * float(o[1]) / 180, pi * float(o[2]) / 180)
        self.model = ent['model']
        self.startAnim = ent.get('start_anim', 'none')
        if self.startAnim is '':self.startAnim = 'none'
        self.triggerAnim = ent.get('trigger_anim', 'action')
        
##        try:
##            self.skin = ent['skin']
##        except KeyError, x:
##            self.skin = None

        model = CEngine.models.registerModel(self.model)

        sm = self.mContainer.getSceneManager()
        m = sylphis.CSceneModel(sm, model)
        m.setPosition(self.origin)
        if m.hasAnim(self.startAnim):
            m.blendAnimCycle(self.startAnim, 1.0, 0.01)
        
        m.yaw(self.angles.y)
        m.roll(self.angles.x)

        self.node = m

        self.cameraName = ent['camera']

    def start(self):
        CSimpleActor.start(self)
        self.camera = self.mCont.actors.getActorByName(self.cameraName)

    def handleEvent(self, *args):
        if args[0] is self.EVENT_TRIGGER:
            self.node.clearAllAnimCycle(0.01)
            if self.node.hasAnim(self.triggerAnim):
                self.node.blendAnimCycle(self.triggerAnim, 1.0, 0.01)
            self.camera.makeActiveCamera()
            return


