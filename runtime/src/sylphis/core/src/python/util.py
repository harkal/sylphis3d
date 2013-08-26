
def manage_del(self):
    #print "Dereferencing", self, self.getReferences()
    self.decReferences()

def manage(m, *args):
    c = m
    c.__del__ = manage_del
    n = c(*args)
    n.incReferences()
    n.incReferences()
    #print "Referencing", n, n.getReferences()
    return n
