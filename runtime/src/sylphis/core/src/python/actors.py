
class CActors:
    def __init__(self, container):
        self.actors = []
        self.namedActors = {}
        
    def append(self, a):
        self.actors.append(a)
        try:
            self.namedActors[a.name] = a
        except AttributeError:
            pass

    def remove(self, a):
        a.part()
        self.actors.remove(a)
        try:
            self.namedActors[a.name] = a
        except AttributeError:
            pass
        
    def update(self, timeDelta):
        for a in self.actors:
            a.update(timeDelta)
