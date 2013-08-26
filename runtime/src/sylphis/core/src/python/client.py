
import sylphis
from actors import *
from light import *

class CClient(sylphis.CClient):
    def __init__(self, *args):
        sylphis.CClient.__init__(self, *args)
        self.self = self;
        self.actors = CActors(self)
        self.player = None
        
    def startup(self):
        sylphis.CClient.startup_c(self)
        #self.getSceneManager().loadWorldMap("../tmpquark/maps/newmap.bsp")
        #self.getEngine().models.registerModel("models/skeleton.model")
        self.getSceneManager().loadWorldMap("h:/tmp/malakia.sbsp");

    def update(self):
        sylphis.CClient.update_c(self)
        e = self.getEngine()
        dt = e.getFrameLength()
        self.actors.update(dt)

        c = self.player.camera
        e.renderer.useCamera(c)

        self.getSceneManager().update(dt)
        self.getSceneManager().render(e.renderer)

        
        
        
