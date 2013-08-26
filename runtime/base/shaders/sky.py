import math
import sylphis

glowAmount = 0.20
glowSub = 0.0

def setGlowAmount(value):
    global glowAmount
    glowAmount = value
    
def getGlowAmount():
    return glowAmount

def setGlowSub(value):
    global glowSub
    glowSub = value

def getGlowSub():
    return glowSub;
    
exposure = 1.0

def setExposure(value):
	global exposure
	exposure = value

def getExposure():
	return exposure * 0.01

InvWaveLength = (0.650,0.570,0.475)
h = (1.650,0.570,1.475)
m_Kr    = 0.035;
m_Kr4PI = m_Kr*4.0*math.pi;
m_Km    = 0.010;
m_Km4PI = m_Km*4.0*math.pi;
m_ESun  = 1000.0;	
m_g     = -0.840;	
m_g2    = m_g * m_g;	

fKrESun = m_Kr * m_ESun
fKmESun = m_Km * m_ESun

def setG(g):
    global m_g
    global m_g2
    m_g = g
    m_g2= g*g

def getG():
    return m_g
    
def getG2():
    return m_g2

def setKr(a):
	global m_Kr, m_Kr4PI, fKrESun
	m_Kr = a
	m_Kr4PI = m_Kr*4.0*math.pi
	fKrESun = m_Kr * m_ESun

def setWaveLength(a,b,c):
	global InvWaveLength
	InvWaveLength = 1.0 / pow(a,4.0), 1.0 / pow(b,4.0), 1.0 / pow(c,4.0)

setWaveLength(0.650,0.570,0.475)

m_fRayleighScaleDepth = 0.25;
m_fMieScaleDepth = 0.1;

fScaleOverScaleDepth = (1.0 / (10.25 - 10.0)) / m_fRayleighScaleDepth

def getInvWaveLength():
	return InvWaveLength
	
lightDir = sylphis.CVector3()
	
def setLightDir_(a,b = 10,c = 1):
	global lightDir
	if b == 10:
		b = 1.0 - a
	lightDir.x = a;
	lightDir.y = b;
	lightDir.z = c;
	lightDir = lightDir.normalized()

def setLightDir(a,b = 10,c = 1):
    import entry
    entry.client.actors.getActorByName('l1').node.setOrientation(sylphis.CQuaternion(1,0,0,a)*sylphis.CQuaternion(0,1,0,0.2))
	
def getLightDir():
	return lightDir.x, lightDir.y, lightDir.z
	

import mthread

def daynight():
	t = 0.0
	mthread.sleep(15.0)
	while t < 10.0:
		setLightDir((t * 0.08) % 2.0)
		t += mthread.sleep(0.0001)
	
mthread.parallelize(daynight)

