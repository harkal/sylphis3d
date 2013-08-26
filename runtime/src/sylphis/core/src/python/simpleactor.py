
import sylphis

class CSimpleActor(sylphis.CActor):
    def __init__(self, *args):
        sylphis.CActor.__init__(self, *args)
        self.self = self;
        self.mCont = args[0]

    def handleEvent(self, eType, *args):
        pass
        
    def join(self, ent):
        self.defs = ent
        try:
            self.name = ent['targetname']
        except KeyError, x:
            pass
        
    def update(self, timeDelta):
        pass

