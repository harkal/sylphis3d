
import sys, sylphis
from string import *
from simpleactor import *

class CInfoActor(CSimpleActor):
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)
      
    def join(self, ent):
        self.defs = ent
        self.type = ent['classname']
        o = split(ent['origin'])
        self.origin = sylphis.CVector3(float(o[0]), float(o[2]), -float(o[1]))
