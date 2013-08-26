"""
This file is part of the "Sylphis" 3D engine
Copyright (c) 2002 - 2007 by Harry Kalogirou

"Sylphis3D Game Engine" is free software; you can redistribute it and/or 
modify it under the terms of the GNU General Public License as 
published by the Free Software Foundation; either version 2 of the 
License, or (at your option) any later version, 
plus a special exception:
  Linking "Sylphis3D Game Engine" statically or dynamically with other 
modules is making a combined work based on this library. Thus, the 
terms and conditions of the GNU General Public License cover the whole 
combination. As a special exception, the copyright holders of this 
library give you permission to link this library with independent 
modules to produce an executable, regardless of the license terms of 
these independent modules, and to copy and distribute the resulting 
executable under terms of your choice, provided that you also meet, 
for each linked independent module, the terms and conditions of the 
license of that module. An independent module is a module which is 
not derived from or based on this program. If you modify this program,
you may extend this exception to your version of the library, but you 
are not obligated to do so. If you do not wish to do so, delete this 
exception statement from your version.

"Sylphis3D Game Engine" is distributed in the hope that it will be 
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Sylphis3D Game Engine"; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
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

