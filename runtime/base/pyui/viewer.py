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

"""Object viewer.
Able to display a widget for any python object. Allows editing of certain python attributes.
"""
import types
import pyui
import pyui.tree
from pyui.locals import *

class Viewer(pyui.widgets.Frame):

    def __init__(self, x, y, target):
        pyui.widgets.Frame.__init__(self, x, y, 400, 200, "%s" % target)
        if not target:
            print "Viewer: target in None"
            self.valid =0
            return
        self.target = target
        try:
            self.numAttributes = len(dir(self.target)) #len(self.target.__dict__)
        except:
            print "Viewer: target has no dict"
            self.valid = 0
            return
        self.setLayout(pyui.layouts.BorderLayoutManager())
        self.typeTabs = pyui.widgets.TabbedPanel()
        self.numPanel = self.typeTabs.addPanel("Numbers")
        self.stringPanel = self.typeTabs.addPanel("Strings")
        self.instancePanel = self.typeTabs.addPanel("Instances")
        self.collectionsPanel = self.typeTabs.addPanel("Collections")
        self.otherPanel = self.typeTabs.addPanel("Other")
        
        for attribute in dir(self.target): #.__dict__.keys():
            edit = self.createViewWidget(getattr(target, attribute), attribute) #target.__dict__[attribute], attribute)

        self.numPanel.setLayout(pyui.layouts.GridLayoutManager(1, len(self.numPanel.children)+1) )
        self.stringPanel.setLayout(pyui.layouts.GridLayoutManager(1, len(self.stringPanel.children)+1) )
        self.instancePanel.setLayout(pyui.layouts.GridLayoutManager(1, len(self.instancePanel.children)+1) )
        self.collectionsPanel.setLayout(pyui.layouts.GridLayoutManager(1, len(self.collectionsPanel.children)+1) )        
        self.otherPanel.setLayout(pyui.layouts.GridLayoutManager(1, len(self.otherPanel.children)+1) )

        self.typeTabs.resize(398,398)
        self.addChild(self.typeTabs, pyui.locals.CENTER)
        self.resize(400, 400)
        self.pack()
        self.valid = 1
        

    def createViewWidget(self, object, attribute):
        t = type(object)
        if t == types.IntType or t == types.LongType or t == types.FloatType:
            n = NumberViewer(self.target, attribute, object)
            self.numPanel.addChild(n)
            return n
        if t == types.InstanceType or t == types.ModuleType:
            n =  InstanceViewer(self.target, attribute, object)
            self.instancePanel.addChild(n)
            return n
        if t == types.StringType:
            n = StringViewer(self.target, attribute, object)
            self.stringPanel.addChild(n)
            return n
        if t == types.ListType:
            n = ListViewer(attribute, object)
            self.collectionsPanel.addChild(n)
            return n
        if t == types.DictType:
            n = DictViewer(attribute, object)
            self.collectionsPanel.addChild(n)
            return n

        if t == types.NoneType:
            n = NoneViewer(attribute)
            self.otherPanel.addChild(n)
            return n
        
        d =  DefaultViewer(attribute, object)
        self.otherPanel.addChild(d)
        return d
        
class NumberViewer(pyui.widgets.Panel):
    def __init__(self, target, attributeName, value):
        self.target = target
        self.attributeName = attributeName
        self.value = value
        pyui.widgets.Panel.__init__(self)
        self.setLayout(pyui.layouts.GridLayoutManager(3,1))
        self.label = pyui.widgets.Label(attributeName)
        self.typeLabel = pyui.widgets.Label(repr(type(value)))
        self.editValue = pyui.widgets.Edit("%d" % value, 64, self.onEnter)
        self.addChild(self.label)
        self.addChild(self.typeLabel)
        self.addChild(self.editValue)
        self.pack()
        
    def onEnter(self, obj):
        self.target.__dict__[self.attributeName] = int(self.editValue.text)

class StringViewer(pyui.widgets.Panel):
    def __init__(self, target, attributeName, value):
        self.target = target
        self.attributeName = attributeName
        self.value = value
        pyui.widgets.Panel.__init__(self)
        self.setLayout(pyui.layouts.GridLayoutManager(3,1))
        self.label = pyui.widgets.Label(attributeName)
        self.typeLabel = pyui.widgets.Label(repr(type(value)))                
        self.editValue = pyui.widgets.Edit(value, 64, self.onEnter)
        self.addChild(self.label)
        self.addChild(self.typeLabel)
        self.addChild(self.editValue)
        self.pack()
        
    def onEnter(self, obj):
        self.target.__dict__[self.attributeName] = self.editValue.text

class InstanceViewer(pyui.widgets.Button):
    def __init__(self, target, attributeName, value):
        self.target = target
        self.attributeName = attributeName
        self.value = value
        pyui.widgets.Button.__init__(self, repr(value), self.onButton)

    def onButton(self, button):
        newViewer = Viewer(self.posX + 30, self.posY + 30, self.value)

class NoneViewer(pyui.widgets.Label):
    def __init__(self, attributeName):
        pyui.widgets.Label.__init__(self, "%s: None" % attributeName)

class ListViewer(pyui.widgets.Panel):
    def __init__(self, attributeName, value):
        self.attributeName = attributeName
        self.value = value
        pyui.widgets.Panel.__init__(self)
        self.setLayout(pyui.layouts.GridLayoutManager(2, 1))

        self.box = pyui.widgets.ListBox()
        for v in self.value:
            self.box.addItem(repr(v), v)
        self.button = pyui.widgets.Button("view %s" % attributeName, self.onView)
        self.addChild(self.box)
        self.addChild(self.button)
        self.pack()
        
    def onView(self, button):
        item = self.box.getSelectedItem()
        if item and item.data:
            Viewer(self.posX + 30, self.posY + 30, item.data)

class DictViewer(pyui.widgets.Panel):
    def __init__(self, attributeName, value):
        self.attributeName = attributeName
        self.value = value
        pyui.widgets.Panel.__init__(self)
        self.setLayout(pyui.layouts.GridLayoutManager(2, 1))

        self.box = pyui.widgets.ListBox()
        for v in self.value.keys():
            self.box.addItem(repr(v), v)
        self.button = pyui.widgets.Button("view %s" % attributeName, self.onView)
        self.addChild(self.box)
        self.addChild(self.button)
        self.pack()
        
    def onView(self, button):
        item = self.box.getSelectedItem()
        if item and item.data:
            Viewer(self.posX + 30, self.posY + 30, self.value[item.data])
    
class DefaultViewer(pyui.widgets.Panel):
    def __init__(self, attributeName, value):
        self.attributeName = attributeName
        self.value = value
        pyui.widgets.Panel.__init__(self)
        self.setLayout(pyui.layouts.GridLayoutManager(3,1))        
        self.label = pyui.widgets.Label("%s:" % attributeName)
        self.typeLabel = pyui.widgets.Label(repr(type(value)))        
        self.editValue = pyui.widgets.Label(repr(value))
        self.addChild(self.label)
        self.addChild(self.typeLabel)
        self.addChild(self.editValue)
        self.pack()
    


class TreeViewerWindow(pyui.widgets.Frame):
    """A tree widget based python object viewer.
    """

    def __init__(self, x, y, w, h, target):
        pyui.widgets.Frame.__init__(self, x, y, w, h, repr(target))
        self.target = target
        self.treePanel = TreeViewPanel(target)
        self.replacePanel(self.treePanel)
        self.treePanel.resize(w,h)


class TreeViewPanel(pyui.widgets.Panel):
    """A panel in a treeViewWindow that includes the tree view of an object.
    """
    def __init__(self, target):
        pyui.widgets.Panel.__init__(self)
        self.target = target
        self.tree = pyui.tree.Tree()
        node = ViewerNode(repr(target), target)
        node.target = target
        node.populated = 0
        self.tree.topNode.addNode(node)
        self.populateNode(node)
        self.registerEvent(pyui.locals.TREENODE_SELECTED, self.onSelect)
        self.addChild(self.tree)

    def resize(self, w, h):
        self.tree.resize(w,h)
        pyui.widgets.Panel.resize(self, w,h)
        
    def populateNode(self, node):
        print "populating", node, node.title, type(node.target)
        t = type(node.target)
        if t == types.ListType or t == types.TupleType:
            for attr in node.target:
                newNode = ViewerNode(repr(attr), attr)
                newNode.populated = 0
                node.addNode(newNode)
        elif t == types.DictType:
            print "populating a dict"
            for attr in node.target.keys():
                newNode = ViewerNode(repr(attr), node.target[attr])
                newNode.populated = 0
                node.addNode(newNode)
        elif t == types.IntType or t == types.LongType or t == types.FloatType:
            newNode = ViewerNode(repr(node.target), None)
            node.addNode(newNode)
        elif t == types.StringType:
            newNode = ViewerNode(repr(node.target), None)
            node.addNode(newNode)
        else:
            for attr in dir(node.target):
                newNode = ViewerNode(attr, getattr(node.target, attr) )
                newNode.populated = 0
                node.addNode(newNode)
        node.populated = 1
        
    def onSelect(self, event):
        if not event.node.populated:
            self.populateNode(event.node)


class ViewerNode(pyui.tree.TreeNode):
    def __init__(self, title, target):
        self.target = target
        bitmap = "none.png"
        t = type(target)
        if t == types.IntType or t == types.LongType or t == types.FloatType:
            bitmap = "numeric.png"
        elif t == types.InstanceType or t == types.ModuleType:
            bitmap = "instance.png"
        elif t == types.StringType:
            bitmap = "string.png"
        elif t == types.DictType:
            bitmap = "dictionary.png"
            print "got a dict"            
        elif t == types.ListType or t == types.TupleType:
            bitmap = "list.png"
        elif t == types.MethodType or t == types.FunctionType or t == types.BuiltinFunctionType or t == types.BuiltinMethodType:
            bitmap = "function.png"
        print "adding", target, bitmap
        pyui.tree.TreeNode.__init__(self, title, bitmap)
