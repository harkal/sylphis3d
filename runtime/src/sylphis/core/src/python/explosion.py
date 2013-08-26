import sys, sylphis
from util import manage
from string import *
from simpleactor import *

class CExplosion(CSimpleActor):
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)

    def join(self, ent):
        self.eng = self.mContainer.getEngine()

        sm = self.mCont.getSceneManager()
        sl = manage(sylphis.CLight, sm)
        sl.setPosition(0, 0, 0)
        self.radius = 10.0
        sl.setRadius(self.radius)
        sl.setColor(sylphis.CVector3(1.0, 0.9, 0.6));
        sl.setPosition(ent['position'])
        sm.getRootSceneNode().addChild(sl)
        sm.getRootSceneNode().addChild(sl)
        self.node = sl
        
        self.eng.shaders.loadAll()

        self.age = 0.0

    def part(self):
        sm = self.mCont.getSceneManager()
        sm.getRootSceneNode().removeChild(self.node)

    def update(self, timeDelta):
        self.age += timeDelta
        if self.age < 0.15:
            self.radius += 1000.0 * timeDelta
            if self.radius > 200.0:
                self.radius = 200.0
        if self.age > 0.3:
            self.radius -= 400.0 * timeDelta
        if self.radius < 0.0:
            self.mCont.actors.remove(self)

        self.node.setRadius(self.radius)
        
            
        
