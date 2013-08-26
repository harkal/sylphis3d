"""
This file is part of the "Sylphis" 3D engine

Copyright (c) 2002 - 2006 by Harry Kalogirou
All rights reserved. Email: harkal@sylphis3d.com

<<--LICENSE-->>

"""

import mthread
import sylphis
import __builtin__

def playAndWait(self, buffer = None):
    if buffer != None:
        self.play(buffer)
    else:
        self.play()
    self.wait()
    mthread.sleep()
    
sylphis.CSoundSource.playAndWait = playAndWait

def waitAction(self):
    self._waitAction()
    mthread.sleep()

sylphis.CSceneModel._waitAction = sylphis.CSceneModel.waitAction
sylphis.CSceneModel.waitAction = waitAction

def CVector3__repr__(self):
    return "%f %f %f" % (self.x, self.y, self.z)

sylphis.CVector3.__repr__ = CVector3__repr__

def CQuaternion__repr__(self):
    return "%f %f %f %f" % (self.x, self.y, self.z, self.w)

sylphis.CQuaternion.__repr__ = CQuaternion__repr__

# Singleton classes
__builtin__.CEngine = sylphis.CEngine.getSingleton()
__builtin__.CPhysicsEnv = sylphis.CPhysicsEnv.getSingleton()


