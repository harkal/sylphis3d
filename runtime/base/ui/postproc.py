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

from shaders import sky

import variablecontrol

class PostProcDialog(pyui.dialogs.Dialog):
    def onCloseButton(self):
	self.close()
	return 1
    
postProcDialog = PostProcDialog(title = "Post processing control", w = 360, h = 150)

def _onOK(self):
    postProcDialog.close()
    return

self = postProcDialog

#self.font = getRenderer().createFont('arialblack', 25, 0)
layout = pyui.layouts.FlowLayoutManager()
self.setLayout(layout)
center = pyui.widgets.Label.CENTER


self.glowLabel = pyui.widgets.Label("Glow effect",  align = center, shadow = 1)

self.okButton = pyui.widgets.Button("OK", _onOK)

self.addChild(self.glowLabel)

self.addChild(variablecontrol.CVariableControl('LightPos : ', sky.setLightDir, -2.0, 1.0, 0.01), layout.NEWLINE)
self.addChild(variablecontrol.CVariableControl('Atmo G : ', sky.setG, -1.0, -0.1, 0.001), layout.NEWLINE)
self.addChild(variablecontrol.CVariableControl('HDR exposure : ', sky.setKr, 0.0001, 0.0025, 0.0001), layout.NEWLINE)
self.pack()

