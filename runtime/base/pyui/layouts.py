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

class LayoutManager:
    """base class for all layout managers
    """
    def __init__(self):
        pass

    def setPanel(self, panel):
        self.panel = panel
        
    def begin(self):
        pass

    def end(self):
        pass
    
    def scanChild(self, child, option):
        pass

    def placeChild(self, child, option):
        pass

    def canResize(self):
        pass

    def destroy(self):
        self.panel = None

class FlowLayoutManager(LayoutManager):
    """Flow layout manager
    preserves sizes of children - horizontal layout
    """
    NEWLINE = 1
    def __init__(self,padding = 0):
        LayoutManager.__init__(self)
        self.padding = padding
        self.lastX = self.padding
        self.lastY = self.padding
        self.maxY = 0
        
    def begin(self):
        self.lastX = self.padding
        self.lastY = self.padding
        self.maxY = 0
        
    def placeChild(self, child, option):
        if (option == self.NEWLINE and self.lastX > self.padding) or (self.lastX > 0 and self.lastX + child.width > self.panel.width):
            self.lastX = self.padding
            self.lastY = self.maxY + self.padding
            
        child.moveto(self.lastX, self.lastY)
        self.lastX = self.lastX + self.padding + child.width
        if self.lastY + child.height > self.maxY:
            self.maxY = self.lastY + child.height

    def canResize(self):
        return false    

class BorderLayoutManager(LayoutManager):
    """Border layout manager
    preserves one-dimensional sizes of children
    will not change the size of the parent
    """
    WEST = 1
    EAST = 2
    NORTH = 3
    SOUTH = 4
    CENTER = 5
    
    def __init__(self,padding = 2):
        LayoutManager.__init__(self)
        self.padding = padding
        self.north = None
        self.south = None
        self.west = None
        self.east = None
        self.center = None

    def begin(self):
        self.middleX0 = 0
        self.middleY0 = 0
        self.middleX1 = self.panel.width
        self.middleY1 = self.panel.height
        
    def scanChild(self, child, option):
        if option == BorderLayoutManager.NORTH:
            self.middleY0 = child.height + self.padding
            self.north = child
        elif option == BorderLayoutManager.SOUTH:
            self.middleY1 = self.panel.height - child.height - self.padding
            self.south = child
        elif option == BorderLayoutManager.EAST:
            self.east = child
            self.middleX1 = self.panel.width - child.width - self.padding
        elif option == BorderLayoutManager.WEST:
            self.middleX0 = child.width + self.padding
            self.west = child
        elif option == BorderLayoutManager.CENTER:
            self.center = child

    def placeChild(self, child, option):
        if self.north:
            self.north.moveto(0,0)
            self.north.resize(self.panel.width, self.north.height)
        if self.south:
            self.south.moveto(0, self.middleY1 + self.padding)
            self.south.resize(self.panel.width, self.south.height)
        if self.west:
            self.west.moveto(0, self.middleY0)
            self.west.resize(self.west.width, self.middleY1 - self.middleY0)
        if self.east:
            self.east.moveto(self.middleX1 + self.padding, self.middleY0)
            self.east.resize(self.east.width, self.middleY1 - self.middleY0)
        if self.center:
            self.center.moveto(self.middleX0, self.middleY0)
            self.center.resize(self.middleX1 - self.middleX0, self.middleY1 - self.middleY0)
        
    def canResize(self):
        return true

    def destroy(self):
        self.north = None
        self.south = None
        self.west = None
        self.east = None
        self.center = None
        layoutManager.destroy(self)

class GridLayoutManager(LayoutManager):
    """Grid layout manager
    resizes children in to a grid
    """
    def __init__(self, columns, rows, padding = 3):
        LayoutManager.__init__(self)
        self.padding = padding
        self.columns = columns
        self.rows = rows
        self.posX = 0
        self.posY = 0
        self.full = 0
        
    def begin(self):
        self.posX = 0
        self.posY = 0
        self.full = 0
        self.columnWidth = self.panel.width / self.columns
        self.rowHeight = self.panel.height / self.rows

    def placeChild(self, child, option):
        if self.full:
            print "grid full!"
            return 
        # NOTE: changing posX/posY directly as an optimization. the correct
        # rect will be calculated below in resize()
        child.posX = self.posX * self.columnWidth + self.padding
        child.posY = self.posY * self.rowHeight + self.padding

        child.resize( self.columnWidth - (self.padding * 2), self.rowHeight - (self.padding * 2) )
        #print child, self.posX, self.posY, self.width, self.height
        self.posX = self.posX + 1
        if self.posX == self.columns:
            self.posX = 0
            self.posY = self.posY + 1
            if self.posY == self.rows:
                self.full = 1

class TableLayoutManager(LayoutManager):
    """Has a table grid. children can be placed into the grid and take up
    rectangular sections of multiple cells.
    """
    def __init__(self, w, h, padding = 2):
        self.width = w
        self.height = h
        self.padding = padding

    def begin(self):
        self.grid = []        
        self.cellWidth = self.panel.width / self.width
        self.cellHeight = self.panel.height / self.height
        # fill grid with empty cells
        for i in range(0,self.width*self.height):
            self.grid.append(None)
        
        
    def placeChild(self, child, option):
        """placing children in the grid takes an x,y position and a width and height
        in cells that the child takes up. If a cell is occupied a child cannot be
        placed there.
        """
        if len(option) != 4:
            raise ("Child option <%s> wrong for <%s>" % (repr(option), child) )
        
        (x, y, w, h) = option
        if x < 0 or y < 0 or x + w > self.width or y + h > self.height:
            print "error - outside of range of tableLayout."
            return
        # check for empty space
        for yy in range(y, y + h):
            for xx in range(x, x + w):
                offset = xx + yy * self.width
                if self.grid[offset]:
                    print "error - Cannot place at %d,%d occupied by %s" % (xx,yy,self.grid[offset])
                    return
        # populate cells
        for yy in range(y, y + h):
            for xx in range(x, x + w):
                offset = xx + yy * self.width
                self.grid[offset] = child
        # place the child
        child.moveto( x * self.cellWidth + self.padding, y * self.cellHeight + self.padding )
        child.resize( w * self.cellWidth - self.padding, h * self.cellHeight - self.padding )

class AbsoluteLayoutManager(LayoutManager):
    """
    absolute layout manager
    uses direct co-ordinates in a virtual space specified when created
    option must be specified as a (x,y) tuple when adding children
    """
    def __init__(self, w = 100, h = 100):
        self.width = w
        self.height = h

    def setPanel(self, panel):
        self.panel = panel
        
    def begin(self):
        pass

    def end(self):
        pass
    
    def placeChild(self, child, option):
        child.moveto(self.panel.width * option[0] / self.width , self.panel.height * option[1] / self.height)

    def canResize(self):
        return true
