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

    
