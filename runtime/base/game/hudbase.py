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

def drawCenteredText(font, str, y = -1):
    (w, h) = font.stringSize(str)
    x = 400.0 - w / 2
    if y == -1:
        y = 300 - h / 2
        font.renderString(str, x, y)

def strZeroPad(n, digits):
    s = str(int(n))
    if len(s) < digits:
        s = ('0' * (digits - len(s))) + s
    return s

class CHUDBase:
    def __init__(self):
        self.crosshair = CEngine.mMaterials.registerMaterial('crosshair')
        self.back1 = CEngine.mMaterials.registerMaterial('hud/malakia1')
        self.hiscores = CEngine.mMaterials.registerMaterial('fonts/hiscore')
        self.font = CEngine.fonts.registerTextureFont("fonts/casual.font")
        self.font2 = CEngine.fonts.registerBitmapFont("fonts/times.ttf", 20)
        self.time = (0,0)
        self.hiscore = 0

    def drawScores(self, score):
        # HISCORE
        CEngine.renderer.drawPic(20, 20, 150, 35, self.hiscores, 0.22, 0.56, 0.78, 0.69)
        # SCORE
        CEngine.renderer.drawPic(625, 20, 150, 35, self.hiscores, 0.22, 0.43, 0.78, 0.56)
        # TIME
        CEngine.renderer.drawPic(315, 15, 170, 50, self.hiscores, 0.22, 0.30, 0.78, 0.43)
        
        # Draw scores
        self.font.renderString(strZeroPad(self.hiscore,6), 30, 40, 280)
        self.font.renderString(strZeroPad(score,6), 635, 40, 280)

        # Draw time
        time = strZeroPad(self.time[0],2) + ':' + strZeroPad(self.time[1],2)
        self.font.renderString(time, 347, 50, 320)

    def render(self, player):
        CEngine.renderer.drawPic(400 - 12, 300 - 12, 24, 24, self.crosshair)
        CEngine.renderer.drawPic(20, 450, 128, 128, self.back1)
        self.drawScores(player.score)
        

        
        
        
