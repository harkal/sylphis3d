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
from pyui.base import Panel

class CVariableControl(Panel):
    def __init__(self, label, func, rangeStart = 0.0, rangeEnd = 1.0, gran = 0.1, initPos = 0.0, format = "%4.2f"):
        Panel.__init__(self)
        self.rangeStart = rangeStart
        self.rangeEnd = rangeEnd
        self.gran = 1 / gran
        self.func = func
        self.format = format

        steps = (rangeEnd - rangeStart) / gran
        self.slide = pyui.widgets.SliderBar( self._onSlide, range = steps, initialPos = self._getPos(initPos))
        self.slide.resize(100, 20)
        self.val = pyui.widgets.Label(self.format % initPos)

        self.addChild(pyui.widgets.Label(label))
        self.addChild(self.slide)
        self.addChild(self.val)
        self.resize(150, 40)

    def _getPos(self, value):
        if value > self.rangeEnd:
            return self.rangeEnd
        if value < self.rangeStart:
            return self.rangeStart

        value /= self.rangeEnd - self.rangeStart
        value -= self.rangeStart

        return value * self.gran

    def _getValue(self, pos):
        pos /= self.gran
        pos += self.rangeStart
        return pos

    def _onSlide(self, value):
        v = self._getValue(value)
        self.func(v)
        self.val.setText(self.format % v)


class CVariableControlWindow(pyui.dialogs.Dialog):
    def __init__(self, title, func, rangeStart = 0.0, rangeEnd = 1.0, gran = 0.1, initPos = 0.0, format = "%4.2f"):
        pyui.dialogs.Dialog.__init__(self, title = title)
        self.addChild(CVariableControl(title, func, rangeStart, rangeEnd, gran, initPos, format))
        self.pack()
                            
    def onCloseButton(self):
	self.close()
	return 1

    
