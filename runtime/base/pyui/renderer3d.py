# PyUI
# Copyright (C) 2001-2002 Sean C. Riley
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of version 2.1 of the GNU Lesser General Public
# License as published by the Free Software Foundation.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

from pyui.rendererBase import RendererBase

class Renderer3DBase(RendererBase):

    def __init__(self, w, h, fullscreen, title):
        RendererBase.__init__(self, w, h, fullscreen, title)
        
    #####################################
    ##  3D interface
    ##
    #####################################

    def createView(self, world):
        1

    def destroyView(self, viewHandle):
        pass

    def createObject(self, model):
        1

    def destroyObject(self, handle):
        pass

    def createWorld(self):
        1

    def destroyWorld(self, worldHandle):
        pass

    def updateWorld(self, worldHandle):
        pass

    def addToWorld(self, worldHandle, objectHandle):
        pass

    def removeFromWorld(self, worldHandle, objectHandle):
        pass

    def setObjectScale(self, objectHandle, scale):
        pass

    def setObjectPos(self, objectHandle, pos):
        pass

    def setObjectAnimation(self, objectHandle, animation, onCompleted = None, blendTime = 0.0, loop = 1):
        pass

    def loadAnimation(self, animation):
        pass

    def setObjectYPR(self, objectHandle, YPR):
        pass
    
    def moveObjectTo(self, objectHandle, location, moveRate, turnRate = 0, onCompleted = None):
        pass

    def moveObject(self, objectHandle, delta, moveRate, turnRate = 0, onCompleted = None):
        pass

    def rotateObjectTo(self, objectHandle, orientation, turnRate, onCompleted = None):
        pass

    def rotateObject(self, objectHandle, delta, turnRate, onCompleted = None):
        pass

    def attachObject(self, objectHandle, toObjectHandle, connectionPointName = "", toConnectionPointName = ""):
        pass

    def detachObject(self, objectHandle, fromObjectHandle):
        pass

    def setViewProjectionMode(self, viewHandle, projectionMode):
        pass

    def setViewParameters(self, viewHandle, parameters):
        pass

    def setCameraYPR(self, viewHandle, YPR):
        pass

    def setCameraPos(self, viewHandle, pos):
        pass

    def getCameraYPR(self, viewHandle):
        pass

    def getCameraPos(self, viewHandle):
        pass

    def getCameraDir(self, viewHandle):
        pass

    def moveCamera(self, viewHandle, offset):
        pass

    def setLightParameters(self, viewHandle, YPR):
        pass

    def getDesktopWindow(self):
        pass

    def attachView(self, windowHandle, viewHandle):
        pass
    
    def pickView(self, viewHandle, xy):
        return (0,(0,0))

    def attachController(self, objectHandle, controllerType, boneName):
        return None

    def setController(self, controllerHandle, **parms):
        pass

    def detachController(self, objectHandle, controllerHandle):
        pass
        
    def getObjectNode(self, objectHandle, nodeName, LODLevel):
    	pass
    	
    def getObjectProjectedPos(self, objectHandle, viewHandle):
    	return (0,0)

    def getNodeProjectedPos(self, nodeHandle, viewHandle):
    	return (0,0)

    def attachWindow(self, windowHandle, xoffset, yoffset, objectHandle, viewHandle, nodeName):
        return 1

    def detachWindow(self, windowHandle):
        pass
    
    def createFont(self, fontName, size, flag):
        return None

    def destroyFont(self, fontHandle):
        pass

    def playSound(self, waveFileName, completionCallback = None):
        pass

    def stopSound(self, waveFileName):
        pass

    def loadSound(self, waveFileName):
        pass

    def playMusic(self, waveFileName, completionCallback = None):
        pass

    def stopMusic(self, waveFileName):
        pass
    
    def loadMusic(self, waveFileName):
        pass
    
    def toggleDebugInfo(self):
        pass

    def setWindowEffect(self, windowHandle, effectName):
        pass

    def createEmptyBody(self, xyz=(0.0,0.0,0.0), label="emptyBody"):
        return 1
    
    def addGeometryNode(self, objectHandle, bone=0):
        return 1

    def addGeometryPiece(self, node, iType, info, offset, ypr, effect = "", effectParams = {}):
        return 1

    def getNodeEffect(self, node, num):
        return 1

    def setEffectParameters(self, effect, parms):
        return 1

    def dumpCache(self):
        pass
