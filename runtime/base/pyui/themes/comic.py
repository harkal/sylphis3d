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

This modules keeps NO state for the drawable objects - it just draws them on demand
from the widgets themselves which hold all the state.

The constants for the theme objects live in pyui/locals.py
"""

import pyui
from pyui.desktop import getRenderer
from pyui.themeBase import ThemeBase

class ComicTheme(ThemeBase):
    """A theme that looks like a comic book!
    """
    
    def __init__(self, renderer, fontFace="comic sans ms", fontSize=10, fontFlags=0):
        ThemeBase.__init__(self, renderer, fontFace, fontSize, fontFlags)
        
        # theme colors
        alpha = 185
        self.fgColor = self.renderer.packColor(0,0,0, alpha)
        self.bgColor1 = self.renderer.packColor(255,255,0,alpha)
        self.bgColor2 = self.renderer.packColor(255,255,120,alpha)
        self.bgColor3 = self.renderer.packColor(255,255,185, alpha)        
        self.bgSelect = self.renderer.packColor(255,255,185, alpha)        
        self.titleColor = self.renderer.packColor(210,210,255,alpha) #255,128,0)
        self.buttonUpA = self.renderer.packColor(255,190,120,alpha)
        self.buttonUpB = self.renderer.packColor(255,130,100,alpha)
        self.buttonRoA = self.renderer.packColor(255,120,80,alpha)
        self.buttonTextColor = self.renderer.packColor(0,255,255,alpha)

        self.titleFont = renderer.createFont(fontFace, int(fontSize*1.5), pyui.locals.BOLD | pyui.locals.ITALIC)
        (self.titleTextWidth, self.titleTextHeight) = self.renderer.getTextSize("M", self.titleFont)

        # customize some sizes
        self.frameBorderTop = self.titleTextHeight + 4
        self.tabsHeight = int(self.titleTextHeight * 1.3)
        self.scrollerSize = 16
          
    #####################################################################
    ###
    ### Widgets specific drawing functions.
    ###
    #####################################################################

    def drawLabel(self, rect, title, color = None, font = None, shadow=0, align=0 ):
        getRenderer().drawRect(self.fgColor, rect)
        getRenderer().drawRect(self.bgColor3, (rect[0]+2, rect[1]+2, rect[2]-4, rect[3]-4) )
        #(title, color) = options
        if not color:
            color = self.fgColor

        if not font:
            font = self.defaultFont
            
        (w,h) = getRenderer().getTextSize(title, font)
        if align == 2:
            posX = rect[0] + rect[2]/2 - w/2
        else:
            posX = rect[0]

        if shadow:
            getRenderer().drawText(title, (posX+shadow, rect[1]+shadow), pyui.colors.black, font)
        getRenderer().drawText(title, (posX, rect[1]), color, font)            
        return (rect[0], rect[1], w, h)
        
    def drawButton(self, rect, title, hasFocus, status, enabled, font=None, shadow=2, fgColor=0, bgColor=0, roColor=0):
        """Options are:  button title and the up/down status.
        status == 0: normal
        status == 1: rollover
        status == 2: down
        """
        (x,y,w,h) = rect
        if shadow < 2:
            shadow = 2
            
        if not fgColor:
            fgColor = self.buttonTextColor
            
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

        size = getRenderer().getTextSize(title, font)
        
        if shadow:
            getRenderer().drawText( title, (x + (w-size[0])/2+shadow, y + (h-size[1])/2+shadow), self.fgColor, font)            
        getRenderer().drawText( title, (x + (w-size[0])/2, y + (h-size[1])/2), fgColor, font)
        return rect

    def drawImageButton(self, rect, filename, title, hasFocus, status):
        if status == 2:
            self.draw3DRect(rect, self.bgColor1, 1)
        else:
            self.draw3DRect(rect, self.bgColor1, 0)
        getRenderer().drawImage((rect[0] + status, rect[1] + status, rect[2]-2, rect[3]-2), filename)
        #self.shadeBorder(rect, 4, 0)
        (w,h) = getRenderer().getTextSize(title)

        if hasFocus:
            self.drawOutlineRect(rect, self.bgColor1, 3)
        
        getRenderer().drawText( title, (rect[0] + (rect[2]-w)/2 + status, rect[1] + (rect[3]-h)/2 + status-1), self.fgColor)
        getRenderer().drawText( title, (rect[0] + (rect[2]-w)/2 + status-1, rect[1] + (rect[3]-h)/2 + status), pyui.colors.white)
        return rect

    def drawCheckBox(self, rect, text, checkState):
        w = rect[2]
        h = rect[3]
        checkboxXpos = rect[0] + 8        
        checkboxYpos = rect[1] + (h/2) - 8

        self.draw3DRect( (checkboxXpos, checkboxYpos, 16, 16), self.bgColor1, 1)
        getRenderer().drawText( text, (checkboxXpos + 20, checkboxYpos ), self.fgColor)

        if checkState:
            getRenderer().drawLine( checkboxXpos + 2, checkboxYpos + 2, checkboxXpos + 14, checkboxYpos + 14, pyui.colors.white)
            getRenderer().drawLine( checkboxXpos + 14, checkboxYpos + 2, checkboxXpos + 2 , checkboxYpos + 14, pyui.colors.white)

        return rect

    def drawSliderBar(self, rect, range, position, BARWIDTH=8):
        interval = rect[2] / float(range)
        half = rect[1] + rect[3]/2
        getRenderer().drawRect(self.bgColor3, rect)
        ## could draw little lines here....
        getRenderer().drawLine( rect[0] + 1, half, rect[0] + rect[2] -2, half, pyui.colors.white)
        getRenderer().drawLine( rect[0] + 1, half + 2, rect[0] + rect[2] -2, half + 2, self.fgColor)
        diff = rect[2] - BARWIDTH
        xpos = float(diff) / range * position
        barHeight = rect[3] - 4
        if barHeight > 20:
            barHeight = 20
            
        self.draw3DRect((rect[0] + xpos, rect[1] + (rect[3]/2) - (barHeight/2), BARWIDTH, barHeight), self.bgColor2, 0)

        getRenderer().drawText("%s/%s" % (position,range), (rect[0] + rect[2] - 40, rect[1]), pyui.colors.red)
        return rect

            
    def drawEdit(self, rect, text, hasFocus, caretPos, selectPos):
        getRenderer().drawRect(self.fgColor, rect)
        getRenderer().drawRect(self.bgColor1, (rect[0]+2, rect[1]+2, rect[2]-4, rect[3]-4) )        
        (x,y,w,h) = rect
        if hasFocus:
            caretX = getRenderer().getTextSize(text[0:caretPos], self.defaultFont)[0]
            if selectPos != None:
                # draw selection highlight
                selectX = getRenderer().getTextSize(text[0:selectPos], self.defaultFont)[0]
                selectX0 = min(caretX, selectX)
                selectX1 = max(caretX, selectX)
                if selectX0 < selectX1:
                    getRenderer().drawRect(self.bgSelect, (x+1+selectX0, y+1, selectX1-selectX0, self.defaultTextHeight))

        getRenderer().drawText(text, (x+1, y+1), self.fgColor, self.defaultFont)

        # draw caret        
        if hasFocus:
            getRenderer().drawRect(self.fgColor, (x+1+caretX, y+1, 1, self.defaultTextHeight))

        return rect

    def drawFrame(self, rect, title, backImage):
        """Draw frame objects.
        Frame objects are complex multi-part widgets. In order to allow the theme some flexibility of how to
        position and draw various interactive frame elements, the drawFrame() method returns a list of hit
        regions. Each element is a tuple consisting of a (region Id, rect). Region IDs are defined in pyui.locals.py.
        The hit list is in top to bottom order for easier iteration.
        """
        hitList = []

        getRenderer().drawRect(self.bgColor1, rect)
        self.drawOutlineRect(rect, pyui.colors.black, 3) #.drawRect(self.fgColor, rect)        

        if backImage:
            self.renderer.drawImage((rect[0]+self.frameBorderLeft,
                                     rect[1]+self.frameBorderTop,
                                     rect[2] - self.frameBorderLeft - self.frameBorderRight,
                                     rect[3] - self.frameBorderTop - self.frameBorderBottom), backImage)

        # draw caption
        (tw,th) = getRenderer().getTextSize(title, self.titleFont)
        (x,y,w,h)=(rect[0], rect[1], tw+24, self.frameBorderTop)
        getRenderer().drawRect(self.fgColor, (x,y+self.frameBorderTop-2,rect[2],2))
        hitList.append((pyui.locals.HIT_FRAME_MOVE, (x,y,rect[2],h)))        
        getRenderer().drawText(title, (x+self.frameBorderLeft+rect[2]/2-tw/2+2, y+self.frameBorderTop/2 - self.defaultTextHeight/2-5), self.fgColor, self.titleFont)
        getRenderer().drawText(title, (x+self.frameBorderLeft+rect[2]/2-tw/2+1, y+self.frameBorderTop/2 - self.defaultTextHeight/2-6), self.titleColor, self.titleFont)

        # draw close button        
        (x,y,w,h)=(rect[0]+rect[2]-self.frameBorderTop+6,rect[1]+5,self.frameBorderTop-12,self.frameBorderTop-10)
        getRenderer().drawText("X", (x, y), self.fgColor, self.titleFont)
        #self.draw3DRect((x,y,w,h), self.bgColor3,0)
        hitList.append((pyui.locals.HIT_FRAME_CLOSE, (x,y,w,h)))

        # resize spot
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
        getRenderer().drawRect(self.buttonUpA, rect)
        return rect
    
    def drawScrollButtonUp(self, rect):
        getRenderer().drawRect(self.fgColor, rect)
        getRenderer().drawRect(self.buttonRoA, (rect[0]+2, rect[1]+2, rect[2]-4, rect[3]-4) )        
        return rect

    def drawScrollButtonDown(self, rect):
        getRenderer().drawRect(self.fgColor, rect)
        getRenderer().drawRect(self.buttonRoA, (rect[0]+2, rect[1]+2, rect[2]-4, rect[3]-4) )        
        return rect

    def drawScrollBar(self, rect):
        getRenderer().drawRect(self.fgColor, rect)
        getRenderer().drawRect(self.buttonRoA, (rect[0]+2, rect[1]+2, rect[2]-4, rect[3]-4) )        
        return rect

    def drawTabItem(self, rect, title, active):
        gap = 3
        (x,y,w,h) = rect
        getRenderer().drawRect(self.fgColor, (x, y+h-1, gap, 1))
        x += gap
        title = " " + title + " "
        w = getRenderer().getTextSize(title, self.defaultFont)[0]
        if active:
            getRenderer().drawRect(self.fgColor, (x, y, w, 1))
            getRenderer().drawRect(self.fgColor, (x, y, 1, h))
            getRenderer().drawRect(self.fgColor, (x+w-1, y, 1, h))
        else:
            getRenderer().drawRect(self.bgColor2, (x, y, w, h))
            self.drawOutlineRect((x, y, w, h), self.fgColor)
            pass
        getRenderer().drawRect(self.bgColor3, (x+w, y+gap, gap, h-gap))

        getRenderer().drawText( title, (x, y), self.fgColor, self.defaultFont)
        return (x,y,w,h)

    def drawTabHeader(self, rect):
        (x,y,w,h) = rect
        getRenderer().drawRect(self.fgColor, (x, y+h-1, w, 1))
        return rect

    def drawMenuBar(self, rect):
        self.draw3DRect(rect, self.bgColor2, 0)
        return rect

    def drawMenuBarItem(self, rect, title, selected):
        title = "   " + title + "   "
        (w,h) = getRenderer().getTextSize(title, self.defaultFont)
        if selected:
            getRenderer().drawRect(self.bgColor3, (rect[0], rect[1], w, rect[3]))
            getRenderer().drawText(title, (rect[0], rect[1]), self.fgColor, self.defaultFont)
        else:
            getRenderer().drawText(title, (rect[0], rect[1]), self.fgColor, self.defaultFont)
        return (rect[0], rect[1], w, rect[3])

    def drawMenu(self, rect):
        getRenderer().drawRect(self.fgColor, rect)
        getRenderer().drawRect(self.bgColor1, (rect[0]+2, rect[1]+2, rect[2]-4, rect[3]-4) )        
        return rect
    
    def drawMenuItem(self, rect, title, selected, icon = None):
        iconSize = 12
        color = self.fgColor
        if selected:
            getRenderer().drawRect(self.fgColor, (rect[0], rect[1], rect[2], self.defaultTextHeight))
            color = self.bgColor1
        if title == '-':
            self.draw3DRect((rect[0], rect[1] + self.defaultTextHeight / 2- 1, rect[2], 2), self.bgColor1, 1)
        else:
            getRenderer().drawText(title, (rect[0] + iconSize, rect[1]), color, self.defaultFont)
        if icon:
            getRenderer().drawImage((rect[0], rect[1]+(self.defaultTextHeight - iconSize) / 2, iconSize, iconSize), icon)
        return (rect[0], rect[1], rect[2], self.defaultTextHeight)

    def drawListBox(self, rect):
        self.drawOutlineRect(rect, self.fgColor, 2)
        getRenderer().drawRect(self.bgColor3, (rect[0]+2, rect[1]+2, rect[2]-4, rect[3]-4) )
        return rect
        
    def drawListBoxItem(self, rect, title, selected, color):
        if selected:
            getRenderer().drawRect(self.bgColor3, (rect[0] +2, rect[1] - 1, rect[2]- 1 - 3, rect[3] ))
            getRenderer().drawText(title, (rect[0]+2, rect[1]), self.fgColor, self.defaultFont)
        else:
            getRenderer().drawText(title, (rect[0]+2, rect[1]), color, self.defaultFont)
        return rect
            
    def drawSplitter(self, rect):
        (x,y,w,h) = rect
        getRenderer().drawRect(self.fgColor, (x,y,w,h))
        return rect
    
    def drawToolTip(self, text, rect):
        getRenderer().drawRect(self.fgColor, rect)
        getRenderer().drawRect(self.bgColor1, (rect[0] + 1, rect[1] + 1, rect[2] - 2, rect[3] - 2))
        getRenderer().drawText(text, (rect[0] + 2, rect[1] + 2), self.fgColor)
        return rect
