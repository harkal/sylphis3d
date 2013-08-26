import sys, sylphis, projectile
from string import *
from simpleactor import *

class CPlayer(CSimpleActor):
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)
        self.camera = sylphis.CCamera(self.mContainer.getSceneManager())
        self.camera.setFov(90.0)
        self.body = sylphis.CRigidBody()
        self.body.mBBox.setBounds(sylphis.CVector3(-15, -49, -15), sylphis.CVector3(15,5,15))
        self.body.setDisabled(1) # We handle our movement "manualy"
        self.body.setListener(self)
        self.omove = sylphis.CActorMover(self.mContainer.getSceneManager())
        self.mContainer.getSceneManager().registerRigidBody(self.body);

        self.lastFire = 0.0
        self.smoothing = 0.0
        self.lastY = 0.0
        print "CPlayer init"

    def join(self, ent):
        self.defs = ent
        o = split(ent['origin'])
        self.origin = sylphis.CVector3(float(o[0]), float(o[2]), -float(o[1]))
        self.origin.y += 27
        self.body.mPosition = self.origin
        self.camera.setPosition(self.origin)

    def updateCamera(self, timeDelta):
        if self.body.isOffGround() == 0:
            self.smoothing += self.body.mPosition.y - self.lastY
            if self.smoothing > 0.0:
                self.smoothing -= self.smoothing * timeDelta * 10.0
            else:
                self.smoothing = 0.0

        self.lastY = self.body.mPosition.y

        self.camera.setPosition(self.body.mPosition)
        self.camera.getPosition().y -= self.smoothing

    def fire(self):
        if(self.lastFire + 0.2 > self.frameTime):
            return
        self.lastFire = self.frameTime
        p = projectile.CProjectile(self.mCont)
        v = sylphis.CVector3(0,200,-500)
        v = self.camera.getOrientation() * v
        p.join({'position':self.camera.getPosition(), 'velocity':v})
        self.mCont.actors.append(p)
        
    def update(self, timeDelta):
        e = self.mContainer.getEngine()
        self.frameTime = e.getFrameTime()
        i = e.getInput()
        dx = i.getPointerDeltaX()
        dy = i.getPointerDeltaY()

        #p = self.camera.getPosition()
        #print p.x, p.y, p.z

        self.camera.yawFixed(-float(dx) / 300)
        self.camera.pitch(-float(dy) / 300)
        self.body.mOrientation = self.camera.getOrientation()

        if i.isPressed(sylphis.kMOUSELEFT):
            self.fire()

        action = 0
        if i.isPressed(sylphis.kW):
            action |= sylphis.ACTION_FORWARD
        if i.isPressed(sylphis.kS):
            action |= sylphis.ACTION_BACKWARD
        if i.isPressed(sylphis.kA):
            action |= sylphis.ACTION_MOVE_LEFT
        if i.isPressed(sylphis.kD):
            action |= sylphis.ACTION_MOVE_RIGHT
        if i.isPressed(sylphis.kSPACE):
            action |= sylphis.ACTION_JUMP
            if self.body.isOffGround() == 0:
                self.body.mVelocity.y = 300

        mr = sylphis.moveresult_t()
        self.omove.playerMove(mr, self.body, action, timeDelta)
        self.body.mPosition = mr.end
        self.updateCamera(timeDelta)

        self.camera.update(timeDelta, 1)
        

