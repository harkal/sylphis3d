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
import sys, sylphis
from string import *
from CSimpleActor import *
from util import manage
import pyui

class CParticleSystem(CSimpleActor):
    __classname__ = 'particle_system'
    __desc__ = 'A particle system'
    __actortype__ = 'e'
    __specs__ = [
        {'name':'emitter', 'doc':'The emitting point of the particle system'}
    ]
    
    emitter = ''
    
    def __init__(self, *args):
        CSimpleActor.__init__(self, *args)

    def join(self, ent):
        CSimpleActor.join(self, ent)
        
        from core.particlesystem import manager
        
        self.partdef = manager.registerParticleSystem(self.particledef)
       	
        m = manage(sylphis.CRenderableMultiMesh, self.mScene)
        p = manage(sylphis.CParticleSystem)
        
        p.setDefinition(self.partdef)
        p.setDefaultShader(self.partdef.getMaterial())
        p.setDieAfterUse(False)
        self.mScene.addParticleSystem(p)
        m.getBound().setBounds(sylphis.CVector3(-1000, -1000, -1000), sylphis.CVector3(1000, 1000, 1000))
        m.addMesh(p)
        	
        m.setPosition(self.origin)
        
        self.node = m
        self.particleSystem = p

    def start(self):
        CSimpleActor.start(self)
        if self.emitter != '':
            emitter = self.mCont.actors.getActorByName(self.emitter)
            if emitter != None:
                self.particleSystem.setEmitter(emitter.node)

    def Action_fire(self, other):
        print "FIRE"
        self.particleSystem.fire()
        	
        	

        

