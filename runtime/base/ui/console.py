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
		
