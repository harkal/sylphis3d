# PyUI
# Copyright (C) 2001-2002 Sean C. Riley
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of version 2.1 of the GNU Lesser General Public
# License as published by the Free Software Foundation.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

import time

from pyui.desktop import getDesktop

class RendererBase:
    """Base class for renderers to implement. The renderer hands out window handles when windows
    are created, and tracks the list of draw primatives for each window. This is for 2D only.
    """
    name = "Base"
    
    def __init__(self, w, h, fullscreen, title):
        self.title = title
        self.w = w
        self.h = h
        self.fullscreen = fullscreen
        self.mustFill = 0
        self.dirtyRects = []
        self.drawList = None
        self.windows = []     # list of top level windows
        self.lastID = 1000    # ID counter to identify objects
        self.done = 0
        self.frame = 0
        self.last = 0
        self.drawBackMethod = None
        self.drawBackArgs = []

    def getScreenSize(self):
        return (self.w, self.h)
    
    def createWindow(self, title):
        pass

    def describeWindow(self, windowHandle, drawList):
        pass

    def moveToFront(self, windowHandle):
        pass

    def destroyWindow(self, windowHandle):
        pass

    def moveWindow(self, windowHandle, x, y):
        pass
    
    def addRect(self, rect):
        self.dirtyRects.append(rect)

    def draw(self, windows):
        for w in windows:
            w.drawWindow(self)

    def update(self):
        pass

    def quit(self):
        pass

    def packColor(self, r, g, b, a = 255):
        return 1

    def unpackColor(self, color):
        return (0,0,0,0)

    def run(self, callback=None):
        """This is a default way of _running_ an application using
        the current renderer.
        """

        while getDesktop() and getDesktop().running:

            self.frame = self.frame + 1
            now = time.time()
            if now - self.last >= 1:
                self.last = now
                print "FPS: %d" % self.frame
                self.frame = 0
            
            if callback:
                callback()
            else:
                getDesktop().draw()
                getDesktop().update()

    def setBackMethod(self, drawBackMethod, *args):
        """Set the method used to draw the background.
        """
        self.drawBackMethod = drawBackMethod
        self.drawBackArgs = args
        
    ####### draw primative functions ######
    
    def drawRect(self, color, rect):
        """Fills a rectangle with the specified color."""
        pass

    def drawText(self, text, pos, color, font = None):
        """Draws the text on the screen in the specified position"""
        pass

    def drawGradient(self, rect, c1, c2, c3, c4):
        """Draws a gradient rectangle"""
        pass

    def drawImage(self, rect, filename, pieceRect = None):
        """Draws an image at a position"""
        pass

    def drawImageRotated(self, rect, filename, rotDegrees, textureEffect):
        # Only implemented for GL, fallback on normal image rendering for now.
        #print "Rotation or Texture Effects unimplemented on this renderer!  Fix in pyui!"
        self.drawImage(rect, filename)

    def drawLine(self, x1, y1, x2, y2, color):
        """Draws a line"""
        pass

    def loadImage(self, filename, label = None):
        pass

    def setClipping(self, rect = None):
        """set the clipping rectangle for the main screen. defaults to clearing the clipping rectangle."""
        pass

    def drawView(self, rect, handle):
        pass
    
    def setMustFill(self):
        self.mustFill = 1

    def getMustFill(self):
        return self.mustFill

    def getTextSize(self, text, font = None):
        return (len(text) * 6, 16)

    def getImageSize(self, filename):
        """Return the width and height of the image
        """
        return (0,0)
    
    def readTimer(self):
        return time.time()

    def doesDirtyRects(self):
        return 0

    def setDrawList(self, drawList):
        self.drawList = drawList

    def setWindowTitle(self, title):
        """sets the title of the OS main window."""
        pass

    def createFont(self, face, size, flags):
        return 1

    def setup2D(self):
        """must be called in background drawing method to do 2D drawing.
        """
        pass

    def teardown2D(self):
        """must be called in background drawing method to end 2D drawing.
        """
        pass
    
