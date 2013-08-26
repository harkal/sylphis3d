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

startDialog = pyui.dialogs.Dialog(title = "Welcome to Sylphis 3D", w = 400, h = 220)

def _onOK(self):
    startDialog.close()
    return

self = startDialog

layout = pyui.layouts.BorderLayoutManager()
self.setLayout(layout)
center = pyui.widgets.Label.CENTER
text = """
Welcome to Sylphis 3D.

There is no world loaded!!

Quick help :
F9  : toggles the user interface
ESC : quit
"""
self.textLabel = pyui.widgets.Label(text, shadow = 1)
self.okButton = pyui.widgets.Button("OK", _onOK)

self.addChild(self.textLabel, layout.CENTER)
self.addChild(self.okButton, layout.SOUTH)
self.pack()


