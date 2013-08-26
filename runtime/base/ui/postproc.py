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

