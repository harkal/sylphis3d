"""
This file is part of the "Sylphis" 3D engine

Copyright (c) 2002 - 2006 by Harry Kalogirou
All rights reserved. Email: harkal@sylphis3d.com

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
import string
import xml.dom.minidom

def getString(dom, name, default = ''):
    try:
        nodes = dom.getElementsByTagName(name)[0].childNodes
    except e:
        return default
    rc = ""
    for node in nodes:
        if node.nodeType == node.TEXT_NODE:
            rc = rc + str(node.data)
    return rc
    
def getVector3(dom, name):
	str = getString(dom, name, '0.0 0.0 0.0')
	x,y,z = string.split(str)
	return sylphis.CVector3(float(x), float(y), float(z))
	

class CPSParser:
    def parseSpawn(self, p):
        self.spawnDirection = str(p.getAttribute('direction'))
        self.spawnVariation = float(str(p.getAttribute('variation')))
        self.spawnVelocityMin = float(str(p.getAttribute('velocitymin')))
        self.spawnVelocityMax = float(str(p.getAttribute('velocitymax')))
       
    def parseCollitions(self, nodes):
        self.collitionsEnabled = False
        if len(nodes) == 0:
            return

        p = nodes[0]
        if str(p.getAttribute('enabled')) == 'true':
            self.collitionsEnabled = True
            
        self.collitionBounce = float(str(p.getAttribute('bounce')))
        	
    def parseEmitter(self, p):
        self.emitterShape = str(p.getAttribute('shape'))
        if self.emitterShape == 'sphere':
       	    self.emitterSize = sylphis.CVector3(float(getString(p, 'radius')), 0.0, 0.0)
        elif self.emitterShape == 'box':
       	    raise 'Unimplemented box parser'    
    	    
        self.emitterRate = float(getString(p, 'rate', '100.0'))
        self.emitterTime = float(getString(p, 'time', '-1.0'))
        
    def parseInterpolator(self, p):
        pairs = {}
        if len(p):
            nodes = p[0].getElementsByTagName('node')
            for node in nodes:
                key = float(str(node.getAttribute('key')))
                data = 0
                fn = node.getElementsByTagName('float')
                if len(fn):
                    data = float(getString(node, 'float'))
                fn = node.getElementsByTagName('vector')
                if len(fn):
                    sdata = getString(node, 'vector')
                    x,y,z = string.split(sdata)
                    data = sylphis.CVector3(float(x), float(y), float(z))

                pairs[key] = data
                
        return pairs
        
    def parseParticleDef(self, p):
        self.particletype = str(p.getAttribute('type'))
        
        node = p.getElementsByTagName('size')[0]
        nodes = node.getElementsByTagName('interpolator')
        self.size = self.parseInterpolator(nodes)
        
        node = p.getElementsByTagName('aspectratio')[0]
        nodes = node.getElementsByTagName('interpolator')
        self.aspect = self.parseInterpolator(nodes)
        	
    	    
    def parseParticleSystem(self, p):
        self.parseParticleDef(p.getElementsByTagName('particledef')[0])
    	      	
        self.material = getString(p, 'material')
        self.budget = int(getString(p, 'budget', '100.0'))
        self.rotation = float(getString(p, 'rotation', '0.0'))
        self.rotationSpeed = float(getString(p, 'rotationspeed', '0.0'))
        self.acceleration = getVector3(p, 'acceleration')
        self.lifeTime = float(getString(p, 'lifetime', '1.0'))
    	    	
        self.parseSpawn(p.getElementsByTagName('spawn')[0])
        self.parseEmitter(p.getElementsByTagName('emitter')[0])
        self.parseCollitions(p.getElementsByTagName('collitions'))
    	    	
    def makeParticleSystemDef(self):
        d = sylphis.CParticleSystemDef()

        d.mBudget = self.budget
        d.mRotation = self.rotation
        d.mRotationSpeed = self.rotationSpeed
        
        if self.particletype == 'tracer':
            d.mParticleType = sylphis.PARTICLE_TRACER
        else:
            d.mParticleType = sylphis.PARTICLE_BILLBOARD


        if self.emitterShape == 'sphere':
            d.mEmitterShape = sylphis.PARTICLE_EMITTER_SPHERE
        else:
            d.mEmitterShape = sylphis.PARTICLE_EMITTER_BOX
            
        d.mEmitterDimensions = self.emitterSize
        d.mEmitRate = self.emitterRate
        d.mEmitTime = self.emitterTime
        
        d.mLifeTime = self.lifeTime
        
        d.mAcceleration = self.acceleration
        
        if self.spawnDirection == 'natural':
            d.mSpawnDirection = sylphis.PARTICLE_SPAWN_DIRECTION_NATURAL
        else:
            d.mSpawnDirection = sylphis.PARTICLE_SPAWN_DIRECTION_AIMED
            
        d.mSpawnDirectionVariation = self.spawnVariation
        d.mSpawnVelocityMax = self.spawnVelocityMax
        d.mSpawnVelocityMin = self.spawnVelocityMin
        
        d.mCollitionsEnabled = self.collitionsEnabled
        d.mBounce = self.collitionBounce
        
        d.setMaterial(self.material)
        
        keys = self.size.keys()
        keys.sort()
        for key in keys:
            d.getSizeEnvelope().insert(self.size[key], key)
            
        keys = self.aspect.keys()
        keys.sort()
        for key in keys:
            d.getAspectRatioEnvelope().insert(self.aspect[key], key)
    		
        return d
		
    def parseString(self, s):
        self.dom = xml.dom.minidom.parseString(s)
        ps = self.dom.getElementsByTagName('particlesystem')
        
        for p in ps:
            self.parseParticleSystem(p)
                        
        return self.makeParticleSystemDef()
        
