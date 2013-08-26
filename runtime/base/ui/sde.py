"""
This file is part of the "Sylphis" 3D engine
Copyright (c) 2002 - 2007 by Harry Kalogirou

	Copyright (c) 2013 Charilaos Kalogirou.
	All rights reserved.

	Redistribution and use in source and binary forms are permitted
	provided that the above copyright notice and this paragraph are
	duplicated in all such forms and that any documentation,
	advertising materials, and other materials related to such
	distribution and use acknowledge that the software was developed
	by Charilaos Kalogirou. The name of the
	Charilaos Kalogirou may not be used to endorse or promote products derived
	from this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
"""

import pyui
from pyui import locals
from pyui.desktop import getRenderer
import actor
import variablecontrol

class CActorClassesWindow(pyui.dialogs.Dialog):
    def onCloseButton(self):
        self.close()
        return 1
    
class CActorWindow(pyui.dialogs.Dialog):
    def __init__(self):
        pyui.dialogs.Dialog.__init__(self, title = 'Actors', w = 200, h = 400)
        self.setLayout(pyui.layouts.BorderLayoutManager())
        self.actorsList = pyui.widgets.ListBox()
        self.addChild(self.actorsList, pyui.locals.CENTER)
        self.pack()
        CClient.actors.registerWatcher(self.update)
        self.update()
        
    def update(self):
        actors = CClient.actors.getActors()
        self.actorsList.clear()
        for a in actors:
            self.actorsList.addItem(a.__dict__.get('name','unnamed') + '(' + a.__classname__ + ')', a)
        
actorsWindow = CActorWindow()
            
actorClassesWindow = CActorClassesWindow(title = "Add actor", w = 230, h = 450)

layout = pyui.layouts.FlowLayoutManager()
actorClassesWindow.setLayout(layout)
center = pyui.widgets.Label.CENTER

actorClassesWindow.glowLabel = pyui.widgets.Label("Glow effect",  align = center, shadow = 1)
listBox = pyui.widgets.ListBox()
classes = actor.actorClasses.keys()
classes.sort()
for k in classes:
    listBox.addItem(k, k)

actorClassesWindow.addChild(pyui.widgets.Label("Actor classes",  align = center, shadow = 1))
actorClassesWindow.addChild(listBox,layout.NEWLINE)

def _onAddActor(self):
    if listBox.getSelectedItem() is None:
        return
    classname = listBox.getSelectedItem().data
    print "******", classname
    CClient.actors.spawn({'classname':classname})
    

actorClassesWindow.addChild(pyui.widgets.Button("Add", _onAddActor))

#self.addChild(self.glowLabel)

#self.addChild(variablecontrol.CVariableControl('LightPos : ', sky.setLightDir, -2.0, 1.0, 0.01), layout.NEWLINE)
#self.addChild(variablecontrol.CVariableControl('Atmo G : ', sky.setG, -1.0, -0.1, 0.001), layout.NEWLINE)
#self.addChild(variablecontrol.CVariableControl('HDR exposure : ', sky.setKr, 0.0001, 0.0025, 0.0001), layout.NEWLINE)
actorClassesWindow.pack()

