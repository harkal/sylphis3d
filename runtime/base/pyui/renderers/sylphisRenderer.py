import sys
import sylphis

import pyui

from pyui.renderer3d import Renderer3DBase
from pyui.desktop import getDesktop

def floatColor(color):
    return (float(color[0]) / 256, float(color[1]) / 256,float(color[2]) / 256,float(color[3]) / 256)

class SylphisRenderer(Renderer3DBase, sylphis.CPyInputListener):

    name = "Sylphis"

    def __init__(self, w, h, fullscreen, title):
        sylphis.CPyInputListener.__init__(self)
        self.self = self
        Renderer3DBase.__init__(self, w, h, fullscreen, title)
        self.frame = 0
        self.last = 0 #time.time()
        self.width = w
        self.height = h
        self.fontId = 50000
        self.fonts = {}
        self.textures = {}
        self.mouseX = 0
        self.mouseY = 0
        self.active = 0

        global input
        input = CEngine.getInput()
        
        global rend
        rend = CEngine.renderer
        
        global efonts
        efonts = CEngine.fonts

        global materials
        materials = CEngine.mMaterials

        self.pointer = materials.registerMaterial("gui/pointer")
        
        input.addListener(self)
        
        pyui.locals.K_RETURN    = sylphis.kENTER
        pyui.locals.K_SHIFT     = sylphis.kLEFTSHIFT
        #pyui.locals.K_CONTROL   = 306
        #pyui.locals.K_ALT       = 308

        #pyui.locals.K_PAGEUP    = 280
        #pyui.locals.K_PAGEDOWN  = 281
        #pyui.locals.K_END       = 279
        #pyui.locals.K_HOME      = 278

        pyui.locals.K_LEFT      = sylphis.kLARROW
        pyui.locals.K_UP        = sylphis.kUARROW
        pyui.locals.K_RIGHT     = sylphis.kRARROW
        pyui.locals.K_DOWN      = sylphis.kDARROW

        #pyui.locals.K_INSERT    = 277
        #pyui.locals.K_DELETE    = 127

        pyui.locals.K_PAD0      = 256
        pyui.locals.K_PAD1      = 257
        pyui.locals.K_PAD2      = 258
        pyui.locals.K_PAD3      = 259
        pyui.locals.K_PAD4      = 260
        pyui.locals.K_PAD5      = 261
        pyui.locals.K_PAD6      = 262
        pyui.locals.K_PAD7      = 263
        pyui.locals.K_PAD8      = 264
        pyui.locals.K_PAD9      = 265

        pyui.locals.K_PADDIVIDE = 267
        pyui.locals.K_PADTIMES  = 268
        pyui.locals.K_PADMINUS  = 269
        pyui.locals.K_PADPLUS   = 270
        pyui.locals.K_PADENTER  = 271
        pyui.locals.K_PADDECIMAL= 266

        pyui.locals.K_F1        = 282
        pyui.locals.K_F2        = 283
        pyui.locals.K_F3        = 284
        pyui.locals.K_F4        = 285
        pyui.locals.K_F5        = 286
        pyui.locals.K_F6        = 287
        pyui.locals.K_F7        = 288
        pyui.locals.K_F8        = 289
        pyui.locals.K_F9        = 290
        pyui.locals.K_F10       = 291
        pyui.locals.K_F11       = 292
        pyui.locals.K_F12       = 293

        #self.keyMap = {
        #    100: pyui.locals.K_LEFT,
        #    101: pyui.locals.K_UP,
        #    102: pyui.locals.K_RIGHT,
        #    103: pyui.locals.K_DOWN
        #    }

        self.drawBackMethod = self.clear
        
    def activate(self):
        self.active = 1
    	
    def deactivate(self):
        self.active = 0
         
    def onKeyPressed(self, key):
        #print "pressed ", key, sylphis.kENTER
        if not self.active:
            if key == sylphis.kF9:
                self.activate()
            return
         
        if key == sylphis.kF9:
            self.deactivate()
                   
        x = self.mouseX
        y = self.mouseY
        if key == sylphis.kMOUSELEFT:
            getDesktop().postUserEvent(pyui.locals.LMOUSEBUTTONDOWN, x, y)
            return
        if key == sylphis.kMOUSERIGHT:
            getDesktop().postUserEvent(pyui.locals.RMOUSEBUTTONDOWN, x, y)
            return
        if key < 0:
            getDesktop().postUserEvent(pyui.locals.CHAR, 0, 0, chr(-key), self.getModifiers() )
        if key > 0 and key < 46:
            getDesktop().postUserEvent(pyui.locals.KEYDOWN, 0, 0, key, self.getModifiers() )

        
    def onKeyReleased(self, key):
        if not self.active:
            return
        x = self.mouseX
        y = self.mouseY
        if key == sylphis.kMOUSELEFT:
            getDesktop().postUserEvent(pyui.locals.LMOUSEBUTTONUP, x, y)
            return
        if key == sylphis.kMOUSERIGHT:
            getDesktop().postUserEvent(pyui.locals.RMOUSEBUTTONUP, x, y)
            return
        getDesktop().postUserEvent(pyui.locals.KEYUP, 0, 0, key, self.getModifiers())
        
    def setScissor(self, rect):
        rend.setScissor(rect[0], rect[1], rect[2], rect[3])


	###############################################################################
	### Draw Primatives functions
	###############################################################################

    def drawRect(self, color, rect):
        """Fills a rectangle with the specified color."""
        rend.GLDisableTexture2D()
        rend.GLBeginQuad()
        color = floatColor(color)
        rend.GLColor4(color[0] , color[1] , color[2], color[3])
        
        rend.GLVertex2(rect[0], rect[1])
        rend.GLVertex2(rect[0] + rect[2], rect[1])
        rend.GLVertex2(rect[0] + rect[2], rect[1] + rect[3])
        rend.GLVertex2(rect[0], rect[1] + rect[3])
        rend.GLEnd()

    def drawText(self, text, pos, color, font = None):
        """Draws the text on the screen in the specified position.
        """
        self.do_text(text, (pos[0], pos[1]), color, font)
        
    def drawGradient(self, rect, c1, c2, c3, c4):
        """Draws a gradient rectangle"""
        c1 = floatColor(c1)
        c2 = floatColor(c2)
        c3 = floatColor(c3)
        c4 = floatColor(c4)
        rend.GLBeginQuad()
        rend.GLColor4( c1[0], c1[1], c1[2], c1[3] )
        rend.GLVertex2(rect[0], rect[1])                        # top left
        rend.GLColor4( c2[0], c2[1], c2[2], c2[3] )        
        rend.GLVertex2(rect[0] + rect[2], rect[1])              # top right
        rend.GLColor4( c4[0], c4[1], c4[2], c4[3] )
        rend.GLVertex2(rect[0] + rect[2], rect[1] + rect[3])    # bottom right
        rend.GLColor4( c3[0], c3[1], c3[2], c3[3] )
        rend.GLVertex2(rect[0], rect[1] + rect[3])              # bottom left
        rend.GLEnd()

        
    def drawLine(self, x1, y1, x2, y2, color):
        """Draws a line"""
        color = floatColor(color)
        rend.GLBeginLine()
        rend.GLColor4( color[0], color[1], color[2], color[3] )
        rend.GLVertex2(x1, y1)
        rend.GLVertex2(x2, y2)
        rend.GLEnd()
        
    def drawImage(self, rect, filename, pieceRect = None):
        """Draws an image at a position."""
        #rend.GLEnableTexture2D()
        #print filename
        #shader = self.loadTexture(filename)
        shader = filename
        rend.drawPic(rect[0], rect[1], rect[2], rect[3], shader)
        rend.renderOverlay2D()

##        glColor4ub( 255, 255, 255, 255 )
##        glEnable(GL_TEXTURE_2D)
##        glBindTexture( GL_TEXTURE_2D, texture)
##
##        glBegin(GL_QUADS)
##        glTexCoord2f(textureCoords[0][0], textureCoords[0][1])
##        glVertex2i( rect[0], rect[1])
##        glTexCoord2f(textureCoords[1][0], textureCoords[1][1])
##        glVertex2i( rect[0] + rect[2], rect[1])
##        glTexCoord2f(textureCoords[2][0], textureCoords[2][1])
##        glVertex2i( rect[0] + rect[2], rect[1] + rect[3])
##        glTexCoord2f(textureCoords[3][0], textureCoords[3][1])
##        glVertex2i( rect[0], rect[1] + rect[3])
##        glEnd()
##
##        glDisable(GL_TEXTURE_2D)
        pass

    def drawImageRotated(self, rect, filename, rotDegrees=0, textureEffect=0):

        """Draws an image at a position."""
        pass
##
##        if textureEffect == TEXTURE_ROTATE_90:
##            textureCoords = [[0.0,0.0],[0.0,1.0],[1.0,1.0],[1.0,0.0]]
##        elif textureEffect == TEXTURE_ROTATE_180:
##            textureCoords = [[1.0,0.0],[0.0,0.0],[0.0,1.0],[1.0,1.0]]
##        elif textureEffect == TEXTURE_ROTATE_270:       
##            textureCoords = [[1.0,1.0],[1.0,0.0],[0.0,0.0],[0.0,1.0]]
##        elif textureEffect == TEXTURE_MIRROR_H:
##            textureCoords = [[1.0,1.0],[0.0,1.0],[0.0,0.0],[1.0,0.0]]
##        elif textureEffect == TEXTURE_MIRROR_V:
##            textureCoords = [[0.0,0.0],[1.0,0.0],[1.0,1.0],[0.0,1.0]]
##        else:
##            textureCoords = [[0.0,1.0],[1.0,1.0],[1.0,0.0],[0.0,0.0]]
##
##        if not self.textures.has_key(filename):
##            self.loadTexture(filename)
##
##        texture = self.textures[filename]
##
##        glColor4ub( 255, 255, 255, 255 )
##        glEnable(GL_TEXTURE_2D)
##        glBindTexture( GL_TEXTURE_2D, texture)
##
##        halfwidth = rect[2] / 2
##        halfheight = rect[3] / 2
##
##        glPushMatrix()
##        glTranslate(rect[0] + (halfwidth), rect[1] + (halfheight), 0.0)
##        glRotate(rotationDegrees, 0.0, 0.0, 1.0)      # Rotate
##
##        glBegin(GL_QUADS)
##        glTexCoord2f(textureCoords[0][0], textureCoords[0][1])
##        glVertex2i( -halfwidth, -halfheight)        
##        glTexCoord2f(textureCoords[1][0], textureCoords[1][1])
##        glVertex2i( halfwidth, -halfheight)
##        glTexCoord2f(textureCoords[2][0], textureCoords[2][1])
##        glVertex2i( halfwidth, halfheight)
##        glTexCoord2f(textureCoords[3][0], textureCoords[3][1])
##        glVertex2i( -halfwidth, halfheight)
##
##        glEnd()
##        glPopMatrix()
##
##        glDisable(GL_TEXTURE_2D)
        
    def loadImage(self, filename, label = None):
        if not filename:
            return
        self.loadTexture(filename, label)

    def setClipping(self, rect = None):
        """set the clipping rectangle for the main screen. defaults to clearing the clipping rectangle.
        NOTE: isn't working..."""
        return
##        if rect:
##            offsets = glGetIntegerv( GL_MODELVIEW_MATRIX )
##            corrected = (offsets[3][0] + rect[0], getDesktop().height - offsets[3][1] - rect[3] - rect[1], rect[2], rect[3])
##            self.clip_stack.append( corrected )
##        elif len( self.clip_stack ):
##            self.clip_stack = self.clip_stack[0:-1]
##
##        if len( self.clip_stack ) and self.clip_stack[-1][2] > 0 and self.clip_stack[-1][3] > 0:
##            glEnable(GL_SCISSOR_TEST)
##            apply( glScissor, self.clip_stack[-1] )
##        else:
##            glDisable(GL_SCISSOR_TEST)
##        pass
        

    ###############################################################################
    ### methods to be implemented by GL wrappers
    ###############################################################################
        
    def draw(self, windows):
        """ The main draw func
        """
        #self.setup2D()

        for i in xrange(len(windows)-1, -1, -1):
            w = windows[i]
            w.dirty = 1
            self.setWindowOrigin(w.posX, w.posY)
            w.drawWindow(self)
            self.unsetWindowOrigin()

        #self.drawText("V", (self.mouseX, self.mouseY), (255,255,255,255))
        #self.teardown2D()
        
    def update(self):
        pass

    def getModifiers(self):
        mods = 0
        if input.isPressed(sylphis.kRIGHTSHIFT) or input.isPressed(sylphis.kLEFTSHIFT):
            mods |= pyui.locals.MOD_SHIFT
        return mods

    def quit(self):
        return

    def clear(self):
#        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
#         self.clip_stack = []
        return


    def packColor(self, r, g, b, a = 255):
        """pack the rgb triplet into a color
        """
        return (r, g, b, a)

    def dirtyCollidingWindows(self, inRect):
        """Dont do dirty rects in 3D"""
        return

    def setup2D(self):
        """Setup everything on the opengl Stack to draw in 2D in a way that can be torn down later.
        """
        rend.enter2DMode();
##        glEnable(GL_SCISSOR_TEST)
##
##        glEnable(GL_BLEND)
##        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)

    def teardown2D(self):
        """tear down the 2D stuff to revert to the previous state.
        """
        rend.exit2DMode()

    def getScreenSize(self):
        """ Returns (width, height) of the scene viewport
        """
        return (self.width, self.height)

    def loadTexture(self, filename, label = None):
        return shaders.addShader(filename)
    
    def setWindowOrigin(self, winX, winY ):
        rend.beginObjectTransformations()
        rend.translate(sylphis.CVector3(winX, winY, 0))

    def unsetWindowOrigin(self):
        rend.endObjectTransformations()
        
    ######################################################
    ## 2D drawing functions.
    ##
    ## These assume that we are in a 2D state as setup by the
    ## setup2D() function.
    ##
    ######################################################

    def do_text(self, text, position, color, font ):
        """Draw some text to the screen using a bitmapped font"""
        if len(text) < 1:
            return

        if not font:
            font = pyui.desktop.getTheme().defaultFont

        color = floatColor(color)
        rend.GLColor4(color[0], color[1], color[2], color[3]);
        font.renderString(text, position[0], position[1])

    def getTextSize(self, text, font = None):
        """gets the width and height of a piece of text."""
        if not font:
            font = pyui.desktop.getTheme().defaultFont
        size = font.stringSize(text)
        
        return size
      
    def createFont(self, fontName, fontSize, flags):
        print "Load font :", fontName

        if fontName == 'comic sans ms':
            fontName = 'times'

        if fontName == 'microsoft sans serif':
        	    fontName = 'times'
        try:
        	    f = efonts.registerBitmapFont("fonts/" + fontName + ".ttf", fontSize)
        	    return f
        except IOError, e:
        	    print fontName, "not found"
        	    
        
##        handle = self.fontId
##        self.fontId += 256
##
##        props = {"name":fontName, "height":(int)(fontSize*1.2), "charset":0, "weight":1, "pitch and family":18}
##        if flags & pyui.locals.ITALIC:
##            props["italic"] = 1
##        if flags & pyui.locals.UNDERLINE:
##            props["underline"] = 1
##        if flags & pyui.locals.BOLD:
##            props["weight"] = 128
##            
##        pf = win32ui.CreateFont( props )
##        hdc = wglGetCurrentDC()
##        pdc = win32ui.CreateDCFromHandle(hdc)
##
##        
##        old = pdc.SelectObject(pf)
##        result = wglUseFontBitmaps(hdc , 0, 255, handle)
##        if not result:
##            print "ERROR!"
##        pdc.SelectObject(old)
##
##        self.fonts[handle] = (fontName, fontSize, flags)
##        del pf
##        del pdc
##
##        return handle
    def run(self, callback=None):
        if not self.active:
            return
        self.mouseX += input.getPointerDeltaX()
        self.mouseY += input.getPointerDeltaY()
        if self.mouseX < 0:
            self.mouseX = 0
        if self.mouseY < 0:
            self.mouseY = 0
        if self.mouseX > self.width:
            self.mouseX = self.width    
        if self.mouseY > self.height:
            self.mouseY = self.height    
            
        getDesktop().postUserEvent(pyui.locals.MOUSEMOVE, self.mouseX,self.mouseY)
        getDesktop().draw()
        getDesktop().update()

        self.drawImage((self.mouseX, self.mouseY, 32, 32), self.pointer)

  
