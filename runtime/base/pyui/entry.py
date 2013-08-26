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

import pyui, string, math
from pyui.desktop import getRenderer, getTheme, getDesktop

class Entry(pyui.widgets.Base):
    """Entry box. multiline + scrollbars.
    """
    canTab = 1  
    
    def __init__( self, text = '' ):
        pyui.widgets.Base.__init__(self)
        self.text = text
        self.caretPos = 0
        self.selectPos = 0
        self.dragging = 0
        self.readonly = 0

        self.font = getRenderer().createFont("courier", 12, 0)
        
        self.scroll = 0, 0
        self.vscroll = pyui.widgets.VScroll()
        self.addChild( self.vscroll )
        self.hscroll = pyui.widgets.HScroll()
        self.addChild( self.hscroll )
        self.registerEvent(pyui.locals.SCROLLPOS, self._pyuiScrollPos)
        
        self.registerEvent(pyui.locals.KEYDOWN, self._pyuiKeyDown)
        self.registerEvent(pyui.locals.KEYUP, self._pyuiKeyUp)
        self.registerEvent(pyui.locals.CHAR, self._pyuiChar)
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self._pyuiMouseDown)
        self.registerEvent(pyui.locals.LMOUSEBUTTONUP, self._pyuiMouseUp)
        self.registerEvent(pyui.locals.MOUSEMOVE, self._pyuiMouseMotion)
        self.registerEvent(pyui.locals.CLICKED, self._pyuiClicked)
        self.resize( 100, 100 )



    def resize(self, w, h):
        pyui.widgets.Base.resize(self, w, h)
        self.vscroll.resize(11, h-10)
        self.vscroll.moveto(w-10, 0)
        self.hscroll.resize(w-10, 11)
        self.hscroll.moveto(0, h-10)
        self.updateScrollBars()
        
    def draw(self, renderer):
        caretPos = self.caretPos + ( 7 * self.text.count( '\t', 0, self.caretPos ) )
        selectPos = self.selectPos + ( 7 * self.text.count( '\t', 0, self.selectPos ) )
        text = self.text.expandtabs( 8 )
        windowRect = ( self.windowRect[0], self.windowRect[1], self.windowRect[2] - 10, self.windowRect[3] - 10 )
        self.drawEntry(renderer, windowRect, text, self.hasFocus(), caretPos, selectPos, self.scroll ) 
        self.vscroll.draw(renderer)
        self.hscroll.draw(renderer)

    def drawEntry(self, renderer, rect, text, hasFocus, caretPos, selectPos, scroll):
        (x,y,w,h) = rect
        renderer.setClipping( (x+2,y+2,w-4,h-4) )

        getTheme().draw3DRect(rect, getTheme().bgColor2, 0)

        font_width, font_heigth = renderer.getTextSize( ' ', self.font )
        grid_width, grid_height = ((w-6) / font_width), ((h-6) / font_heigth)

        r, i = 0, 0
        selFrom, selTo = caretPos > selectPos and ( selectPos, caretPos ) or ( caretPos, selectPos )

        for line in text.split( '\n' ):
            line_len = len( line )
            if scroll[1] <= r:
                ly = ( r - scroll[1] ) * font_heigth

                # draw selection
                haveSelStart = 0 <= ( selFrom - i ) <= line_len
                haveSelEnd = 0 <= ( selTo - i ) <= line_len
                sx1, sx2 = 0, w
                if haveSelStart:
                    sx1 = max( (selFrom - i - scroll[0]) * font_width, 0 )
                if haveSelEnd:
                    sx2 = (selTo - i - scroll[0]) * font_width
                elif not haveSelStart and ( 0 > ( selTo - i ) or ( selFrom - i ) > line_len ):
                    sx2 = -1

                if sx2 > 0:
                    renderer.drawRect(getTheme().bgSelect,  (x+3+sx1, y+3+ly, sx2 - sx1, font_heigth))

                # draw caret
                if 0 <= ( caretPos - i ) <= line_len:
                    caret = (caretPos - i - scroll[0]) * font_width
                    if 0 <= caret <= w:
                        renderer.drawRect(getTheme().fgColor, (x+3+caret, y+3+ly, 1, font_heigth))

                # draw text
                if line_len > scroll[0]:
                    renderer.drawText( line[scroll[0]:scroll[0]+grid_width],( x+4, y+ly ), self.fgColor )

            i += line_len + 1
            r += 1
            if r >= scroll[1] + grid_height:
                break

        renderer.setClipping( None )
        return rect

    
    def getGrid( self ):
        (x,y,w,h) = self.windowRect
        font_width, font_heigth = getRenderer().getTextSize( ' ', self.font )
        return int(float(w-16) / float(font_width)), int(float(h-16) / float(font_heigth))
    
    def updateScrollBars(self):
        grid_width, grid_height = self.getGrid()
        self.vscroll.setNumItems( self.text.count('\n') + 1, grid_height )
        self.vscroll.scrollToItem( self.scroll[1] )
        self.hscroll.setNumItems( max( map( len, self.text.split('\n') ) ), grid_width )
        self.hscroll.scrollToItem( self.scroll[0] )
    
    def scrollToCursor(self):
        (x,y,w,h) = self.windowRect

        font_width, font_heigth = getRenderer().getTextSize( ' ', self.font )
        grid_width, grid_height = self.getGrid()

        caretPos = self.caretPos + ( 7 * self.text.count( '\t', 0, self.caretPos ) )
        text = self.text.expandtabs( 8 )
        
        row = text.count( '\n', 0, caretPos )
        if row > 0: col = caretPos - text.rfind( '\n', 0, caretPos ) -1
        else: col = caretPos
        
        adj = (caretPos == len( text ) or text[caretPos] == '\n') and 1 or 0
        
        if col - adj < self.scroll[0]:
            self.scroll = max( col - adj, 0 ), self.scroll[1]
        elif col > self.scroll[0] + grid_width:
            self.scroll = col - grid_width, self.scroll[1]
        
        if row < self.scroll[1]:
            self.scroll = self.scroll[0], row
        elif row >= self.scroll[1] + grid_height:
            self.scroll = self.scroll[0], row - grid_height + 1
        
        self.updateScrollBars()
        
    def _pyuiScrollPos(self, event):
        if event.id == self.vscroll.id:
            self.scroll = self.scroll[0], event.pos
            self.setDirty(1)
        elif event.id == self.hscroll.id:
            self.scroll = event.pos, self.scroll[1]
            self.setDirty(1)
            
    def setText(self, text):
        """external function to set the text and move the caret to the start"""
        self.text = text
        self.caretPos = 0
        self.selectPos = self.caretPos
        self.scrollToCursor()
        self.setDirty(1)

    def setReadOnly(self, option):
        """external function to set the text and move the caret to the start"""
        self.readonly = option

    def loseFocus(self):
        #print "edit losing focus"
        pyui.widgets.Base.loseFocus(self)

    def getFocus(self):
        #print "edit losing focus"
        pyui.widgets.Base.getFocus(self)

    def findMousePos(self, pos):
        # put hit position in window relative coords
        x = pos[0] - self.rect[0] - 2
        y = pos[1] - self.rect[1] - 5
        
        font_width, font_heigth = getRenderer().getTextSize( ' ', self.font )
        
        col = max( math.floor( (float(x) / float(font_width)) ) + self.scroll[0], 0 )
        row = max( math.floor( (float(y) / float(font_heigth)) ) + self.scroll[1], 0 )
        
        r, c, i = 0, 0, 0
        while r != row:
            try: i = self.text.index( '\n', i ) + 1
            except: break
            r += 1
        
        while c < col:
            if i >= len( self.text) or self.text[i] == '\n':
                break
            elif self.text[i] == '\t':
                if ( c + 4 > col ): break
                c += 8
            else:
                c += 1
            i += 1

        return min( max( i, 0 ), len( self.text ) )
        
    def _pyuiMouseDown(self, event):
        if not self.hit(event.pos):
            return 0

        self.getFocus()
        self.caretPos = self.findMousePos(event.pos)
        self.selectPos = self.caretPos
        self.dragging = 1
        self.scrollToCursor()
        self.setDirty(1)
        return 1

    def _pyuiMouseMotion(self, event):
        if not self.dragging:
            return 0
        self.caretPos = self.findMousePos(event.pos)
        self.scrollToCursor()
        self.setDirty(1)
        return 1

    def _pyuiMouseUp(self, event):
        if not self.dragging:
            return 0
        self.caretPos = self.findMousePos(event.pos)
        self.dragging = 0
        self.setDirty(1)
        return 1

    def _pyuiClicked(self, event):
        if event.id == self.id:
            self.getFocus()
            return 0

    def deleteSelected(self):
        if self.selectPos == self.caretPos:
            return
        if self.caretPos > self.selectPos:
            (self.caretPos, self.selectPos) = (self.selectPos, self.caretPos)

        self.text = self.text[:self.caretPos] + self.text[self.selectPos:]
        self.setDirty(1)
        self.selectPos = self.caretPos

    def _pyuiKeyDown(self, event):
            
        if getDesktop().getFocus() != self:
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
            self.scrollToCursor()
            self.setDirty(1)
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
            self.scrollToCursor()
            self.setDirty(1)
            return 1

        if event.key == pyui.locals.K_UP:
            try:
                start = self.text.rindex( '\n', 0, self.caretPos )
                prev_start = self.text.rfind( '\n', 0, start - 1 )
                self.caretPos = min( prev_start - start + self.caretPos, start )
            except:
                return 1
            
            if not (event.mods & pyui.locals.MOD_SHIFT):
                self.selectPos = self.caretPos
            self.scrollToCursor()
            self.setDirty(1)
            return 1
        
        if event.key == pyui.locals.K_DOWN:
            try:
                start = self.text.rfind( '\n', 0, self.caretPos )
                next_start = self.text.index( '\n', self.caretPos )
                try: next_end = self.text.index( '\n', next_start + 1 )
                except: next_end = len( self.text )
                self.caretPos = min( next_start - start + self.caretPos, next_end )
            except:
                return 1
            
            if not (event.mods & pyui.locals.MOD_SHIFT):
                self.selectPos = self.caretPos
            self.scrollToCursor()
            self.setDirty(1)
            return 1

        if event.key == pyui.locals.K_HOME:
            try: self.caretPos = self.text.rindex( '\n', 0, self.caretPos ) + 1
            except: self.caretPos = 0
            if not (event.mods & pyui.locals.MOD_SHIFT):
                self.selectPos = self.caretPos
            self.scrollToCursor()
            self.setDirty(1)
            return 1

        if event.key == pyui.locals.K_END:
            try: self.caretPos = self.text.index( '\n', self.caretPos )
            except: self.caretPos = len(self.text)
            if not (event.mods & pyui.locals.MOD_SHIFT):
                self.selectPos = self.caretPos
            self.scrollToCursor()
            self.setDirty(1)
            return 1
        
        if self.readonly:
            return 1
        
        if event.key == pyui.locals.K_BACKSPACE:
            if self.selectPos != self.caretPos:
                self.deleteSelected()
            elif self.caretPos > 0:
                self.text = self.text[:self.caretPos-1] + self.text[self.caretPos:]
                self.caretPos = self.caretPos - 1
                self.selectPos = self.caretPos
            self.scrollToCursor()
            self.setDirty(1)
            return 1

        if event.key == pyui.locals.K_DELETE:
            if self.selectPos != self.caretPos:
                self.deleteSelected()
            elif self.caretPos < len(self.text):
                self.text = self.text[:self.caretPos] + self.text[self.caretPos+1:]
                self.selectPos = self.caretPos
            self.scrollToCursor()
            self.setDirty(1)
            return 1

        if event.key == pyui.locals.K_RETURN:
            self.text = self.text[:self.caretPos] + '\n' + self.text[self.caretPos:]
            self.caretPos += 1
            self.selectPos = self.caretPos
            self.setDirty(1)
            return 1

        if event.key < 0 or event.key > 255 or chr( event.key ) not in string.printable:
            return 0
        
        if self.caretPos != self.selectPos:
            self.deleteSelected()
            return 1

        return 0

    def _pyuiChar(self, event):
        """ARG! use CHAR messages for real characters.
        """
        if ord(event.key) < 32 or ord(event.key) > 128:
            return 1
        # add regular text to the box
        self.text = self.text[:self.caretPos] + event.key + self.text[self.caretPos:]
        self.caretPos += 1
        self.selectPos = self.caretPos
        self.scrollToCursor()
        self.setDirty(1)
        return 1

    def _pyuiKeyUp(self, event):
        if getDesktop().getFocus() != self.id:
            return 0

        if event.key == pyui.locals.K_SHIFT:
            self.shifting = 0
            return 1

        return 0
