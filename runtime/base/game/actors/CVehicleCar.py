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
import sys, sylphis
from sylphis import CVector3
from string import *
from CVehicle import *
from util import manage
import pyui

class CVehicleCar(CVehicle):
    __classname__ = 'base_vehicle_car'
    __desc__ = 'Base car vehicle class'
    __actortype__ = 'e'
    def __init__(self, *args):
        CVehicle.__init__(self, *args)

    length = 100.0
    width = 50.0
    height = 40.0
    steer = 0.0
    def join(self, ent):
        CVehicle.join(self, ent)
        self.mScene.getRootSceneNode().addChild(self.carModel)
        for i in self.tireModels:
            self.mScene.getRootSceneNode().addChild(i)
            
        self.node = self.carModel

    def initBody(self):
        self.body = manage(sylphis.CRigidBox, 0, self.carModel, CVector3(self.width,self.height, self.length*0.9))
        self.body.setMass(50.0)
        #self.body.fitToModel(self.carModel)
        self.body.setListener(self)
        #self.body.applyGravity(False)
        self.body.setPosition(self.origin)
        self.body.setDamping(0.0000)
        self.body.setAngDamping(2.0)
        self.body.setMaxAngularVelocity(100.0)
        self.body.setDamageFactor(0.001)
        CPhysicsEnv.registerRigidBody(self.body)

        self.tireBody = []
        for i in range(4):
            self.tireBody.append(manage(sylphis.CRigidSphere, 0, self.tireModels[i], 25))
            self.tireBody[i].setMass(2.2)
            self.tireBody[i].fitToModel(self.tireModels[i])
            self.tireBody[i].setListener(self)
            self.tireBody[i].setDamping(0.0001)
            self.tireBody[i].setAngDamping(0.0)
            self.tireBody[i].setMaxAngularVelocity(200.0)
            self.tireBody[i].setDamageFactor(0.001)
            CPhysicsEnv.registerRigidBody(self.tireBody[i])

        pos = self.origin + CVector3(self.width * 0.45, -self.height * 0.4, -self.length * 0.45)
        self.tireBody[0].setPosition(pos)
        pos = self.origin + CVector3(-self.width * 0.45, -self.height * 0.4, -self.length * 0.45)
        self.tireBody[1].setPosition(pos)
        pos = self.origin + CVector3(self.width * 0.45, -self.height * 0.4, self.length * 0.45)
        self.tireBody[2].setPosition(pos)
        pos = self.origin + CVector3(-self.width * 0.45, -self.height * 0.4, self.length * 0.45)
        self.tireBody[3].setPosition(pos)

        joint = []
        #for i in range(2):
        #    joint.append(manage(sylphis.CJointHinge, 0, 0))
            #joint[i].setAxis1(CVector3(0,1,0))
        #    joint[i].setAxis(CVector3(1,0,0))
            #joint[i].setLoStop(0.0)
            #joint[i].setHiStop(0.0)
        #    joint[i].attach(self.body, self.tireBody[i])
        #    joint[i].setAnchor(self.tireBody[i].getPosition())
            
        for i in range(4):
            joint.append(manage(sylphis.CJointDoubleHinge, 0, 0))
            joint[i].attach(self.body, self.tireBody[i])
            joint[i].setAnchor(self.tireBody[i].getPosition())
            joint[i].setAxis1(CVector3(0,1,0))
            if i == 0:
                joint[i].setAxis2(CVector3(1,0,0))
            else:
                joint[i].setAxis2(CVector3(-1,0,0))
            
            joint[i].setSuspensionCFM(0.00005)
            joint[i].setSuspensionERP(0.00012)
            
            
        for i in range(2,4):            
            joint[i].setLoStop(0.0)
            joint[i].setHiStop(0.0)
            
        for i in range(2):            
            joint[i].setLoStop(-0.6)
            joint[i].setHiStop(0.6)
            
        for i in range(4):
            self.tireBody[i].setSleep(False)

            
        self.joint = joint
        
    def setSteer(self, angle):
        angle = max(angle, -0.7)
        angle = min(angle, 0.7)
        for i in range(2):
            d = angle - self.joint[i].getWeelAngle()
            d *= 140.0
            self.joint[i].setWeelVelocity(d)
            self.joint[i].setWeelMaxForce(18000.0)
            
        return angle
        
    def gas(self, g):
        self.joint[0].setMotorVelocity(g)
        self.joint[0].setMotorMaxForce(15000.2)
        self.joint[1].setMotorVelocity(-g)
        self.joint[1].setMotorMaxForce(15000.2)
        self.joint[2].setMotorVelocity(-g)
        self.joint[2].setMotorMaxForce(15000.2)
        self.joint[3].setMotorVelocity(-g)
        self.joint[3].setMotorMaxForce(15000.2)
        if g == 0.0:
            for i in range(4):
                self.joint[i].setMotorMaxForce(0.0)
            
        
    def update(self, timeDelta):
        i = sylphis.CEngine.getSingleton().getInput()

        if i.isPressed(sylphis.kR):
            self.gas(200.0)
        elif i.isPressed(sylphis.kE):
            self.gas(-200.0)
        else:
            self.gas(0.0)
            
        if i.isPressed(sylphis.kJ):
            self.steer -= 1.0 * timeDelta
        elif i.isPressed(sylphis.kK):
            self.steer += 1.0 * timeDelta
        else:
            self.steer = self.steer * 0.5
            
        self.steer = self.setSteer(self.steer)

    def precache(self):
        self.carModel = manage(sylphis.CSceneMD3, self.mScene)
        m = CEngine.models.registerMD3Model('models/jeep.md3')
        #m = CEngine.models.registerSMFModel('models/car.smf')
        self.carModel.setModel(m)

        m = CEngine.models.registerMD3Model('models/tire.md3')
        #m = CEngine.models.registerSMFModel('models/tire.smf')
        self.tireModels = []
        for i in range(4):
            self.tireModels.append(manage(sylphis.CSceneMD3, self.mScene))
            self.tireModels[i].setModel(m)
            

            
