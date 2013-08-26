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

class CTriggerBase(CControlerBase):
    __specs__ = [
        {'name':'delay', 'doc':'The delay taken before triggering'},
        {'name':'once', 'type':'bool','doc':'Will it trigger once and then die?'}
    ]
    delay = 0.0
    once = 0
    def __init__(self, *args):
        CControlerBase.__init__(self, *args)

    def Action_trigger(self, other):
        """
        Signal for the trigger (Out)
        """
        pass
        
    def triggerWithDelay(self, source):
        #print "triggerWithDelay", source.__class__
        self.sleep(self.delay)
        self.triggerTargets(source)

    def triggerTargets(self, source):
        self.Action_trigger(source)
        for name in self.targetNames:
            #print "triggerTargets : ", name
            self.sendEventByName(name, self.EVENT_TRIGGER, self, source)

        # If we are marked to only trigger once we remove
        # ourselfs. This is done with parallelize since we
        # can get here from self.startup() if the delay is zero
        if self.once:
            self.parallelize(self.remove)

    def trigger(self, source = None):
        if self.delay == 0.0:
            self.triggerTargets(source)
        else:
            self.parallelize(self.triggerWithDelay, source)

