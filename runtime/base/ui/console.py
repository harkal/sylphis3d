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

import sys
import sylphis
import string
import pyui
from pyui.desktop import getDesktop

class CConsoleOutput(pyui.dialogs.ConsoleOutput):
    def __init__(self):
        pyui.dialogs.ConsoleOutput.__init__(self)
        
    def write(self, text):
        text = string.rstrip(text)
        if len(text) < 1:
            return
        text = string.replace(text, "\n", " ")
        text = string.replace(text, "\r", " ")      
        self.append(text)
        getDesktop().postUserEvent(pyui.dialogs.EVENT_OUTPUT)
        
    def append(self, text):
        if len(self.lines) > 100:
            del self.lines[0]
        self.lines.append(text)

class CConsole(pyui.dialogs.Console, sylphis.CPyConsole):
	def __init__(self, x, y, w, h):
		sylphis.CPyConsole.__init__(self)
		pyui.dialogs.Console.__init__(self, x, y, w, h)
		self.output = CConsoleOutput()

		self.self = self
		self.execCommand("from entry import client")
		self.execCommand("import mthread")
		self.execCommand("vfs = CEngine.vfs")

	def execCommand(self, command):
            try:
                exec command in globals(), self.locals
	    except:
                print "Exception on command '%s':" % command
                print ">    %s" % sys.exc_value
		
	def _pyuiGo(self, object):
            command = self.inputBox.text
            print ">%s" % command
            self.execCommand(command)
            return 1
		
	def printString(self, s):
		self.output.write(s)
		
	def onCloseButton(self):
		self.setShow(False)
		return 1
		
	def destroy(self):
		pyui.dialogs.Console.destroy(self)
		
