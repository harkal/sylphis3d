import sys, sylphis, math
from string import *
from simpleactor import *

class CLight(CSimpleActor):
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)
      
    def join(self, ent):
        CSimpleActor.join(self, ent)
        o = split(ent['origin'])
        self.origin = sylphis.CVector3(float(o[0]), float(o[2]), -float(o[1]))
        o = split(ent['_color'])
        self.color = sylphis.CVector3(float(o[0]), float(o[1]), float(o[2]))
        self.radius = float(ent['light'])

        try:
            self.type = int(ent['type'])
        except KeyError, x:
            print "WARNING : CLight property '" + str(x) + "' not defined"
            self.type = 0

        try:
            self.freq = float(ent['freq'])
            self.phase = float(ent['phase'])
        except KeyError, x:
            print "WARNING : CLight property '" + str(x) + "' not defined"
            self.freq = 1.0
            self.phase = 0.0

        sm = self.mContainer.getSceneManager()
        sl = sylphis.CLight(sm)
        sl.setPosition(self.origin)
        sl.setColor(self.color)
        sl.setIsShadowCaster(ent['shadows'] == '1')
        sl.setRadius(self.radius)
        sl.setType(self.type)
        sl.setFreq(self.freq)
        sl.setPhase(self.phase)

        try:
            self.filter = ent['filter']
            self.filterMap = sm.getEngine().textures.registerCubeMap(self.filter)
            sl.setFilter(self.filterMap)
        except KeyError, x:
            self.filter = "nofilter"

        try:
            o = split(ent['angles'])
            pi = 3.14159
            self.angles = sylphis.CVector3(-pi * float(o[0]) / 180, pi * float(o[1]) / 180, pi * float(o[2]) / 180)

            sl.yaw(self.angles.y)
            sl.roll(self.angles.x)
 
            print self.angles.x, self.angles.y, self.angles.z
        except KeyError, x:
            pass

        sm.getRootSceneNode().addChild(sl)
        self.node = sl  # TENENTES!!!!
        self.time = 0
        
    def update(self, timeDelta):
        if self.filter != "nofilter":
            return
        self.time += timeDelta;
        self.node.setPosition(self.origin + sylphis.CVector3(20 * math.sin(self.time * 1.8),math.fabs(15 * math.sin(self.time)), 30 * math.sin(self.time)))
        pass
        
