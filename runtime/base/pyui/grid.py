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

"""A scrollable grid class for PyUI. The elements in the grid are PyUI widgets.
"""

import pyui
import copy

from pyui.desktop import getDesktop, getTheme, getRenderer

class GridPanel(pyui.widgets.Panel):
    """A scrollable grid class. I have a grid of cells of which only some will
    be visible at any time.
    """
    def __init__(self, visibleWidth, visibleHeight, useColumnHeaders = 1, useRowHeaders = 1):
        self.vWidth = visibleWidth
        self.vHeight = visibleHeight
        self.scrollPos = 0
        pyui.widgets.Panel.__init__(self)
        
        self.setLayout(pyui.layouts.BorderLayoutManager())
        self.cheader = ColumnHeaders(visibleWidth)
        self.rheader = RowHeaders(visibleHeight)
        self.scrollBar = pyui.widgets.VScroll()
        self.scrollBar.resize(10, 50)
        self.cellPanel = CellPanel(visibleWidth, visibleHeight)

        if useColumnHeaders:
            self.addChild(self.cheader, pyui.layouts.BorderLayoutManager.NORTH)
        if useRowHeaders:
            self.addChild(self.rheader, pyui.layouts.BorderLayoutManager.WEST)
            
        self.addChild(self.cellPanel, pyui.layouts.BorderLayoutManager.CENTER)
        self.addChild(self.scrollBar, pyui.layouts.BorderLayoutManager.EAST)        
        self.pack()

    def resize(self, w, h):
        print "Resizing GridPanel", w, h
        pyui.widgets.Panel.resize(self, w, h)

    def setColumnName(self, columnNum, name):
        self.cheader.setColumnName(columnNum, name)

    def setRowName(self, rowNum, name):
        self.rheader.setRowName(rowNum, name)

    def getCellAt(self, x, y):
        """return a cell at the co-ordinates.
        """
        return self.cellPanel.getCellAt(x, y)

    def putCellAt(self, widget, x, y):
        """put a widget into the grid at the co-ordinates.
        """
        return self.cellPanel.putCellAt(widget, x, y)
    
    def removeCellAt(self, x, y):
        """remove a widget from the grid
        """
        return self.cellPanel.removeCellAt(x, y)

    def findCellAt(self, posX, posY):
        """Find the cell at the x,y pixel position. Pass-through to the inner grid panel.
        """
        return self.cellPanel.findCellAt(posX, posY) 

    def findCoordinatesAt(self, posX, posY):
        """convert screen co-ordinates into grid co-ordinates.
        """
        return self.cellPanel.findCoordinatesAt(posX, posY)

    def clear(self):
        return self.cellPanel.clear()
    
class CellPanel(pyui.widgets.Panel):
    """The inner cell grid of a GridPanel.
    """
    def __init__(self, vWidth, vHeight):
        pyui.widgets.Panel.__init__(self)
        self.vWidth = float(vWidth)
        self.vHeight = float(vHeight)
        self.cells = {}
        self.scrollPos = 0
        self.cellWidth = 1
        self.cellHeight = 1
        self.numRows = vHeight

        self.registerEvent(pyui.locals.SCROLLPOS, self.onScroll)
        
    def resize(self, width, height):
        pyui.widgets.Panel.resize(self, width, height)
        self.cellWidth = self.windowRect[2] / self.vWidth
        self.cellHeight = self.windowRect[3] / self.vHeight
        self.setupAllCells()
        
    def setupAllCells(self):
        for key in self.cells.keys():
            if key[1] >= self.scrollPos and key[1] < self.scrollPos + self.vHeight:
                self.setupCell( self.cells[key], key[0], key[1])
                self.cells[key].setShow(1)
            else:
                self.cells[key].setShow(0)                
        
    def getCellAt(self, x, y):
        return self.cells.get( (x,y), None)

    def removeCellAt(self, x, y):
        cell = self.cells.get( (x,y), None)
        if cell:
            cell.destroy()
            self.children.remove(cell)
            del self.cells[ (x,y) ]
            self.setDirty(1)

    def clear(self):
        tmp = copy.copy(self.children)
        for cell in tmp:
            self.removeCellAt( cell.gridPosition[0], cell.gridPosition[1] )
            
    def putCellAt(self, widget, x, y):
        if self.cells.has_key( (x,y) ):
            print "Error: already a widget at (%s,%s)" % (x,y)
            return 0
        self.addChild(widget)
        self.cells[ (x,y) ] = widget
        self.setupCell(widget, x, y)
        if y > self.numRows:
            self.numRows = y + 1
            self.parent.scrollBar.setNumItems(y+1, self.vHeight)
        return 1

    def setupCell(self, widget, x, y):
        """this moves and positions the cell. it also sets "gridPosition" so the cell
        knows where in the grid it lives.
        """
        if y >= self.scrollPos and y < self.scrollPos + self.vHeight:
            widget.setShow(1)
        else:
            widget.setShow(0)
        
        #print "setup cell", x, y
        widget.gridPosition = (x,y)
        widget.moveto( self.cellWidth * x + 2,
                       self.cellHeight * (y-self.scrollPos) + 2)
        widget.resize( self.cellWidth -4, self.cellHeight -4)
            
    def onScroll(self, event):
        if event.id == self.parent.scrollBar.id:
            self.scrollPos = event.pos
            self.setupAllCells()
            self.setDirty(1)
            self.window.setDirty(1)
            return 1
        return 0

    def findCellAt(self, posX, posY):
        """find the cell at x,y
        """
        x = int((posX - self.rect[0]) / self.cellWidth)
        y = int((posY - self.rect[1]) / self.cellHeight) + self.scrollPos
        return self.cells.get( (x,y), None)

    def findCoordinatesAt(self, posX, posY):
        x = int((posX - self.rect[0]) / self.cellWidth)
        y = int((posY - self.rect[1]) / self.cellHeight) + self.scrollPos
        return (x,y)
        
    def draw(self, renderer):
        """only draw the visible widgets.
        """
        for key in self.cells.keys():
            if key[1] >= self.scrollPos and key[1] < self.scrollPos + self.vHeight:
                self.cells[key].draw(renderer)
        xpos = self.windowRect[0]
        ypos = self.windowRect[1]
        w = self.windowRect[2]
        h = self.windowRect[3]
        cellw = w / self.vWidth
        cellh = h / self.vHeight
        
        for x in range(0, self.vWidth+1):
            renderer.drawLine(xpos + x * cellw, ypos, xpos + x * cellw, ypos + h, pyui.colors.white)
        
        for y in range(0,self.vHeight):    
            renderer.drawLine(xpos, ypos + y * cellh, xpos + w, ypos + y * cellh, pyui.colors.white)

        
    
class ColumnHeaders(pyui.widgets.Panel):
    """The column headers for the GridPanel.
    """
    def __init__(self, numColumns):
        pyui.widgets.Panel.__init__(self)
        self.setLayout(pyui.layouts.TableLayoutManager(numColumns, 1))
        for i in range(0, numColumns):
            self.addChild( pyui.widgets.Button("---"), (i, 0, 1, 1) )
        self.resize(self.rect[2], 22)

    def setColumnName(self, columnNum, name):
        self.children[columnNum].setText(name)

class RowHeaders(pyui.widgets.Panel):
    """The row headers for the GridPanel.
    """
    def __init__(self, numRows):
        pyui.widgets.Panel.__init__(self)
        self.setLayout(pyui.layouts.TableLayoutManager(1, numRows) )
        for i in range(0, numRows):
            self.addChild( pyui.widgets.Button("%d" % i), (0, i, 1, 1) )
        self.resize(22, self.rect[3])

    def setRowName(self, rowNum, name):
        self.children[rowNum].setText(name)
