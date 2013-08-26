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
import console
import sylphis

import configfile
import cmdline

res = configfile.gCfgFile.r_videomode
if res == 2:
    pyui.init(800, 600)
elif res == 3:
    pyui.init(1024, 768)
elif res == 4:
    pyui.init(1152, 864)
elif res == 5:
    pyui.init(1280, 1024)
else:
    pyui.init(640, 480)

from menus import menubar

con = console.CConsole(30,30,480,300)
con.show = 0

if hasattr(configfile.gCfgFile, 'startup_dialog'):
    try:
        if cmdline.getParamValue('map') is None:
            st = getObject(configfile.gCfgFile.startup_dialog)
            st.doModal()
            pyui.activate()
    except AttributeError:
        print "WARNING : '" + configfile.gCfgFile.startup_dialog + "' not found!"
    



