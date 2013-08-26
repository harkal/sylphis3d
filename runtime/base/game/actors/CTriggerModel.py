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
        
