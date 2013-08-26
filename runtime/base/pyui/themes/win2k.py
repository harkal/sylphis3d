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
"""

import pyui
from pyui.themeBase import ThemeBase

class Win2kTheme(ThemeBase):
    """A theme that looks like win2k
    """

    def __init__(self, renderer, fontFace="microsoft sans serif", fontSize=10, fontFlags=0):
        ThemeBase.__init__(self, renderer, fontFace, fontSize, fontFlags)

        # theme colors        
        self.fgColor = renderer.packColor(0,0,0)
        self.bgSelect = renderer.packColor(200,224,216)
        self.bgColor1 = renderer.packColor(200,224,216)
        self.bgColor2 = renderer.packColor(136,192,184)
        self.bgColor3 = renderer.packColor(220,210,222)
        self.listSelect = renderer.packColor(1,9,131)
        self.bgMenuBar = self.bgColor2
        self.bgMenuPanel = self.bgColor2
        self.bgMenuSelect = self.listSelect
        self.black = renderer.packColor(0,0,0)
        self.white = renderer.packColor(255,255,255)
        self.shadeHi0 = renderer.packColor(255,255,255,0)
        self.shadeHi1 = renderer.packColor(255,255,255,255)
        self.shadeLo0 = renderer.packColor(0,0,0,0)
        self.shadeLo1 = renderer.packColor(0,0,0,255)
        self.buttonUpA = renderer.packColor(210,200,215)
        self.buttonUpB = renderer.packColor(190,180,180)
        self.buttonRoA = renderer.packColor(150,196,189)
        self.buttonRoB = renderer.packColor(180,190,200)
        self.buttonDnA = renderer.packColor(170,170,170)
        self.buttonDnB = renderer.packColor(180,180,200)
        self.buttonFocus = renderer.packColor(128,192,255,128)
        self.captionA = renderer.packColor(0,0,100)
        self.captionB = renderer.packColor(24,180,192)
        self.frameColor = renderer.packColor(120,120,140)
        self.shadowMedium = renderer.packColor(0,0,0,128)

        self.scrollerSize = 14
        
    #####################################################################
    ###
    ### Widgets specific drawing functions.
    ###
    #####################################################################


    def drawLabel(self, rect, title, color = None, font = None, shadow=0, align=0 ):
        return ThemeBase.drawLabel(self, rect, title, color, font, shadow, align)
        
    def drawButton(self, rect, title, hasFocus, status, enabled, font=None, shadow=0, fgColor=0, bgColor=0, roColor=0):
        """Options are:  button title and the up/down status.
        status == 0: normal
        status == 1: rollover
        status == 2: down
        """
        (x,y,w,h) = rect

        if not fgColor:
            fgColor = self.fgColor
            
        if not bgColor:
            bgColor = self.buttonUpB

        if not roColor:
            roColor = self.buttonRoA
            
        if status == 0:
            self.draw3DRect(rect, bgColor, 0)
        elif status == 1:
            self.draw3DRect(rect, roColor, 0)
        elif status == 2:
            self.draw3DRect(rect, roColor, 1)
            x += 1
            y += 1

        if not font:
            font=self.defaultFont

        size = self.renderer.getTextSize(title, font)
        
        if shadow:
            self.renderer.drawText( title, (x + (w-size[0])/2+shadow, y + (h-size[1])/2+shadow), pyui.colors.black, font)            
        self.renderer.drawText( title, (x + (w-size[0])/2, y + (h-size[1])/2), fgColor, font)
        return rect

    def drawImageButton(self, rect, filename, title, hasFocus, status):
        if status == 2:
            self.draw3DRect(rect, self.bgColor1, 1)
        else:
            self.draw3DRect(rect, self.bgColor1, 0)
        self.renderer.drawImage((rect[0] + status, rect[1] + status, rect[2]-2, rect[3]-2), filename)
        #self.shadeBorder(rect, 4, 0)
        (w,h) = self.renderer.getTextSize(title)

        if hasFocus:
            self.drawOutlineRect(rect, self.buttonFocus, 3)
        
        self.renderer.drawText( title, (rect[0] + (rect[2]-w)/2 + status, rect[1] + (rect[3]-h)/2 + status-1), self.black)
        self.renderer.drawText( title, (rect[0] + (rect[2]-w)/2 + status-1, rect[1] + (rect[3]-h)/2 + status), self.white)


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
        self.renderer.drawLine( rect[0] + 1, half, rect[0] + rect[2] -2, half, pyui.colors.white)
        self.renderer.drawLine( rect[0] + 1, half + 2, rect[0] + rect[2] -2, half + 2, pyui.colors.black)
        diff = rect[2] - BARWIDTH
        xpos = float(diff) / range * position
        barHeight = rect[3] - 4
        if barHeight > 20:
            barHeight = 20
            
        self.draw3DRect((rect[0] + xpos, rect[1] + (rect[3]/2) - (barHeight/2), BARWIDTH, barHeight), self.bgMenuBar, 0)

        self.renderer.drawText("%s/%s" % (position,range), (rect[0] + rect[2] - 40, rect[1]), pyui.colors.red)

            
    def drawEdit(self, rect, text, hasFocus, caretPos, selectPos):
        self.draw3DRect(rect, self.bgColor1, 1)
        (x,y,w,h) = rect
        if hasFocus:
            caretX = self.renderer.getTextSize(text[0:caretPos], self.defaultFont)[0]
            if selectPos != None:
                # draw selection highlight
                selectX = self.renderer.getTextSize(text[0:selectPos], self.defaultFont)[0]
                selectX0 = min(caretX, selectX)
                selectX1 = max(caretX, selectX)
                if selectX0 < selectX1:
                    self.renderer.drawRect(self.bgSelect, (x+1+selectX0, y+1, selectX1-selectX0, self.defaultTextHeight))

        self.renderer.drawText(text, (x+1, y+1), self.fgColor, self.defaultFont)

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
        self.drawGradient((x, y, w, h), 1, self.captionA, self.captionB)
        self.renderer.drawText( title, (x + self.frameBorderLeft + 1, y + self.frameBorderTop/2 - self.defaultTextHeight/2), self.bgColor1, self.defaultFont)
        #self.shadeBorder((x,y,w,h), 3, 0)
        hitList.append((pyui.locals.HIT_FRAME_MOVE, (x,y,w,h)))

        # draw close button        
        (x,y,w,h)=(rect[0]+rect[2]-self.frameBorderTop+6,rect[1]+5,self.frameBorderTop-12,self.frameBorderTop-10)
        self.draw3DRect((x,y,w,h), self.bgColor2,0)
        self.renderer.drawText("x", (x+2, y-2), self.fgColor)
        hitList.append((pyui.locals.HIT_FRAME_CLOSE, (x,y,w,h)))

        (x,y,w,h) = rect
        self.renderer.drawLine(x,y,x+w,y, self.white)
        self.renderer.drawLine(x,y,x,y+h, self.white)        
        self.renderer.drawLine(x+w,y,x+w,y+h, self.black)
        self.renderer.drawLine(x+w,y+h,x,y+h, self.black)                
        
        # left border
        (x,y,w,h)=(rect[0],
                   rect[1]+self.frameBorderTop,
                   1,
                   rect[3]-self.frameBorderTop)

        # right border                
        (x,y,w,h)=(rect[0]+rect[2]-4,
                   rect[1]+self.frameBorderTop,
                   4,
                   rect[3]-self.frameBorderTop)
        hitList.append((pyui.locals.HIT_FRAME_RESIZE_RIGHT, (x,y,w,h)))

        # bottom border
        (x,y,w,h)=(rect[0],
                   rect[1]+rect[3]-4,
                   rect[2],
                   4)
        hitList.append((pyui.locals.HIT_FRAME_RESIZE_BOTTOM, (x,y,w,h)))

        # lower right corner
        (x,y,w,h)=(rect[0]+rect[2]-self.frameBorderRight,
                   rect[1]+rect[3]-self.frameBorderBottom,
                   self.frameBorderRight,
                   self.frameBorderBottom)
        self.draw3DRect((x,y,w,h), self.bgColor2, 0)
        hitList.append((pyui.locals.HIT_FRAME_RESIZE_BOTTOM_RIGHT, (x,y,w,h)))

        hitList.reverse()        
        return hitList        


    def drawScrollBack(self, rect):
        self.drawGradient(rect, 1, self.buttonUpB, self.buttonUpA)
        
    def drawScrollButtonUp(self, rect):
        self.draw3DRect(rect, self.bgColor2, 0)
        return rect

    def drawScrollButtonDown(self, rect):
        self.draw3DRect(rect, self.bgColor2, 0)
        return rect

    def drawScrollBar(self, rect):
        self.draw3DRect(rect, self.bgColor2, 0, 2)
        return rect


    def drawTabItem(self, rect, title, active):
        gap = 3
        (x,y,w,h) = rect
        self.renderer.drawRect(self.black, (x, y+h-1, gap, 1))
        x += gap
        title = " " + title + " "
        w = self.renderer.getTextSize(title, self.defaultFont)[0]
        if active:
            self.renderer.drawRect(self.black, (x, y, w, 1))
            self.renderer.drawRect(self.black, (x, y, 1, h))
            self.renderer.drawRect(self.black, (x+w-1, y, 1, h))
        else:
            self.renderer.drawRect(self.bgColor2, (x, y, w, h))
            self.drawOutlineRect((x, y, w, h), self.black)
            pass
        self.renderer.drawRect(self.shadowMedium, (x+w, y+gap, gap, h-gap))

        self.renderer.drawText( title, (x, y), self.fgColor, self.defaultFont)
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
        (w,h) = self.renderer.getTextSize(title, self.defaultFont)
        if selected:
            self.renderer.drawRect(self.bgMenuSelect, (rect[0], rect[1], w, rect[3]))
            self.renderer.drawText(title, (rect[0], rect[1]), self.bgColor1, self.defaultFont)
        else:
            self.renderer.drawText(title, (rect[0], rect[1]), self.fgColor, self.defaultFont)
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
        color = self.fgColor
        if selected:
            self.renderer.drawRect(self.bgMenuSelect, (rect[0], rect[1], rect[2], self.defaultTextHeight))
            color = self.bgColor1
        if title == '-':
            self.draw3DRect((rect[0], rect[1] + self.defaultTextHeight / 2- 1, rect[2], 2), self.bgMenuPanel, 1)
        else:
            self.renderer.drawText(title, (rect[0] + iconSize, rect[1]), color, self.defaultFont)
        if icon:
            self.renderer.drawImage((rect[0], rect[1]+(self.defaultTextHeight - iconSize) / 2, iconSize, iconSize), icon)
        return (rect[0], rect[1], rect[2], self.defaultTextHeight)

    
    def drawListBox(self, rect):
        self.drawOutlineRect(rect, pyui.colors.black)

    def drawListBoxItem(self, rect, title, selected, color):
        if selected:
            self.renderer.drawRect(self.listSelect, (rect[0] +2, rect[1] - 1, rect[2]- 1 - 3, rect[3] ))
            self.renderer.drawText(title, (rect[0], rect[1]), self.bgColor1, self.defaultFont)
        else:
            self.renderer.drawText(title, (rect[0], rect[1]), color, self.defaultFont)
        return rect
            
    def drawSplitter(self, rect):
        (x,y,w,h) = rect
        self.renderer.drawRect(self.frameColor, (x,y,w,h))
        #self.shadeBorder((x,y,w,h), 2, 0)
        #self.draw3DRect(rect,self.bgColor3, 0)

    def drawToolTip(self, text, rect):
        self.renderer.drawRect(self.black, rect)
        self.renderer.drawRect(pyui.colors.yellow, (rect[0] + 1, rect[1] + 1, rect[2] - 2, rect[3] - 2))
        self.renderer.drawText(text, (rect[0] + 2, rect[1] + 2), pyui.colors.black)
