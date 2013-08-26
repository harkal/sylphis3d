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

import sylphis
from CWeapon import *
from CProjectile import *

class CPistol(CWeapon):
    __classname__ = 'CPistol'
    model = 'models/pistol.model'
    fire_sound = 'sounds/shot.wav'
    dryfire_sound = 'sounds/dryfire.wav'
    reload_sound = 'sounds/reload.wav'
    auto_reload = 1
    fire_delay = 0.5
    ammo_class = 'CBullet'
    clip_ammo = 5
    clips = 10
    ammo = 10
    
class CBullet(CProjectile):
    __classname__ = 'CBullet'
    model = ''
    hit_sound = 'sounds/bullet_hit.wav'
    health = 5.0
    damage_factor = 100.0
    velocity = sylphis.CVector3(0, 0, -4000.0)
    angular_velocity = sylphis.CVector3(0, 0, 0)
    damping = 0.0
    radius = 3.0
    bounce = 0
    mass = 1.0
    fuse = 2.0
    detonate_on_fuse = 1
    detonate_on_death = 1
    detonate_on_world = 1
    detonate_on_actor = 1