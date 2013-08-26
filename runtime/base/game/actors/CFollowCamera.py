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
from CCamera import *
import time, mthread

class CFollowCamera(CCamera):
    __classname__ = 'follow_camera'
    __desc__ = 'A third person camera that can follow the target actor arround'
    __actortype__ = 'e'
    
    orientation = sylphis.CQuaternion()

    def update(self, timeDelta):
        pos = sylphis.CVector3(self.targetNode.getPosition())
        ori = sylphis.CQuaternion(self.targetNode.getOrientation())

        q = sylphis.CQuaternion()
        q.fromAngleAxis(sylphis.CVector3(1,0,0), -0.4)
        ori = ori * q;
        
        self.orientation.lerp(1.0 * timeDelta, ori)
        
        dir = self.orientation * sylphis.CVector3(0,0,1)
        dir = dir * 250.0
        end = pos + dir
        
        tr = sylphis.CTraceResult()
        self.targetBody.setDisabled(True)
        self.mScene.raytrace(tr, pos, end, self.targetBody)
        self.targetBody.setDisabled(False)
        tr.frac -= 0.2
        self.node.setPosition(pos + dir * tr.frac)
        self.node.setOrientation(self.orientation)
        
        
