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

"""Dialogs for the pyui library.
"""

import sys
import os, stat
import string
import re

import pyui
from pyui import locals
from pyui.desktop import getDesktop, getRenderer, getTheme

EVENT_OUTPUT = pyui.desktop.getUserEvent()

class ConsoleOutput:
    def __init__(self):

        self.lines = []
        self.oldout = sys.stdout
        self.olderr = sys.stderr
        
    def write(self, text):
        self.oldout.write("%s" % text)
        text = string.rstrip(text)
        if len(text) < 1:
            return
        text = string.replace(text, "\n", " ")
        text = string.replace(text, "\r", " ")      
        self.append(text)
        getDesktop().postUserEvent(EVENT_OUTPUT)

    def beginCapture(self):
        sys.stdout = self
        sys.stderr = self

    def endCapture(self):
        sys.stdout = self.oldout
        self.stderr = self.olderr

    def getLines(self):
        return self.lines
    
    def clear(self):
        self.lines = []

    def __del__(self):
        self.endCapture()
        
#####################
# edit box with history
#####################
class ConsoleEdit(pyui.widgets.Edit):
    def __init__(self, text, max, execCallback):
        pyui.widgets.Edit.__init__(self, text, max, self._pyuiEnter)

        self.history = []
        self.historyPos = 0
        self.execCallback = execCallback
        self.registerEvent(locals.KEYDOWN, self._pyuiKeyDown)

    def _pyuiEnter(self, object):
        self.execCallback(self)
        self.history.append(self.text)
        self.historyPos = len(self.history) - 1
        self.setText("")
        self.setDirty()
        return 1

    def _pyuiKeyDown(self, event):
        if not self.hasFocus():
            return 0

        if event.key == locals.K_UP:
            if self.history:
                self.historyPos = (self.historyPos - 1) % len(self.history)
                self.setText(self.history[self.historyPos])
            return 1

        if event.key == locals.K_DOWN:
            if self.history:
                self.historyPos = (self.historyPos + 1) % len(self.history)
                self.setText(self.history[self.historyPos])
            return 1
                
        return pyui.widgets.Edit._pyuiKeyDown(self, event)
    
#####################
# box to display lines of text for console output
# or chat windows...
####################
class LineDisplay(pyui.widgets.Base):
    def __init__(self):
        pyui.widgets.Base.__init__(self)
        #self.bgColor = (0,0,49, 255)
        self.lines = []
        self.displayLines = []
        self.numVisible = self.height / locals.TEXT_HEIGHT
        self.numItems = 0
        self.topItem = 0
        self.rewrap = 0
        self.textWidth = 0
        self.vscroll = pyui.widgets.VScroll()
        self.addChild(self.vscroll)
        self.registerEvent(locals.SCROLLPOS, self._pyuiScrollPos)

    def clear(self):
        self.lines = []
        self.displayLines = []
        self.numVisible = self.height / locals.TEXT_HEIGHT
        self.numItems = 0
        self.topItem = 0
        self.rewrap = 0

    def rewrapAll(self):
        self.displayLines = []
        for (line, color) in self.lines:
            self.wrapLine(line, color)
        numLines = len(self.displayLines)
        self.topItem = numLines - self.numVisible
        self.vscroll.setNumItems(numLines, self.numVisible)
        self.vscroll.scrollToItem(self.topItem)

    def wrapLine(self, line, color):
        """Add a line of text to the display lines list with wrapping."""
        (words, spaces) = self.splitLine(line)

        displayLine = ""
        width = 0
        space = ""
        spaceWidth = 0
        while words:
            word = words.pop(0)
            wordWidth = getRenderer().getTextSize(word)[0]

            if width + spaceWidth + wordWidth <= self.textWidth:
                displayLine = displayLine + space + word
                width += spaceWidth + wordWidth
            else:
                self.addDisplayLine(displayLine, color)
                displayLine = word
                width = wordWidth
            space = spaces.pop(0)
            spaceWidth = getRenderer().getTextSize(space)[0]
                
        if displayLine:
            self.addDisplayLine(displayLine, color)

    def splitLine(self, line):
        """Works like split(), but also returns whitespace between words"""
        words = []
        spaces = []
        nEnd = 0
        while nEnd < len(line):
            nStart = nEnd
            while nEnd < len(line) and not line[nEnd].isspace():
                nEnd += 1
            words.append(line[nStart:nEnd])
                
            nStart = nEnd
            nEnd += 1
            while nEnd < len(line) and line[nEnd].isspace():
                nEnd += 1
            spaces.append(line[nStart:nEnd])

        return (words,spaces)

    def addLine(self, line, color = None):
        """This adds lines to the display. it does text wrapping."""
        if not color:
            color = getTheme().fgColor
            
        self.lines.append((line, color))
        if len(self.lines) > 100:
            del self.lines[0]
        self.wrapLine(line, color)
        numLines = len(self.displayLines)
        self.topItem = numLines - self.numVisible
        self.vscroll.setNumItems(numLines, self.numVisible)
        self.vscroll.scrollToItem(self.topItem)

    def addDisplayLine(self, displayLine, color):
        self.displayLines.append((displayLine, color))
        if len(self.displayLines) > 100:
            del self.displayLines[0]
        self.setDirty()

    def draw(self, renderer):
        #renderer.drawRect(getTheme().bgColor2, self.windowRect)
        if self.rewrap:
            self.rewrapAll()
            self.rewrap = 0
            
        i = 0
        for (line, color) in self.displayLines:
            if i >= self.topItem and i < (self.topItem + self.numVisible):
                renderer.drawText(line, (self.windowRect[0]+2, self.windowRect[1]+2+((i-self.topItem)*locals.TEXT_HEIGHT)), color )
            i += 1
        self.vscroll.draw(renderer)
        self.clearDirty()
        
    def _pyuiScrollPos(self, event):
        if event.id == self.vscroll.id:
            self.topItem = event.pos
            self.setDirty(1)

    def resize(self, w, h):
        pyui.widgets.Base.resize(self, w,h)
        self.vscroll.resize(getTheme().getScrollerSize(), h)
        self.vscroll.moveto(w-getTheme().getScrollerSize(), 0)
        self.textWidth = self.width - self.vscroll.width
        self.numVisible = self.height / locals.TEXT_HEIGHT
        self.rewrap = 1

    def destroy(self):
        self.vscroll.destroy()
        self.vscroll = None
        pyui.widgets.Base.destroy(self)

####################################
# python console window
class Console(pyui.widgets.Frame):

    def __init__(self, x, y, w, h):
        pyui.widgets.Frame.__init__(self, x, y, w, h, "Console")
        self.setLayout(pyui.layouts.BorderLayoutManager())
        self.output = ConsoleOutput()
        self.locals = {}

        # create gui objects
        self.inputBox = ConsoleEdit("", 80, self._pyuiGo)
        self.goButton = pyui.widgets.Button("Go", self._pyuiGo)
        self.outputBox = LineDisplay()

        self.panel = pyui.widgets.Panel()
        self.panel.setLayout(pyui.layouts.BorderLayoutManager())
        self.panel.addChild(self.inputBox, locals.CENTER)
        self.panel.addChild(self.goButton,locals.EAST)

        self.addChild(self.outputBox, locals.CENTER)
        self.addChild(self.panel, locals.SOUTH)
        self.panel.setWindow(self)
        self.pack()

        self.registerEvent(EVENT_OUTPUT, self._pyuiOutput)

    def _pyuiOutput(self, event):
        if not self.output:
            return 1
        lines = self.output.getLines()
        for l in lines:
            self.outputBox.addLine(l)
        self.output.clear()
        self.setDirty()
        return 1

    def _pyuiGo(self, object):
        command = self.inputBox.text
        self.output.beginCapture()
        try:
            print ">%s" % command
            exec command in globals(), self.locals
        except:
            print "Exception on command '%s':" % command
            print ">    %s" % sys.exc_value
        self.output.endCapture()
        return 1

    def destroy(self):
        #sys.stdout = None
        self.inputBox = None
        self.goButton = None
        self.outputBox = None
        self.panel = None
        pyui.widgets.Frame.destroy(self)
        self.output = None


##################################
# Dialog
#
##################################
class Dialog(pyui.widgets.Frame):
    def __init__(self, x = -1, y = -1, w = 300, h = 200, title = None):
        # center if position not specified
        if x < 0:
            x = (getDesktop().width - w) / 2
        if y < 0:
            y = (getDesktop().height - h) / 2
        pyui.widgets.Frame.__init__(self, x, y, w, h, title)
        self.modal = -1   # this is set to the return value of the dialog
        self.setShow(1)
        self.setDirty()
        self.cb = None
        
    def doModal(self, cb = None):
        self.setShow(1)
        self.cb = cb
        getDesktop().setModal(self)

    def close(self, value = 1):
        #print "closed - " , value
        self.modal = value
        getDesktop().setModal(None)
        self.setShow(0)
        self.loseFocus()
        self.postEvent(locals.DIALOGCLOSED)
        if self.cb:
            self.cb(value)

    def destroy(self):
        if getDesktop().getModal() == self:
            getDesktop().setModal(None)
        pyui.widgets.Frame.destroy(self)

class StdDialog(Dialog):
    def __init__(self, title, text):

        size = getRenderer().getTextSize(title)
        Dialog.__init__(self, title = title)
        self.setLayout(pyui.layouts.BorderLayoutManager())

        self.textLabel = pyui.widgets.Label(text)
        self.textLabel.setText(text)
        self.buttonPanel = pyui.widgets.Panel()
        self.buttonPanel.setLayout(pyui.layouts.BorderLayoutManager())
        self.okButton = pyui.widgets.Button("OK", self._pyuiOK)
        self.okButton.resize(self.innerWidth/2, self.okButton.height)
        self.cancelButton = pyui.widgets.Button("Cancel", self._pyuiCancel)
        self.cancelButton.resize(self.innerWidth/2, self.cancelButton.height)     
        self.buttonPanel.addChild(self.okButton, locals.WEST)
        self.buttonPanel.addChild(self.cancelButton, locals.EAST)
        self.buttonPanel.pack()
        
        self.addChild(self.textLabel, locals.CENTER)
        self.addChild(self.buttonPanel, locals.SOUTH)

        self.pack()

    def _pyuiOK(self, button):
        self.close(1)

    def _pyuiCancel(self, button):
        self.close(0)

    def draw(self, renderer):
        #print "drawing!!!"
        return Dialog.draw(self, renderer)

    def destroy(self):
        self.buttonPanel = None
        self.okButton = None
        self.cancelButton = None
        self.textLabel = None
        Dialog.destroy(self)


class FileDialog(Dialog):
    """A dialog to allow the user to select a file. Allows regex wildcard filters
    and calls the "callback" method when done.

    Always use the forward slash '/' to separate paths, not the other slash '\'.

    The filter matches using python regular expressions. Note that the wildcard for
    matching anything is ".*" not just "*" as the asterisk is a "repeating character"
    modifier in the regular expression language...
    """
    def __init__(self, startDir, callback, filter = ".*"):
        currentDir = startDir.replace('\\','/')
        self.callback = callback
        self.filter = filter
        Dialog.__init__(self, -1, -1, 400,240, "File Dialog")
        self.setLayout(pyui.layouts.TableLayoutManager(6,8))
        
        self.dirLabel = pyui.widgets.Label("Directory:")
        self.fileLabel = pyui.widgets.Label("Filename:")
        self.filterLabel = pyui.widgets.Label("Filter:")

        self.dirBox = pyui.widgets.Label(currentDir)
        self.filesBox = pyui.widgets.ListBox(self._pyuiSelected, self._pyuiDoubleClicked)
        self.nameBox = pyui.widgets.Label("")
        self.filterBox = pyui.widgets.Edit(self.filter,10,self._pyuiFilter)

        self.dirButton = pyui.widgets.Button("Up", self._pyuiUp)
        self.openButton = pyui.widgets.Button("Open", self._pyuiOpen)
        self.closeButton = pyui.widgets.Button("Close", self._pyuiClose)

        self.addChild( self.dirLabel,    (0,0,2,1) )
        self.addChild( self.fileLabel,   (0,6,2,1) )
        self.addChild( self.filterLabel, (0,7,2,1) )
        self.addChild( self.dirBox,      (2,0,3,1) )
        self.addChild( self.filesBox,    (0,1,6,5) )
        self.addChild( self.nameBox,     (2,6,3,1) )
        self.addChild( self.filterBox,   (2,7,3,1) )
        self.addChild( self.dirButton,   (5,0,1,1) )
        self.addChild( self.openButton,  (5,6,1,1) )
        self.addChild( self.closeButton, (5,7,1,1) )        

        self.pack()
        self.setCurrentDir(currentDir)

    def setCurrentDir(self, newDir):
        """This will fail if newDir is not a valid directory.
        """
        try:
            info = os.stat(newDir)
            isdir = stat.S_ISDIR(info[stat.ST_MODE])
        except OSError, e:
            print "Invalid Dir:", newDir
            return None
        if isdir:
            self.currentDir = newDir
            return self.populateDir()
        return None
            
    def populateDir(self):
        """Load the current directory. Load directories first, then all
        the files.
        """
        self.filesBox.clear()
        self.dirBox.setText(self.currentDir)
        self.nameBox.setText("")
        all = os.listdir(self.currentDir+"/")
        files = []
        for filename in all:
            info = os.stat(self.currentDir+"/"+filename)
            isdir = stat.S_ISDIR(info[stat.ST_MODE])
            if isdir:
                self.filesBox.addItem(filename, 1, pyui.colors.blue)
            else:
                files.append(filename)
        for filename in files:
            if re.search(self.filter, filename):
                self.filesBox.addItem(filename, 0, pyui.colors.black)

    def _pyuiFilter(self, filter):
        self.filter = filter.text
        self.populateDir()
        self.filterBox.setText(self.filter)
        return 1
    
    def _pyuiUp(self, button):
        path = self.currentDir.split("/")[:-1]
        self.setCurrentDir( string.join(path, "/") )

    def _pyuiSelected(self, item):
        if not item:
            self.nameBox.setText("")
        else:
            self.nameBox.setText(item.name)
        return 1

    def _pyuiDoubleClicked(self, item):
        if not item:
            self.nameBox.setText("")
        else:
            self.nameBox.setText(item.name)
            self._pyuiOpen(None)
        return 1
        
    def _pyuiOpen(self, button):
        """Open a file or a directory
        """
        if len(self.nameBox.text) == 0:
            return 0
        fullpath = self.currentDir+"/"+self.nameBox.text
        info = os.stat(fullpath)
        isdir = stat.S_ISDIR(info[stat.ST_MODE])
        if isdir:
            self.setCurrentDir( fullpath )
        else:
            self.close(1)
            self.callback(fullpath)
        return 1

    def _pyuiClose(self, button):
        self.close(0)
    
class ColorDialog(Dialog):
    """Allows the user to select a color.
    """
    def __init__(self, callback, r=255, g=0, b=0):
        self.callback = callback
        self.r = r
        self.g = g
        self.b = b
        
        Dialog.__init__(self, -1, -1, 400,240, "Color Dialog")
        self.setLayout(pyui.layouts.TableLayoutManager(4,9))

        self.colorGradient = ColorGradient(None, self)
        self.colorStrip = ColorStrip(None, self)
        self.colorSolid = ColorSolid(None)

        self.okButton = pyui.widgets.Button("OK", self._pyuiOK)
        self.cancelButton = pyui.widgets.Button("Cancel", self._pyuiCancel)
        
        self.redLabel = pyui.widgets.Label("Red:")
        self.greenLabel = pyui.widgets.Label("Green:")
        self.blueLabel = pyui.widgets.Label("Blue:")

        self.redBox = pyui.widgets.SliderBar(self._pyuiRed, 255, r)
        self.greenBox = pyui.widgets.SliderBar(self._pyuiGreen, 255, g)
        self.blueBox = pyui.widgets.SliderBar(self._pyuiBlue, 255, b)

        self.addChild( self.colorGradient,  (0,0,2,5) )
        self.addChild( self.colorStrip,     (2,0,2,5) )
        self.addChild( self.colorSolid,     (0,5,1,3) )
        self.addChild( self.redLabel,       (1,5,1,1) )
        self.addChild( self.greenLabel,     (1,6,1,1) )
        self.addChild( self.blueLabel,      (1,7,1,1) )        
        self.addChild( self.redBox,         (2,5,2,1) )
        self.addChild( self.greenBox,       (2,6,2,1) )
        self.addChild( self.blueBox,        (2,7,2,1) )        
        self.addChild( self.cancelButton,   (0,8,2,1) )
        self.addChild( self.okButton,       (2,8,2,1) )        

        self.pack()
        self.setColor()
        
    def setColor(self, strip=1):
        self.color = pyui.desktop.getRenderer().packColor(self.r,self.g,self.b)
        self.colorSolid.color = self.color
        if strip:
            self.colorStrip.color = self.color
        self.setDirty(1)
        
    def _pyuiOK(self, button):
        self.callback(self.color)
        self.close(1)

    def _pyuiCancel(self, button):
        self.close(0)

    def _pyuiRed(self, value):
        self.r = value
        self.setColor()

    def _pyuiGreen(self, value):
        self.g = value
        self.setColor()
        pass

    def _pyuiBlue(self, value):
        self.b = value
        self.setColor()

    def setRGB(self, r, g, b,strip=1):
        self.r = r
        self.g = g
        self.b = b
        self.redBox.setValue(r)
        self.greenBox.setValue(g)
        self.blueBox.setValue(b)        
        self.setColor(strip)
    
class ColorSolid(pyui.widgets.Base):
    def __init__(self, color):
        pyui.widgets.Base.__init__(self)
        self.color = color

    def draw(self, renderer):
        renderer.drawRect(pyui.colors.black, self.windowRect)
        renderer.drawRect(self.color, (self.windowRect[0]+2,self.windowRect[1]+2,self.windowRect[2]-4,self.windowRect[3]-4) )

class ColorGradient(pyui.widgets.Base):

    colors = [
        (255,0,0),
        (255,255,0),
        (0,255,0),
        (0,255,255),
        (0,0,255),
        (255,0,255),
        (255,0,0)
        ]

    segments = 6
    
    def __init__(self, color, dialog):
        pyui.widgets.Base.__init__(self)
        self.color = color
        self.dialog = dialog
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self._pyuiMouseDown)         

    def draw(self, renderer):
        renderer.drawRect(pyui.colors.black, self.windowRect)
        top = self.windowRect[1]+2
        height = self.windowRect[3]-4
        width = self.windowRect[2] / float(self.segments)
        for i in range(0,self.segments):
            renderer.drawGradient( (self.windowRect[0]+int(i*width),top,int(width+1),height),
                                   apply(renderer.packColor,self.colors[i]),
                                   apply(renderer.packColor,self.colors[i+1]),
                                   apply(renderer.packColor,self.colors[i]),
                                   apply(renderer.packColor,self.colors[i+1]))

    def _pyuiMouseDown(self, event):
        if not self.hit(event.pos):
            return 0
        x = event.pos[0] - self.rect[0]
        ratio = float(x) / float(self.windowRect[2])
        hitSegment = int(ratio * self.segments)
        before = self.colors[hitSegment]
        after = self.colors[hitSegment+1]
        innerRatio = (ratio * self.segments) - hitSegment

        newColor = []
        for i in range(0,3):
            diff = after[i] - before[i]
            value = before[i] + innerRatio*diff
            newColor.append(int(value))
        apply(self.dialog.setRGB, newColor)
        return 1
        
class ColorStrip(pyui.widgets.Base):
    def __init__(self, color, dialog):
        pyui.widgets.Base.__init__(self)
        self.color = color
        self.dialog = dialog
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self._pyuiMouseDown)
        
    def draw(self, renderer):
        renderer.drawRect(pyui.colors.black, self.windowRect)
        top = self.windowRect[1]

        w = self.windowRect[2]-4
        h = self.windowRect[3]
        rect1 =(self.windowRect[0]+2,self.windowRect[1], w, h/2)
        rect2 =(self.windowRect[0]+2,self.windowRect[1]+(h/2), w, h/2)
        renderer.drawGradient(rect1, pyui.colors.white, pyui.colors.white, self.color, self.color )
        renderer.drawGradient(rect2,self.color, self.color, pyui.colors.black, pyui.colors.black )        
        
    def _pyuiMouseDown(self, event):
        if not self.hit(event.pos):
            return 0
        y = event.pos[1] - self.rect[1]
        ratio = 2 - (float(y) / float(self.windowRect[3]))*2

        newColor = []
        for i in range(0,3):
            if ratio <= 1.0:
                value = self.color[i] * ratio
            else:
                if self.color[i] == 0:
                    value = 255 * (ratio/2)
                elif self.color[i] == 255:
                    value = 255
                else:
                    value = self.color[i] + (255 - self.color[i]) * (ratio/2)
            newColor.append(int(value))
        newColor.append(0) # this make setRGB not reset self.color
        apply(self.dialog.setRGB, newColor)
        return 1
