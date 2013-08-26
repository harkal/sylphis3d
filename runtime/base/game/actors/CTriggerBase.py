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

