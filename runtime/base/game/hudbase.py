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
        

        
        
        
