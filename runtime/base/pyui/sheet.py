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

import pyui
from pyui.desktop import getTheme

class SheetEntry(pyui.widgets.Edit):
    def __init__(self, sheet):
        pyui.widgets.Edit.__init__(self, "", 100, self.onEnter)
        self.setShow(0)
        self.row=0
        self.col=0
        self.sheet = sheet
        self.registerEvent(pyui.locals.KEYDOWN, self.onKeyDown)

    def loseFocus(self):
        self.handler(self, 0)
        return pyui.widgets.Edit.loseFocus(self)

    def activateAt(self, col, row):
        self.col = col
        self.row = row
        (x, y, w, h) = self.sheet.getCellCoordinates(col, row)        
        self.setShow(1)
        self.moveto(x+1, y)
        self.resize(w-2,h)
        value = self.sheet.getCellValue(col, row)
        if value == None:
            self.setText("")
        else:
            self.setText( "%s"%value )
        self.getFocus()

    def deactivate(self):
        self.setShow(0)
        self.setText("")
        self.setDirty(1)

    def onEnter(self, box, gotoNext=1):
        if len(self.text):
            self.sheet.setCellValue(self.col, self.row, self.text)
        self.deactivate()
        if gotoNext:
            if self.row >= self.sheet.visibleRows-1:
                newRow = self.row
            else:
                newRow = self.row+1
            self.activateAt(self.col, newRow)
        return 1

    def onKeyDown(self, event):
        if event.key == pyui.locals.K_DOWN:
            if self.row >= self.sheet.visibleRows-1:
                return 0
            self.onEnter(None,0)
            self.activateAt(self.col, self.row+1)
            return 1
        if event.key == pyui.locals.K_UP:
            if self.row <= 1:
                return 0
            self.onEnter(None,0)
            self.activateAt(self.col, self.row-1)
            return 1

        if event.key == pyui.locals.K_LEFT:
            if self.col <= 1:
                return 0
            self.onEnter(None,0)
            self.activateAt(self.col-1, self.row)
            return 1
        if event.key == pyui.locals.K_RIGHT:
            if self.col >= self.sheet.visibleColumns-1:
                return 0
            self.onEnter(None,0)
            self.activateAt(self.col+1, self.row)
            return 1

        return pyui.widgets.Edit._pyuiKeyDown(self, event)
    

class SheetInfo:
    def __init__(self, start, span, title, readonly):
        self.title = title
        self.start = start
        self.span = span
        self.readonly = readonly
        
class Sheet(pyui.widgets.Base):
    """Spreadsheet-like widget that contains cells that have values. The <handler>
    passed in will be called when a cell value is changed with (x, y, value)
    as arguments. handler is not called when a vale is set for the first time.
    """
    
    defaultColumnWidth = 40
    defaultRowHeight = 16
    def __init__(self, changeHandler=None, insertHandler=None):
        self.changeHandler = changeHandler
        self.insertHandler = insertHandler
        self.vscroll = pyui.widgets.VScroll()
        self.hscroll = pyui.widgets.HScroll()
        
        pyui.widgets.Base.__init__(self)
        self.columnWidths = {}     # key of x, widths are in pixels
        self.rowHeights = {}       # key of y, heights are in pixels
        self.columnTitles = {}     # key of x
        self.rowTitles = {}        # key of y
        self.readOnlyColumns = {}  # key of x
        self.readOnlyRows = {}     # key of y
        self.cells = {}            # key of (x,y)
        
        self.columnTitleHeight = self.defaultRowHeight
        self.rowTitleWidth = self.defaultColumnWidth

        self.highestRow = 0
        self.highestColumn = 0
        self.visibleColumns = 0
        self.visibleRows = 0
        self.resizingRow = 0
        self.resizingColumn = 0
        self.resizingStart = 0
        
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self.onLMouseDown)
        self.registerEvent(pyui.locals.LMOUSEBUTTONUP, self.onLMouseUp)
        self.registerEvent(pyui.locals.MOUSEMOVE, self.onMouseMove)
        self.registerEvent(pyui.locals.SCROLLPOS, self.onScroll)                

        self.entryBox = SheetEntry(self)
        
        self.addChild(self.entryBox)
        self.addChild(self.vscroll)
        self.addChild(self.hscroll)

    def clear(self):
        self.cells = {}
        
    def resize(self, width, height):
        pyui.widgets.Base.resize(self, width, height)
        self.vscroll.resize(12, height)
        self.hscroll.resize(width, 12)
        self.calculateVisibility()

    def calculateVisibility(self):
        # calculate visible columns
        x=1
        posX = self.getColumnWidth(0)
        while 1:
            w = self.getColumnWidth(x+self.hscroll.currentItem)
            if posX+w >= self.width-12:
                break
            posX = posX + w
            x = x + 1
        
        # calculate visible rows
        y=1
        posY = self.getRowHeight(0)
        while 1:
            h = self.getRowHeight(y+self.vscroll.currentItem)
            if posY+h > self.height-12:
                break
            posY = posY + h
            y = y + 1

        # NOTE: x and y now represent the number of visible columns and rows
        #print "calculating: visX", x, "visY", y, "posX:", posX, "posY:", posY
        self.visibleColumns = x
        self.hscroll.setNumItems(self.highestColumn+1, self.visibleColumns)
        self.visibleRows = y
        self.vscroll.setNumItems(self.highestRow+1, self.visibleRows)
        

    def calcSize(self):
        pyui.widgets.Base.calcSize(self)
        self.vscroll.moveto(self.width-12, 0)
        self.hscroll.moveto(0, self.height-12)
        
    ###### Titles for rows and columns #####
        
    def setColumnTitle(self, colNum, title):
        (w,h) = pyui.desktop.getRenderer().getTextSize(title)
        if self.columnWidths.get(colNum):
            self.columnWidths[colNum] = max(self.columnWidths[colNum], w+12)
        else:
            self.columnWidths[colNum] = w+12
        self.columnTitles[colNum] = title
        self.setDirty(1)

    def setRowTitle(self, rowNum, title):
        (w,h) = pyui.desktop.getRenderer().getTextSize(title) 
        self.rowTitleWidth = max(self.rowTitleWidth, w+2)
        self.rowTitles[rowNum] = title
        self.setDirty(1)

    def getColumnTitle(self, x):
        title = self.columnTitles.get(x)
        if not title:
            title = chr(65+x)
        return title

    def getRowTitle(self, y):
        title = self.rowTitles.get(y)
        if not title:
            title = "%d"%y
        return title

   ########## Widths and Heights ########
    
    def getWidthAndHeightFor(self, x, y):
        return (self.getColumnWidth(x), self.getRowHeight(y) )
                
    def getColumnWidth(self, x):
        width = self.columnWidths.get(x)
        if not width:
            width = self.defaultColumnWidth
        return width

    def getRowHeight(self, y):
        height = self.rowHeights.get(y)
        if not height:
            height = self.defaultRowHeight
        return height

    def setColumnWidth(self, colNum, width):
        if width < 2:
            return
        self.columnWidths[colNum] = width
        self.setDirty(1)

    def setRowHeight(self, rowNum, height):
        if height < 2:
            return
        self.rowHeights[rowNum] = height
        self.setDirty(1)


    ########## Read Only   ########

    def setColumnReadOnly(self, x):
        self.readOnlyColumns[x] = 1
        self.setDirty(1)

    def setRowReadOnly(self, y):
        self.readOnlyRows[y] = 1
        self.setDirty(1)

    def getColumnReadOnly(self, x):
        return self.readOnlyColumns.get(x)

    def getRowReadOnly(self, y):
        return self.readOnlyRows.get(y)
    
    ########## Event Handlers  ########

    def onScroll(self, event):
        self.entryBox.deactivate()
        self.setDirty(1)
        return 1
        
    def onLMouseDown(self, event):
        if not self.hit(event.pos):
            return 0

        x = event.pos[0] - self.rect[0]
        y = event.pos[1] - self.rect[1]

        if x < self.rowTitleWidth:
            # determine if it hits a row boundary
            posY =0
            for rowNum in range(self.vscroll.currentItem, self.vscroll.currentItem+self.visibleRows):
                posY += self.getRowHeight(rowNum)
                if abs(posY - y) < 4:
                    self.resizingRow = rowNum
                    self.resizingStart = y
                    return 1

        if y < self.columnTitleHeight:
            # determine if it hits a columns boundary
            posX =0
            for colNum in range(self.hscroll.currentItem, self.hscroll.currentItem+self.visibleColumns):
                posX += self.getColumnWidth(colNum)
                if abs(posX - x) < 4:
                    self.resizingColumn = colNum
                    self.resizingStart = x
                    return 1
                
        (colNum, rowNum) = self.findCellAt(x,y)
        if colNum <= self.hscroll.currentItem or rowNum <= self.vscroll.currentItem:
            print "ERROR: value too low:", colNum, rowNum
            self.entryBox.deactivate()
            return 1
        if colNum >= self.visibleColumns+self.hscroll.currentItem or rowNum >= self.visibleRows+self.vscroll.currentItem:
            print "ERROR: value too high:", colNum, rowNum
            self.entryBox.deactivate()            
            return 1
        if self.getColumnReadOnly(colNum) or self.getRowReadOnly(rowNum):
            self.entryBox.deactivate()            
            return 0
        if self.entryBox.show:
            self.entryBox.loseFocus()
        self.entryBox.activateAt(colNum, rowNum)
        return 1

    def onLMouseUp(self, event):
        if self.resizingRow:
            self.resizingRow = 0
        if self.resizingColumn:
            self.resizingColumn = 0
        return 0

    def onMouseMove(self, event):
        if not self.hit(event.pos):
            return 0
        x = event.pos[0] - self.rect[0]
        y = event.pos[1] - self.rect[1]

        if self.resizingColumn:
            diffX = x - self.resizingStart
            self.resizingStart = x
            self.setColumnWidth(self.resizingColumn, self.getColumnWidth(self.resizingColumn) + diffX)
            self.calculateVisibility()            
            return 1

        if self.resizingRow:
            diffY = y - self.resizingStart
            self.resizingStart = y
            self.setRowHeight(self.resizingRow, self.getRowHeight(self.resizingRow) + diffY)
            self.calculateVisibility()            
            return 1

        return 0
        
    ######## Handle Cells and values #######

    def setCellValue(self, x, y, value):
        old = self.cells.get( (x,y), None)
        if old == value:
            return
        self.checkHighestRow(y)
        self.checkHighestColumn(x)
        self.cells[(x,y)] = value
        (w,h) = pyui.desktop.getRenderer().getTextSize("%s"%value)
        if w+12 >= self.getColumnWidth(x):
            self.setColumnWidth(x, w+12)
        if old:
            if self.changeHandler:
                self.changeHandler(x,y,value)
        else:
            if self.insertHandler:
                self.insertHandler(x,y,value)
        self.setDirty(1)

    def getCellValue(self, x, y):
        return self.cells.get( (x,y) )

    def findCellAt(self, x, y):
        posY = 0
        for rowNum in range(self.vscroll.currentItem,self.vscroll.currentItem+ self.visibleRows+1):
            if rowNum == self.vscroll.currentItem:
                h = self.getRowHeight(0)
            else:
                h = self.getRowHeight(rowNum)
            if y >= posY and y < posY + h:
                posX = 0
                for colNum in range(self.hscroll.currentItem, self.hscroll.currentItem+self.visibleColumns+1):
                    if colNum == self.hscroll.currentItem:
                        w = self.getColumnWidth(0)
                    else:
                        w = self.getColumnWidth(colNum)
                    if x > posX and x < posX + w:
                        return (colNum, rowNum)
                    posX += w
            posY += h
        raise "no cell found at (%s,%s)" % (x,y)
        
    def getCellCoordinates(self, colNum, rowNum):
        posY = self.getRowHeight(0)
        for r in range(self.vscroll.currentItem+1, rowNum):
            if r >= self.vscroll.currentItem:
                h = self.getRowHeight(r)
            posY += h

        posX = self.getColumnWidth(0)
        for c in range(self.hscroll.currentItem+1, colNum):
            if c >= self.hscroll.currentItem:
                w = self.getColumnWidth(c)
            posX += w
        return (posX, posY, self.getColumnWidth(colNum), self.getRowHeight(rowNum) )


    def checkHighestRow(self, rowNum):
        if rowNum > self.highestRow:
            self.highestRow = rowNum
            self.calculateVisibility()                        
            self.vscroll.setNumItems(self.highestRow+1, self.visibleRows)
            self.setDirty(1)
            
    def checkHighestColumn(self, colNum):
        if colNum > self.highestColumn:
            self.highestColumn = colNum
            self.calculateVisibility()                        
            self.hscroll.setNumItems(self.highestColumn+1, self.visibleColumns)
            self.setDirty(1)
            
    ############# Drawing  ################
    
    def draw(self, renderer):
        renderer.drawRect(pyui.colors.white, self.windowRect)
        (w,h) = self.getWidthAndHeightFor(0,0)
        getTheme().drawButton( (self.windowRect[0],self.windowRect[1],w, h), "", 0,0,1)

        # draw column titles and lines
        posX = self.getColumnWidth(0)
        for x in range(1, self.visibleColumns):
            (w,h) = self.getWidthAndHeightFor(x+self.hscroll.currentItem,0)
            getTheme().drawButton( (self.windowRect[0] + posX,
                                    self.windowRect[1],
                                    w, h), self.getColumnTitle(x+self.hscroll.currentItem),
                                   0,0,1)
            renderer.drawLine(self.windowRect[0] + posX - 1,
                              self.windowRect[1], self.windowRect[0] + posX - 1,
                              self.windowRect[1] + self.windowRect[3]-12, pyui.colors.grey)

            if self.getColumnReadOnly(x+self.hscroll.currentItem):
                renderer.drawRect( renderer.packColor(200,200,180), (self.windowRect[0]+posX,self.windowRect[1]+h,
                                                      self.getColumnWidth(x+self.hscroll.currentItem),self.windowRect[3]-12-h) )

            posX = posX + w

        renderer.drawLine(self.windowRect[0] + posX - 1, self.windowRect[1],
                          self.windowRect[0] + posX - 1, self.windowRect[1] + self.windowRect[3]-12, pyui.colors.grey)
        
        # draw row titles and lines
        posY = self.getRowHeight(self.vscroll.currentItem)
        for y in range(1, self.visibleRows):
            (w,h) = self.getWidthAndHeightFor(0,y+self.vscroll.currentItem)
            getTheme().drawButton( (self.windowRect[0],
                                    self.windowRect[1] + posY,
                                    w, h), self.getRowTitle(y+self.vscroll.currentItem),
                                   0,0,1)
            renderer.drawLine(self.windowRect[0], self.windowRect[1] + posY,
                              self.windowRect[0] + self.windowRect[2]-12, 
                              self.windowRect[1] + posY, pyui.colors.grey)
            
            posY = posY + h

        renderer.drawLine(self.windowRect[0],self.windowRect[1] + posY,
                          self.windowRect[0] + self.windowRect[2]-12, self.windowRect[1] + posY, pyui.colors.grey)
        
        # draw cell contents
        posY = self.getRowHeight(self.vscroll.currentItem)
        for y in range(1, self.visibleRows):
            posX = self.getColumnWidth(0)
            for x in range(1, self.visibleColumns):
                value = self.cells.get( (x+self.hscroll.currentItem,y+self.vscroll.currentItem) )
                if value:
                    renderer.drawText( "%s"%value, (self.windowRect[0]+posX+2, self.windowRect[1]+posY), pyui.colors.black)
                posX += self.getColumnWidth(x+self.hscroll.currentItem)
            posY += self.getRowHeight(y+self.vscroll.currentItem)

        if self.entryBox.show:
            self.entryBox.draw(renderer)
        self.vscroll.draw(renderer)
        self.hscroll.draw(renderer)        
