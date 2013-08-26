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

veri = 0

import pyui
from pyui.desktop import getDesktop, getTheme, getRenderer
from pyui import layouts

class Base:
    """Base GUI object that all other drawable object derive from.
    this object implements:
        - containment of other GUI objects
        - position and size
        - event handling
        - unique identity of all GUI objects

    self.rect is in absolute co-ordinates, self.windowRect is in relative window co-ordinates.
    self.posX and self.posY are relative to the parent object
    """
    canTab = 0     # this widget acts as a tab stop
    widgetLabel = "Base"
    
    def __init__(self):
        """Initialize and register the base widget. widgets are added to the global widget list initially.
        """
        self.parent = None
        self.window = None
        self.posX = 0
        self.posY = 0
        self.width = 1
        self.height = 1
        self.show = 1
        self.dirty = 1
        self.fgColor = getDesktop().theme.getFgColor()
        self.bgColor = getTheme().getBgColor()
        self.children = []
        self.eventMap = {}
        self.calcSize()
        getDesktop().registerWidget(self)
        self.popup = None
        self.tooltipText = ""

    def hit(self, pos):
        """Check for a hit using absolute coordinates.
        """
        if self.rect[0] < pos[0] < self.rect[0]+self.rect[2] and self.rect[1] < pos[1] < self.rect[1]+self.rect[3]:
            return 1
        else:
            return 0

    def getFocus(self):
        """Acquire the gui system's focus. only one Base may have the focus
        """
        if isinstance(self, Window):
            getDesktop().activateWindow(self)
        else:            
            getDesktop().activateWindow(self.window)
        getDesktop().setFocus(self)
        self.setDirty()
            
    def loseFocus(self):
        """lose the gui system's focus.
        """
        getDesktop().setFocus(None)
        self.setDirty()

    def hasFocus(self):
        return getDesktop().getFocus() == self

    def postEvent(self, eventType):
        """Post an event to be processed next time through the event loop
        """
        return getDesktop().postEvent(eventType, self.id)

    def calcSize(self):
        """This sets up self.rect to be absolute co-ordinates. also sets up self.windowRect
        to be relative to the upper left of the parent Window
        """
        (x, y) = (self.posX, self.posY)
        p = self.parent
        while p and not isinstance(p, Window):
            x += p.posX
            y += p.posY
            p = p.parent
            
        if self.window:
            self.rect = (x + self.window.posX, y + self.window.posY, self.width, self.height)
            self.windowRect = (x, y, self.width, self.height)
        else:
            self.rect = (self.posX, self.posY, self.width, self.height)
            self.windowRect = (0, 0, self.width, self.height)
            
        for child in self.children:
            child.calcSize()

    def addChild(self, child):
        """Add a child widget.
        """
        self.children.append(child)
        child.setWindow(self.window)
        child.setParent(self)

    def removeChild(self, child):
        try:
            self.children.remove(child)
            child.setParent(None)
            return child            
        except:
            print "ERROR: couldn't find the child to remove."
            return None

    def addPopup(self, popup):
        ### arg... dont know about popups here..
        ### assert isinstance(popup, MenuPopup)
        self.popup = popup

    def setParent(self, parent):
        """Set the parent of this widget
        """
        self.parent = parent

    def setWindow(self, window):
        self.window = window
        for child in self.children:
            child.setWindow(window)

    def draw(self, renderer):
        """To be overridden by derived objects. 
        """
        pass
        
    def handleEvent(self, event):
        """ event processing for base objects
        """
        if not self.show:
            return
        i = len(self.children) - 1
        while i > -1:
            child = self.children[i]
            if child.handleEvent(event):
                return 1
            i = i  - 1
        if self.eventMap.has_key(event.type):
            if self.eventMap[event.type](event):
                return 1

        # popup handling here so it's not overridden with subclass event behavior
        if self.popup and event.type == pyui.locals.RMOUSEBUTTONDOWN and self.hit(event.pos):
            self.popup.activate(event.pos[0], event.pos[1])
            return 1
        return 0

    def moveto(self, x, y):
        """move to absolute position.
        """
        self.posX = x
        self.posY = y
        self.calcSize()
        
    def move(self, dx, dy):
        """move relative to current position.
        """
        self.posX = self.posX + dx
        self.posY = self.posY + dy
        self.calcSize()

    def resize(self, w, h):
        """ resize absolute size of the widget
        """
        self.setDirty()
        self.width = w
        self.height = h
        self.calcSize()

    def registerEvent(self, eventType, handler):
        """Setup handler for an event
        """
        self.eventMap[eventType] = handler

    def unregisterEvent(self, eventType):
        """Remove handler for an event
        """
        if self.eventMap.has_key(eventType):
            del self.eventMap[eventType]

    def pack(self):
        """used by panels & layout managers
        """
        pass

    def setDirty(self, collide = 1):
        """Sets this widget to redraw itself and notifies window.
        """
        self.dirty = 1
        if self.window:
            self.window.setDirty()
        
    def clearDirty(self):
        """Clears this widgets dirty flag.
        """
        self.dirty = 0


    def destroy(self):
        """Call this to remove all references to the widget from the system.
        """
        #print "destroying %s (%d)" % (self, self.id)
        self.window = None
        self.setParent(None)
        if self.popup:
            self.popup.destroy()
            self.popup = None
        if self.children:
            for child in self.children:
                child.destroy()
            self.children = []
        self.eventMap.clear()
        getDesktop().destroyWidget(self)

    def setShow(self,value):
        self.show = value
        self.setDirty()
        for child in self.children:
            child.setShow(value)
        
    def __del__(self):
        #print "Deleting widget %s (%d)" % (self, self.id)
        pass

    def getToolTipInfo(self, pos):
        """return a tuple of the text and rectangle for the tooltip for when the
        mouse is in <pos> within the window. This uses the member variable toolTipInfo
        if it is populated.
        """
        if self.tooltipText:
            return (self.tooltipText,  (pos[0]-50, pos[1]-20, 120, 30) )
        else:
            return None

    def checkHit(self, pos):
        if not self.show:
            return None
        if self.hit(pos):
            for child in self.children:
                result = child.checkHit(pos)
                if result:
                    return result
            return self
        else:
            return None


class Panel(Base):
    """Used for handling interior window drawing and layouts.
    This is the simplest type of panel that other are derived from.
    """
    def __init__(self):
        Base.__init__(self)
        self.childOptions = {}
        self.registerEvent(pyui.locals.KEYDOWN, self._pyuiKeyDown)
        self.setLayout(layouts.FlowLayoutManager())

    def setLayout(self, layout):
        self.layout = layout
        self.layout.setPanel(self)
        
    def draw(self, renderer):
        """draw the panel's children
        """
        # draw user-added children
        if self.show:
            for child in self.children:
                child.draw(renderer)

    def getFocus(self):
        self.nextTab()

    def nextTab(self, step = 1):
        # see if a child currently has focus
        for i in xrange(len(self.children)):
            if self.children[i].hasFocus():
                tab = i + step
                break
        else:
            tab = 0
            
        for i in xrange(0, len(self.children)):
            tab = tab % len(self.children)
            child = self.children[tab]
            
            if isinstance(child, Panel) and child.nextTab(step):
                break
            
            if child.canTab:
                break

            tab += step
        else:
            return 0
        self.children[tab].getFocus()
        return 1

    def _pyuiKeyDown(self, event):
        if event.key == pyui.locals.K_TAB:
            if event.mods & pyui.locals.MOD_SHIFT:
                self.nextTab(-1)
            else:
                self.nextTab(+1)
            return 0
        #print "Unhandled key in panel:", event.key
        return 0

    def addChild(self, child, option = None):
        Base.addChild(self, child)
        self.childOptions[child.id] = option
    
        if child.width > self.width:
            self.width = child.width
        if child.height > self.height:
            self.height = child.height
        # panel cannot be larger than parent
        if self.parent:
            if self.width > self.parent.width:
                self.width = self.parent.width
            if self.height > self.parent.height:
                self.height = self.parent.height

    def pack(self):
        self.layout.begin()
        for child in self.children:
            self.layout.scanChild(child, self.childOptions[child.id])
        for child in self.children:
            self.layout.placeChild(child, self.childOptions[child.id])
            #print "self: %s (%d,%d) child: %s (%d,%d)" % ( self, self.width, self.height,child, child.width, child.height)
            child.pack()
        self.layout.end()

    def resize(self, w, h):
        Base.resize(self, w, h)
        self.pack()

    def destroy(self):
        del self.layout
        self.layout = None
        Base.destroy(self)


class Window(Base):
    """window - contains other objects. 
    Windows have a main panel (_panel) which can be replaced with a custom panel. this main panel is
    kept the size of the interior of the window.
    """
    def __init__(self, x, y, w, h, topmost = 0):
        self._panel = Panel()
        Base.__init__(self)
        self.topMost = topmost
        self._panel.moveto(0,0)
        # the content panel is added as a child through Base::addChild to avoid recursively adding it to itself
        Base.addChild(self, self._panel)
        self._panel.setWindow(self)
        self.drawCommands = []
        # these are drawing callbacks to draw _after_ all the widgets are drawn
        self.drawLastCallbacks = [] 
        if self.__dict__.has_key("title"):
            self.handle = getRenderer().createWindow(self.title)
        else:
            self.handle = getRenderer().createWindow("")
        self.moveto(x, y)
        self.resize(w, h)
        getDesktop().addWindow(self)
            
    def draw(self, renderer):
        """draw the children of this window. this can be over-ridden by derived classes
        to do drawing on the window.
        """
        if self.show:
            # draw user-added children
            for child in self.children:
                child.draw(renderer)

    def drawWindow(self, renderer):
        """Sets up window drawing for the renderer, and then calls standard draw() method.
        """
        if self.dirty:
            self.drawCommands = []
            renderer.setDrawList(self.drawCommands)
            renderer.setClipping((0, 0, self.width, self.height))
            self.draw(renderer)
            for callback in self.drawLastCallbacks:
                callback(renderer)
            self.drawLastCallbacks = []
            self.clearDirty()
            renderer.describeWindow(self.handle, self.drawCommands)
            return 1


    def resize(self, w, h):
        Base.resize(self, w, h)
        self._panel.resize(w,h)

    def addDrawCallback(self, callback):
        self.drawLastCallbacks.append(callback)
        
    # for windows, children get added to the content panel
    def addChild(self, child, option = None):
        self._panel.addChild(child, option)
        child.calcSize()

    def move(self, x, y):
        Base.move(self, x,y)
        getRenderer().moveWindow(self.handle, self.posX, self.posY)

    def moveto(self, x, y):
        Base.moveto(self, x,y)
        getRenderer().moveWindow(self.handle, self.posX, self.posY)
        
    def setLayout(self, layout):
        self._panel.setLayout(layout)
        layout.setPanel(self._panel)

    def pack(self):
        self._panel.pack()

    def destroy(self):
        if self.handle:
            getRenderer().destroyWindow(self.handle)
        self._panel = None            
        self.handle = 0
        self.drawList = []
        Base.destroy(self)

    def replacePanel(self, panel):
        for c in self.children:
            if c.id == self._panel.id:
                self.children = []
                c.destroy()
        self._panel = panel
        Base.addChild(self, self._panel)
        panel.setWindow(self)
        self._panel.moveto(0,0)
        self._panel.resize(self.width, self.height)
        
    def setDirty(self, collide = 1):
        if self.dirty:
            return
        if collide:
            getDesktop().dirtyCollidingWindows(self.rect)
        Base.setDirty(self)
    
    def setTopMost(self, value):
        if value == 0:
            self.topMost = 0
        else:
            self.topMost = 1
        print "set topmost to ", self.topMost            


    def setShow(self, value):
        if value:
            getDesktop().activateWindow(self)
        return Base.setShow(self, value)
