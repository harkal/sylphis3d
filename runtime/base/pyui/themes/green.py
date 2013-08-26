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
"""A green a black line theme.
"""
import pyui


from pyui.themeBase import ThemeBase

class GreenTheme(ThemeBase):

    def __init__(self, renderer):
        ThemeBase.__init__(self, renderer, "courier", 10, 0)        

        self.frameBorderLeft = 1
        self.frameBorderRight = 1
        self.frameBorderBottom = 1
        
        self.green = renderer.packColor(0,255,0)
        self.black = renderer.packColor(0,0,0)

    #####################################################################
    ###
    ###   Utility drawing functions not specific to any widgets
    ###
    #####################################################################    

    def draw3DRect(self, rect, color, reverse):
        """Draw a 3D rectangle to the screen or a surface. Defaults to the screen.
        (internal)
        """
        if rect[2] < 2 or rect[3] < 2:
            return
        self.renderer.drawRect(self.black, rect)
        self.renderer.drawLine( rect[0], rect[1], rect[0] + rect[2], rect[1], self.green)
        self.renderer.drawLine( rect[0] + rect[2], rect[1], rect[0] + rect[2], rect[1] + rect[3], self.green)
        self.renderer.drawLine( rect[0] + rect[2], rect[1] + rect[3], rect[0], rect[1] + rect[3], self.green)
        self.renderer.drawLine( rect[0], rect[1] + rect[3], rect[0], rect[1], self.green)            

    def drawRect(self, rect, color):
        self.draw3DRect(rect, self.green, 0)

    def drawGradient(self, rect, vertical, c1, c2):
        self.draw3DRect(rect, self.green, 0)        
        
    #####################################################################
    ###
    ### Widgets specific drawing functions.
    ###
    #####################################################################    

    def drawFrame(self, rect, title, backImage):
        """Draw frame objects.
        Frame objects are complex multi-part widgets. In order to allow the theme some flexibility of how to
        position and draw various interactive frame elements, the drawFrame() method returns a list of hit
        regions. Each element is a tuple consisting of a (region Id, rect). Region IDs are defined in locals.py.
        The hit list is in top to bottom order for easier iteration.
        """
        offset = 8
        hitList = []

        self.renderer.drawRect(pyui.colors.green, rect)
        self.renderer.drawRect(pyui.colors.black, (rect[0]+2, rect[1]+2, rect[2]-4, rect[3]-4) )
        
        if backImage:
            self.renderer.drawImage((rect[0]+self.frameBorderLeft,
                                     rect[1]+self.frameBorderTop,
                                     rect[2] - self.frameBorderLeft - self.frameBorderRight,
                                     rect[3] - self.frameBorderTop - self.frameBorderBottom), backImage)
        
        # draw caption bar        
        (x,y,w,h)=(rect[0], rect[1], rect[2], self.frameBorderTop)
        self.drawOutlineRect((x, y, w, h), self.green, 1)
        self.renderer.drawText( title, (x + self.frameBorderLeft + 1, y + self.frameBorderTop/2 - 8), self.green)
        hitList.append((pyui.locals.HIT_FRAME_MOVE, (x,y,w,h)))

        # draw close button        
        (x,y,w,h)=(rect[0]+rect[2]-self.frameBorderTop+3,rect[1]+3,self.frameBorderTop-6,self.frameBorderTop-6)
        self.renderer.drawRect((x,y,w,h), self.green)
        hitList.append((pyui.locals.HIT_FRAME_CLOSE, (x,y,w,h)))

        # left border
        (x,y,w,h)=(rect[0],rect[1]+self.frameBorderTop,self.frameBorderLeft,rect[3]-self.frameBorderTop-self.frameBorderBottom)
        self.drawRect((x,y,w,h), self.green)

        # right border                
        (x,y,w,h)=(rect[0]+rect[2]-self.frameBorderRight,rect[1]+self.frameBorderTop,self.frameBorderRight,rect[3]-self.frameBorderTop-self.frameBorderBottom)
        self.drawRect((x,y,w,h), self.green)
        hitList.append((pyui.locals.HIT_FRAME_RESIZE_RIGHT, (x,y,w,h)))

        # bottom border
        (x,y,w,h)=(rect[0]+self.frameBorderLeft,rect[1]+rect[3]-self.frameBorderBottom,rect[2]-self.frameBorderLeft-self.frameBorderRight,self.frameBorderBottom)
        self.drawRect((x,y,w,h), self.green)
        hitList.append((pyui.locals.HIT_FRAME_RESIZE_BOTTOM, (x,y,w,h)))

        # lower left corner
        (x,y,w,h)=(rect[0],rect[1]+rect[3]-self.frameBorderBottom,self.frameBorderLeft,self.frameBorderBottom)
        self.drawRect((x,y,w,h), self.green)

        # lower right corner
        (x,y,w,h)=(rect[0]+rect[2]-self.frameBorderRight,rect[1]+rect[3]-self.frameBorderBottom,self.frameBorderRight,self.frameBorderBottom)
        self.drawRect((x,y,w,h), self.green)
        hitList.append((pyui.locals.HIT_FRAME_RESIZE_BOTTOM_RIGHT, (x,y,w,h)))
        
        hitList.reverse()        
        return hitList        
        
