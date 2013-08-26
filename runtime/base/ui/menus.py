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
import ui
import sde
from configfile import gCfgFile
from entry import client

def onbutton(self):
    shutdown()
    
def tongleConsole(self):
    ui.con.show = not ui.con.show

def debugPortals(self):
    gCfgFile.sm_debugportals = not gCfgFile.sm_debugportals

def debugFPS(self):
    client.drawFPS = not client.drawFPS

def debugTime(self):
    client.drawPerf = not client.drawPerf

def onFileSelected(filePath):
    client.loadMap(filePath)
    pyui.deactivate()
    return
    
def onLoadMap(self):
    fileDialog = pyui.dialogs.FileDialog("maps", onFileSelected, ".*cmap")
    return

def onAbout(self):
    from about import aboutDialog
    aboutDialog.doModal()

def onPostProc(self):
    from postproc import postProcDialog
    postProcDialog.setShow(True)

def change():
    CEngine.destroyRenderingView()
    CEngine.createRenderingView()
                
menubar = pyui.widgets.MenuBar()
m = pyui.widgets.Menu("Game")
m.addItem("Load Map", onLoadMap)
m.addItem("-")
m.addItem("Quit", onbutton)
menubar.addMenu(m)
        
if gCfgFile.devel_menus:
    m = pyui.widgets.Menu("Developer")
    m.addItem("Toggle console", tongleConsole)
    m.addItem("Tongle debug portals", debugPortals)
    m.addItem("Tongle frames per second", debugFPS)
    m.addItem("Tongle timings", debugTime)
    m.addItem("Test post-processing", onPostProc)
    menubar.addMenu(m)

m = pyui.widgets.Menu("Help")
m.addItem("About", onAbout)
menubar.addMenu(m)
                
