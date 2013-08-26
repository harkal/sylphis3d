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
from CJoint import *
from util import manage

class CJointDoubleHinge(CJoint):
    __classname__ = 'joint_double_hinge'
    __desc__ = 'A double hinge joint'
    __specs__ = [
        {'name':'axis1', 'doc':'The first axis'},
        {'name':'axis2', 'doc':'The second axis'},
        {'name':'lo_stop', 'doc':'The low stop angle'},
        {'name':'hi_stop', 'doc':'The high stop angle'},
        {'name':'stop_softness', 'doc':'How soft will the limits be'}
    ]
    axis1 = '0 0 1'
    axis2 = '1 0 0'
    lo_stop = -90.0
    hi_stop = 90.0
    stop_softness = 0.0001
    def __init__(self, *args):
        CJoint.__init__(self, *args)
      
    def join(self, ent):
        CJoint.join(self, ent)
        o = split(self.axis1)
        self.axis1 = sylphis.CVector3(float(o[0]), float(o[1]), float(o[2]))
        self.axis1.swapHand()
        o = split(self.axis2)
        self.axis2 = sylphis.CVector3(float(o[0]), float(o[1]), float(o[2]))
        self.axis2.swapHand()
        self.loStop = self.lo_stop / 180 * math.pi
        self.hiStop = self.hi_stop / 180 * math.pi
        self.cfm = self.stop_softness

        self.joint = manage(sylphis.CJointDoubleHinge, 0, 0)
        
    def start(self):
        CJoint.start(self)
        self.joint.setAxis1(self.axis1)
        self.joint.setAxis2(self.axis2)
        self.joint.setLoStop(self.loStop)
        self.joint.setHiStop(self.hiStop)
        #self.joint.setCFM(self.cfm)
