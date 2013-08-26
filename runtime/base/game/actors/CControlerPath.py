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

                
