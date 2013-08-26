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

import sylphis
from CWeapon import *
from CProjectile import *

class CGrenadeLauncher(CWeapon):
    __classname__ = 'CGrenadeLauncher'
    model = 'models/pistol.model'
    fire_sound = 'sounds/shot.wav'
    dryfire_sound = 'sounds/dryfire.wav'
    reload_sound = 'sounds/reload.wav'
    auto_reload = 1
    fire_delay = 0.5
    ammo_class = 'CGrenade'
    clip_ammo = 5
    clips = 10
    ammo = 10
    
class CGrenade(CProjectile):
    __classname__ = 'CGrenade'
    model = 'models/bball.md3'
    hit_sound = 'sounds/bullet_hit.wav'
    health = 50.0
    velocity = sylphis.CVector3(0, 0, -800)
    angular_velocity = sylphis.CVector3(0, 0, 0)
    damping = 0.0001
    radius = 20.0
    bounce = 0
    mass = 10.0
    fuse = 5.0
    damage_factor = 0.01
    detonate_on_fuse = 1
    detonate_on_death = 0
    detonate_on_world = 0
    detonate_on_actor = 1
    
