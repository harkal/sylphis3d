
import pyui
from pyui import locals
from pyui.desktop import getRenderer

aboutDialog = pyui.dialogs.Dialog(title = "About Sylphis", w = 400, h = 220)

def _onOK(self):
    aboutDialog.close()
    return

self = aboutDialog

#self.font = getRenderer().createFont('arialblack', 25, 0)
layout = pyui.layouts.BorderLayoutManager()
self.setLayout(layout)
center = pyui.widgets.Label.CENTER
text = """
Sylphis 3D Engine
Version  0.95

Copyright (c) 2002 - 2006 Harry Kalogirou
All rights reserved

Website : http://www.sylphis3d.com
"""
self.textLabel = pyui.widgets.Label(text,  align = center, shadow = 1)
self.okButton = pyui.widgets.Button("OK", _onOK)

self.addChild(self.textLabel, layout.CENTER)
self.addChild(self.okButton, layout.SOUTH)
self.pack()



