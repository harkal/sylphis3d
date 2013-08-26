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

"""Widgets in the pyui library. Widgets are smaller GUI objects that can be contained in
windows.  

Widgets dont do any actual drawing. Widgets maintain all the state and provide behaviour, but
use themes for drawing.

All handler methods for widgets are prefixed with "_pyui" so that they
never conflict with a user defined event handler method.

"""
import layouts
import pyui

from pyui.desktop import getDesktop, getTheme, getRenderer

from pyui.base import Base, Panel, Window

class Label(Base):
    """Label object has a text label. uses default font if font is not specified.
    """
    widgetLabel = "Label"
    LEFT   = 0
    RIGHT  = 1
    CENTER = 2    
    
    def __init__(self,text, color = None, font = None, shadow=0, align=0):
        Base.__init__(self)
        self.font = font
        self.shadow = shadow
        self.align = align
        if color:
            self.color = color
        else:
            self.color = self.fgColor
        self.setText(text)

    def draw(self, renderer):
        if self.show:
            getTheme().drawLabel(self.windowRect, self.text, color=self.color, font=self.font, shadow=self.shadow, align=self.align)

    def setText(self, text):
        """Set the text of the label. sets the dirty flag.
        """
        self.text = text
        if len(text) == 0:
            text = " "
        (self.width,self.height) = getRenderer().getTextSize(text)
        self.setDirty()

    def setColor(self, color = None):
        if color == None:
            self.color = self.fgColor
        else:
            self.color = color
            
        self.setDirty()

class Picture(Base):
    """Picture/Image object. warning: this does not clip to it's resized psize by default.

    The pieceRect arg is used to draw portions of images. This can be
    useful for flipbook style animations. The PieceRect is a tuple of
    (x position, y position, width, height). When the image is divided
    into pieces (width * height) and the single piece at (x,y) is
    drawn.
    """
    def __init__(self, filename, pieceRect = (0,0,1,1)):
        Base.__init__(self)
        self.setFilename(filename)
        self.tooltipText = self.filename
        self.pieceRect = pieceRect

    def setRotation(self, irotationDeg):
        self.irotationDegrees = irotationDeg

    def draw(self, renderer):
        if self.show and self.filename:
            renderer.drawImage(self.windowRect, self.filename, self.pieceRect)

    def setFilename(self, filename):
        self.filename = filename
        self.setDirty(1)
        
    def setPiece(self, x, y, w, h):
        self.pieceRect = (x, y, w, h)
        self.setDirty(1)

class Button(Base):
    """button object. has a text label. Handler will be called when it is pressed.
    """
    widgetLabel = "Button"
    IDLE = 0
    ROLLOVER = 1
    DOWN = 2

    canTab = 1    
    
    def __init__(self, text, handler = None, font=None, shadow=0, fgColor=None, bgColor=None, roColor=None):
        Base.__init__(self)
        self.handler = handler
        self.font=font
        self.shadow=shadow
        self.fgColor=fgColor
        self.bgColor=bgColor
        self.roColor=roColor

        self.setText(text)
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self._pyuiMouseDown)
        self.registerEvent(pyui.locals.LMOUSEBUTTONUP, self._pyuiMouseUp)
        self.registerEvent(pyui.locals.KEYDOWN, self._pyuiKeyDown)
        self.registerEvent(pyui.locals.KEYUP, self._pyuiKeyUp)
        self.registerEvent(pyui.locals.MOUSEMOVE, self._pyuiMouseMotion)
        self.registerEvent(pyui.locals.CLICKED, self._pyuiClicked)
        self.status = Button.IDLE
        self.capture = 0
        self.enabled = 1
        self.tooltipText = text

    def draw(self, renderer):
        if self.show:
            getTheme().drawButton(self.windowRect, self.text, self.hasFocus(), self.status, self.enabled, self.font,
                                  self.shadow, self.fgColor, self.bgColor, self.roColor)
        
    def setText(self, text, dontResize=0):
        """Pass 1 to dontResize if you dont want the button to resize itself to the
        text passed in.
        """
        self.text = text
        if len(text) < 1:
            text = " "

        if not dontResize:
            # FIXME: ARG! this break size of layed-out buttons...
            if self.font:
                font = self.font
            else:
                font = getTheme().defaultFont

            size = getRenderer().getTextSize("  "+text+"  ", font)
            self.resize(size[0], int(size[1] * 1.5))

    def getText(self):
        return self.text
    
    def _pyuiClicked(self, event):
        if event.id == self.id and self.handler:
            self.handler(self)
            return 1
        return 0

    def _pyuiMouseMotion(self, event):
        if self.capture:
            if self.hit(event.pos):
                if self.status != Button.DOWN:
                    self.status = Button.DOWN
                    self.setDirty()
            else:
                if self.status == Button.DOWN:
                    self.status = Button.ROLLOVER
                    self.setDirty()
            return 1

        if self.hit(event.pos):
            if self.status != Button.ROLLOVER:
                getDesktop().getTheme().setButtonCursor()
                self.status = Button.ROLLOVER
                self.setDirty()
                return 0
        else:
            if self.status == Button.ROLLOVER:
                getDesktop().getTheme().setArrowCursor()
                self.status = Button.IDLE
                self.setDirty()
                return 0
        return 0            

    def _pyuiMouseDown(self, event):
        if not self.hit(event.pos):
            return 0
        if not self.enabled:
            return 1
        self.status = Button.DOWN
        self.getFocus()
        self.capture = 1
        self.setDirty()
        return 1

    def _pyuiMouseUp(self, event):
        if self.capture:
            self.capture = 0
            if self.status == Button.DOWN and self.enabled:
                self.postEvent(pyui.locals.CLICKED)
                self.status = Button.IDLE
                self.setDirty()
            return 1
        return 0

    def _pyuiKeyDown(self, event):
        if not self.hasFocus():
            return 0
        if event.key == pyui.locals.K_SPACE and self.enabled:
            self.status = Button.DOWN
            self.setDirty()
            return 1
        return 0

    def _pyuiKeyUp(self, event):
        if not self.hasFocus():
            return 0
        if event.key == pyui.locals.K_SPACE and self.enabled:
            self.postEvent(pyui.locals.CLICKED)
            self.status = Button.IDLE
            self.setDirty()
            return 1
        return 0 

    def enable(self):
        self.enabled = 1
        self.setDirty(1)

    def disable(self):
        self.enabled = 0
        self.setDirty(1)

class ImageButton(Button):
    """Same as regular button except it has an image instead of text.
    """
    widgetLabel = "ImageButton"    
    def __init__(self, filename, handler, text = "", ghostFilename = ""):
        Button.__init__(self, text, handler)
        self.filename = filename
        self.ghostFilename = ghostFilename

    def draw(self, renderer):
        if self.show:
            if self.enabled or self.ghostFilename == "":
                getTheme().drawImageButton(self.windowRect, self.filename, self.text, self.hasFocus(), self.status )
            else:
                getTheme().drawImageButton(self.windowRect, self.ghostFilename, self.text, self.hasFocus(), self.status )

    def setFilename(self, filename):
        self.filename = filename
        self.setDirty(1)
        
class Edit(Base):
    """Edit box. accepts input from user. some emacs-like editing functionality.
    """
    canTab = 1     
    widgetLabel = "Edit"       
    def __init__(self,text, max, handler):
        Base.__init__(self)
        self.handler = handler
        self.caretPos = None
        self.selectPos = None
        self.setText(text)        
        self.dragging = 0
        self.max = max
        self.resize(self.width, int(getTheme().defaultTextHeight*1.5))
        #print "Edit widget sized to", self.width, self.height
        self.registerEvent(pyui.locals.KEYDOWN, self._pyuiKeyDown)
        self.registerEvent(pyui.locals.CHAR, self._pyuiChar)
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self._pyuiMouseDown)
        self.registerEvent(pyui.locals.LMOUSEBUTTONUP, self._pyuiMouseUp)
        self.registerEvent(pyui.locals.MOUSEMOVE, self._pyuiMouseMotion)
        self.registerEvent(pyui.locals.CLICKED, self._pyuiClicked)

    def draw(self, renderer):
        getTheme().drawEdit(self.windowRect,self.text, self.hasFocus(), self.caretPos, self.selectPos) 
            
    def setText(self, text):
        """external function to set the text and move the caret to the end"""
        if not text:
            self.text = ""
            self.caretPos = 0
            self.selectPos = 0
            return
        self.text = text
        self.caretPos = len(text)
        self.selectPos = self.caretPos
        self.setDirty()

    def loseFocus(self):
        Base.loseFocus(self)
        self.caretPos = 0
        self.selectPos = 0

    def getFocus(self):
        Base.getFocus(self)
        self.selectPos = len(self.text)
        self.caretPos = len(self.text)

    def findMousePos(self, pos):
        # put hit position in window relative coords
        x = pos[0] - self.rect[0]
        y = pos[1] - self.rect[1]
                
        # find the horizontal position within the text by binary search
        l,r = 0, len(self.text)
        c = 0
        while l < r:
            c = (l + r + 1) / 2
            w = getRenderer().getTextSize(self.text[l:c])[0]
            if x >= w:
                l = c
                x -= w
            else:
                if r == c:
                    if x > w / 2:
                        l = l + 1
                    break
                r = c
        return l                
        
    def _pyuiMouseDown(self, event):
        if not self.hit(event.pos):
            return 0

        self.getFocus()
        self.caretPos = self.findMousePos(event.pos)
        self.selectPos = self.caretPos
        self.dragging = 1
        self.setDirty()
        return 1

    def _pyuiMouseMotion(self, event):
        if not self.dragging:
            return 0
        self.caretPos = self.findMousePos(event.pos)
        self.setDirty()
        return 1

    def _pyuiMouseUp(self, event):
        if not self.dragging:
            return 0
        self.caretPos = self.findMousePos(event.pos)
        self.dragging = 0
        self.setDirty()
        return 1

    def _pyuiClicked(self, event):
        if event.id == self.id:
            ret = 0
            if self.handler:
                ret = self.handler(self)
            # Should we clear the text here??
            #self.setText("")
            return ret

    def deleteSelected(self):
        if self.selectPos == self.caretPos:
            return
        if self.caretPos > self.selectPos:
            (self.caretPos, self.selectPos) = (self.selectPos, self.caretPos)

        self.text = self.text[:self.caretPos] + self.text[self.selectPos:]
        self.setDirty()
        self.selectPos = self.caretPos

    def _pyuiKeyDown(self, event):
        if not self.hasFocus():
            return 0

        if event.key == pyui.locals.K_LEFT:
            if self.caretPos > 0:
                self.caretPos -= 1
            if (event.mods & pyui.locals.MOD_CONTROL):
                while self.caretPos > 0 and self.text[self.caretPos - 1].isspace():
                    self.caretPos -= 1
                while self.caretPos > 0 and not self.text[self.caretPos - 1].isspace():
                    self.caretPos -= 1
            if not (event.mods & pyui.locals.MOD_SHIFT):
                self.selectPos = self.caretPos
            self.setDirty()
            return 1

        if event.key == pyui.locals.K_RIGHT:
            if self.caretPos < len(self.text):
                self.caretPos += 1
            if (event.mods & pyui.locals.MOD_CONTROL):
                while self.caretPos < len(self.text) and not self.text[self.caretPos].isspace():
                    self.caretPos += 1
                while self.caretPos < len(self.text) and self.text[self.caretPos].isspace():
                    self.caretPos += 1
            if not (event.mods & pyui.locals.MOD_SHIFT):
                self.selectPos = self.caretPos
            self.setDirty()
            return 1

        if event.key == pyui.locals.K_HOME:
            self.caretPos = 0
            if not (event.mods & pyui.locals.MOD_SHIFT):
                self.selectPos = self.caretPos
            self.setDirty()
            return 1

        if event.key == pyui.locals.K_END:
            self.caretPos = len(self.text)
            if not (event.mods & pyui.locals.MOD_SHIFT):
                self.selectPos = self.caretPos
            self.setDirty()
            return 1

        if event.key == pyui.locals.K_BACKSPACE:
            if self.selectPos != self.caretPos:
                self.deleteSelected()
            elif self.caretPos > 0:
                self.text = self.text[:self.caretPos-1] + self.text[self.caretPos:]
                self.caretPos -= 1
                self.selectPos = self.caretPos
            self.setDirty()
            return 1

        if event.key == pyui.locals.K_DELETE:
            if self.selectPos != self.caretPos:
                self.deleteSelected()
            elif self.caretPos < len(self.text):
                self.text = self.text[:self.caretPos] + self.text[self.caretPos+1:]
                self.selectPos = self.caretPos
            self.setDirty()
            return 1

        if event.key == pyui.locals.K_RETURN:
            # invoke handler
            self.postEvent(pyui.locals.CLICKED)
            return 1

        # handle key presses - these are really handled by onChar
        if event.key >= 32 and event.key < 128:
            # add regular text to the box
            self.text = self.text[:self.caretPos] + chr(event.key) + self.text[self.caretPos:]
            self.caretPos += 1
            self.selectPos = self.caretPos
            self.setDirty()
            
            return 1
        
        return 0

    def _pyuiChar(self, event):
        if not self.hasFocus():
            return 0

        if ord(event.key) < 32 or ord(event.key) > 128:
            return 0

        if self.caretPos != self.selectPos:
            self.deleteSelected()

        if len(self.text) > self.max:
            return 1

        # add regular text to the box
        self.text = self.text[:self.caretPos] + event.key + self.text[self.caretPos:]
        self.caretPos += 1
        self.selectPos = self.caretPos
        self.setDirty()
        return 1

class NumberEdit(Edit):
    """I am an edit box that will only take numbers as input.  I will only allow
    decimel points if asked to, and will only allow the first character to be a minus sign.
    """
    def __init__(self,text, max, handler, allowDecimels):

        Edit.__init__(self, text, max, handler)

        self.allowDecimels = allowDecimels
        self.getValue()   # just check the initial data to see if it's legal
        
    def _pyuiChar(self, event):
        if not self.hasFocus():
            return 0
                
        ordValue = ord(event.key)
        if ordValue < 45 or ordValue == 47 or ordValue > 57:
            return 0

        if ordValue == 45 and len(self.text) != 0:
            return 0  # only the first character can be a minus sign.

        if ordValue == 46:  # only one decimel point allowed.
            if self.allowDecimels == 0:
                return 0
            
            for char in self.text:
                if char == '.':
                    return 0

        return Edit._pyuiChar(self, event)    

    def getValue(self):
        """Returns the integer or real version of this data.
        """
        if self.text == "":
            self.text = "0"
            self.setDirty()            
        
        try:
            if self.allowDecimels:
                return float(self.text)
            else:
                return int(self.text)
        except ValueError:
            raise 'NumberEdit got an invalid value in it somehow ("' + self.text + '")'
            return 0

    def setValue(self, value):
        try:
            self.setText(str(value))
        except:
            raise 'NumberEdit could not convert value to string'
    

class Password(Edit):
    widgetLabel = "Password"       
    def draw(self, renderer):
        hidden = len(self.text) * "*"
        getTheme().drawEdit(self.windowRect, hidden, self.hasFocus(), self.caretPos, self.selectPos)        


class Scroll(Base):
    """Base scroll bar.
    """
    widgetLabel = "Scroll"           
    def __init__(self):
        self.scrollPos = 0 # pixel position of scroll bar
        self.currentItem = 0
        self.status = 0
        self.barSize = 1
        self.barSpace = 1
        self.numItems = 1
        self.numVisible = 1
        self.interval = 1
        Base.__init__(self)
        self.setupBar()
        self.setupPos()
        self.start = 0
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self._pyuiMouseDown)
        self.registerEvent(pyui.locals.LMOUSEBUTTONUP, self._pyuiMouseUp)
        self.registerEvent(pyui.locals.MOUSEMOVE, self._pyuiMouseMotion)

    def setNumItems(self, numItems, numVisible):
        if numItems == 0:
            self.numItems = 1
        else:
            self.numItems = numItems
        self.numVisible = numVisible
        self.setupBar()
        self.setupPos()
        self.setDirty(1)
        
    def setupBar(self):
        if self.alignment == 'v':
            self.barSpace = self.height - (getTheme().getScrollerSize()*2 + 2)
        else:
            self.barSpace = self.width - (getTheme().getScrollerSize()*2 + 2)
	
        if self.barSpace < 1:
            self.barSpace = 1
	
        if self.numItems < self.numVisible:
            self.barSize = self.barSpace
        else:
            self.barSize = self.barSpace * self.numVisible / self.numItems
        if self.barSize < 5:
            self.barSize = 5
	
        if self.scrollPos > self.barSpace - self.barSize:
            self.scrollPos = max( self.barSpace - self.barSize, 0 )

    def setupPos(self):
        self.pos = getTheme().getScrollerSize()+ 1 + self.scrollPos

        if self.barSpace == self.barSize:
            item = 0
        else:
            item = (self.scrollPos/float((self.barSpace-self.barSize))) * (self.numItems-self.numVisible)
            if item >= self.numItems - self.numVisible:
                item = self.numItems - self.numVisible
        if item != self.currentItem:
            e = self.postEvent(pyui.locals.SCROLLPOS)
            e.pos = int(item)
            self.currentItem = int(item)

    def draw(self, renderer):
        if not self.show:
            return
            
        theme = getTheme()
        theme.drawScrollBack(self.windowRect)
        if self.alignment == 'v':
            theme.drawScrollButtonUp((self.windowRect[0],self.windowRect[1],self.width,getTheme().getScrollerSize()) )
            theme.drawScrollButtonDown((self.windowRect[0],self.windowRect[1]+self.height-getTheme().getScrollerSize(),self.width,getTheme().getScrollerSize()))
            theme.drawScrollBar((self.windowRect[0], self.windowRect[1]+self.pos, self.width, self.barSize))
        else:
            theme.drawScrollButtonUp((self.windowRect[0],self.windowRect[1],getTheme().getScrollerSize(),self.height) )
            theme.drawScrollButtonDown((self.windowRect[0]+self.width-getTheme().getScrollerSize(),self.windowRect[1],getTheme().getScrollerSize(),self.height))
            theme.drawScrollBar((self.windowRect[0]+self.pos, self.windowRect[1], self.barSize, self.height))
        
    def _pyuiMouseDown(self, event):
        if not self.hit(event.pos):
            self.status = 0
            return 0
        localpos = (event.pos[0] - self.rect[0], event.pos[1] - self.rect[1])
        
	if self.alignment == 'v':
            p = localpos[1]
            extent = self.height
	else:
            p = localpos[0]
            extent = self.width
        
	if p < getTheme().getScrollerSize():     # up button scroll
            if self.currentItem > 0: self.scrollToItem( self.currentItem - 1 )
            return 1
        if p > extent - getTheme().getScrollerSize():    # down button scroll
            if self.currentItem < self.numItems: self.scrollToItem( self.currentItem + 1 )
            return 1
        if self.pos < p < self.pos + self.barSize:     # <-- *** Just cleaned comparsion
            self.status = 1 # we are scrolling
            self.start = p
            self.setDirty(1)
            return 1
        else:     # unnocupied bar space
            self.scrollPos = p - getTheme().getScrollerSize() - 1 - self.barSize/2    #center bar on cursor position
            if self.scrollPos < 0:
                self.scrollPos = 0
            elif self.scrollPos > self.barSpace - self.barSize:
                self.scrollPos = self.barSpace - self.barSize
            self.setupPos()
            self.status = 1
            self.start = p
            self.setDirty(1)
            return 1

    def _pyuiMouseUp(self, event):
        if not self.hit(event.pos):        
            self.status = 0
            return 0
        self.status = 0

    def _pyuiMouseMotion(self, event):
        if self.status:
            localpos = (event.pos[0] - self.rect[0], event.pos[1] - self.rect[1])            
	
	    if self.alignment == 'v':
                p = localpos[1]
	    else:
                p = localpos[0]
        
            diff = p - self.start
            self.scrollPos = self.scrollPos + diff
            if self.scrollPos < 0:
                self.scrollPos = 0
            elif self.scrollPos > self.barSpace - self.barSize:
                self.scrollPos = self.barSpace - self.barSize
            self.start = p
            self.setupPos()
            self.setDirty(1)
            return 1

    def resize(self, w, h):
        Base.resize(self, w, h)
        self.setupBar()
        self.setupPos()
        
    def calcSize(self):
        Base.calcSize(self)
        self.setupBar()
        self.setupPos()

    def scrollToItem(self, itemNum):
        #curr = (self.scrollPos/float(self.barSpace)) * self.numItems
	if int(self.currentItem) != int(itemNum):
            self.scrollPos = (itemNum/float(self.numItems)) * float(self.barSpace)
            if self.scrollPos < 0:
                self.scrollPos = 0
            elif self.scrollPos > self.barSpace - self.barSize:
                self.scrollPos = self.barSpace - self.barSize
            self.setupPos()

class VScroll(Scroll):
    """Vertical scroll bar.
    """
    def __init__(self):
        self.alignment = 'v'
        Scroll.__init__(self)

class HScroll(Scroll):
    """Horizontal scroll bar.
    """
    def __init__(self):
        self.alignment = 'h'
        Scroll.__init__(self)

        
class ListBoxItem:
    """Used by ListBox to track items.
    """
    def __init__(self, name, data, fg, bg):
        self.name = name
        self.data = data
        self.color = fg

class ListBox(Base):
    """List Box has a scrollable list of selectable items.
       List box behavior should incorporate the right mouse button -BrianU 10-31-02
    """
    canTab = 1    
    widgetLabel = "ListBox"               
    def __init__(self, onSelected = None, onDouble = None):
        Base.__init__(self)
        self.items = []
        self.numVisible = self.height / getTheme().defaultTextHeight
        self.numItems = 0
        self.topItem = 0
        self.selected = -1
        self.vscroll = VScroll()
        self.addChild(self.vscroll)
        self.registerEvent(pyui.locals.SCROLLPOS, self._pyuiScrollPos)
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self._pyuiLButtonDown)
        self.registerEvent(pyui.locals.RMOUSEBUTTONDOWN, self._pyuiLButtonDown)
        self.registerEvent(pyui.locals.LMOUSEBUTTONUP, self._pyuiLButtonUp)
        self.registerEvent(pyui.locals.RMOUSEBUTTONUP, self._pyuiLButtonUp)
        self.registerEvent(pyui.locals.LMOUSEDBLCLICK, self._pyuiDoubleClick)

        self.registerEvent(pyui.locals.LIST_SELECTED, self._pyuiSelectEvent)
        self.registerEvent(pyui.locals.LIST_DBLCLICK, self._pyuiDoubleEvent)
        
        self.resize(100,100)

        self.selectHandler = onSelected
        self.doubleHandler = onDouble

    def clearAllItems(self):
        self.items = []
        self.numVisible = self.height / getTheme().defaultTextHeight
        self.numItems = 0
        self.topItem = 0
        self.selected = -1
        
    def populateList(self, items):
    	for item in items:
    		self.addItem(item, None)    		
    	self.sortByName()
    
    def addItem(self, itemText, itemData, color = None):
        """add an item to the list box. the data value is stored for the item
        and will be available when events occur on that item.
        """
        if color == None:
            color = self.fgColor
        item = ListBoxItem(itemText, itemData, color, color)
        self.items.append(item)
        self.numItems = len(self.items)
        self.vscroll.setNumItems(self.numItems, self.numVisible)
        self.setDirty()

    def removeItem(self, itemText):
        i = 0
        for item in self.items:
            if item.name == itemText:
                if i <= self.selected:
                    self.selected -= 1
                self.items.pop(i)
                #print "removed %s" % itemText
                break
            #print itemText, item.name
            i = i + 1
        self.numItems = len(self.items)
        self.vscroll.setNumItems(self.numItems, self.numVisible)
        self.setDirty()
            
    def removeItemByData(self, itemData):
        i = 0
        for item in self.items:
            if item.data == itemData:
                if i <= self.selected:
                    self.selected -= 1
                self.items.pop(i)
                break
            i = i + 1
        self.numItems = len(self.items)
        self.vscroll.setNumItems(self.numItems, self.numVisible)
        self.setDirty()

    def getItemByData(self, itemData):
        for item in self.items:
            if item.data == itemData:
                return item
        return None

    def draw(self, renderer):
        if self.show:
            getTheme().drawListBox(self.windowRect)
            i = 0
            for item in self.items:
                if i >= self.topItem and i < self.topItem + self.numVisible:
                    if i == self.selected:
                        getTheme().drawListBoxItem( (self.windowRect[0]+1,
                                                     self.windowRect[1]+2 + (i-self.topItem) * getTheme().defaultTextHeight,
                                                     self.width - getTheme().getScrollerSize(),
                                                     getTheme().defaultTextHeight-2),
                                                    item.name, 1, item.color)
                    else:
                        getTheme().drawListBoxItem( (self.windowRect[0]+1,
                                                     self.windowRect[1]+2 + (i-self.topItem) * getTheme().defaultTextHeight,
                                                     self.width - getTheme().getScrollerSize(),
                                                     getTheme().defaultTextHeight-2),
                                                    item.name, 0, item.color)                    
                i = i + 1
            self.vscroll.draw(renderer)
        
    def _pyuiScrollPos(self, event):
        if event.id == self.vscroll.id:
            self.topItem = event.pos
            self.setDirty()

    def _pyuiLButtonDown(self, event):
        if not self.hit(event.pos):
            return 0
        item =  int( (event.pos[1] - self.rect[1]) / getTheme().defaultTextHeight )
        self.selected = item + self.topItem
        self.postEvent(pyui.locals.LIST_SELECTED)
        self.setDirty()
        return 0

    def _pyuiLButtonUp(self, event):
        return 0

    def _pyuiDoubleClick(self, event):
        if not self.hit(event.pos):
            return 0
        item =  int( (event.pos[1] - self.rect[1]) / getTheme().defaultTextHeight )
        self.selected = item + self.topItem
        self.postEvent(pyui.locals.LIST_DBLCLICK)
        self.setDirty()
        return 1
        
    def getSelectedItem(self):
        if self.selected > -1 and self.selected < len(self.items):
            return self.items[self.selected]
        return None

    def setSelectedItem(self, name):
        i=0
        for item in self.items:
            if item.name == name:
                self.selected = i
                self.setDirty()
                break
            i = i + 1
            
    def resize(self, w, h):
        Base.resize(self, w, h)
        self.numVisible = int (self.height / getTheme().defaultTextHeight )
        self.vscroll.setNumItems(self.numItems, self.numVisible)
        self.vscroll.resize(getTheme().getScrollerSize(), h)
        self.vscroll.moveto(w-getTheme().getScrollerSize(), 0)

    def clearSelection(self):
        self.selected = -1
        self.setDirty()
    
    def clear(self):
        self.items = []
        self.numItems = 0
        self.vscroll.setNumItems(0, self.numVisible)
        self.selected = -1
        self.setDirty()

    def _pyuiSelectEvent(self, event):
        if event.id == self.id:
            if self.selectHandler:
                self.selectHandler( self.getSelectedItem() )
                return 1
        return 0

    def _pyuiDoubleEvent(self, event):
        if event.id == self.id:
            if self.doubleHandler:
                self.doubleHandler( self.getSelectedItem() )
                return 1
        return 0

    def sortByName(self):
        self.items.sort(self.itemCompareByName)
        self.setDirty()

    def sortByData(self):
        self.items.sort(self.itemCompareByData)
        self.setDirty()
        
    def itemCompareByName(self, item1, item2):
        return cmp(item1.name, item2.name)

    def itemCompareByData(self, item1, item2):
        return cmp(item1.data, item2.data)
    
class DropDownBox(Base):
    """A drop-down selection box. Pass the number of lines to be visible in the drop-down
    list to the constructor.
    """
    BUTTON_WIDTH = 10
    def __init__(self, numVisible = 10, onSelected = None):
        Base.__init__(self)
        self.numVisible = numVisible
        self.selectionList = ListBox(self._pyuiSelectedEvent)
        self.selectionList.setShow(0)
        self.addChild(self.selectionList)
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self._pyuiMouseDown)
        self.selectHandler = onSelected
        
    ## pass-through methods to the list box
        
    def addItem(self, itemText, itemData, color = None):
        self.selectionList.addItem(itemText, itemData, color)
        self.selectionList.selected = len(self.selectionList.items) -1

    def getSelectedItem(self):
        return self.selectionList.getSelectedItem()

    def removeItem(self, text):
        self.selectionList.removeItem(text)

    def clearSelection(self):
        self.selectionList.clearSelection()

    def clear(self):
        self.selectionList.clear()
        
    def draw(self, renderer):
        """Draw the selected item at all times, and draw the list when expanded.
        """
        if self.selectionList.selected == -1:
            text = ""
        else:
            item =  self.selectionList.getSelectedItem()
            if item:
                text = item.name
            else:
                text = "None"
        #getTheme().drawBack3( self.windowRect )
        getTheme().drawLabel( (self.windowRect[0], self.windowRect[1], self.width - self.BUTTON_WIDTH, self.height), text)
        getTheme().drawButton( (self.windowRect[0] + self.width - self.BUTTON_WIDTH, self.windowRect[1], self.BUTTON_WIDTH, self.height), "V", 0, 0, 1)
        if self.selectionList.show:
            self.window.addDrawCallback(self.selectionList.draw)

    def _pyuiMouseDown(self, event):
        if not self.hit(event.pos):
            if self.selectionList.show:
                self.selectionList.setShow(0)
            return 0
        x = event.pos[0] - self.rect[0]
        y = event.pos[1] - self.rect[1]
        if x > self.width - self.BUTTON_WIDTH and x < self.width:
            if self.selectionList.show == 0:
                self.positionSelectionList()            
                self.selectionList.setShow(1)
            else:
                self.selectionList.setShow(0)
        return 1

    def _pyuiSelectedEvent(self, item):
        self.item = item
        getDesktop().addCallback(self.hideMe, 0.33)
        return 1

    def hideMe(self, interval):
        if self.selectHandler:
            self.selectHandler(self.item)
        self.selectionList.setShow(0)
        return 1
    
    def resize(self, width, height):
        Base.resize(self, width, height)
        self.positionSelectionList()

    def positionSelectionList(self):
        self.selectionList.resize(self.width, self.numVisible * getTheme().defaultTextHeight)
        self.selectionList.moveto(0, self.height)

class CheckBox(Base):
    """A checkbox that has two states - on and off. It toggles between them when clicked.
    onCheck is a method to be called when the checkbox changes state.
    """
    def __init__(self, text, onCheck = None):
        self.checkState = 0
        self.text = text
        self._pyuiCheck = onCheck
        Base.__init__(self)
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self._pyuiMouseDown)

    def setCheck(self, value):
        if value:
            self.checkState = 1
        else:
            self.checkState = 0

        if self._pyuiCheck:
            self._pyuiCheck(self.checkState)
        self.setDirty(1)

    def _pyuiMouseDown(self, event):
        if not self.hit(event.pos):
            return 0
        if self.checkState:
            self.setCheck(0)
        else:
            self.setCheck(1)
        return 1

    def draw(self, renderer):
        getTheme().drawCheckBox(self.windowRect, self.text, self.checkState)
        
class SliderBar(Base):
    """A horizontal slider bar. Has a slider handle that the user can drag to change its value.
    the onSlide method will be called when the value of the slider changes.
    """
    BARWIDTH = 8
    def __init__(self, onSlide, range = 1, initialPos = 1):
        self.range = range
        self.position = initialPos
        self.sliding = 0
        self.slidePos = 0
        self.stepInterval = 1
        self._pyuiSlide = onSlide
        Base.__init__(self)
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self._pyuiMouseDown)
        self.registerEvent(pyui.locals.LMOUSEBUTTONUP, self._pyuiMouseUp)
        self.registerEvent(pyui.locals.MOUSEMOVE, self._pyuiMouseMotion)

    def resize(self, w, h):
        Base.resize(self,w,h)
        self.stepInterval = (float(self.width) - self.BARWIDTH) / self.range
        
    def draw(self, renderer):
        getTheme().drawSliderBar(self.windowRect, self.range, self.position, self.BARWIDTH)

    def setValue(self, newValue):
        if newValue < 0 or newValue > self.range:
            return
        self.position = newValue
        self.setDirty(1)
        
    def _pyuiMouseDown(self, event):
        if not self.hit(event.pos):
            return 0
        if self.sliding:
            return 0
        x = event.pos[0] - self.rect[0]
        barpos = self.stepInterval * self.position
        if x > barpos and x < barpos + self.BARWIDTH:
            self.sliding = 1
            self.slidePos = x
            return 1
        return 0

    def _pyuiMouseUp(self, event):
        if not self.sliding:
            return 0
        self.sliding = 0
        return 1

    def _pyuiMouseMotion(self, event):
        if not self.sliding:
            return 0
        x = event.pos[0] - self.rect[0]            
        diff = x - self.slidePos 
        newPosition = ((self.position * self.stepInterval) + diff) / self.stepInterval
        realdiff = newPosition - self.position
        if abs(realdiff) > 1:
            self.position = self.position + int(realdiff)
            if self.position < 0:
                self.position = 0
            if self.position >= self.range:
                self.position = self.range

            if self._pyuiSlide:
                self._pyuiSlide(self.position)
            self.slidePos = self.position * self.stepInterval
            self.setDirty(1)
            return 1
        return 0


        
class TabbedPanel(Panel):
    """A panel with multiple panels that are activated by tabs along the top of the panel.
    The inner panels can be created by this panel or existing panels can be added in.
    """
    #tabsHeight = getTheme().defaultTextHeight + 8
    
    def __init__(self):
        Panel.__init__(self)
        self.activePanel = None
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self._pyuiMouseDown) 
        self.tabPanels = {}             # mapping of titles to panels
        self.activePanel = None
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self._pyuiMouseDown)
        self.registerEvent(pyui.locals.KEYDOWN, self._pyuiKeyDown)        

    def removePanel(self, title):
        """Remove an existing panel by its name.
        """
        self.removeChild(self.tabPanels[title])
        del self.tabPanels[title]

    def addPanel(self, title, panel = None):
        """Can add an existing panel, or have a panel created by default.
        Adding an existing panel is useful for adding special panel types (splitters/tabs)
        """
        if not panel:
            panel = Panel()
        self.addChild(panel)
        panel.moveto(0, getTheme().getTabsHeight())
        panel.resize(self.width, self.height-getTheme().getTabsHeight())
        panel.calcSize()
        
        panel.tabTitle = title
        self.tabPanels[title] = panel
        if not self.activePanel:
            self.activePanel = panel
            self.setDirty()
        return panel

    def getPanel(self, number):
        return self.children[number]
    
    def resize(self, w, h):
        """Only resize the current tab. other tabs are resized when switched to later if it is required."""
        #print "tabbed panel resizing ", w, h, self
        Base.resize(self, w, h)
        if self.activePanel:
            self.activePanel.resize(w, h-getTheme().getTabsHeight())

    def pack(self):
        # dont pack self.. just pack the child panels.
        for p in self.children:
            p.pack()

    def draw(self, renderer):
        #draw tabs
        #getTheme().drawBack1(self.windowRect)
        x = self.windowRect[0]
        h = getTheme().getTabsHeight()
        for p in self.children:
            rect = getTheme().drawTabItem((x, self.windowRect[1] + 2, 0, h), p.tabTitle, p == self.activePanel)
            p.tabRect = rect
            x = rect[0] + rect[2]

        # draw remaining tab header
        getTheme().drawTabHeader((x, self.windowRect[1] + 2, self.windowRect[2] - x, h))
        # draw active panel
        if self.activePanel:
            self.activePanel.draw(renderer)

    def _pyuiMouseDown(self, event):
        if not self.hit(event.pos):
            return 0
        x = event.pos[0]# - self.rect[0]
        y = event.pos[1] - self.rect[1]
        if 0 <= y < getTheme().getTabsHeight():
            for p in self.children:
                if p.tabRect[0] <= x < p.tabRect[0]+p.tabRect[2]:
                    #hit! activate this tab
                    self.activatePanel(p)
                    return 0
        self.getFocus()
        return 0

    def activatePanel(self, p):
        if p != self.activePanel:
            for panel in self.children:
                if panel == p:
                    panel.setShow(1)
                else:
                    panel.setShow(0)
            self.activePanel = p
            # resize the tab if required.
            if p.width != self.width or p.height != self.height-getTheme().getTabsHeight():
                p.resize(self.width, self.height-getTheme().getTabsHeight())
            self.setDirty()
            p.getFocus()

    def nextPanel(self):
        for i in range(0,len(self.children)):
            if self.children[i] == self.activePanel:
                if i == len(self.children)-1:
                    print "activating" , 0, self.children[0]                    
                    self.activatePanel(self.children[0])
                    break
                else:
                    print "activating" , i, self.children[i+1]
                    self.activatePanel(self.children[i+1])
                    break
                       
    def _pyuiKeyDown(self, event):
        if event.mods & pyui.locals.MOD_CONTROL:
            if event.key == pyui.locals.K_TAB:
                self.nextPanel()
                return 1
            number = event.key - ord('0') - 1
            if number >= 0 and number < 10:
                if number < len(self.children):
                    self.activatePanel(self.children[number])
                    return 1
                    
        return 0
    
    def handleEvent(self, event):
        if not self.show:
            return
        if not self.children:
            return 0
        if self.activePanel.handleEvent(event):
            return 1
        if self.eventMap.has_key(event.type):
            if self.eventMap[event.type](event):
                return 1
        return 0

    def getFocus(self):
        # hand off focus to active panel
        if self.activePanel:
            self.activePanel.getFocus()

    def checkHit(self, pos):
        #pos = (pos[0] - self.rect[0], pos[1] - self.rect[1])
        #print self, self.windowRect, self.rect, pos
        if pos[0] > self.rect[0] and \
           pos[0] < self.rect[0] + self.rect[2] and \
           pos[1] > self.rect[1] and \
           pos[1] < self.rect[1] + self.rect[3]:
        
            result = self.activePanel.checkHit(pos)
            if result:
                return result
            return self
        else:
            return None

class SplitterPanel(Panel):
    """A panel that is split in half - vertically or horizontally.
    Can use pixels or percentage to split. Each side of the split is a panel.
    The default panels can be replaced with custom panels.

    There is a middle bar of the splitter panel. This middle bar _could_ be used
    to resize it...
    """

    VERTICAL = 0
    HORIZONTAL = 1

    PIXELS = 0
    PERCENTAGE = 1

    PADDING = 2
    
    def __init__(self, direction = VERTICAL, method = PERCENTAGE, ratio = 50 ):
        self.direction = direction  # vertical/horizontal
        self.method = method        # pixels/percentage
        self.ratio = ratio          # number of pixels or percentage
        self.splitPos = 0           # pixel width/height of first panel
        if self.method == SplitterPanel.PERCENTAGE:
            self.ratio = float(ratio) / 100.0
        Panel.__init__(self)
        self.panel1 = Panel()
        self.panel2 = Panel()
        self.addChild(self.panel1)
        self.addChild(self.panel2)

    def setVerticalSplit(self, x):
        self.panel1.moveto(0,0)
        self.panel1.resize(x-self.PADDING, self.height)
        self.panel2.moveto(x+self.PADDING, 0)
        self.panel2.resize(self.width-x-self.PADDING, self.height)
        self.splitPos = x

    def setHorizontalSplit(self, y):
        self.panel1.moveto(0,0)
        self.panel1.resize(self.width, y-self.PADDING)
        self.panel2.moveto(0, y+self.PADDING)
        self.panel2.resize(self.width, self.height-y-self.PADDING)
        self.splitPos = y

    def resize(self, w, h):
        Base.resize(self, w,h)
        #print "splitter resizing", w, h
        if self.method == SplitterPanel.PIXELS:
            if self.direction == SplitterPanel.VERTICAL:
                if self.ratio >= 0:
                    self.setVerticalSplit(self.ratio)
                else:
                    self.setVerticalSplit(self.width + self.ratio)
            if self.direction == SplitterPanel.HORIZONTAL:
                if self.ratio >= 0:
                    self.setHorizontalSplit(self.ratio)
                else:
                    self.setHorizontalSplit(self.height + self.ratio)
        else:
            if self.direction == SplitterPanel.VERTICAL:
                self.setVerticalSplit(self.width * self.ratio)
            if self.direction == SplitterPanel.HORIZONTAL:
                self.setHorizontalSplit(self.height * self.ratio)

    def draw(self, renderer):
        self.panel1.draw(renderer)

        if self.direction == SplitterPanel.HORIZONTAL:
            getTheme().drawSplitter(
                                 (self.windowRect[0], self.windowRect[1]+self.splitPos-self.PADDING, self.width, self.PADDING*2))
        else:
            getTheme().drawSplitter(            
                                 (self.windowRect[0]+self.splitPos-self.PADDING, self.windowRect[1], 2*self.PADDING, self.height))
            
        self.panel2.draw(renderer)
        
    def pack(self):
        self.panel1.pack()
        self.panel2.pack()
        
    def getFirstPanel(self):
        """ returns the left or top panel
        """
        return self.panel1

    def getSecondPanel(self):
        """ returns to right or bottom panel
        """
        return self.panel2

    def replaceFirstPanel(self, panel):
        panel.moveto(self.panel1.posX, self.panel1.posY)
        for c in self.children:
            if c.id == self.panel1.id:
                self.children.remove(c)
                c.destroy()
        self.addChild(panel)
        self.panel1 = panel
        self.resize(self.width, self.height)
        
    def replaceSecondPanel(self, panel):
        panel.moveto(self.panel2.posX, self.panel2.posY)
        for c in self.children:
            if c.id == self.panel2.id:
                self.children.remove(c)
                c.destroy()
        self.addChild(panel)
        self.panel2 = panel
        self.resize(self.width, self.height)

class FormPanel(Panel):
    """A Panel that shows data about an object and allows it to be updated.
    The "fields" argument is a list of data fields to populate the panel with. It
    is in the format:
       [ (type, name, label, vspan, data),
         (type, name, label, vspan, data)
       ]

    where type is one the fieldTypes below, vspan is the vertical height of the widget,
    and data is speficic data for the type of form widget to be used.
    """

    fieldTypes = [
        "string",
        "int",
        "text",
        "password",
        "slider",
        "checkbox",
        "list",
        "dropdownlist",
        "label"
        ]
    
    def __init__(self, fieldList):
        self.fieldList = fieldList
        Panel.__init__(self)
        self.object = None
        # setup layout
        num = 0

        span = 0
        for t, n, l, vspan, d in fieldList:
            span = span + vspan
            
        self.setLayout(pyui.layouts.TableLayoutManager( 3, span))
        for fieldType, fieldName, fieldLabel, fieldSpan, fieldData in fieldList:
            newLabel = Label(fieldLabel)
            newWidget = self.createFormWidget(fieldType, fieldData)
            self.addChild( newLabel, (0,num,1,fieldSpan) )
            self.addChild( newWidget, (1,num,2,fieldSpan) )

            self.__dict__["label_%s" % fieldName] = newLabel
            self.__dict__["widget_%s" % fieldName] = newWidget
            
            num = num + fieldSpan
        self.pack()

    def populate(self, object):
        """populate the data fields from the supplied object
        """
        self.object = object
        for fieldType, fieldName, fieldLabel, fieldSpan, fieldDefault in self.fieldList:
            formWidget = self.__dict__["widget_%s" % fieldName]
            value = object.__dict__.get(fieldName, None)
            self.populateFormWidget(fieldType, formWidget, value)
        self.setDirty(1)

    def process(self):
        """This takes the data in the form and updates it into the source object.
        This assumes that the form has already been populated...
        """
        for fieldType, fieldName, fieldLabel, fieldSpan, fieldData in self.fieldList:
            formWidget = self.__dict__["widget_%s" % fieldName]
            self.processFormWidget(fieldType, fieldName, formWidget)
        
    def createFormWidget(self, fieldType, fieldData):
        """Create the right kind of widget based on the fieldType.
        """
        tmp = "create_%s" % fieldType
        createMethod = getattr(self, tmp)
        if not createMethod:
            raise "No widget of type: %s" % tmp
        return createMethod(fieldData)

    def populateFormWidget(self, fieldType, formWidget, value):
        tmp = "populate_%s" % fieldType
        populateMethod = getattr(self, tmp)
        if not populateMethod:
            raise "No widget of type: %s" % fieldType
        return populateMethod(formWidget, value)

    def processFormWidget(self, fieldType, fieldName, formWidget):
        if not self.object:
            raise "No object to process to!"
        tmp = "process_%s" % fieldType
        processMethod = getattr(self, tmp)
        if not processMethod:
            raise "No process method for %s" % fieldType
        return processMethod(formWidget, fieldName)
    
    ##### Widget Creation Methods. #####
    
    def create_string(self, size):
        return Edit("", size, self._pyuiEdit)

    def create_password(self, size):
        return Password("", size, self._pyuiEdit)

    def create_int(self, dummy):
        return NumberEdit("", 12, self._pyuiEdit, 0)

    def create_text(self, size):
        #NOTE: make this a LineDisplay that can be edited...
        return Edit("", size, self._pyuiEdit)

    def create_slider(self, range):
        return SliderBar(self._pyuiSlide, range)

    def create_checkbox(self, title):
        return CheckBox(title, self._pyuiCheck)
    
    def create_list(self, dummy):
        return ListBox()

    def create_dropdownlist(self, numVisible):
        return DropDownBox(numVisible)

    def create_label(self, dummy):
        return Label("")
    
    ###### Widget Populate Methods. #######

    def populate_string(self, formWidget, value):
        if not value:
            formWidget.setText("None")
        else:
            formWidget.setText("%s" % value)

    populate_password = populate_string
    populate_int = populate_string
    populate_text = populate_string
    populate_label = populate_string
    
    def populate_slider(self, formWidget, value):
        formWidget.position = value

    def populate_checkbox(self, formWidget, value):
        formWidget.setCheck(value)
        
    def populate_list(self, formWidget, items):
        #TODO: make a way to get a text value for an item
        formWidget.clear()
        for item in items:
            formWidget.addItem(repr(item), item)

    populate_dropdownlist = populate_list

    ##### Widget Processing Methods #####

    def process_string(self, formWidget, fieldName):
        setattr(self.object, fieldName, formWidget.text)

    process_text = process_string
    process_password = process_string

    def process_label(self, formWidget, fieldName):
        pass

    def process_list(self, formWidget, fieldName):
        pass

    process_dropdownlist = process_list

    def process_slider(self, formWidget, fieldName):
        setattr(self.object, fieldName, formWidget.position)

    def process_checkbox(self, formWidget, fieldName):
        setattr(self.object, fieldName, formWidget.checkState)
        
    def process_int(self, formWidget, fieldName):
        setattr(self.object, fieldName, int(formWidget.text) )
        
    ##### Widget handler methods ######
    
    def _pyuiSlide(self, value):
        #print "slid to ", value
        pass
            
    def _pyuiEdit(self, edit):
        #print "changing value for ", edit
        return 1

    def _pyuiCheck(self, value):
        #print "checkbox hit"
        pass
            
class ViewPanel(Panel):
    """A rectangle intended as a viewport into a 3D world. The implementation of worlds is very renderer
    specific. The only real constraint placed by PyUI is that the world lifetime is controlled by
    createWorld/destroyWorld. The handle returned from createWorld should be used to identify it in
    any further operations.
    """
    def __init__(self, world, windowHandle, width, height):
        Panel.__init__(self)
        self.viewHandle = getRenderer().createView(world)
        self.world = world
        self.windowHandle = windowHandle

        ## attach to the desktop background windowHandle.
        primView = (pyui.locals.VIEW, 0, 0, width, height, self.viewHandle)

        # add the world view to the desktop window 'container'
        getRenderer().describeWindow(windowHandle, [primView,])

    def destroy(self):
        #print "Destroying view:", self.viewHandle
        self.windowHandle = None
        getRenderer().destroyView(self.viewHandle)
        Panel.destroy(self)        

    def draw(self, renderer):
        renderer.drawView(self.windowRect, self.viewHandle)

    def setEffect(self, effectName):
        getRenderer().setWindowEffect(self.windowHandle, effectName)

    
class Frame(Window):
    """A frame is a window that has a titlebar and borders. it is resizable and movable by dragging the titlebar.
    """
    def __init__(self, x, y, w, h, title, topmost = 0):
        self.theme = getTheme()
        self.innerWidth = w
        self.innerHeight = h
        self.title = title

        self.panelOffsetLeft = 0
        self.panelOffsetTop = 0
        self.panelOffsetRight = 0
        self.panelOffsetBottom = 0            
        
        Window.__init__(self, x, y, w, h, topmost)
        self.setTitle(self.title)
        
        self.panelOffsetLeft = self.theme.getFrameBorderLeft()
        self.panelOffsetTop = self.theme.getFrameBorderTop()
        self.panelOffsetRight = self.theme.getFrameBorderRight()
        self.panelOffsetBottom = self.theme.getFrameBorderBottom()            
            
        self._panel.moveto(self.panelOffsetLeft, self.panelOffsetTop)
        w += self.panelOffsetLeft + self.panelOffsetRight
        h += self.panelOffsetTop + self.panelOffsetBottom
            
        self.resize(w, h)
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self._pyuiMouseDown)
        self.registerEvent(pyui.locals.LMOUSEBUTTONUP, self._pyuiMouseUp)
        self.registerEvent(pyui.locals.MOUSEMOVE, self._pyuiMouseMotion)
        self.moving = 0
        self.resizing = 0
        self.startX = 0
        self.startY = 0
        self.resizingCursor=0
        self.backImage=None

        self.menuBar = None

    def addMenuBar(self, menuBar):
        self.menuBar = menuBar
        
    def setTitle(self, title):
        self.title = title

    def setBackImage(self, filename):
        self.backImage = filename
        #getRenderer().loadImage(filename)
        
    def draw(self, renderer):
        """Draws to the actual frame if the renderer requires it.
        """
        if not self.show:
            return
        self.hitList = getTheme().drawFrame( (0,0,self.width, self.height), self.title, self.backImage)
        Window.draw(self, renderer)        
        
    def replacePanel(self, panel):
        Window.replacePanel(self, panel)
        self._panel.moveto(self.panelOffsetLeft, self.panelOffsetTop)
        self._panel.resize(self.innerWidth, self.innerHeight)


    def hitFrameRegion(self, pos):
        # put hit position in window relative coords
        x = pos[0] - self.rect[0]
        y = pos[1] - self.rect[1]

        # scan through hit regions        
        for (regionId, rect) in self.hitList:
            if x >= rect[0] and y >= rect[1] and x < rect[0]+rect[2] and y < rect[1]+rect[3]:
                return regionId
        else:
            return None

    def _pyuiMouseMotion(self, event):
        if self.moving:
            mouseX = event.pos[0] - self.posX
            mouseY = event.pos[1] - self.posY
            self.move( mouseX - self.startX, mouseY - self.startY)
            getRenderer().moveWindow(self.handle, self.posX, self.posY)
            return 1
        if self.resizing:
            mouseX = event.pos[0] - self.posX
            mouseY = event.pos[1] - self.posY
            if mouseX < 64:
                mouseX = 64
            if mouseY < 64:
                mouseY = 64
            self.resize( self.width + mouseX - self.startX, self.height + mouseY - self.startY)
            (self.startX, self.startY) = (mouseX, mouseY)
            return 1
        if event.pos[0] > self.posX + self.innerWidth and event.pos[1] > self.posY + self.innerHeight and not self.resizingCursor:
            self.resizingCursor=1
            self.theme.setResizeCursor()
        elif self.resizingCursor:
            self.resizingCursor=0
            self.theme.setArrowCursor()            
        
        if not self.hit(event.pos):
            if self.resizingCursor and not self.resizing:
                self.resizingCursor=0
                self.theme.setArrowCursor()                            
            return 0
        else:
            return 1
        
    def _pyuiMouseDown(self, event):
        if not self.hit(event.pos):
            return 0

        self.getFocus()
        regionId = self.hitFrameRegion(event.pos)
        
        # check for closing            
        if regionId == pyui.locals.HIT_FRAME_CLOSE:
            if hasattr(self, "onCloseButton"):
                return self.onCloseButton()
            return self._pyuiCloseButton()
        
        # check for moving
        if regionId == pyui.locals.HIT_FRAME_MOVE:
            self.moving  = 1
            self.startX = event.pos[0] - self.posX
            self.startY = event.pos[1] - self.posY
            return 1
        
        # check for resizing
        if regionId == pyui.locals.HIT_FRAME_RESIZE_BOTTOM_RIGHT:
            self.resizing = 1
            self.startX = event.pos[0] - self.posX
            self.startY = event.pos[1] - self.posY
            return 1

        return 1

    def _pyuiMouseUp(self, event):
        if self.moving:
            self.moving = 0
            return 1
        if self.resizing:
            self.resizing = 0
            return 1
        if self.resizingCursor:
            self.resizingCursor=0
            self.theme.setArrowCursor()                                        
        if not self.hit(event.pos):
            return 0
        return 1

    def resize(self, w, h):
        if w < 64:
            w = 64
        if h < 64:
            h = 64
        Base.resize(self, w, h)
        self.innerWidth = w - self.panelOffsetLeft - self.panelOffsetRight
        self.innerHeight = h - self.panelOffsetTop - self.panelOffsetBottom
        self._panel.moveto(self.panelOffsetLeft, self.panelOffsetTop)
        self._panel.resize(self.innerWidth, self.innerHeight)

    def _pyuiCloseButton(self):
        #print "Destroying window", self
        self.destroy()
        return 1            
        

class MenuItem:
    """Used by menu widget to track items. Can have an icon 16x16 in size.
    """
    def __init__(self, handler, text, subMenu):
        self.handler = handler
        self.text = text
        (width, height) = getRenderer().getTextSize(text, getTheme().defaultFont)
        self.width = width
        self.subMenu = subMenu
        self.icon = None
        self.rect = (0,0,0,0)
        self.canActivate = handler or subMenu

    def setIcon(self, icon):
        self.icon = icon

class Menu(Window):
    """Menu that can be floating or attached to a menuBar.
    """
    iconWidth = 20  # fixme[pmf]: actually defined in theme
    minWidth = 20
    border = 4
    def __init__(self, title):
        Window.__init__(self, 0,0,100,100, topmost = 1)
        self.menuTitle = title
        self.width = self.minWidth
        self.items = []
        self.active = None
        self.subActive = None
        self.setShow(0)
        # no register events; we'll receive events from the menu bar (or other parent menu)

    def draw(self, renderer):
        if self.show:
            getTheme().drawMenu((0,0,self.width, self.height))
            y = self.border
            for item in self.items:
                rect = getTheme().drawMenuItem( (self.border, y, self.width - self.border*2, 0),
                                     item.text, item == self.active, item.icon )
                item.rect = (rect[0], rect[1], rect[2], rect[3])
                y += rect[3]

    def setShow(self, show):
        if show:
            self.getFocus()
        if self.subActive:
            self.subActive.setShow(0)
        self.subActive = None
        self.active = None
        Base.setShow(self,show)

    def setActive(self, item):
        if item == self.active:
            return
        if self.subActive:
            self.subActive.setShow(0)
            
        # can't use menu items without an event attached
        if item and not item.canActivate:
            self.active = None
            return
        
        self.active = item
        if item:
            self.subActive = item.subMenu
            if self.subActive:
                self.activateSubmenu(item)
        self.setDirty()

    def _pyuiMouseMotion(self, event):
        # give active submenu first chance
        if self.subActive and self.subActive._pyuiMouseMotion(event):
            return 1
        item = self.findItem(event.pos)
        if item and item != self.active:
            self.setActive(item)
        return item != None

    def _pyuiMouseDown(self, event):
        # give active submenu first chance
        if self.subActive and self.subActive._pyuiMouseDown(event):
            return 1
        item = self.findItem(event.pos)
        if item != self.active:
            self.setActive(item)
        return item != None

    def _pyuiMouseUp(self, event):
        # give active submenu first chance
        if self.subActive and self.subActive._pyuiMouseUp(event):
            return 1
        item = self.findItem(event.pos)
        if item != self.active:
            self.setActive(item)
        if not item:
            return 0
        if item.subMenu:
            return 1
        if not item.canActivate:
            return 1
        print "picked menu item:", item.text
        if item.handler:
            item.handler(item)
        #e = self.postEvent(item.eventType)
        #e.item = item
        self.postEvent(pyui.locals.MENU_EXIT)
        return 1

    def activateSubmenu(self, item):
        self.subActive = item.subMenu
        (x,y) = (self.posX + item.rect[2], self.posY + item.rect[1] - self.border)
        if x + item.subMenu.width > getDesktop().width:
            # try moving to left of menu
            x -= self.width + item.subMenu.width
            if x < 0:
                # the menu won't fit, nor to the left of the parent menu, nor to the right. What to do?
                # Align the submenu to the right margin.
                x = getDesktop().width - item.subMenu.width
                item.subMenu.moveto(getDesktop().width - item.subMenu.width, self.posY + item.subMenu.height * getTheme().defaultTextHeight)
        if y + item.subMenu.height > getDesktop().height:
            y = getDesktop().height - item.subMenu.height
            if y < 0:
                raise "No room for submenu!"
        item.subMenu.moveto(x, y)
        item.subMenu.setShow(1)
        
    def addItem(self, title, handler = None, subMenu = None):
        """Add an item to the menu.
        """
        if subMenu:
            title = title + "..."
        newItem = MenuItem(handler, title, subMenu)
        self.items.append(newItem)

        h = getTheme().defaultTextHeight * len(self.items) + self.border * 2
        w = self.minWidth
        for item in self.items:
            if item.width > w:
                w = item.width
        self.resize(w + self.iconWidth * 2 + self.border * 2, h)
        self.setDirty()
        return newItem

    def changeItemTitle(self, oldTitle, newTitle, newHandler):
        for item in self.items:
            if item.text == oldTitle:
                item.text = newTitle
                item.handler = newHandler
                self.setDirty(1)
                break
        
            
    def findItem(self, pos):
        if not self.hit(pos):
            return None

        # put hit position in window relative coords
        x = pos[0] - self.posX
        y = pos[1] - self.posY
        for item in self.items:
            if x >= item.rect[0] and y >= item.rect[1] and x < item.rect[0]+item.rect[2] and y < item.rect[1]+item.rect[3]:
                return item
        return None
    

class MenuPopup(Menu):
    """Menu that can be floating or attached to a menuBar.
    """
    def __init__(self):
        Menu.__init__(self, "")
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self._pyuiMouseDown)
        self.registerEvent(pyui.locals.RMOUSEBUTTONDOWN, self._pyuiMouseDown)
        self.registerEvent(pyui.locals.LMOUSEBUTTONUP, self._pyuiMouseUp)
        self.registerEvent(pyui.locals.MOUSEMOVE, self._pyuiMouseMotion)
        self.registerEvent(pyui.locals.MENU_EXIT, self._pyuiMenuExit)

    def activate(self, x, y):
    	# can set up context sensitive stuff here.
        self.moveto(x, y)
        self.setShow(1)

    def _pyuiMenuExit(self, event):
        if self.show:
            self.setShow(0)
            return 1
        return 0

    def _pyuiMouseMotion(self, event):
        if self.show:
            return Menu._pyuiMouseMotion(self, event)
        return 0
                
    def _pyuiMouseDown(self, event):
        if self.show:
            if Menu._pyuiMouseDown(self, event):
                return 1
            self.setShow(0)
        return 0
    
    def _pyuiMouseUp(self, event):
        if self.show:
            if Menu._pyuiMouseUp(self, event):
                return 1
        return 0


class MenuBar(Window):
    """Menu bar that fits at the top of the screen or the top of a window.
    """
    border = 1
    def __init__(self):
        w = getDesktop().width        
        Window.__init__(self, 0,0, w, getTheme().defaultTextHeight + 4, topmost = 1)
        self.setShow(1)
        self.menus = []
        self.hitList = []
        self.active = None
        self.highlight = None
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self._pyuiMouseDown)
        self.registerEvent(pyui.locals.LMOUSEBUTTONUP, self._pyuiMouseUp)
        self.registerEvent(pyui.locals.MOUSEMOVE, self._pyuiMouseMotion)
        self.registerEvent(pyui.locals.MENU_EXIT, self._pyuiMenuExit)

    def draw(self, renderer):
        if self.show:
            getTheme().drawMenuBar(self.windowRect)
            h = self.height - 2 * self.border
            x = self.border
            self.hitList = []
            for menu in self.menus:
                rect = getTheme().drawMenuBarItem( (x, self.border, 0, h),
                                     menu.menuTitle, menu == self.highlight)
                menu.moveto(self.posX + rect[0] - self.border, self.posY + self.height)
                self.hitList.append((menu, rect))
                x += rect[2]
        
    def addMenu(self, menu):
        self.menus.append(menu)

    def setActiveMenu(self, menu):
        if self.active:
            self.active.setShow(0)
        self.active = menu
        self.highlight = menu
        if ( menu ):
            menu.setShow(1)
        self.setDirty()

    def _pyuiMenuExit(self, event):
        if self.active:
            self.setActiveMenu(None)
            return 1
        return 0

    def _pyuiMouseMotion(self, event):
        # give active child first chance
        if self.active and self.active._pyuiMouseMotion(event):
            return 1
        menu = self.findMenu(event.pos)
        if self.active:
            if menu and menu != self.active:
                self.setActiveMenu(menu)
        else:
            if menu != self.highlight:
                self.highlight = menu
                self.setDirty()
        return 0
                
    def _pyuiMouseDown(self, event):
        # give active child first chance
        if self.active and self.active._pyuiMouseDown(event):
            return 1
        menu = self.findMenu(event.pos)
        if menu != self.active:
            self.setActiveMenu(menu)
            return 1
        return 0

    def _pyuiMouseUp(self, event):
        # give active child first chance
        if self.active and self.active._pyuiMouseUp(event):
            return 1
        menu = self.findMenu(event.pos)
        if self.active and not menu:
            self.setActiveMenu(None)
            return 1
        return 0

    def setParent(self, parent):
        Base.setParent(self, parent)

    def findMenu(self, pos):
        if not self.hit(pos):                             
            return None

        # put hit position in window relative coords
        x = pos[0] - self.posX
        y = pos[1] - self.posY
        for (menu, rect) in self.hitList:
            if x >= rect[0] and y >= rect[1] and x < rect[0]+rect[2] and y < rect[1]+rect[3]:
                return menu
        else:
            return None

    def destroy(self):
        for menu in self.menus:
            menu.destroy()
            del menu
        self.menus = None
        self.hitList = None
        Window.destroy(self)

class ViewWindow(Frame):
    """A window that contains a ViewPanel
    """
    def __init__(self, x, y, w, h, world):
        Frame.__init__(self, x, y, w, h, "3d!")
        panel = ViewPanel(world)
        self.replacePanel(panel)
        self.world = world

class TooltipWindow(Window):
    """A window that displays tooltips.
    """
    def __init__(self, x, y, w, h):
        Window.__init__(self, x, y, w, h, 1)#, "tooltip")
        self.setLayout(pyui.layouts.BorderLayoutManager())
        self.text = ""
        self.setShow(0)
        
    def activate(self, text, rect):
        #print "Enabling", self, rect
        self.text = text
        self.moveto(rect[0], rect[1])
        self.resize(rect[2], rect[3])
        self.setShow(1)

    def draw(self, renderer):
        if self.show:
            getTheme().drawToolTip(self.text, self.windowRect)

class Desktop3DWindow(Window):
    """Special type of Window that is the "background" 3D viewport.
    """
    def __init__(self):
        self._panel = Panel()
        Base.__init__(self)

        # this is identical to Window.__init__
        self.topMost = 0
        self._panel.moveto(0,0)
        Base.addChild(self, self._panel)
        self._panel.setWindow(self)
        getTheme().setArrowCursor()
        self.drawCommands = []
        self.drawLastCallbacks = []
        
        # this is different.. use the desktop
        self.handle = getRenderer().getDesktopWindow()
        self.moveto(0, 0)
        self.resize(getDesktop().width, getDesktop().height)
        getDesktop().windows.insert(0,self)

    def destroy(self):
        """dont destroy the background window!
        """
        self._panel = None
        self.handle = 0
        Base.destroy(self)

class AttachedWindow(Window):
    """A Window that is attached to a 3D object.
    NOTE: careful that these may not have co-ordinates updated - dont make these interactive
    NOTE: maybe all windows should have this functionality... then Frames could use it...
    """
    def __init__(self, xoffset, yoffset, width, height, objectHandle, viewHandle, nodeName = None):
        Window.__init__(self, 0, 0, width, height)
        controllerHandle = getRenderer().attachController(objectHandle, pyui.locals.CONTROLLER_2DWINDOW, nodeName)
        getRenderer().setController(controllerHandle, window = self.handle, offset = (xoffset, yoffset), view = viewHandle)
        self.viewHandle = viewHandle
        self.objectHandle = objectHandle
        self.controllerHandle = controllerHandle
        self.nodeName = nodeName
        self.xoffset = xoffset
        self.yoffset = yoffset
        
    def destroy(self):
        getRenderer().detachController(self.objectHandle, self.controllerHandle)
        self.controllerHandle = None
        Window.destroy(self)


class MenuBarWidget(Base):
    """Menu bar that fits at the top of a window.
    """
    border = 1
    def __init__(self):
        Base.__init__(self)
	self.resize( 0, getTheme().defaultTextHeight + 4 )
        self.setShow(1)
        self.menus = []
        self.hitList = []
        self.active = None
        self.highlight = None
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self._pyuiMouseDown)
        self.registerEvent(pyui.locals.LMOUSEBUTTONUP, self._pyuiMouseUp)
        self.registerEvent(pyui.locals.MOUSEMOVE, self._pyuiMouseMotion)
        self.registerEvent(pyui.locals.MENU_EXIT, self._pyuiMenuExit)

    def draw(self, renderer):
        getTheme().drawMenuBar(self.windowRect)
        h = self.height - 2 * self.border
        x = self.border + self.windowRect[0]
        y = self.border + self.windowRect[1]
        self.hitList = []
        for menu in self.menus:
            rect = getTheme().drawMenuBarItem( (x, y, 0, h), menu.menuTitle, menu == self.highlight)
            menu.moveto( self.rect[0] + x - self.windowRect[0],
	                 self.rect[1] + self.windowRect[3] )
            self.hitList.append((menu, rect))
            x += rect[2]
        
    def addMenu(self, menu):
        self.menus.append(menu)

    def setActiveMenu(self, menu):
        if self.active:
            self.active.setShow(0)
        self.active = menu
        self.highlight = menu
        if ( menu ):
            menu.setShow(1)
        self.setDirty(1)

    def _pyuiMenuExit(self, event):
        if self.active:
            self.setActiveMenu(None)
            return 1
        return 0

    def _pyuiMouseMotion(self, event):
        # give active child first chance
        if self.active and self.active._pyuiMouseMotion(event):
            return 1
        menu = self.findMenu(event.pos)
        if self.active:
            if menu and menu != self.active:
                self.setActiveMenu(menu)
        else:
            if menu != self.highlight:
                self.highlight = menu
                self.setDirty(1)
        return 0
                
    def _pyuiMouseDown(self, event):
        # give active child first chance
        if self.active and self.active._pyuiMouseDown(event):
            return 1
        menu = self.findMenu(event.pos)
        if menu != self.active:
            self.setActiveMenu(menu)
            return 1
        return 0

    def _pyuiMouseUp(self, event):
        # give active child first chance
        if self.active and self.active._pyuiMouseUp(event):
            return 1
        menu = self.findMenu(event.pos)
        if self.active and not menu:
            self.setActiveMenu(None)
            return 1
        return 0

    def setParent(self, parent):
        Base.setParent(self, parent)

    def findMenu(self, pos):
        if not self.hit(pos):                             
            return None

        # put hit position in window relative coords
        x = pos[0] - self.rect[0]
        y = pos[1] - self.rect[1] + self.windowRect[1]
        for (menu, rect) in self.hitList:
            if x >= rect[0] and y >= rect[1] and x < rect[0]+rect[2] and y < rect[1]+rect[3]:
                return menu
        else:
            return None

    def destroy(self):
        for menu in self.menus:
            menu.destroy()
            del menu
        self.menus = None
        self.hitList = None
        Base.destroy(self)

