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
                
