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
from CTriggerBase import *
from CMiscModel import *

class CTriggerModel(CTriggerBase):
    __classname__ = 'trigger_model'
    __specs__ = [
        {'name':'delay', 'doc':'The delay taken before triggering'},
        {'name':'once', 'type':'bool','doc':'Will it trigger once and then die?'}
    ]
    def __init__(self, *args):
        CTriggerBase.__init__(self, *args)

    def join(self, ent):
        CTriggerBase.join(self, ent)
        
        o = split(ent['origin'])
        origin = sylphis.CVector3(float(o[0]), float(o[2]), -float(o[1]))
        
        if self.model_id is None:
            raise "No model_id for trigger"
        
        model = self.mScene.getWorldMapModel(int(self.model_id))

        m = manage(sylphis.CSceneMD3, self.mScene)
        m.setModel(model)
        m.setPosition(origin)

        rigidbody = 1
        if rigidbody == 1:
            self.body = manage(sylphis.CRigidBox, 0, m, sylphis.CVector3(20, 50, 20))
        elif rigidbody == 2:
            self.body = manage(sylphis.CRigidSphere, 0, m, 20)
        elif rigidbody == 3:
            self.body = manage(sylphis.CRigidCylinder, 0, m)
        else:
            return

        self.body.makeImmovable()
        self.body.setSolid(0)
        self.body.fitToModel(m)
        self.body.setListener(self)
        self.body.setPosition(origin)
        self.body.setContents(sylphis.CONTENTS_TRIGGER)
        self.body.setCollisionMask(sylphis.CONTENTS_BODY)
        CPhysicsEnv.registerRigidBody(self.body)

    def handleEvent(self, *args):
        if args[0] == 11:
            other = args[1]
            # Only trigger if touched by a body
            if(other.getContents() & sylphis.CONTENTS_BODY):
                self.trigger(other)
        
