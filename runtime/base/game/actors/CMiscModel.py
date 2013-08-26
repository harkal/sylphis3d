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
            

