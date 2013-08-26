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
        
        
