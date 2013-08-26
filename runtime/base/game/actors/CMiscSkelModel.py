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
from util import manage
from CSimpleActor import *

class CMiscSkelModel(CSimpleActor):
    __classname__ = 'misc_skel_model'
    __desc__ = 'A skeletal model'
    __actortype__ = 'e'
    __specs__ = [
        {'name':'model', 'doc':'The filename that contains the model'},
        {'name':'anim', 'doc':'The animation to run on the model. e.g. ''idle'''},
        {'name':'skin', 'doc':'The skin to apply on the model. (Not implemented yet)'}
    ]
    model = ''
    anim = 'idle'
    skin = ''
    
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)
      
    def join(self, ent):
        CSimpleActor.join(self, ent)
        pi = 3.14159 # FIXME
        try:
            o = split(ent['angles'])
        except KeyError,x:
            ang = ent.get('angle', '0')
            o = ('0', ang, '0')
        self.angles = sylphis.CVector3(-pi * float(o[0]) / 180, pi * float(o[1]) / 180, pi * float(o[2]) / 180)

        model = CEngine.models.registerSkelModel(self.model)

        bt = manage(sylphis.CBoneControler)
        bt.setTranslation(sylphis.CVector3(0,0,0))
        q = sylphis.CQuaternion()
        q.fromAngleAxis(sylphis.CVector3(1,0,0), 10.0)
        bt.setRotation(q)
        bt.setBoneId(0)
        
        sa = manage(sylphis.CSkelAnimation)
        sa.setDuration(100.0)
        sa.addBoneTrack(bt)

        #print "Controlers :", bt, sa

        sm = self.mScene
        m = sylphis.CSceneModel(sm, model)
        m.setPosition(self.origin)
        m.blendAnimCycle(self.anim, 1.0, 0.1)

        m.executeAction(sa, 'test')
        
        m.yaw(self.angles.y)
        m.roll(self.angles.x)

        self.node = m
        
      
