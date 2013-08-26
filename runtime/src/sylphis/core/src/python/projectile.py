import sys, sylphis
from util import manage
from string import *
from simpleactor import *
from explosion import *

class CProjectile(CSimpleActor):
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)

    def handleEvent(self, eType, *args):
        if eType == 10:
            self.node.setPosition(args[0], args[1], args[2])

    def join(self, ent):
        e = self.mContainer.getEngine()
        md2 = e.renderables.registerMD2("models/grenade.md2")
        sm = self.mCont.getSceneManager()
        
        m = manage(sylphis.CRenderableMultiMesh, sm)
        m.setPosition(ent['position'])
        m.addMesh(md2, "textures/grenade")
        self.node = m
        
        sm.getRootSceneNode().addChild(self.node)
        e.shaders.loadAll()
        self.body = manage(sylphis.CRigidBody)
        self.body.setListener(self)
        self.body.mPosition = self.node.getPosition()
        self.body.mVelocity = ent['velocity'] 
        self.body.mBBox.setBounds(sylphis.CVector3(-3, -3, -3), sylphis.CVector3(3,3,3))
        sm.registerRigidBody(self.body)

        self.age = 0.0

    def part(self):
        sm = self.mCont.getSceneManager()
        sm.unregisterRigidBody(self.body)
        sm.getRootSceneNode().removeChild(self.node)

    def explode(self):
        e = CExplosion(self.mCont)
        e.join({'position':self.body.mPosition})
        self.mCont.actors.append(e)

    def update(self, timeDelta):
        self.age += timeDelta
        if self.age > 5.0:
            self.explode()
            self.mCont.actors.remove(self)

        t = self.body.mVelocity.length() / 100
        if t < 0.5:
            return
        self.node.yaw(t * timeDelta)
        self.node.roll(t * timeDelta)
    
