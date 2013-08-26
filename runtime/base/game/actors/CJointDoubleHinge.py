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
