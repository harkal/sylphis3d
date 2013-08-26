import sys, sylphis, math
from string import *
from simpleactor import *

class CMiscModel(CSimpleActor):
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)
      
    def join(self, ent):
        CSimpleActor.join(self, ent)
        o = split(ent['origin'])
        self.origin = sylphis.CVector3(float(o[0]), float(o[2]), -float(o[1]))
        pi = 3.14159 # FIXME
        o = split(ent['angles'])
        self.angles = sylphis.CVector3(pi * float(o[0]) / 180, pi * float(o[1]) / 180, pi * float(o[2]) / 180)
        self.model = ent['model']
        try:
            self.shader = ent['shader']
        except KeyError, x:
            self.shader = self.model

        e = self.mContainer.getEngine()
        md2 = e.renderables.registerMD2(self.model)

        sm = self.mContainer.getSceneManager()
        m = sylphis.CRenderableMultiMesh(sm)
        m.setPosition(self.origin)
        m.addMesh(md2, self.shader)
        
        m.yaw(self.angles.y)
        m.roll(self.angles.x)

        print self.angles.x, self.angles.y, self.angles.z

        sm.getRootSceneNode().addChild(m)
        self.node = m
        self.frame = 0
        
    def update(self, timeDelta):
        self.frame += timeDelta
        self.node.setFrame(self.frame)
        
        self.node.yaw(timeDelta * 0.7)
        #self.node.setPosition(self.origin + sylphis.CVector3( 100 * math.sin(self.frame/2),0,0))
        pass
        
