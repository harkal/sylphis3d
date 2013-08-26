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

import sys, sylphis, mthread
from string import *
from CControlerBase import *
from util import manage

LINEAR = 0
SPLINE = 1

class CControlerPath(CControlerBase):
    __classname__ = 'controler_path'
    __desc__ = 'Controls other actors to follow a path'
    __actortype__ = 'e'
    __specs__ = [
        {'name':'pathnodes', 'doc':'The series of actors making the path'},
        {'name':'pathtype', 'select':{LINEAR:'LINEAR', SPLINE:'SPLINE'}, 'doc':'The type of interpolation to use'},
        {'name':'lookatpathnodes', 'doc':'The series of actors making the path'},
        {'name':'lookatpathtype', 'select':{LINEAR:'LINEAR', SPLINE:'SPLINE'}, 'doc':'The type of interpolation to use'},
        {'name':'speed', 'doc':'The speed to travel the path'}
    ]
    pathnodes = ''
    pathtype = LINEAR
    lookatpathnodes = ''
    lookatpathtype = LINEAR
    speed = 1.0
    loop = -1.0
    
    def start(self):
        CControlerBase.start(self)
        pathNodeNames = split(self.pathnodes)
        if self.pathtype == LINEAR:
            self.path = manage(sylphis.CInterpolatorVector3)
        else:
            self.path = manage(sylphis.CSplineInterpolatorVector3)
        i = 0.0
        time = 0.0
        for name in pathNodeNames:
            node = self.mCont.actors.getActorByName(name)
            if node is None:
                continue
            if hasattr(node, 'time'):
                time += float(node.time)
            else:
                if i > 0:time += 1 #let the first node be at zero
            self.path.insert(node.origin, time / self.speed)
            #print node, time / self.speed
            i += 1.0

        if self.loop == 0.0:
            self.loop = time / self.speed
        if self.loop == -1.0:
            self.loop = 0.0

        pathNodeNames = split(self.lookatpathnodes)
        if self.lookatpathtype == LINEAR:
            self.lookatpath = manage(sylphis.CInterpolatorVector3)
        else:
            self.lookatpath = manage(sylphis.CSplineInterpolatorVector3)
        i = 0.0
        time = 0.0
        for name in pathNodeNames:
            node = self.mCont.actors.getActorByName(name)
            if node is None:
                continue
            if hasattr(node, 'time'):
                time += float(node.time)
            else:
                if i > 0:time += 1
            self.lookatpath.insert(node.origin, time / self.speed)
            #print node.__classname__, time / self.speed
            i += 1.0

    def Action_startControling(self, other):
        for name in self.targetNames:
            target = self.mCont.actors.getActorByName(name)
            if target is None:
                continue
            target.node.followPath(self.path, 0.0, self.loop)
            if hasattr(target, 'body'):
                target.body.followPath(self.path, 0.0, self.loop)
            if self.lookatpathnodes != '':
                target.node.lookAtPath(self.lookatpath, 0.0, self.loop)  
        
    def Action_stopControling(self, other):
        for name in self.targetNames:
            target = self.mCont.actors.getActorByName(name)
            if target is None:
                continue
            target.node.stopFollowPath()
            if hasattr(target, 'body'):
                target.body.stopFollowPath()
            if self.lookatpathnodes != '':
                target.node.stopLookAtPath()

                
