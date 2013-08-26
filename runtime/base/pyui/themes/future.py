7# PyUI
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

"""Pyui Themes.
Themes are a method of customizing the drawing of widgets in a pyui GUI.

This modules keeps NO state for the drable objects - it just draws them on demand
from the widgets themselves which hold all the state.

The constants for the theme objects live in pyui/locals.py


Themes have a default font that is used for any widgets that dont specify a font.
"""

import pyui
from pyui.themeBase import ThemeBase

class FutureTheme(ThemeBase):
    """ Futuristic looking theme.
    """

    def __init__(self, renderer, fontFace="times", fontSize=17, fontFlags=0):
        ThemeBase.__init__(self, renderer, fontFace, fontSize, fontFlags)        

        # theme colors        
        self.fgColor = renderer.packColor(255,255,255)
        self.bgSelect = renderer.packColor(150,160,140)
        self.bgColor1 = renderer.packColor(150,140,150,224)
        self.bgColor2 = renderer.packColor(60,40,50,128)
        self.bgColor3 = renderer.packColor(60,50,50,128)
        self.bgMenuBar = renderer.packColor(60,60,60)
        self.bgMenuPanel = renderer.packColor(60,60,70,224)
        self.bgMenuSelect = renderer.packColor(125,120,120)
        self.black = renderer.packColor(0,0,0)
        self.white = renderer.packColor(255,255,255)
        self.shadeHi0 = renderer.packColor(255,255,255,0)
        self.shadeHi1 = renderer.packColor(255,255,255,255)
        self.shadeLo0 = renderer.packColor(0,0,0,0)
        self.shadeLo1 = renderer.packColor(0,0,0,255)
        self.buttonUpA = renderer.packColor(210,200,215)
        self.buttonUpB = renderer.packColor(150,150,160)
        self.buttonRoA = renderer.packColor(230,240,250)
        self.buttonRoB = renderer.packColor(180,190,200)
        self.buttonDnA = renderer.packColor(100,110,120)
        self.buttonDnB = renderer.packColor(180,180,200)
        self.buttonFocus = renderer.packColor(128,192,255,128)
        self.captionA = renderer.packColor(40,40,50)
        self.captionB = renderer.packColor(130,130,140)
        self.frameColor = renderer.packColor(120,120,140)
        self.shadowLight = renderer.packColor(0,0,0,64)
        self.shadowMedium = renderer.packColor(0,0,0,128)
        self.shadowDark = renderer.packColor(0,0,0,192)

    
    #####################################################################
    ###
    ###   Utility drawing functions not specific to any widgets
    ###
    #####################################################################    

    def shadeBorder(self, rect, d, reverse):
        """Draw a 3D rectangle to the screen or a surface. Defaults to the screen.
        (internal)
        """
        (x,y,w,h) = rect
        if reverse:
            self.renderer.drawGradient((x,y,d,h), self.shadeLo1, self.shadeLo0, self.shadeLo1, self.shadeLo0)
            self.renderer.drawGradient((x,y,w,d), self.shadeLo1, self.shadeLo1, self.shadeLo0, self.shadeLo0)
            self.renderer.drawGradient((x+w-d,y,d,h), self.shadeHi0, self.shadeHi1, self.shadeHi0, self.shadeHi1)
            self.renderer.drawGradient((x,y+h-d,w,d), self.shadeHi0, self.shadeHi0, self.shadeHi1, self.shadeHi1)
        else:
            self.renderer.drawGradient((x,y,d,h), self.shadeHi1, self.shadeHi0, self.shadeHi1, self.shadeHi0)
            self.renderer.drawGradient((x,y,w,d), self.shadeHi1, self.shadeHi1, self.shadeHi0, self.shadeHi0)
            self.renderer.drawGradient((x+w-d,y,d,h), self.shadeLo0, self.shadeLo1, self.shadeLo0, self.shadeLo1)
            self.renderer.drawGradient((x,y+h-d,w,d), self.shadeLo0, self.shadeLo0, self.shadeLo1, self.shadeLo1)


    #####################################################################
    ###
    ### Widgets specific drawing functions.
    ###
    #####################################################################    
    def drawButton(self, rect, title, hasFocus, status, enabled, font=None, shadow=0,fgColor=0, bgColor=0,roColor=0):
        """Options are:  button title and the up/down status.
        status == 0: normal
        status == 1: rollover
        status == 2: down
        """
        (x,y,w,h) = rect
        color = self.fgColor        
        if enabled == 0:
            self.drawGradient(rect, 0, self.bgColor1, self.bgColor2)
            self.shadeBorder(rect, 4, 1)
            color = self.buttonUpB
        elif status == 0:
            self.drawGradient(rect, 0, self.buttonUpA, self.buttonUpB)
            self.shadeBorder(rect, 4, 0)
        elif status == 1:
            self.drawGradient(rect, 0, self.buttonRoA, self.buttonRoB)
            self.shadeBorder(rect, 4, 0)
        elif status == 2:
            self.drawGradient(rect, 0, self.buttonDnA, self.buttonDnB)
            self.shadeBorder(rect, 4, 1)
            x += 3
            y += 3

        if hasFocus:
            self.drawOutlineRect(rect, self.buttonFocus, 3)

        if not font:
            font=self.defaultFont
            
        size = self.renderer.getTextSize(title, font)
        if shadow:
            self.renderer.drawText( title, (x + (w-size[0])/2+shadow, y + (h-size[1])/2+shadow), pyui.colors.black, font)            
        self.renderer.drawText( title, (x + (w-size[0])/2, y + (h-size[1])/2), color, font)
        return rect

    def drawImageButton(self, rect, filename, title, hasFocus, status):
        if status == 2:
            self.draw3DRect(rect, self.bgColor1, 1)
        else:
            self.draw3DRect(rect, self.bgColor1, 0)
        self.renderer.drawImage((rect[0] + status, rect[1] + status, rect[2]-2, rect[3]-2), filename)
        self.shadeBorder(rect, 4, 0)
        (w,h) = self.renderer.getTextSize(title)

        if hasFocus:
            self.drawOutlineRect(rect, self.buttonFocus, 3)
        
        self.renderer.drawText( title, (rect[0] + (rect[2]-w)/2 + status, rect[1] + (rect[3]-h)/2 + status-1), self.black)
        self.renderer.drawText( title, (rect[0] + (rect[2]-w)/2 + status-1, rect[1] + (rect[3]-h)/2 + status), self.white)

    def drawLabel(self, rect, title, color = None, font = None, shadow=0, align=0 ):
        return ThemeBase.drawLabel(self, rect, title, color, font, shadow, align)

    def drawCheckBox(self, rect, text, checkState):
        w = rect[2]
        h = rect[3]
        checkboxXpos = rect[0] + 8        
        checkboxYpos = rect[1] + (h/2) - 8

        self.draw3DRect( (checkboxXpos, checkboxYpos, 16, 16), self.bgColor1, 1)
        self.renderer.drawText( text, (checkboxXpos + 20, checkboxYpos ), self.fgColor)

        if checkState:
            self.renderer.drawLine( checkboxXpos + 2, checkboxYpos + 2, checkboxXpos + 14, checkboxYpos + 14, pyui.colors.white)
            self.renderer.drawLine( checkboxXpos + 14, checkboxYpos + 2, checkboxXpos + 2 , checkboxYpos + 14, pyui.colors.white)            

    def drawSliderBar(self, rect, range, position, BARWIDTH=8):
        interval = rect[2] / float(range)
        half = rect[1] + rect[3]/2
        self.renderer.drawRect(self.bgSelect, rect)
        ## could draw little lines here....
#        for i in xrange(1,range):
#            self.renderer.drawLine( rect[0] + i*interval, half-4, rect[0] + i*interval, half+4, pyui.colors.black)        
        self.renderer.drawLine( rect[0] + 1, half, rect[0] + rect[2] -2, half, pyui.colors.white)
        self.renderer.drawLine( rect[0] + 1, half + 2, rect[0] + rect[2] -2, half + 2, pyui.colors.black)
        diff = rect[2] - BARWIDTH
        xpos = float(diff) / range * position
        self.drawGradient((rect[0] + xpos, rect[1] + 2, BARWIDTH, rect[3] - 4), 0, pyui.colors.grey, pyui.colors.white)
        self.shadeBorder((rect[0] + xpos, rect[1] + 2, BARWIDTH, rect[3] - 4), 3, 0)

        #self.renderer.drawText("%s/%s" % (position,range), (rect[0] + rect[2] - 40, rect[1]), pyui.colors.red)

            
    def drawEdit(self, rect, text, hasFocus, caretPos, selectPos):
        self.draw3DRect(rect, self.bgColor2, 0)
        (x,y,w,h) = rect
        if hasFocus:
            caretX = self.renderer.getTextSize(text[0:caretPos])[0]
            if selectPos != None:
                # draw selection highlight
                selectX = self.renderer.getTextSize(text[0:selectPos])[0]
                selectX0 = min(caretX, selectX)
                selectX1 = max(caretX, selectX)
                if selectX0 < selectX1:
                    self.renderer.drawRect(self.bgSelect, (x+1+selectX0, y+1, selectX1-selectX0, self.defaultTextHeight))

        self.renderer.drawText(text, (x+1, y+1), self.fgColor)

        # draw caret        
        if hasFocus:
            self.renderer.drawRect(self.fgColor, (x+1+caretX, y+1, 1, self.defaultTextHeight))

        return rect

    def drawFrame(self, rect, title, backImage):
        """Draw frame objects.
        Frame objects are complex multi-part widgets. In order to allow the theme some flexibility of how to
        position and draw various interactive frame elements, the drawFrame() method returns a list of hit
        regions. Each element is a tuple consisting of a (region Id, rect). Region IDs are defined in locals.py.
        The hit list is in top to bottom order for easier iteration.
        """
        offset = 8
        hitList = []

        self.renderer.drawRect(self.bgColor1, rect)

        if backImage:
            self.renderer.drawImage((rect[0]+self.frameBorderLeft,
                                     rect[1]+self.frameBorderTop,
                                     rect[2] - self.frameBorderLeft - self.frameBorderRight,
                                     rect[3] - self.frameBorderTop - self.frameBorderBottom), backImage)
        
        # draw caption bar        
        (x,y,w,h)=(rect[0], rect[1], rect[2], self.frameBorderTop)
        self.drawGradient((x, y, w, h), 0, self.captionA, self.captionB)
        self.renderer.drawText( title, (x + self.frameBorderLeft + 1, y + self.frameBorderTop/2 - 8), self.fgColor, self.defaultFont)
        self.shadeBorder((x,y,w,h), 3, 0)
        hitList.append((pyui.locals.HIT_FRAME_MOVE, (x,y,w,h)))

        # draw close button        
        (x,y,w,h)=(rect[0]+rect[2]-self.frameBorderTop+3,rect[1]+3,self.frameBorderTop-6,self.frameBorderTop-6)
        self.drawGradient((x,y,w,h), 0, self.buttonUpA, self.buttonUpB)
        self.shadeBorder((x,y,w,h), 3, 0)
        self.shadeBorder((x+4,y+4,w-8,h-8), 3, 1)
        hitList.append((pyui.locals.HIT_FRAME_CLOSE, (x,y,w,h)))

        # left border
        (x,y,w,h)=(rect[0],rect[1]+self.frameBorderTop,self.frameBorderLeft,rect[3]-self.frameBorderTop-self.frameBorderBottom)
        self.renderer.drawRect(self.frameColor,(x,y,w,h))
        self.shadeBorder((x,y,w,h), 2, 0)

        # right border                
        (x,y,w,h)=(rect[0]+rect[2]-self.frameBorderRight,rect[1]+self.frameBorderTop,self.frameBorderRight,rect[3]-self.frameBorderTop-self.frameBorderBottom)
        self.renderer.drawRect(self.frameColor, (x,y,w,h))
        self.shadeBorder((x,y,w,h), 2, 0)
        hitList.append((pyui.locals.HIT_FRAME_RESIZE_RIGHT, (x,y,w,h)))

        # bottom border
        (x,y,w,h)=(rect[0]+self.frameBorderLeft,rect[1]+rect[3]-self.frameBorderBottom,rect[2]-self.frameBorderLeft-self.frameBorderRight,self.frameBorderBottom)
        self.renderer.drawRect(self.frameColor, (x,y,w,h))
        self.shadeBorder((x,y,w,h), 2, 0)
        hitList.append((pyui.locals.HIT_FRAME_RESIZE_BOTTOM, (x,y,w,h)))

        # lower left corner
        (x,y,w,h)=(rect[0],rect[1]+rect[3]-self.frameBorderBottom,self.frameBorderLeft,self.frameBorderBottom)
        self.renderer.drawRect(self.frameColor, (x,y,w,h))
        self.shadeBorder((x,y,w,h), 2, 0)

        # lower right corner
        (x,y,w,h)=(rect[0]+rect[2]-self.frameBorderRight,rect[1]+rect[3]-self.frameBorderBottom,self.frameBorderRight,self.frameBorderBottom)
        self.renderer.drawRect(self.frameColor, (x,y,w,h))
        self.shadeBorder((x,y,w,h), 2, 0)
        hitList.append((pyui.locals.HIT_FRAME_RESIZE_BOTTOM_RIGHT, (x,y,w,h)))
        
        # draw shadow
        (x,y,w,h)=(rect[0], rect[1], rect[2], rect[3])
        self.renderer.drawRect( self.shadowMedium, (x+w, y+offset, offset, h - offset) )
        self.renderer.drawRect( self.shadowMedium, (x+offset, y+h, w, offset) )

        hitList.reverse()        
        return hitList        
        
    def drawScrollButtonUp(self, rect):
        self.draw3DRect(rect, self.bgColor1, 0)
        return rect

    def drawScrollButtonDown(self, rect):
        self.draw3DRect(rect, self.bgColor1, 0)
        return rect

    def drawScrollBar(self, rect):
        self.draw3DRect(rect, self.bgColor1, 0)
        return rect


    def drawTabItem(self, rect, title, active):
        gap = 3
        (x,y,w,h) = rect
        self.renderer.drawRect(self.black, (x, y+h-1, gap, 1))
        x += gap
        title = " " + title + " "
        w = self.renderer.getTextSize(title)[0]
        if active:
            self.renderer.drawRect(self.black, (x, y, w, 1))
            self.renderer.drawRect(self.black, (x, y, 1, h))
            self.renderer.drawRect(self.black, (x+w-1, y, 1, h))
        else:
            self.renderer.drawRect(self.shadowLight, (x, y, w, h))
            self.drawOutlineRect((x, y, w, h), self.black)
        self.renderer.drawRect(self.shadowMedium, (x+w, y+gap, gap, h-gap))

        self.renderer.drawText( title, (x, y), self.fgColor)
        return (x,y,w,h)

    def drawTabHeader(self, rect):
        (x,y,w,h) = rect
        self.renderer.drawRect(self.black, (x, y+h-1, w, 1))
        return rect

    def drawMenuBar(self, rect):
        self.draw3DRect(rect, self.bgMenuBar, 0)
        return rect

    def drawMenuBarItem(self, rect, title, selected):
        title = "   " + title + "   "
        (w,h) = self.renderer.getTextSize(title)
        if selected:
            self.renderer.drawRect(self.bgMenuSelect, (rect[0], rect[1], w, rect[3]))
            self.renderer.drawText(title, (rect[0], rect[1]), self.fgColor)
        else:
            self.renderer.drawText(title, (rect[0], rect[1]), self.fgColor)
        return (rect[0], rect[1], w, rect[3])

    def drawMenu(self, rect):
        self.draw3DRect(rect, self.bgMenuPanel, 0)
        # draw shadow
        offset = 4
        (x,y,w,h)=(rect[0], rect[1], rect[2], rect[3])
        self.renderer.drawRect( self.shadowMedium, (x+w, y+offset, offset, h - offset) )
        self.renderer.drawRect( self.shadowMedium, (x+offset, y+h, w, offset) )
        return rect
    
    def drawMenuItem(self, rect, title, selected, icon = None):
        iconSize = 12
        if selected:
            self.renderer.drawRect(self.bgMenuSelect, (rect[0], rect[1], rect[2], self.defaultTextHeight))
        if title == '-':
            self.draw3DRect((rect[0], rect[1] + self.defaultTextHeight / 2- 1, rect[2], 2), self.bgMenuPanel, 1)
        else:
            self.renderer.drawText(title, (rect[0] + iconSize, rect[1]), self.fgColor)
        if icon:
            self.renderer.drawImage((rect[0], rect[1]+(self.defaultTextHeight - iconSize) / 2, iconSize, iconSize), icon)
        return (rect[0], rect[1], rect[2], self.defaultTextHeight)

    def drawListBox(self, rect):
        self.renderer.drawRect(self.bgColor1, rect)
        
    def drawListBoxItem(self, rect, title, selected, color):
        def getCharsThatFit(text, rect):
            for count in range(len(text),0,-1):
                size = self.renderer.getTextSize(text[:count])
                if size[0] < rect[2]:
                    return count
            return 0
        count = getCharsThatFit(title, rect)
        if selected:
            self.draw3DRect( (rect[0] - 1, rect[1] - 1, rect[2]- 1 - 5, rect[3] ), self.fgColor, 0)
            self.renderer.drawText(title[:count], (rect[0], rect[1]), self.bgColor2)
        else:
            self.renderer.drawText(title[:count], (rect[0], rect[1]), color)
        return rect
            
    def drawSplitter(self, rect):
        (x,y,w,h) = rect
        self.renderer.drawRect(self.frameColor, (x,y,w,h))
        self.shadeBorder((x,y,w,h), 2, 0)
        #self.draw3DRect(rect,self.bgColor3, 0)

    def drawToolTip(self, text, rect):
        self.renderer.drawRect(self.black, rect)
        self.renderer.drawRect(pyui.colors.yellow, (rect[0] + 1, rect[1] + 1, rect[2] - 2, rect[3] - 2))
        self.renderer.drawText(text, (rect[0] + 2, rect[1] + 2), pyui.colors.black)
