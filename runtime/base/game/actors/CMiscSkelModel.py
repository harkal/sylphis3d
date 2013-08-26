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
        
      
