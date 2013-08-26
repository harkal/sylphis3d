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
from CSimpleActor import *
from util import manage
from math import pi

def getDamage(self, body):
    #print "Velocity :", body.getVelocity().length(), body.getDamageFactor()
    return body.getVelocity().length() * body.getDamageFactor()

class CMiscModel(CSimpleActor):
    ## 
    ## Actor specification 
    ## 
    __classname__ = 'misc_model'
    __desc__ = 'A simple model, that can have a physical responce'
    __actortype__ = 'e'
    __specs__ = [
        {'name':'model', 'doc':'The filename that contains the model'},
        {'name':'anim', 'doc':'The animation to run on the model. e.g. ''idle'''},
        {'name':'rigidbody', 'select':{0:'NONE', 1:'BOX', 2:'SPHERE', 5:'TRIMESH'}, 'doc':'This is the rigidbody shape that the engine will try to fit to'},
        {'name':'mass', 'doc':'The mass of the rigid body'},
        {'name':'damping', 'doc':'Air friction'},
        {'name':'damage_factor', 'doc':'Damage factor'},
        {'name':'damage_thresh', 'doc':'Damage threshold under which no damage is taken.'},
        {'name':'health', 'doc':'Health'},
        {'name':'shader', 'doc':'The skin to apply on the model'},
        {'name':'noshadow', 'doc':'Dont cast shadows'}
    ]
    rigidbody = 0
    rigidflags = 0
    mass = 1.0
    damping = 0.01
    damage_factor = 0.001
    damage_thresh = 30.0
    health = 100.0
    angulardamping = 0.01
    maxangularvel = 50.0
    model_id = None
    noshadow = 0

    ##
    ## Implementation
    ##
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)
      
    def join(self, ent):
        CSimpleActor.join(self, ent)
        try:
            o = split(ent['angles'])
        except KeyError,x:
            ang = ent.get('angle', '0')
            o = ('0', ang, '0')
        self.angles = sylphis.CVector3(-pi * float(o[0]) / 180, pi * float(o[1]) / 180, pi * float(o[2]) / 180)
        self.model = ent.get('model', '')
        try:
            self.shader = ent['shader']
        except KeyError, x:
            self.shader = self.model
        
        #CSimpleActor.start(self)
        sm = self.mScene

        m = manage(sylphis.CSceneMD3, sm)
        
        worldmodel = self.model_id
        if worldmodel is None:
            ext = self.model[-3:]
            if ext == 'md3':
                model = CEngine.models.registerMD3Model(self.model)
            else:
                model = CEngine.models.registerSMFModel(self.model)
        else:
            model = sm.getWorldMapModel(int(worldmodel))

        m.setModel(model)
        m.setPosition(self.origin)
        m.yaw(self.angles.y)
        m.roll(self.angles.x)

        if self.noshadow:
            m.setFlags(sylphis.CSceneModel.NOSHADOW)

        self.node = m
        self.frame = 0

        rigidbody = self.rigidbody
        
        print "RIGID:",rigidbody
        
        if rigidbody != 5:
        	    return

        if rigidbody == 1:
            self.body = manage(sylphis.CRigidBox, 0, self.node, sylphis.CVector3(20, 50, 20))
        elif rigidbody == 2:
            self.body = manage(sylphis.CRigidSphere, 0, self.node, 20)
        elif rigidbody == 3:
            self.body = manage(sylphis.CRigidCylinder, 0, self.node)
        elif rigidbody == 4:
            self.body = manage(sylphis.CRigidComplex, 0, self.node)
            self.body.addBox(.1, 80.0, 9.0, 80.0)
            v = sylphis.CVector3(0.0,0,0.0)
            v.swapHand()
            self.body.translateLast(v)
            self.body.addLastMass()
            
            self.body.addBox(.01, 10.0, 40.0, 10.0)
            v = sylphis.CVector3(-15.0, 15.0, -12.5)
            v.swapHand()
            self.body.translateLast(v)
            self.body.addLastMass()

            self.body.addBox(.01, 10.0, 40.0, 10.0)
            v = sylphis.CVector3(15.0, 15.0, -12.5)
            v.swapHand()
            self.body.translateLast(v)
            self.body.addLastMass()

            self.body.addBox(.01, 10.0, 40.0, 10.0)
            v = sylphis.CVector3(15.0, -15.0, -12.5)
            v.swapHand()
            self.body.translateLast(v)
            self.body.addLastMass()

            self.body.addBox(.01, 10.0, 40.0, 10.0)
            v = sylphis.CVector3(-15.0, -15.0, -12.5)
            v.swapHand()
            self.body.translateLast(v)
            self.body.addLastMass()
            
            self.body.fixate()
        elif rigidbody == 5:
            self.body = manage(sylphis.CRigidBodyStatic, 0, self.node)
            # Set the position now
            self.body.setPosition(self.node.getPosition())
            self.body.setOrientation(self.node.getOrientation())
            self.body.setMeshData(model)
        else:
            return

        if self.rigidflags & 1:
            self.body.makeImmovable()
        
        self.body.setMass(self.mass)
        self.body.fitToModel(self.node)
        self.body.setListener(self)
        self.body.setPosition(self.node.getPosition())
        self.body.setOrientation(self.node.getOrientation())
        self.body.setDamping(self.damping)
        self.body.setAngDamping(self.angulardamping)
        self.body.setMaxAngularVelocity(self.maxangularvel)
        self.body.setDamageFactor(self.damage_factor)
            
        CPhysicsEnv.registerRigidBody(self.body)

    def applyDamage(self, damage):
        if damage > self.damage_thresh:
            self.health -= damage - self.damage_thresh
            print "Health :", self.health
            if self.health <= 0:
                self.body.setSleep(False)
                self.body.addForce(sylphis.CVector3(0,100,0))
                self.remove()

    def handleEvent(self, *args):
        #print "CMiscModel Event", args
        if args[0] is self.EVENT_COLLISION:
            if args[1] is None:
                return
            damage = getDamage(self, args[1])
            self.applyDamage(damage)
            

