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

from pyui.desktop import getDesktop, getTheme, getRenderer

itemHeight = 18

class Tree(pyui.widgets.Base):
    """A tree widget. Has a built-in tree node, plus user added nodes. The built-in top
    node is not drawn, only children of it are drawn. Nodes can be added at any time to
    the tree.
    """

    OPEN = 1
    CLOSED = 2
    openButton   = "pyui/images/open.png"
    closedButton = "pyui/images/closed.png"
    emptyButton  = "pyui/images/empty.png"
    default      = "pyui/images/folder.png"
    
    def __init__(self):
        pyui.widgets.Base.__init__(self)
        
        self.selectedNode = None
        self.registerEvent(pyui.locals.LMOUSEBUTTONDOWN, self.onMouseDown)
        self.registerEvent(pyui.locals.SCROLLPOS, self.onScrollPos)
        self.vscroll = pyui.widgets.VScroll()
        self.addChild(self.vscroll)        
        self.top = 0
        self.numNodes = 0  #number of visible nodes
        self.topNode = TreeNode(None, None)
        self.topNode.tree = self
        self.topNode.parent = None
        self.topNode.status = Tree.OPEN

    def resize(self, w, h):
        pyui.widgets.Base.resize(self, w, h)
        self.vscroll.moveto(self.posX + self.width - 10, self.posY)
        self.vscroll.resize(10, self.height)
        self.vscroll.setNumItems(self.numNodes, self.height / itemHeight)        
        
    def addNode(self, node):
        self.topNode.addNode(node)
        self.countNodes()

    def getSelectedNode(self):
        return self.findSelected(self.topNode)

    def findSelected(self, node):
        """help method to recursively find the selected node.
        """
        if node.selected:
            return node
        else:
            for child in node.nodes:
                ret = self.findSelected(child)
                if ret:
                    return ret
        return
    
    def onScrollPos(self, event):
        if event.id == self.vscroll.id:
            self.top = event.pos
            self.setDirty(1)
            #print "%d Nodes %d visible %d top" % (self.numNodes, self.height/26, self.top)            
    
    def onMouseDown(self, event):
        if not self.hit(event.pos):
            return 0

        # find the node clicked on.
        (x,y) = (event.pos[0] - self.rect[0], event.pos[1] - self.rect[1])        
        node = self.findNode(y)
        print "got node", node
        if node:
            if x < node.posX:
                return 0
            if x < node.posX + 16:
                if node.status == Tree.CLOSED and len(node.nodes) > 0:
                    node.status = Tree.OPEN
                    self.countNodes()                    
                else:
                    node.status = Tree.CLOSED
                    self.countNodes()                    
                return 1
            if node.selected:
                return 0            
            node.selected = 1
            if self.selectedNode:
                self.selectedNode.selected = 0
            self.selectedNode = node
            e = self.postEvent(pyui.locals.TREENODE_SELECTED)
            e.node = node
            self.setDirty(1)
            
        return 1
        
    def findNode(self, posY):
        return self.topNode.findNode(posY)

    def countNodes(self):
        """Count the number of visible nodes. used for the scroll bar.
        """
        self.numNodes = self.topNode.countNodes()
        self.vscroll.setNumItems(self.numNodes, self.height / itemHeight)
        return self.numNodes

    def draw(self, renderer):
        """Draw all the top-level nodes."""
        renderer.drawRect( pyui.colors.white, self.windowRect)
        x = 0
        y = 0
        self.drawPos = 0
        self.topNode.drawSubNodes(renderer, x, y)
        for child in self.children:
            child.draw(renderer)
        
    def destroy(self):
        self.topNode.destroy()
        self.topNode = None
        self.vscroll = None
        return pyui.widgets.Base.destroy(self)
            

class TreeNode:
    """A Node in the Tree widgets. Can be open or closed. Uses a default icon of a folder.
    """
    def __init__(self, title, icon = "folder.png"):
        self.title = title
        self.icon = icon
        self.status = Tree.CLOSED
        self.nodes = []    # list of sub nodes
        self.selected = 0
        self.posX = 0
        self.posY = 99999

    def addNode(self, node):
        node.tree = self.tree
        node.parent = self
        self.nodes.append(node)

    def checkY(self, y):
        # check for direct hit
        #print "checking %s y: %d self.posY: %d" % ( repr(self), y, self.posY)
        if y > self.posY and y < self.posY + itemHeight:
            return self
        if self.status == Tree.OPEN:
            # check for open children
            for node in self.nodes:
                subNode = node.checkY(y)
                if subNode:
                    return subNode
        #print "checked %s" % ( repr(self) )
        return None

    def countNodes(self):
        num = 0
        for node in self.nodes:
            num = num + 1
            if node.status == Tree.OPEN:
                num = num + node.countNodes()
        return num
        
    def draw(self, renderer, x, y):
        """draw the node and it's sub-nodes and return the height it required to draw, and the number of nodes drawn.
        """
        diffY = 0
        self.posY = y
        self.posX = x
        
        if self.posY +itemHeight > self.tree.height:
            return 0

        if self.tree.drawPos >= self.tree.top:
            # draw toggle button
            if self.status == Tree.OPEN:
                renderer.drawImage( (self.tree.windowRect[0] + x, self.tree.windowRect[1] + y, 16, 16), Tree.openButton )
            else:
                if len(self.nodes) > 0:
                    renderer.drawImage( (self.tree.windowRect[0] + x, self.tree.windowRect[1] + y, 16, 16), Tree.closedButton )
                else:
                    renderer.drawImage( (self.tree.windowRect[0] + x, self.tree.windowRect[1] + y, 16, 16), Tree.emptyButton )                

            # draw horizontal connector line
            renderer.drawLine(self.tree.windowRect[0] + x + 16, self.tree.windowRect[1] + y + 8, self.tree.windowRect[0] + x + 32, self.tree.windowRect[1] + y + 8, pyui.colors.grey)
        
            # draw icon
            renderer.drawImage( (self.tree.windowRect[0] + x + 32, self.tree.windowRect[1] + y, 16, 16), self.icon)
            
            # draw title
            if self.selected:
                getRenderer().drawRect(pyui.colors.grey, (self.tree.windowRect[0] + x + 48, self.tree.windowRect[1] + y, len(self.title)*12, itemHeight) )
                getRenderer().drawText( self.title, (self.tree.windowRect[0] + x + 48, self.tree.windowRect[1] + y), pyui.colors.white)
            else:
                getRenderer().drawText( self.title, (self.tree.windowRect[0] + x + 48, self.tree.windowRect[1] + y), pyui.colors.grey)

            y = y +itemHeight
            diffY = diffY + itemHeight

        self.tree.drawPos = self.tree.drawPos + 1

        if self.status == Tree.OPEN and len(self.nodes) > 0:
            dy = self.drawSubNodes(renderer, x, y)
            y = y + dy
            diffY = diffY + dy


        return diffY

    def drawSubNodes(self, renderer, x, y):
        diffY = 0
        for node in self.nodes:
            if self.tree.drawPos >= self.tree.top:
                dy = node.draw(renderer, x + 32, y )
                y = y + dy
                diffY = diffY + dy
            else:
                dy = node.draw(renderer, x + 32, y )
                y = y + dy
                diffY = diffY + dy
                
        return diffY


    def findNode(self, y):
        if y > self.posY and y < self.posY + itemHeight:
            return self
        if self.status == Tree.OPEN and len(self.nodes) > 0:        
            for node in self.nodes:
                result = node.findNode(y)
                if result:
                    return result
        return None
            
            
    def destroy(self):
        for node in self.nodes:
            node.destroy()
        self.nodes = None
        self.parent = None
        self.tree = None


def init():
    pyui.core.loadPyuiImage(Tree.openButton)
    pyui.core.loadPyuiImage(Tree.closedButton)
    pyui.core.loadPyuiImage(Tree.emptyButton)
    pyui.core.loadPyuiImage(Tree.default)        
    pyui.core.loadPyuiImage("none.png")
    pyui.core.loadPyuiImage("numeric.png")
    pyui.core.loadPyuiImage("instance.png")
    pyui.core.loadPyuiImage("string.png") 
    pyui.core.loadPyuiImage("list.png")
    pyui.core.loadPyuiImage("dictionary.png")
    pyui.core.loadPyuiImage("function.png")        
