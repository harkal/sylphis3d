"""
This file is part of the "Sylphis" 3D engine
Copyright (c) 2002 - 2007 by Harry Kalogirou

"Sylphis3D Game Engine" is free software; you can redistribute it and/or 
modify it under the terms of the GNU General Public License as 
published by the Free Software Foundation; either version 2 of the 
License, or (at your option) any later version, 
plus a special exception:
  Linking "Sylphis3D Game Engine" statically or dynamically with other 
modules is making a combined work based on this library. Thus, the 
terms and conditions of the GNU General Public License cover the whole 
combination. As a special exception, the copyright holders of this 
library give you permission to link this library with independent 
modules to produce an executable, regardless of the license terms of 
these independent modules, and to copy and distribute the resulting 
executable under terms of your choice, provided that you also meet, 
for each linked independent module, the terms and conditions of the 
license of that module. An independent module is a module which is 
not derived from or based on this program. If you modify this program,
you may extend this exception to your version of the library, but you 
are not obligated to do so. If you do not wish to do so, delete this 
exception statement from your version.

"Sylphis3D Game Engine" is distributed in the hope that it will be 
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Sylphis3D Game Engine"; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
"""

import sylphis
import actor
import cmdline
import mthread
import entry
import hudbase
import gc

import pyui

class CClientInput(sylphis.CPyInputListener):
    def __init__(self):
        sylphis.CPyInputListener.__init__(self)
        self.self = self
        
    def onKeyPressed(self, key):
        if key == -sylphis.kESC:
            shutdown()
        if key == sylphis.kF1:
            entry.client.unloadMap()

    def onKeyReleased(self, key):
        pass

import math

fadeStart = 0.0
fadeSec = 1.0
fadeColorStart = 0.0
fadeColorRate = 1.0

def faderAmount():
    global fadeStart, fadeSec, fadeColorStart, fadeColorRate
    time = (CEngine.getFrameTime() - fadeStart) * fadeSec
    return fadeColorStart + fadeColorRate * time

def fadeScreen(sec, start, end):
    global fadeStart, fadeSec, fadeColorStart, fadeColorRate
    fadeStart = CEngine.getFrameTime()
    fadeSec = 1.0 / sec
    fadeColorStart = start
    fadeColorRate = end - start

def fadeInScreen(sec):
    fadeScreen(sec, 0.0, 1.0)

def fadeOutScreen(sec):
    fadeScreen(sec, 1.0, 0.0)

class CPyClient(sylphis.CClient):
    def __init__(self, *args):
        sylphis.CClient.__init__(self, *args)
        self.self = self;
        self.player = None
        self.menuActive = 0
        self.drawFPS = 0
        self.drawPerf = 0
        self.mapLoaded = 0
        self.postEffects = {}
        

    def loadMap(self, fileName):
        self.getSceneManager().loadWorldMap(fileName)
        self.actors.start()
        c = self.player.camera
        CEngine.renderer.useCamera(c)
        CEngine.mSoundEnv.setActiveListener(self.player.slistener)
        CEngine.setRunning(True)
        self.mapLoaded = 1
        #CEngine.setFPS(30.0)
        #CEngine.renderer.setMotionBlurAmount(0.0)
                        

    def unloadMap(self):
        print "Client::unloadMap"
        if self.mapLoaded:
            print "... pausing world"
            CEngine.setRunning(False)
            print "... dectivating listener"
            CEngine.mSoundEnv.setActiveListener(None)
            print "... dectivating camera"
            CEngine.renderer.useCamera(None)
            print "... releasing player"
            self.player = None
            print "... Removing actors"
            self.actors.removeAll()
            print "... unloading CMAP file"
            self.getSceneManager().unloadWorldMap()
            print "... collecting garbage (%d)" % gc.collect()
            print "... unloading media"
            CEngine.models.freeUnused()
            #FIXME: Unload materials
            CEngine.textures.freeUnused()
            CEngine.renderables.freeUnused()
            CEngine.mSoundEnv.freeUnused()
            
            self.mapLoaded = False

    def startup(self):
        sylphis.CClient.startup_c(self)
        self.actors = actor.CActors(self)
        
        # Create the hud
        self.hud = hudbase.CHUDBase()
        
        # Register the console to the global 
        # Sylphis console
        import ui
        e = CEngine
        #e.renderer.setMotionBlurAmount(0.3)
        e.con.addConsole(ui.con)
        self.dFont = CEngine.getDefaultFont()

        # Load map fire requested by command line arguments
        map = cmdline.getParamValue('map')
        if map != None:
            self.loadMap("maps/" + map)
        else:
            pyui.activate()
           
        self.inputListener = CClientInput()
        CEngine.getInput().addListener(self.inputListener)

        self.faderShader = CEngine.mMaterials.registerMaterial('fader')
        self.glareShader = CEngine.mMaterials.registerMaterial('glare')
        self.oldFilmGlare = CEngine.mMaterials.registerMaterial('oldfilmglare')
        self.oldFilm = CEngine.mMaterials.registerMaterial('oldfilm')
        self.motionBlur = CEngine.mMaterials.registerMaterial('motionblur')
        
        self.bufferA = CEngine.mMaterials.registerMaterial('bufferA')
        self.bufferAb = CEngine.mMaterials.registerMaterial('bufferAb')

        self.timeLeft = 300.0
        
        mthread.parallelize(self.updateHudTime)
        #self.useOldFilmGlare(True)
        #self.useOldFilm(True)
        #self.useGlare(True)
        #self.useMotionBlur(True)

        #CEngine.renderer.setShadows(False)

    def updateHudTime(self):
        while self.timeLeft > 0.0:
            self.timeLeft -= mthread.sleep(1.0)
            mins = self.timeLeft // 60.0
            secs = self.timeLeft % 60.0
            self.hud.time = (mins, secs)

    def useGlare(self, b):
        if b:
            self.postEffects['glare'] = self.glareShader
        else:
            del self.postEffects['glare']

    def useOldFilm(self, b):
        if b:
            self.postEffects['oldfilm'] = self.oldFilm
        else:
            del self.postEffects['oldfilm']

    def useOldFilmGlare(self, b):
        if b:
            self.postEffects['oldfilmglare'] = self.oldFilmGlare
        else:
            del self.postEffects['oldfilmglare']

    def useMotionBlur(self, b):
        if b:
            self.postEffects['motionBlur'] = self.motionBlur
        else:
            del self.postEffects['motionBlur']
         
    def updateActors(self, dt):
#        if not pyui.isActive() and self.mapLoaded:
        if self.mapLoaded:
            self.actors.update(dt)
        
    def update(self):
        sylphis.CClient.update_c(self)
        e = CEngine
        dt = e.getFrameLength()

        if self.mapLoaded:
            self.getSceneManager().update(dt)

        e.renderer.beginFrame()

        for s in self.postEffects.values():
            e.renderer.drawPic(0, 0, 800, 600, s)
        
        #e.renderer.calcAverageLuma()    
        #e.renderer.drawPic(50, 50, 300, 300, self.bufferA, 0.0, 0.0, 1.0 + 3.0 / 64.0, 1.0 + 3.0 / 64.0)
        #e.renderer.drawPic(400, 50, 300, 300, self.bufferAb, 0.0, 0.0, 1.0 + 3.0 / 64.0, 1.0 + 3.0 / 64.0)

        #if faderAmount() < 1.0:
        #    e.renderer.drawPic(0, 0, 800, 600, self.faderShader)

        if self.mapLoaded:
            self.getSceneManager().render(e.renderer)

        e.renderer.enter2DMode()

        # Render the postprocess quads
        #e.renderer.renderOverlay2D()

        # Run and render the GUI
        pyui.run()

        if self.drawFPS:
            self._drawFPS()
            self._drawEPS()

        if self.drawPerf:
            self._drawPerformaceInfo()

##        if not pyui.isActive():
##            self.hud.render(self.player)

        e.renderer.exit2DMode()
        e.renderer.endFrame()
        

        
    def _drawFPS(self):
        s = "%4.1f FPS\n" % CEngine.getFPS()
        CEngine.renderer.GLColor4(0.0, 1.0, 0.0)
        self.dFont.renderString(s, 5, 30)

    def _drawPerformaceInfo(self):
        CEngine.renderer.GLColor4(0.5, 1.0, 0.5)
        self.dFont.renderString("\n\n", 5, 30)
        self.getSceneManager().renderPerformaceInfo()
        CEngine.renderer.renderPerformaceInfo()

    def _drawEPS(self):
        f = CEngine.getDefaultFont()
        s = "%10.1f EPS\n" % CEngine.renderer.getEPS()
        CEngine.renderer.GLColor4(0.0, 1.0, 0.0)
        self.dFont.renderString(s, 5, 40)
        

        

        
        
        
