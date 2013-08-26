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

"""Pyui Themes.
Themes are a method of customizing the drawing of widgets in a pyui GUI.

This modules keeps NO state for the drable objects - it just draws them on demand
from the widgets themselves which hold all the state.

The constants for the theme objects live in pyui/locals.py

Themes have a default font that is used for any widgets that dont specify a font.
"""

import locals
import pyui
from string import split
from pyui.desktop import getRenderer, getDesktop

class ThemeBase:
    """Theme objects (like buttons) are drawn through methods for each type of widget.
    It returns the rect that the object draw fit into.

    The rect passed in should always be in window coordinates.
    """

    def __init__(self, renderer, fontFace="times", fontSize=17, fontFlags=0):
        self.renderer = renderer
        self.desktop = getDesktop()

        self.fgColor = renderer.packColor(255,255,255)
        self.bgColor = renderer.packColor(0,0,0)
        
        # setup default font
        self.defaultFontFace = fontFace
        self.defaultFontSize = fontSize
        self.defaultFontFlags = fontFlags
        self.defaultFont = renderer.createFont(fontFace, fontSize, fontFlags)
        (self.defaultTextWidth, self.defaultTextHeight) = renderer.getTextSize("M", self.defaultFont)

        # setup widget offsets
        self.frameBorderLeft = 4
        self.frameBorderRight = 4
        self.frameBorderTop = self.defaultTextHeight + 4
        self.frameBorderBottom = 4
        self.tabsHeight = int(self.defaultTextHeight * 1.3)
        self.scrollerSize = 10
        

    ### Information about the theme..
    def getFrameBorderTop(self):
        return self.frameBorderTop

    def getFrameBorderLeft(self):
        return self.frameBorderLeft
    
    def getFrameBorderRight(self):
        return self.frameBorderRight
    
    def getFrameBorderBottom(self):
        return self.frameBorderBottom
    
    def getTabsHeight(self):
        return self.tabsHeight
    
    def getScrollerSize(self):
        return self.scrollerSize

    def getFgColor(self):
        return self.fgColor

    def getBgColor(self):
        return self.bgColor

        ### mouse cursor functions
    def setArrowCursor(self):
        pass

    def setResizeCursor(self):
        pass

    def setButtonCursor(self):
        pass

    def setWaitCursor(self):
        pass

    #####################################################################
    ###
    ###   Utility drawing functions not specific to any widgets
    ###
    #####################################################################    

    def draw3DRect(self, rect, color, reverse, thick=1):
        """Draw a 3D rectangle
        """
        (r,g,b,a) = self.renderer.unpackColor(color)
        a=255
        colorLo = self.renderer.packColor(r/2, g/2, b/2, a)
        colorHi = self.renderer.packColor(128 + r/2, 128 + g/2, 128 + b/2, a)

        if reverse:
            (colorLo, colorHi) = (colorHi, colorLo)
            
        (x,y,w,h) = rect
        if w < 2 or h < 2:
            return

        self.renderer.drawRect( colorHi, (x, y, w-thick, thick) )
        self.renderer.drawRect( colorHi, (x, y+thick, thick, h-thick) )
        if w > 2 and h > 2:
            self.renderer.drawRect( color, (x+thick, y+thick, w-thick*2, h-thick*2) )
        self.renderer.drawRect( colorLo, (x+thick, y+h-thick, w-thick, thick) )
        self.renderer.drawRect( colorLo, (x+w-thick, y+thick, thick, h-thick*2) )

    def drawOutlineRect(self, rect, color, thick=1):
        (x,y,w,h) = rect
        self.renderer.drawRect(color, (x,y,w,thick))
        self.renderer.drawRect(color, (x,y+thick,thick,h-2*thick))
        self.renderer.drawRect(color, (x+w-thick,y+thick,thick,h-2*thick))
        self.renderer.drawRect(color, (x,y+h-thick,w,thick))

    def drawGradient(self, rect, vertical, c1, c2):
        if vertical:
            self.renderer.drawGradient(rect, c1, c2, c1, c2)            
        else:
            self.renderer.drawGradient(rect, c1, c1, c2, c2)
        
    #####################################################################
    ###
    ### Widgets specific drawing functions.
    ### These are the methods for actual themes to implement.
    ###
    #####################################################################    
    def drawButton(self, rect, title, hasFocus, status, enabled, font=None, shadow=0,fgColor=0, bgColor=0,roColor=0):
        return rect
    
    def drawImageButton(self, rect, filename, title, hasFocus, status):
        return rect
    
    def drawLabel(self, rect, title, color = None, font = None, shadow=0, align=0 ):
        #(title, color) = options
        if not color:
            color = self.fgColor

        if not font:
            font = self.defaultFont

        lines = split(title, '\n')
        ypos = rect[1]
        for line in lines:
            (w,h) = self.renderer.getTextSize(line, font)
            if align == 2:
                posX = rect[0] + rect[2]/2 - w/2
            else:
                posX = rect[0]

            if shadow:
                self.renderer.drawText(line, (posX+shadow, ypos+shadow), pyui.colors.black, font)
            self.renderer.drawText(line, (posX, ypos), color, font)
            ypos += h
        return (rect[0], rect[1], w, ypos - rect[1])
    
    def drawCheckBox(self, rect, text, checkState):
        return rect
    
    def drawSliderBar(self, rect, range, position, BARWIDTH=8):
        return rect
    
    def drawEdit(self, rect, text, hasFocus, caretPos, selectPos):
        return rect

    def drawSplitter(self, rect):
        return rect

    def drawToolTip(self, text, rect):
        return rect    
    
    # scroll bar methods
    def drawScrollBack(self, rect):
        return rect
    
    def drawScrollButtonUp(self, rect):
        return rect

    def drawScrollButtonDown(self, rect):
        return rect

    def drawScrollBar(self, rect):
        return rect

    # tabbed panel methods
    def drawTabItem(self, rect, title, active):
        return rect
    
    def drawTabHeader(self, rect):
        return rect

    # menu methods
    def drawMenuBar(self, rect):
        return rect

    def drawMenuBarItem(self, rect, title, selected):
        return rect

    def drawMenu(self, rect):
        return rect
    
    def drawMenuItem(self, rect, title, selected, icon = None):
        return rect

    # list box methods
    def drawListBox(self, rect):
        return rect
        
    def drawListBoxItem(self, rect, title, selected, color):
        return rect

    # frame methods
    def drawFrame(self, rect, title):
        return rect
