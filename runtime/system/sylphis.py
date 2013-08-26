# This file was created automatically by SWIG.
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.
import _sylphis
def _swig_setattr(self,class_type,name,value):
    if (name == "this"):
        if isinstance(value, class_type):
            self.__dict__[name] = value.this
            if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
            del value.thisown
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    self.__dict__[name] = value

def _swig_getattr(self,class_type,name):
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


class CSerializable(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSerializable, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CSerializable, name)
    __swig_getmethods__["factory"] = lambda x: _sylphis.CSerializable_factory
    if _newclass:factory = staticmethod(_sylphis.CSerializable_factory)
    __swig_getmethods__["cast"] = lambda x: _sylphis.CSerializable_cast
    if _newclass:cast = staticmethod(_sylphis.CSerializable_cast)
    def getClassName(*args): return apply(_sylphis.CSerializable_getClassName,args)
    def getClassInfo(*args): return apply(_sylphis.CSerializable_getClassInfo,args)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSerializable,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSerializable):
        try:
            if self.thisown: destroy(self)
        except: pass
    def save(*args): return apply(_sylphis.CSerializable_save,args)
    def load(*args): return apply(_sylphis.CSerializable_load,args)
    def __repr__(self):
        return "<C CSerializable instance at %s>" % (self.this,)

class CSerializablePtr(CSerializable):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSerializable
_sylphis.CSerializable_swigregister(CSerializablePtr)
cvar = _sylphis.cvar
CSerializable_factory = _sylphis.CSerializable_factory

CSerializable_cast = _sylphis.CSerializable_cast


class CSerializer(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSerializer, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CSerializer, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSerializer,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSerializer):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setFile(*args): return apply(_sylphis.CSerializer_setFile,args)
    def getFile(*args): return apply(_sylphis.CSerializer_getFile,args)
    def readShort(*args): return apply(_sylphis.CSerializer_readShort,args)
    def readUnsignedShort(*args): return apply(_sylphis.CSerializer_readUnsignedShort,args)
    def readChar(*args): return apply(_sylphis.CSerializer_readChar,args)
    def readUnsignedChar(*args): return apply(_sylphis.CSerializer_readUnsignedChar,args)
    def readInt(*args): return apply(_sylphis.CSerializer_readInt,args)
    def readUnsignedInt(*args): return apply(_sylphis.CSerializer_readUnsignedInt,args)
    def readFloat(*args): return apply(_sylphis.CSerializer_readFloat,args)
    def readDouble(*args): return apply(_sylphis.CSerializer_readDouble,args)
    def readBool(*args): return apply(_sylphis.CSerializer_readBool,args)
    def readString(*args): return apply(_sylphis.CSerializer_readString,args)
    def readPointer(*args): return apply(_sylphis.CSerializer_readPointer,args)
    def readSerializable(*args): return apply(_sylphis.CSerializer_readSerializable,args)
    def writeShort(*args): return apply(_sylphis.CSerializer_writeShort,args)
    def writeUnsignedShort(*args): return apply(_sylphis.CSerializer_writeUnsignedShort,args)
    def writeChar(*args): return apply(_sylphis.CSerializer_writeChar,args)
    def writeUnsignedChar(*args): return apply(_sylphis.CSerializer_writeUnsignedChar,args)
    def writeInt(*args): return apply(_sylphis.CSerializer_writeInt,args)
    def writeUnsignedInt(*args): return apply(_sylphis.CSerializer_writeUnsignedInt,args)
    def writeFloat(*args): return apply(_sylphis.CSerializer_writeFloat,args)
    def writeDouble(*args): return apply(_sylphis.CSerializer_writeDouble,args)
    def writeBool(*args): return apply(_sylphis.CSerializer_writeBool,args)
    def writeString(*args): return apply(_sylphis.CSerializer_writeString,args)
    def writePointer(*args): return apply(_sylphis.CSerializer_writePointer,args)
    def writeSerializable(*args): return apply(_sylphis.CSerializer_writeSerializable,args)
    def __repr__(self):
        return "<C CSerializer instance at %s>" % (self.this,)

class CSerializerPtr(CSerializer):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSerializer
_sylphis.CSerializer_swigregister(CSerializerPtr)

class CRefObject(CSerializable):
    __swig_setmethods__ = {}
    for _s in [CSerializable]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CRefObject, name, value)
    __swig_getmethods__ = {}
    for _s in [CSerializable]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CRefObject, name)
    __swig_getmethods__["factory"] = lambda x: _sylphis.CRefObject_factory
    if _newclass:factory = staticmethod(_sylphis.CRefObject_factory)
    __swig_getmethods__["cast"] = lambda x: _sylphis.CRefObject_cast
    if _newclass:cast = staticmethod(_sylphis.CRefObject_cast)
    def getClassName(*args): return apply(_sylphis.CRefObject_getClassName,args)
    def getClassInfo(*args): return apply(_sylphis.CRefObject_getClassInfo,args)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CRefObject,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CRefObject):
        try:
            if self.thisown: destroy(self)
        except: pass
    def incReferences(*args): return apply(_sylphis.CRefObject_incReferences,args)
    def decReferences(*args): return apply(_sylphis.CRefObject_decReferences,args)
    def getCreationTime(*args): return apply(_sylphis.CRefObject_getCreationTime,args)
    def getLiveTime(*args): return apply(_sylphis.CRefObject_getLiveTime,args)
    __swig_getmethods__["getTotalRefObjects"] = lambda x: _sylphis.CRefObject_getTotalRefObjects
    if _newclass:getTotalRefObjects = staticmethod(_sylphis.CRefObject_getTotalRefObjects)
    def getReferences(*args): return apply(_sylphis.CRefObject_getReferences,args)
    def save(*args): return apply(_sylphis.CRefObject_save,args)
    def load(*args): return apply(_sylphis.CRefObject_load,args)
    def __repr__(self):
        return "<C CRefObject instance at %s>" % (self.this,)

class CRefObjectPtr(CRefObject):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CRefObject
_sylphis.CRefObject_swigregister(CRefObjectPtr)
CRefObject_factory = _sylphis.CRefObject_factory

CRefObject_cast = _sylphis.CRefObject_cast

CRefObject_getTotalRefObjects = _sylphis.CRefObject_getTotalRefObjects


class CException(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CException, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CException, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CException,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CException):
        try:
            if self.thisown: destroy(self)
        except: pass
    def getExplanation(*args): return apply(_sylphis.CException_getExplanation,args)
    def what(*args): return apply(_sylphis.CException_what,args)
    def __repr__(self):
        return "<C CException instance at %s>" % (self.this,)

class CExceptionPtr(CException):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CException
_sylphis.CException_swigregister(CExceptionPtr)

class CConsole(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CConsole, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CConsole, name)
    def printf(*args): return apply(_sylphis.CConsole_printf,args)
    def errorf(*args): return apply(_sylphis.CConsole_errorf,args)
    def printString(*args): return apply(_sylphis.CConsole_printString,args)
    def printError(*args): return apply(_sylphis.CConsole_printError,args)
    def __del__(self, destroy= _sylphis.delete_CConsole):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C CConsole instance at %s>" % (self.this,)

class CConsolePtr(CConsole):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CConsole
_sylphis.CConsole_swigregister(CConsolePtr)

class CConsoleTTY(CConsole):
    __swig_setmethods__ = {}
    for _s in [CConsole]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CConsoleTTY, name, value)
    __swig_getmethods__ = {}
    for _s in [CConsole]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CConsoleTTY, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CConsoleTTY,args)
        self.thisown = 1
    def printString(*args): return apply(_sylphis.CConsoleTTY_printString,args)
    def printError(*args): return apply(_sylphis.CConsoleTTY_printError,args)
    def __del__(self, destroy= _sylphis.delete_CConsoleTTY):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CConsoleTTY instance at %s>" % (self.this,)

class CConsoleTTYPtr(CConsoleTTY):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CConsoleTTY
_sylphis.CConsoleTTY_swigregister(CConsoleTTYPtr)

class CConsoleLog(CConsole):
    __swig_setmethods__ = {}
    for _s in [CConsole]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CConsoleLog, name, value)
    __swig_getmethods__ = {}
    for _s in [CConsole]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CConsoleLog, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CConsoleLog,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CConsoleLog):
        try:
            if self.thisown: destroy(self)
        except: pass
    def printString(*args): return apply(_sylphis.CConsoleLog_printString,args)
    def __repr__(self):
        return "<C CConsoleLog instance at %s>" % (self.this,)

class CConsoleLogPtr(CConsoleLog):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CConsoleLog
_sylphis.CConsoleLog_swigregister(CConsoleLogPtr)

class CConsoleMulti(CConsole):
    __swig_setmethods__ = {}
    for _s in [CConsole]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CConsoleMulti, name, value)
    __swig_getmethods__ = {}
    for _s in [CConsole]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CConsoleMulti, name)
    def __del__(self, destroy= _sylphis.delete_CConsoleMulti):
        try:
            if self.thisown: destroy(self)
        except: pass
    def printString(*args): return apply(_sylphis.CConsoleMulti_printString,args)
    def printError(*args): return apply(_sylphis.CConsoleMulti_printError,args)
    def addConsole(*args): return apply(_sylphis.CConsoleMulti_addConsole,args)
    def freeConsoles(*args): return apply(_sylphis.CConsoleMulti_freeConsoles,args)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CConsoleMulti,args)
        self.thisown = 1
    def __repr__(self):
        return "<C CConsoleMulti instance at %s>" % (self.this,)

class CConsoleMultiPtr(CConsoleMulti):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CConsoleMulti
_sylphis.CConsoleMulti_swigregister(CConsoleMultiPtr)

class CTimer(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTimer, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CTimer, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CTimer,args)
        self.thisown = 1
    def getTime(*args): return apply(_sylphis.CTimer_getTime,args)
    def __del__(self, destroy= _sylphis.delete_CTimer):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CTimer instance at %s>" % (self.this,)

class CTimerPtr(CTimer):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTimer
_sylphis.CTimer_swigregister(CTimerPtr)

class CTimerStoppable(CTimer):
    __swig_setmethods__ = {}
    for _s in [CTimer]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTimerStoppable, name, value)
    __swig_getmethods__ = {}
    for _s in [CTimer]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CTimerStoppable, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CTimerStoppable,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CTimerStoppable):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CTimerStoppable instance at %s>" % (self.this,)

class CTimerStoppablePtr(CTimerStoppable):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTimerStoppable
_sylphis.CTimerStoppable_swigregister(CTimerStoppablePtr)

class CTimerAdvanced(CTimer):
    __swig_setmethods__ = {}
    for _s in [CTimer]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTimerAdvanced, name, value)
    __swig_getmethods__ = {}
    for _s in [CTimer]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CTimerAdvanced, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CTimerAdvanced,args)
        self.thisown = 1
    def start(*args): return apply(_sylphis.CTimerAdvanced_start,args)
    def stop(*args): return apply(_sylphis.CTimerAdvanced_stop,args)
    def reset(*args): return apply(_sylphis.CTimerAdvanced_reset,args)
    def getAvrTime(*args): return apply(_sylphis.CTimerAdvanced_getAvrTime,args)
    def __del__(self, destroy= _sylphis.delete_CTimerAdvanced):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CTimerAdvanced instance at %s>" % (self.this,)

class CTimerAdvancedPtr(CTimerAdvanced):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTimerAdvanced
_sylphis.CTimerAdvanced_swigregister(CTimerAdvancedPtr)

class CVector3(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CVector3, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CVector3, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CVector3,args)
        self.thisown = 1
    def __xor__(*args): return apply(_sylphis.CVector3___xor__,args)
    def __add__(*args): return apply(_sylphis.CVector3___add__,args)
    def __sub__(*args): return apply(_sylphis.CVector3___sub__,args)
    def __neg__(*args): return apply(_sylphis.CVector3___neg__,args)
    def __mul__(*args): return apply(_sylphis.CVector3___mul__,args)
    def __div__(*args): return apply(_sylphis.CVector3___div__,args)
    def __iadd__(*args): return apply(_sylphis.CVector3___iadd__,args)
    def __isub__(*args): return apply(_sylphis.CVector3___isub__,args)
    def __imul__(*args): return apply(_sylphis.CVector3___imul__,args)
    def __idiv__(*args): return apply(_sylphis.CVector3___idiv__,args)
    def __eq__(*args): return apply(_sylphis.CVector3___eq__,args)
    def length(*args): return apply(_sylphis.CVector3_length,args)
    def normalized(*args): return apply(_sylphis.CVector3_normalized,args)
    def lerp(*args): return apply(_sylphis.CVector3_lerp,args)
    def swapHand(*args): return apply(_sylphis.CVector3_swapHand,args)
    __swig_setmethods__["x"] = _sylphis.CVector3_x_set
    __swig_getmethods__["x"] = _sylphis.CVector3_x_get
    if _newclass:x = property(_sylphis.CVector3_x_get,_sylphis.CVector3_x_set)
    __swig_setmethods__["y"] = _sylphis.CVector3_y_set
    __swig_getmethods__["y"] = _sylphis.CVector3_y_get
    if _newclass:y = property(_sylphis.CVector3_y_get,_sylphis.CVector3_y_set)
    __swig_setmethods__["z"] = _sylphis.CVector3_z_set
    __swig_getmethods__["z"] = _sylphis.CVector3_z_get
    if _newclass:z = property(_sylphis.CVector3_z_get,_sylphis.CVector3_z_set)
    ZERO = _sylphis.cvar.CVector3_ZERO
    UNIT_X = _sylphis.cvar.CVector3_UNIT_X
    UNIT_Y = _sylphis.cvar.CVector3_UNIT_Y
    UNIT_Z = _sylphis.cvar.CVector3_UNIT_Z
    def save(*args): return apply(_sylphis.CVector3_save,args)
    def load(*args): return apply(_sylphis.CVector3_load,args)
    def __del__(self, destroy= _sylphis.delete_CVector3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CVector3 instance at %s>" % (self.this,)

class CVector3Ptr(CVector3):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CVector3
_sylphis.CVector3_swigregister(CVector3Ptr)

class hash_CVector3(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, hash_CVector3, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, hash_CVector3, name)
    def __call__(*args): return apply(_sylphis.hash_CVector3___call__,args)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_hash_CVector3,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_hash_CVector3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C hash_CVector3 instance at %s>" % (self.this,)

class hash_CVector3Ptr(hash_CVector3):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = hash_CVector3
_sylphis.hash_CVector3_swigregister(hash_CVector3Ptr)

class CVector2(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CVector2, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CVector2, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CVector2,args)
        self.thisown = 1
    def __add__(*args): return apply(_sylphis.CVector2___add__,args)
    def __sub__(*args): return apply(_sylphis.CVector2___sub__,args)
    def __neg__(*args): return apply(_sylphis.CVector2___neg__,args)
    def __mul__(*args): return apply(_sylphis.CVector2___mul__,args)
    def __div__(*args): return apply(_sylphis.CVector2___div__,args)
    def __iadd__(*args): return apply(_sylphis.CVector2___iadd__,args)
    def __isub__(*args): return apply(_sylphis.CVector2___isub__,args)
    def __imul__(*args): return apply(_sylphis.CVector2___imul__,args)
    def __idiv__(*args): return apply(_sylphis.CVector2___idiv__,args)
    def length(*args): return apply(_sylphis.CVector2_length,args)
    def normalized(*args): return apply(_sylphis.CVector2_normalized,args)
    __swig_setmethods__["x"] = _sylphis.CVector2_x_set
    __swig_getmethods__["x"] = _sylphis.CVector2_x_get
    if _newclass:x = property(_sylphis.CVector2_x_get,_sylphis.CVector2_x_set)
    __swig_setmethods__["y"] = _sylphis.CVector2_y_set
    __swig_getmethods__["y"] = _sylphis.CVector2_y_get
    if _newclass:y = property(_sylphis.CVector2_y_get,_sylphis.CVector2_y_set)
    def save(*args): return apply(_sylphis.CVector2_save,args)
    def load(*args): return apply(_sylphis.CVector2_load,args)
    def __del__(self, destroy= _sylphis.delete_CVector2):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CVector2 instance at %s>" % (self.this,)

class CVector2Ptr(CVector2):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CVector2
_sylphis.CVector2_swigregister(CVector2Ptr)

class CPlane(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CPlane, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CPlane, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CPlane,args)
        self.thisown = 1
    FRONT = _sylphis.CPlane_FRONT
    BACK = _sylphis.CPlane_BACK
    CROSS = _sylphis.CPlane_CROSS
    ON = _sylphis.CPlane_ON
    def setNormal(*args): return apply(_sylphis.CPlane_setNormal,args)
    def distance(*args): return apply(_sylphis.CPlane_distance,args)
    def lineIntersection(*args): return apply(_sylphis.CPlane_lineIntersection,args)
    def rayIntersection(*args): return apply(_sylphis.CPlane_rayIntersection,args)
    def valid(*args): return apply(_sylphis.CPlane_valid,args)
    def normalize(*args): return apply(_sylphis.CPlane_normalize,args)
    def save(*args): return apply(_sylphis.CPlane_save,args)
    def load(*args): return apply(_sylphis.CPlane_load,args)
    __swig_setmethods__["dist"] = _sylphis.CPlane_dist_set
    __swig_getmethods__["dist"] = _sylphis.CPlane_dist_get
    if _newclass:dist = property(_sylphis.CPlane_dist_get,_sylphis.CPlane_dist_set)
    __swig_setmethods__["normal"] = _sylphis.CPlane_normal_set
    __swig_getmethods__["normal"] = _sylphis.CPlane_normal_get
    if _newclass:normal = property(_sylphis.CPlane_normal_get,_sylphis.CPlane_normal_set)
    __swig_setmethods__["onAxe"] = _sylphis.CPlane_onAxe_set
    __swig_getmethods__["onAxe"] = _sylphis.CPlane_onAxe_get
    if _newclass:onAxe = property(_sylphis.CPlane_onAxe_get,_sylphis.CPlane_onAxe_set)
    __swig_setmethods__["factor"] = _sylphis.CPlane_factor_set
    __swig_getmethods__["factor"] = _sylphis.CPlane_factor_get
    if _newclass:factor = property(_sylphis.CPlane_factor_get,_sylphis.CPlane_factor_set)
    def __del__(self, destroy= _sylphis.delete_CPlane):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CPlane instance at %s>" % (self.this,)

class CPlanePtr(CPlane):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CPlane
_sylphis.CPlane_swigregister(CPlanePtr)

class CQuaternion(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CQuaternion, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CQuaternion, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CQuaternion,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CQuaternion):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __add__(*args): return apply(_sylphis.CQuaternion___add__,args)
    def __sub__(*args): return apply(_sylphis.CQuaternion___sub__,args)
    def __neg__(*args): return apply(_sylphis.CQuaternion___neg__,args)
    def __mul__(*args): return apply(_sylphis.CQuaternion___mul__,args)
    def invert(*args): return apply(_sylphis.CQuaternion_invert,args)
    def toRotationMatrix(*args): return apply(_sylphis.CQuaternion_toRotationMatrix,args)
    def fromRotationMatrix(*args): return apply(_sylphis.CQuaternion_fromRotationMatrix,args)
    def fromAngleAxis(*args): return apply(_sylphis.CQuaternion_fromAngleAxis,args)
    def fromAxes(*args): return apply(_sylphis.CQuaternion_fromAxes,args)
    def swapHand(*args): return apply(_sylphis.CQuaternion_swapHand,args)
    def lerp(*args): return apply(_sylphis.CQuaternion_lerp,args)
    __swig_setmethods__["x"] = _sylphis.CQuaternion_x_set
    __swig_getmethods__["x"] = _sylphis.CQuaternion_x_get
    if _newclass:x = property(_sylphis.CQuaternion_x_get,_sylphis.CQuaternion_x_set)
    __swig_setmethods__["y"] = _sylphis.CQuaternion_y_set
    __swig_getmethods__["y"] = _sylphis.CQuaternion_y_get
    if _newclass:y = property(_sylphis.CQuaternion_y_get,_sylphis.CQuaternion_y_set)
    __swig_setmethods__["z"] = _sylphis.CQuaternion_z_set
    __swig_getmethods__["z"] = _sylphis.CQuaternion_z_get
    if _newclass:z = property(_sylphis.CQuaternion_z_get,_sylphis.CQuaternion_z_set)
    __swig_setmethods__["w"] = _sylphis.CQuaternion_w_set
    __swig_getmethods__["w"] = _sylphis.CQuaternion_w_get
    if _newclass:w = property(_sylphis.CQuaternion_w_get,_sylphis.CQuaternion_w_set)
    def save(*args): return apply(_sylphis.CQuaternion_save,args)
    def load(*args): return apply(_sylphis.CQuaternion_load,args)
    def __repr__(self):
        return "<C CQuaternion instance at %s>" % (self.this,)

class CQuaternionPtr(CQuaternion):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CQuaternion
_sylphis.CQuaternion_swigregister(CQuaternionPtr)

class CMatrix4(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CMatrix4, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CMatrix4, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CMatrix4,args)
        self.thisown = 1
    def zero(*args): return apply(_sylphis.CMatrix4_zero,args)
    def identity(*args): return apply(_sylphis.CMatrix4_identity,args)
    def rotateZ(*args): return apply(_sylphis.CMatrix4_rotateZ,args)
    def rotateY(*args): return apply(_sylphis.CMatrix4_rotateY,args)
    def rotateX(*args): return apply(_sylphis.CMatrix4_rotateX,args)
    def rotate(*args): return apply(_sylphis.CMatrix4_rotate,args)
    def getInverted(*args): return apply(_sylphis.CMatrix4_getInverted,args)
    def invert(*args): return apply(_sylphis.CMatrix4_invert,args)
    def transpose(*args): return apply(_sylphis.CMatrix4_transpose,args)
    def setTransform(*args): return apply(_sylphis.CMatrix4_setTransform,args)
    def __imul__(*args): return apply(_sylphis.CMatrix4___imul__,args)
    __swig_setmethods__["m"] = _sylphis.CMatrix4_m_set
    __swig_getmethods__["m"] = _sylphis.CMatrix4_m_get
    if _newclass:m = property(_sylphis.CMatrix4_m_get,_sylphis.CMatrix4_m_set)
    def __del__(self, destroy= _sylphis.delete_CMatrix4):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CMatrix4 instance at %s>" % (self.this,)

class CMatrix4Ptr(CMatrix4):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CMatrix4
_sylphis.CMatrix4_swigregister(CMatrix4Ptr)

class CMatrix3(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CMatrix3, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CMatrix3, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CMatrix3,args)
        self.thisown = 1
    def zero(*args): return apply(_sylphis.CMatrix3_zero,args)
    def identity(*args): return apply(_sylphis.CMatrix3_identity,args)
    def rotateZ(*args): return apply(_sylphis.CMatrix3_rotateZ,args)
    def rotateY(*args): return apply(_sylphis.CMatrix3_rotateY,args)
    def rotateX(*args): return apply(_sylphis.CMatrix3_rotateX,args)
    def rotate(*args): return apply(_sylphis.CMatrix3_rotate,args)
    def getInverted(*args): return apply(_sylphis.CMatrix3_getInverted,args)
    def invert(*args): return apply(_sylphis.CMatrix3_invert,args)
    def transpose(*args): return apply(_sylphis.CMatrix3_transpose,args)
    def __imul__(*args): return apply(_sylphis.CMatrix3___imul__,args)
    __swig_setmethods__["m"] = _sylphis.CMatrix3_m_set
    __swig_getmethods__["m"] = _sylphis.CMatrix3_m_get
    if _newclass:m = property(_sylphis.CMatrix3_m_get,_sylphis.CMatrix3_m_set)
    def __del__(self, destroy= _sylphis.delete_CMatrix3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CMatrix3 instance at %s>" % (self.this,)

class CMatrix3Ptr(CMatrix3):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CMatrix3
_sylphis.CMatrix3_swigregister(CMatrix3Ptr)

class CBound(CSerializable):
    __swig_setmethods__ = {}
    for _s in [CSerializable]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CBound, name, value)
    __swig_getmethods__ = {}
    for _s in [CSerializable]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CBound, name)
    AABOX = _sylphis.CBound_AABOX
    FRUSTUM = _sylphis.CBound_FRUSTUM
    def __del__(self, destroy= _sylphis.delete_CBound):
        try:
            if self.thisown: destroy(self)
        except: pass
    def getType(*args): return apply(_sylphis.CBound_getType,args)
    def distance(*args): return apply(_sylphis.CBound_distance,args)
    def translate(*args): return apply(_sylphis.CBound_translate,args)
    def collide(*args): return apply(_sylphis.CBound_collide,args)
    def render(*args): return apply(_sylphis.CBound_render,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C CBound instance at %s>" % (self.this,)

class CBoundPtr(CBound):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CBound
_sylphis.CBound_swigregister(CBoundPtr)

class CAABoundBox(CBound):
    __swig_setmethods__ = {}
    for _s in [CBound]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CAABoundBox, name, value)
    __swig_getmethods__ = {}
    for _s in [CBound]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CAABoundBox, name)
    __swig_getmethods__["factory"] = lambda x: _sylphis.CAABoundBox_factory
    if _newclass:factory = staticmethod(_sylphis.CAABoundBox_factory)
    __swig_getmethods__["cast"] = lambda x: _sylphis.CAABoundBox_cast
    if _newclass:cast = staticmethod(_sylphis.CAABoundBox_cast)
    def getClassName(*args): return apply(_sylphis.CAABoundBox_getClassName,args)
    def getClassInfo(*args): return apply(_sylphis.CAABoundBox_getClassInfo,args)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CAABoundBox,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CAABoundBox):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setBounds(*args): return apply(_sylphis.CAABoundBox_setBounds,args)
    def setMin(*args): return apply(_sylphis.CAABoundBox_setMin,args)
    def getMin(*args): return apply(_sylphis.CAABoundBox_getMin,args)
    def setMax(*args): return apply(_sylphis.CAABoundBox_setMax,args)
    def getMax(*args): return apply(_sylphis.CAABoundBox_getMax,args)
    def addPoint(*args): return apply(_sylphis.CAABoundBox_addPoint,args)
    def addBound(*args): return apply(_sylphis.CAABoundBox_addBound,args)
    def distance(*args): return apply(_sylphis.CAABoundBox_distance,args)
    def isBehind(*args): return apply(_sylphis.CAABoundBox_isBehind,args)
    def translate(*args): return apply(_sylphis.CAABoundBox_translate,args)
    def getType(*args): return apply(_sylphis.CAABoundBox_getType,args)
    def collide(*args): return apply(_sylphis.CAABoundBox_collide,args)
    def render(*args): return apply(_sylphis.CAABoundBox_render,args)
    def save(*args): return apply(_sylphis.CAABoundBox_save,args)
    def load(*args): return apply(_sylphis.CAABoundBox_load,args)
    def __repr__(self):
        return "<C CAABoundBox instance at %s>" % (self.this,)

class CAABoundBoxPtr(CAABoundBox):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CAABoundBox
_sylphis.CAABoundBox_swigregister(CAABoundBoxPtr)
CAABoundBox_factory = _sylphis.CAABoundBox_factory

CAABoundBox_cast = _sylphis.CAABoundBox_cast


class CTokenizerBase(CRefObject):
    __swig_setmethods__ = {}
    for _s in [CRefObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTokenizerBase, name, value)
    __swig_getmethods__ = {}
    for _s in [CRefObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CTokenizerBase, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CTokenizerBase,args)
        self.thisown = 1
    def nextToken(*args): return apply(_sylphis.CTokenizerBase_nextToken,args)
    def expect(*args): return apply(_sylphis.CTokenizerBase_expect,args)
    __swig_getmethods__["lowerCase"] = lambda x: _sylphis.CTokenizerBase_lowerCase
    if _newclass:lowerCase = staticmethod(_sylphis.CTokenizerBase_lowerCase)
    def setLowerCaseTokens(*args): return apply(_sylphis.CTokenizerBase_setLowerCaseTokens,args)
    def skipToNextLine(*args): return apply(_sylphis.CTokenizerBase_skipToNextLine,args)
    def skipToToken(*args): return apply(_sylphis.CTokenizerBase_skipToToken,args)
    def bookmark(*args): return apply(_sylphis.CTokenizerBase_bookmark,args)
    def gotoBookmark(*args): return apply(_sylphis.CTokenizerBase_gotoBookmark,args)
    def backToStartOfLine(*args): return apply(_sylphis.CTokenizerBase_backToStartOfLine,args)
    def skipWhitespace(*args): return apply(_sylphis.CTokenizerBase_skipWhitespace,args)
    def nextChar(*args): return apply(_sylphis.CTokenizerBase_nextChar,args)
    def currentChar(*args): return apply(_sylphis.CTokenizerBase_currentChar,args)
    def getToken(*args): return apply(_sylphis.CTokenizerBase_getToken,args)
    def getLine(*args): return apply(_sylphis.CTokenizerBase_getLine,args)
    def __del__(self, destroy= _sylphis.delete_CTokenizerBase):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CTokenizerBase instance at %s>" % (self.this,)

class CTokenizerBasePtr(CTokenizerBase):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTokenizerBase
_sylphis.CTokenizerBase_swigregister(CTokenizerBasePtr)
CTokenizerBase_lowerCase = _sylphis.CTokenizerBase_lowerCase


class CTokenizerException(CException):
    __swig_setmethods__ = {}
    for _s in [CException]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTokenizerException, name, value)
    __swig_getmethods__ = {}
    for _s in [CException]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CTokenizerException, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CTokenizerException,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CTokenizerException):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CTokenizerException instance at %s>" % (self.this,)

class CTokenizerExceptionPtr(CTokenizerException):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTokenizerException
_sylphis.CTokenizerException_swigregister(CTokenizerExceptionPtr)

class CSeparator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSeparator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CSeparator, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSeparator,args)
        self.thisown = 1
    __swig_setmethods__["sep"] = _sylphis.CSeparator_sep_set
    __swig_getmethods__["sep"] = _sylphis.CSeparator_sep_get
    if _newclass:sep = property(_sylphis.CSeparator_sep_get,_sylphis.CSeparator_sep_set)
    def __del__(self, destroy= _sylphis.delete_CSeparator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CSeparator instance at %s>" % (self.this,)

class CSeparatorPtr(CSeparator):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSeparator
_sylphis.CSeparator_swigregister(CSeparatorPtr)

class CTokenizer(CTokenizerBase):
    __swig_setmethods__ = {}
    for _s in [CTokenizerBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTokenizer, name, value)
    __swig_getmethods__ = {}
    for _s in [CTokenizerBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CTokenizer, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CTokenizer,args)
        self.thisown = 1
    def nextToken(*args): return apply(_sylphis.CTokenizer_nextToken,args)
    def addSeparator(*args): return apply(_sylphis.CTokenizer_addSeparator,args)
    def __del__(self, destroy= _sylphis.delete_CTokenizer):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CTokenizer instance at %s>" % (self.this,)

class CTokenizerPtr(CTokenizer):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTokenizer
_sylphis.CTokenizer_swigregister(CTokenizerPtr)

class CTokenizerNoComments(CTokenizer):
    __swig_setmethods__ = {}
    for _s in [CTokenizer]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTokenizerNoComments, name, value)
    __swig_getmethods__ = {}
    for _s in [CTokenizer]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CTokenizerNoComments, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CTokenizerNoComments,args)
        self.thisown = 1
    def nextToken(*args): return apply(_sylphis.CTokenizerNoComments_nextToken,args)
    def __del__(self, destroy= _sylphis.delete_CTokenizerNoComments):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CTokenizerNoComments instance at %s>" % (self.this,)

class CTokenizerNoCommentsPtr(CTokenizerNoComments):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTokenizerNoComments
_sylphis.CTokenizerNoComments_swigregister(CTokenizerNoCommentsPtr)

class CInputDevice(CRefObject):
    __swig_setmethods__ = {}
    for _s in [CRefObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CInputDevice, name, value)
    __swig_getmethods__ = {}
    for _s in [CRefObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CInputDevice, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CInputDevice,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CInputDevice):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setInputHandler(*args): return apply(_sylphis.CInputDevice_setInputHandler,args)
    def getInputHandler(*args): return apply(_sylphis.CInputDevice_getInputHandler,args)
    def getPointerX(*args): return apply(_sylphis.CInputDevice_getPointerX,args)
    def getPointerY(*args): return apply(_sylphis.CInputDevice_getPointerY,args)
    def getPointerXY(*args): return apply(_sylphis.CInputDevice_getPointerXY,args)
    def setPointer(*args): return apply(_sylphis.CInputDevice_setPointer,args)
    def getPointerCenter(*args): return apply(_sylphis.CInputDevice_getPointerCenter,args)
    def centerPointer(*args): return apply(_sylphis.CInputDevice_centerPointer,args)
    def __repr__(self):
        return "<C CInputDevice instance at %s>" % (self.this,)

class CInputDevicePtr(CInputDevice):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CInputDevice
_sylphis.CInputDevice_swigregister(CInputDevicePtr)

class CInputListener(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CInputListener, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CInputListener, name)
    def onKeyPressed(*args): return apply(_sylphis.CInputListener_onKeyPressed,args)
    def onKeyReleased(*args): return apply(_sylphis.CInputListener_onKeyReleased,args)
    def __del__(self, destroy= _sylphis.delete_CInputListener):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C CInputListener instance at %s>" % (self.this,)

class CInputListenerPtr(CInputListener):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CInputListener
_sylphis.CInputListener_swigregister(CInputListenerPtr)

class CPyInputListener(CInputListener):
    __swig_setmethods__ = {}
    for _s in [CInputListener]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CPyInputListener, name, value)
    __swig_getmethods__ = {}
    for _s in [CInputListener]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CPyInputListener, name)
    __swig_setmethods__["key"] = _sylphis.CPyInputListener_key_set
    __swig_getmethods__["key"] = _sylphis.CPyInputListener_key_get
    if _newclass:key = property(_sylphis.CPyInputListener_key_get,_sylphis.CPyInputListener_key_set)
    def onKeyPressed(*args): return apply(_sylphis.CPyInputListener_onKeyPressed,args)
    def onKeyReleased(*args): return apply(_sylphis.CPyInputListener_onKeyReleased,args)
    __swig_setmethods__["self"] = _sylphis.CPyInputListener_self_set
    __swig_getmethods__["self"] = _sylphis.CPyInputListener_self_get
    if _newclass:self = property(_sylphis.CPyInputListener_self_get,_sylphis.CPyInputListener_self_set)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CPyInputListener,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CPyInputListener):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CPyInputListener instance at %s>" % (self.this,)

class CPyInputListenerPtr(CPyInputListener):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CPyInputListener
_sylphis.CPyInputListener_swigregister(CPyInputListenerPtr)

class CInput(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CInput, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CInput, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CInput,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CInput):
        try:
            if self.thisown: destroy(self)
        except: pass
    def addKeyDevice(*args): return apply(_sylphis.CInput_addKeyDevice,args)
    def addPointingDevice(*args): return apply(_sylphis.CInput_addPointingDevice,args)
    def clearDevices(*args): return apply(_sylphis.CInput_clearDevices,args)
    def addListener(*args): return apply(_sylphis.CInput_addListener,args)
    def isPressed(*args): return apply(_sylphis.CInput_isPressed,args)
    def keyPressed(*args): return apply(_sylphis.CInput_keyPressed,args)
    def keyReleased(*args): return apply(_sylphis.CInput_keyReleased,args)
    def getGrabPointer(*args): return apply(_sylphis.CInput_getGrabPointer,args)
    def setGrabPointer(*args): return apply(_sylphis.CInput_setGrabPointer,args)
    def getPointerX(*args): return apply(_sylphis.CInput_getPointerX,args)
    def getPointerY(*args): return apply(_sylphis.CInput_getPointerY,args)
    def getPointerXY(*args): return apply(_sylphis.CInput_getPointerXY,args)
    def setPointer(*args): return apply(_sylphis.CInput_setPointer,args)
    def getPointerDelta(*args): return apply(_sylphis.CInput_getPointerDelta,args)
    def getPointerDeltaX(*args): return apply(_sylphis.CInput_getPointerDeltaX,args)
    def getPointerDeltaY(*args): return apply(_sylphis.CInput_getPointerDeltaY,args)
    def __repr__(self):
        return "<C CInput instance at %s>" % (self.this,)

class CInputPtr(CInput):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CInput
_sylphis.CInput_swigregister(CInputPtr)

class CSingletonEngine(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSingletonEngine, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CSingletonEngine, name)
    __swig_getmethods__["getSingleton"] = lambda x: _sylphis.CSingletonEngine_getSingleton
    if _newclass:getSingleton = staticmethod(_sylphis.CSingletonEngine_getSingleton)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C CSingletonEngine instance at %s>" % (self.this,)

class CSingletonEnginePtr(CSingletonEngine):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSingletonEngine
_sylphis.CSingletonEngine_swigregister(CSingletonEnginePtr)
kSYSREQ = cvar.kSYSREQ
kCAPSLOCK = cvar.kCAPSLOCK
kNUMLOCK = cvar.kNUMLOCK
kSCROLLLOCK = cvar.kSCROLLLOCK
kLEFTCTRL = cvar.kLEFTCTRL
kLEFTSHIFT = cvar.kLEFTSHIFT
kRIGHTCTRL = cvar.kRIGHTCTRL
kRIGHTSHIFT = cvar.kRIGHTSHIFT
kESC = cvar.kESC
kBACKSPACE = cvar.kBACKSPACE
kENTER = cvar.kENTER
kSPACE = cvar.kSPACE
kTAB = cvar.kTAB
kF1 = cvar.kF1
kF2 = cvar.kF2
kF3 = cvar.kF3
kF4 = cvar.kF4
kF5 = cvar.kF5
kF6 = cvar.kF6
kF7 = cvar.kF7
kF8 = cvar.kF8
kF9 = cvar.kF9
kF10 = cvar.kF10
kF11 = cvar.kF11
kF12 = cvar.kF12
kA = cvar.kA
kB = cvar.kB
kC = cvar.kC
kD = cvar.kD
kE = cvar.kE
kF = cvar.kF
kG = cvar.kG
kH = cvar.kH
kI = cvar.kI
kJ = cvar.kJ
kK = cvar.kK
kL = cvar.kL
kM = cvar.kM
kN = cvar.kN
kO = cvar.kO
kP = cvar.kP
kQ = cvar.kQ
kR = cvar.kR
kS = cvar.kS
kT = cvar.kT
kU = cvar.kU
kV = cvar.kV
kW = cvar.kW
kX = cvar.kX
kY = cvar.kY
kZ = cvar.kZ
k1 = cvar.k1
k2 = cvar.k2
k3 = cvar.k3
k4 = cvar.k4
k5 = cvar.k5
k6 = cvar.k6
k7 = cvar.k7
k8 = cvar.k8
k9 = cvar.k9
k0 = cvar.k0
kMINUS = cvar.kMINUS
kEQUAL = cvar.kEQUAL
kLBRACKET = cvar.kLBRACKET
kRBRACKET = cvar.kRBRACKET
kSEMICOLON = cvar.kSEMICOLON
kTICK = cvar.kTICK
kAPOSTROPHE = cvar.kAPOSTROPHE
kBACKSLASH = cvar.kBACKSLASH
kCOMMA = cvar.kCOMMA
kPERIOD = cvar.kPERIOD
kSLASH = cvar.kSLASH
kINS = cvar.kINS
kDEL = cvar.kDEL
kHOME = cvar.kHOME
kEND = cvar.kEND
kPGUP = cvar.kPGUP
kPGDN = cvar.kPGDN
kLARROW = cvar.kLARROW
kRARROW = cvar.kRARROW
kUARROW = cvar.kUARROW
kDARROW = cvar.kDARROW
kKEYPAD0 = cvar.kKEYPAD0
kKEYPAD1 = cvar.kKEYPAD1
kKEYPAD2 = cvar.kKEYPAD2
kKEYPAD3 = cvar.kKEYPAD3
kKEYPAD4 = cvar.kKEYPAD4
kKEYPAD5 = cvar.kKEYPAD5
kKEYPAD6 = cvar.kKEYPAD6
kKEYPAD7 = cvar.kKEYPAD7
kKEYPAD8 = cvar.kKEYPAD8
kKEYPAD9 = cvar.kKEYPAD9
kKEYPADDEL = cvar.kKEYPADDEL
kKEYPADSTAR = cvar.kKEYPADSTAR
kKEYPADMINUS = cvar.kKEYPADMINUS
kKEYPADPLUS = cvar.kKEYPADPLUS
kKEYPADENTER = cvar.kKEYPADENTER
kKEYPADSLASH = cvar.kKEYPADSLASH
kMOUSERIGHT = cvar.kMOUSERIGHT
kMOUSELEFT = cvar.kMOUSELEFT
CSingletonEngine_getSingleton = _sylphis.CSingletonEngine_getSingleton


class CEngine(CSingletonEngine):
    __swig_setmethods__ = {}
    for _s in [CSingletonEngine]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CEngine, name, value)
    __swig_getmethods__ = {}
    for _s in [CSingletonEngine]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CEngine, name)
    __swig_setmethods__["vfs"] = _sylphis.CEngine_vfs_set
    __swig_getmethods__["vfs"] = _sylphis.CEngine_vfs_get
    if _newclass:vfs = property(_sylphis.CEngine_vfs_get,_sylphis.CEngine_vfs_set)
    __swig_setmethods__["con"] = _sylphis.CEngine_con_set
    __swig_getmethods__["con"] = _sylphis.CEngine_con_get
    if _newclass:con = property(_sylphis.CEngine_con_get,_sylphis.CEngine_con_set)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CEngine,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CEngine):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["getSingleton"] = lambda x: _sylphis.CEngine_getSingleton
    if _newclass:getSingleton = staticmethod(_sylphis.CEngine_getSingleton)
    def setClient(*args): return apply(_sylphis.CEngine_setClient,args)
    def startup(*args): return apply(_sylphis.CEngine_startup,args)
    def shutdown(*args): return apply(_sylphis.CEngine_shutdown,args)
    def needUpdate(*args): return apply(_sylphis.CEngine_needUpdate,args)
    def setRunning(*args): return apply(_sylphis.CEngine_setRunning,args)
    def isRunning(*args): return apply(_sylphis.CEngine_isRunning,args)
    def update(*args): return apply(_sylphis.CEngine_update,args)
    def getFrameLength(*args): return apply(_sylphis.CEngine_getFrameLength,args)
    def getFrameTime(*args): return apply(_sylphis.CEngine_getFrameTime,args)
    def getWorldTime(*args): return apply(_sylphis.CEngine_getWorldTime,args)
    def setWorldTime(*args): return apply(_sylphis.CEngine_setWorldTime,args)
    def getFrameCount(*args): return apply(_sylphis.CEngine_getFrameCount,args)
    def getFPS(*args): return apply(_sylphis.CEngine_getFPS,args)
    def setFPS(*args): return apply(_sylphis.CEngine_setFPS,args)
    def getVersion(*args): return apply(_sylphis.CEngine_getVersion,args)
    def getDefaultFont(*args): return apply(_sylphis.CEngine_getDefaultFont,args)
    def getInput(*args): return apply(_sylphis.CEngine_getInput,args)
    def getClient(*args): return apply(_sylphis.CEngine_getClient,args)
    def setPyModule(*args): return apply(_sylphis.CEngine_setPyModule,args)
    def getPyModule(*args): return apply(_sylphis.CEngine_getPyModule,args)
    def createRenderingView(*args): return apply(_sylphis.CEngine_createRenderingView,args)
    def destroyRenderingView(*args): return apply(_sylphis.CEngine_destroyRenderingView,args)
    def saveGame(*args): return apply(_sylphis.CEngine_saveGame,args)
    def loadGame(*args): return apply(_sylphis.CEngine_loadGame,args)
    __swig_setmethods__["rView"] = _sylphis.CEngine_rView_set
    __swig_getmethods__["rView"] = _sylphis.CEngine_rView_get
    if _newclass:rView = property(_sylphis.CEngine_rView_get,_sylphis.CEngine_rView_set)
    __swig_setmethods__["mRSystem"] = _sylphis.CEngine_mRSystem_set
    __swig_getmethods__["mRSystem"] = _sylphis.CEngine_mRSystem_get
    if _newclass:mRSystem = property(_sylphis.CEngine_mRSystem_get,_sylphis.CEngine_mRSystem_set)
    __swig_setmethods__["renderer"] = _sylphis.CEngine_renderer_set
    __swig_getmethods__["renderer"] = _sylphis.CEngine_renderer_get
    if _newclass:renderer = property(_sylphis.CEngine_renderer_get,_sylphis.CEngine_renderer_set)
    __swig_setmethods__["mSoundEnv"] = _sylphis.CEngine_mSoundEnv_set
    __swig_getmethods__["mSoundEnv"] = _sylphis.CEngine_mSoundEnv_get
    if _newclass:mSoundEnv = property(_sylphis.CEngine_mSoundEnv_get,_sylphis.CEngine_mSoundEnv_set)
    __swig_setmethods__["timer"] = _sylphis.CEngine_timer_set
    __swig_getmethods__["timer"] = _sylphis.CEngine_timer_get
    if _newclass:timer = property(_sylphis.CEngine_timer_get,_sylphis.CEngine_timer_set)
    __swig_setmethods__["mMaterials"] = _sylphis.CEngine_mMaterials_set
    __swig_getmethods__["mMaterials"] = _sylphis.CEngine_mMaterials_get
    if _newclass:mMaterials = property(_sylphis.CEngine_mMaterials_get,_sylphis.CEngine_mMaterials_set)
    __swig_setmethods__["textures"] = _sylphis.CEngine_textures_set
    __swig_getmethods__["textures"] = _sylphis.CEngine_textures_get
    if _newclass:textures = property(_sylphis.CEngine_textures_get,_sylphis.CEngine_textures_set)
    __swig_setmethods__["renderables"] = _sylphis.CEngine_renderables_set
    __swig_getmethods__["renderables"] = _sylphis.CEngine_renderables_get
    if _newclass:renderables = property(_sylphis.CEngine_renderables_get,_sylphis.CEngine_renderables_set)
    __swig_setmethods__["models"] = _sylphis.CEngine_models_set
    __swig_getmethods__["models"] = _sylphis.CEngine_models_get
    if _newclass:models = property(_sylphis.CEngine_models_get,_sylphis.CEngine_models_set)
    __swig_setmethods__["fonts"] = _sylphis.CEngine_fonts_set
    __swig_getmethods__["fonts"] = _sylphis.CEngine_fonts_get
    if _newclass:fonts = property(_sylphis.CEngine_fonts_get,_sylphis.CEngine_fonts_set)
    def __repr__(self):
        return "<C CEngine instance at %s>" % (self.this,)

class CEnginePtr(CEngine):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CEngine
_sylphis.CEngine_swigregister(CEnginePtr)
CEngine_getSingleton = _sylphis.CEngine_getSingleton


class CClient(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CClient, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CClient, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CClient,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CClient):
        try:
            if self.thisown: destroy(self)
        except: pass
    def startup(*args): return apply(_sylphis.CClient_startup,args)
    def startup_c(*args): return apply(_sylphis.CClient_startup_c,args)
    def shutdown(*args): return apply(_sylphis.CClient_shutdown,args)
    def update(*args): return apply(_sylphis.CClient_update,args)
    def update_c(*args): return apply(_sylphis.CClient_update_c,args)
    def updateActors(*args): return apply(_sylphis.CClient_updateActors,args)
    def updateActors_c(*args): return apply(_sylphis.CClient_updateActors_c,args)
    __swig_setmethods__["self"] = _sylphis.CClient_self_set
    __swig_getmethods__["self"] = _sylphis.CClient_self_get
    if _newclass:self = property(_sylphis.CClient_self_get,_sylphis.CClient_self_set)
    def setSceneManager(*args): return apply(_sylphis.CClient_setSceneManager,args)
    def getSceneManager(*args): return apply(_sylphis.CClient_getSceneManager,args)
    def __repr__(self):
        return "<C CClient instance at %s>" % (self.this,)

class CClientPtr(CClient):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CClient
_sylphis.CClient_swigregister(CClientPtr)

class CWaitable(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CWaitable, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CWaitable, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CWaitable,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CWaitable):
        try:
            if self.thisown: destroy(self)
        except: pass
    def wait(*args): return apply(_sylphis.CWaitable_wait,args)
    def wake(*args): return apply(_sylphis.CWaitable_wake,args)
    def __repr__(self):
        return "<C CWaitable instance at %s>" % (self.this,)

class CWaitablePtr(CWaitable):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CWaitable
_sylphis.CWaitable_swigregister(CWaitablePtr)

class CPyConsole(CConsole):
    __swig_setmethods__ = {}
    for _s in [CConsole]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CPyConsole, name, value)
    __swig_getmethods__ = {}
    for _s in [CConsole]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CPyConsole, name)
    def printString(*args): return apply(_sylphis.CPyConsole_printString,args)
    __swig_setmethods__["self"] = _sylphis.CPyConsole_self_set
    __swig_getmethods__["self"] = _sylphis.CPyConsole_self_get
    if _newclass:self = property(_sylphis.CPyConsole_self_get,_sylphis.CPyConsole_self_set)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CPyConsole,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CPyConsole):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CPyConsole instance at %s>" % (self.this,)

class CPyConsolePtr(CPyConsole):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CPyConsole
_sylphis.CPyConsole_swigregister(CPyConsolePtr)

class CFile(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CFile, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CFile, name)
    FILE_WRITE = _sylphis.CFile_FILE_WRITE
    FILE_READ = _sylphis.CFile_FILE_READ
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CFile,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CFile):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setVirtualFS(*args): return apply(_sylphis.CFile_setVirtualFS,args)
    def getVirtualFS(*args): return apply(_sylphis.CFile_getVirtualFS,args)
    def setSize(*args): return apply(_sylphis.CFile_setSize,args)
    def getSize(*args): return apply(_sylphis.CFile_getSize,args)
    def getName(*args): return apply(_sylphis.CFile_getName,args)
    def setName(*args): return apply(_sylphis.CFile_setName,args)
    def loadData(*args): return apply(_sylphis.CFile_loadData,args)
    def getData(*args): return apply(_sylphis.CFile_getData,args)
    def getDataCopy(*args): return apply(_sylphis.CFile_getDataCopy,args)
    def freeData(*args): return apply(_sylphis.CFile_freeData,args)
    def read(*args): return apply(_sylphis.CFile_read,args)
    def readShort(*args): return apply(_sylphis.CFile_readShort,args)
    def readUnsignedShort(*args): return apply(_sylphis.CFile_readUnsignedShort,args)
    def readChar(*args): return apply(_sylphis.CFile_readChar,args)
    def readUnsignedChar(*args): return apply(_sylphis.CFile_readUnsignedChar,args)
    def readInt(*args): return apply(_sylphis.CFile_readInt,args)
    def readUnsignedInt(*args): return apply(_sylphis.CFile_readUnsignedInt,args)
    def readFloat(*args): return apply(_sylphis.CFile_readFloat,args)
    def readDouble(*args): return apply(_sylphis.CFile_readDouble,args)
    def readBool(*args): return apply(_sylphis.CFile_readBool,args)
    def readString(*args): return apply(_sylphis.CFile_readString,args)
    def write(*args): return apply(_sylphis.CFile_write,args)
    def writeShort(*args): return apply(_sylphis.CFile_writeShort,args)
    def writeUnsignedShort(*args): return apply(_sylphis.CFile_writeUnsignedShort,args)
    def writeChar(*args): return apply(_sylphis.CFile_writeChar,args)
    def writeUnsignedChar(*args): return apply(_sylphis.CFile_writeUnsignedChar,args)
    def writeInt(*args): return apply(_sylphis.CFile_writeInt,args)
    def writeUnsignedInt(*args): return apply(_sylphis.CFile_writeUnsignedInt,args)
    def writeFloat(*args): return apply(_sylphis.CFile_writeFloat,args)
    def writeDouble(*args): return apply(_sylphis.CFile_writeDouble,args)
    def writeBool(*args): return apply(_sylphis.CFile_writeBool,args)
    def writeString(*args): return apply(_sylphis.CFile_writeString,args)
    def setIsDirectory(*args): return apply(_sylphis.CFile_setIsDirectory,args)
    def isDirectory(*args): return apply(_sylphis.CFile_isDirectory,args)
    def setStream(*args): return apply(_sylphis.CFile_setStream,args)
    def getStream(*args): return apply(_sylphis.CFile_getStream,args)
    def setMode(*args): return apply(_sylphis.CFile_setMode,args)
    def getMode(*args): return apply(_sylphis.CFile_getMode,args)
    __swig_getmethods__["chopExtention"] = lambda x: _sylphis.CFile_chopExtention
    if _newclass:chopExtention = staticmethod(_sylphis.CFile_chopExtention)
    def __repr__(self):
        return "<C CFile instance at %s>" % (self.this,)

class CFilePtr(CFile):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CFile
_sylphis.CFile_swigregister(CFilePtr)
CFile_chopExtention = _sylphis.CFile_chopExtention


class CFileText(CFile):
    __swig_setmethods__ = {}
    for _s in [CFile]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CFileText, name, value)
    __swig_getmethods__ = {}
    for _s in [CFile]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CFileText, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CFileText,args)
        self.thisown = 1
    def setSize(*args): return apply(_sylphis.CFileText_setSize,args)
    def getSize(*args): return apply(_sylphis.CFileText_getSize,args)
    def readline(*args): return apply(_sylphis.CFileText_readline,args)
    def read(*args): return apply(_sylphis.CFileText_read,args)
    def __del__(self, destroy= _sylphis.delete_CFileText):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CFileText instance at %s>" % (self.this,)

class CFileTextPtr(CFileText):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CFileText
_sylphis.CFileText_swigregister(CFileTextPtr)

class CFileCollection(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CFileCollection, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CFileCollection, name)
    NativeCollection = _sylphis.CFileCollection_NativeCollection
    QuakePAK = _sylphis.CFileCollection_QuakePAK
    Directory = _sylphis.CFileCollection_Directory
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CFileCollection,args)
        self.thisown = 1
    def readFile(*args): return apply(_sylphis.CFileCollection_readFile,args)
    def __del__(self, destroy= _sylphis.delete_CFileCollection):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CFileCollection instance at %s>" % (self.this,)

class CFileCollectionPtr(CFileCollection):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CFileCollection
_sylphis.CFileCollection_swigregister(CFileCollectionPtr)

class CVirtualFS(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CVirtualFS, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CVirtualFS, name)
    def startup(*args): return apply(_sylphis.CVirtualFS_startup,args)
    def setBaseDir(*args): return apply(_sylphis.CVirtualFS_setBaseDir,args)
    def addCollection(*args): return apply(_sylphis.CVirtualFS_addCollection,args)
    def mount(*args): return apply(_sylphis.CVirtualFS_mount,args)
    def findFiles(*args): return apply(_sylphis.CVirtualFS_findFiles,args)
    def readFile(*args): return apply(_sylphis.CVirtualFS_readFile,args)
    def createFile(*args): return apply(_sylphis.CVirtualFS_createFile,args)
    def openFile(*args): return apply(_sylphis.CVirtualFS_openFile,args)
    def setDebug(*args): return apply(_sylphis.CVirtualFS_setDebug,args)
    def getDebug(*args): return apply(_sylphis.CVirtualFS_getDebug,args)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CVirtualFS,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CVirtualFS):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CVirtualFS instance at %s>" % (self.this,)

class CVirtualFSPtr(CVirtualFS):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CVirtualFS
_sylphis.CVirtualFS_swigregister(CVirtualFSPtr)

class CResource(CRefObject):
    __swig_setmethods__ = {}
    for _s in [CRefObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CResource, name, value)
    __swig_getmethods__ = {}
    for _s in [CRefObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CResource, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CResource,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CResource):
        try:
            if self.thisown: destroy(self)
        except: pass
    def load(*args): return apply(_sylphis.CResource_load,args)
    def unload(*args): return apply(_sylphis.CResource_unload,args)
    def getName(*args): return apply(_sylphis.CResource_getName,args)
    def setName(*args): return apply(_sylphis.CResource_setName,args)
    def isLoaded(*args): return apply(_sylphis.CResource_isLoaded,args)
    def __eq__(*args): return apply(_sylphis.CResource___eq__,args)
    def __repr__(self):
        return "<C CResource instance at %s>" % (self.this,)

class CResourcePtr(CResource):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CResource
_sylphis.CResource_swigregister(CResourcePtr)

class CResourceManager(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CResourceManager, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CResourceManager, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CResourceManager,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CResourceManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    def find(*args): return apply(_sylphis.CResourceManager_find,args)
    def loadAll(*args): return apply(_sylphis.CResourceManager_loadAll,args)
    def freeAll(*args): return apply(_sylphis.CResourceManager_freeAll,args)
    def freeUnused(*args): return apply(_sylphis.CResourceManager_freeUnused,args)
    def getResources(*args): return apply(_sylphis.CResourceManager_getResources,args)
    def __repr__(self):
        return "<C CResourceManager instance at %s>" % (self.this,)

class CResourceManagerPtr(CResourceManager):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CResourceManager
_sylphis.CResourceManager_swigregister(CResourceManagerPtr)

SHADER_NOMIPMAPS = _sylphis.SHADER_NOMIPMAPS
SHADER_NOPICMIP = _sylphis.SHADER_NOPICMIP
SHADER_CLAMP = _sylphis.SHADER_CLAMP
class CTexture(CResource):
    __swig_setmethods__ = {}
    for _s in [CResource]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTexture, name, value)
    __swig_getmethods__ = {}
    for _s in [CResource]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CTexture, name)
    TEX_TYPE_1D = _sylphis.CTexture_TEX_TYPE_1D
    TEX_TYPE_2D = _sylphis.CTexture_TEX_TYPE_2D
    TEX_TYPE_RECT = _sylphis.CTexture_TEX_TYPE_RECT
    TEX_TYPE_3D = _sylphis.CTexture_TEX_TYPE_3D
    TEX_TYPE_CUBE = _sylphis.CTexture_TEX_TYPE_CUBE
    def __del__(self, destroy= _sylphis.delete_CTexture):
        try:
            if self.thisown: destroy(self)
        except: pass
    def activate(*args): return apply(_sylphis.CTexture_activate,args)
    def deactivate(*args): return apply(_sylphis.CTexture_deactivate,args)
    def enable(*args): return apply(_sylphis.CTexture_enable,args)
    def disable(*args): return apply(_sylphis.CTexture_disable,args)
    def markUsed(*args): return apply(_sylphis.CTexture_markUsed,args)
    def wasUsed(*args): return apply(_sylphis.CTexture_wasUsed,args)
    def setMemoryUsage(*args): return apply(_sylphis.CTexture_setMemoryUsage,args)
    def getMemoryUsage(*args): return apply(_sylphis.CTexture_getMemoryUsage,args)
    def getType(*args): return apply(_sylphis.CTexture_getType,args)
    def copyFromFrameBuffer(*args): return apply(_sylphis.CTexture_copyFromFrameBuffer,args)
    def getDimentions(*args): return apply(_sylphis.CTexture_getDimentions,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C CTexture instance at %s>" % (self.this,)

class CTexturePtr(CTexture):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTexture
_sylphis.CTexture_swigregister(CTexturePtr)

class CTexture1D(CTexture):
    __swig_setmethods__ = {}
    for _s in [CTexture]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTexture1D, name, value)
    __swig_getmethods__ = {}
    for _s in [CTexture]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CTexture1D, name)
    def makeTexture(*args): return apply(_sylphis.CTexture1D_makeTexture,args)
    def __del__(self, destroy= _sylphis.delete_CTexture1D):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C CTexture1D instance at %s>" % (self.this,)

class CTexture1DPtr(CTexture1D):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTexture1D
_sylphis.CTexture1D_swigregister(CTexture1DPtr)

class CTexture2D(CTexture):
    __swig_setmethods__ = {}
    for _s in [CTexture]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTexture2D, name, value)
    __swig_getmethods__ = {}
    for _s in [CTexture]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CTexture2D, name)
    def makeTexture(*args): return apply(_sylphis.CTexture2D_makeTexture,args)
    def __del__(self, destroy= _sylphis.delete_CTexture2D):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C CTexture2D instance at %s>" % (self.this,)

class CTexture2DPtr(CTexture2D):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTexture2D
_sylphis.CTexture2D_swigregister(CTexture2DPtr)

class CTextureAnim(CTexture):
    __swig_setmethods__ = {}
    for _s in [CTexture]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTextureAnim, name, value)
    __swig_getmethods__ = {}
    for _s in [CTexture]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CTextureAnim, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CTextureAnim,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CTextureAnim):
        try:
            if self.thisown: destroy(self)
        except: pass
    def activate(*args): return apply(_sylphis.CTextureAnim_activate,args)
    def addTexture(*args): return apply(_sylphis.CTextureAnim_addTexture,args)
    def __repr__(self):
        return "<C CTextureAnim instance at %s>" % (self.this,)

class CTextureAnimPtr(CTextureAnim):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTextureAnim
_sylphis.CTextureAnim_swigregister(CTextureAnimPtr)

class CTextureCube(CTexture):
    __swig_setmethods__ = {}
    for _s in [CTexture]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTextureCube, name, value)
    __swig_getmethods__ = {}
    for _s in [CTexture]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CTextureCube, name)
    def __del__(self, destroy= _sylphis.delete_CTextureCube):
        try:
            if self.thisown: destroy(self)
        except: pass
    def makeTexture(*args): return apply(_sylphis.CTextureCube_makeTexture,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C CTextureCube instance at %s>" % (self.this,)

class CTextureCubePtr(CTextureCube):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTextureCube
_sylphis.CTextureCube_swigregister(CTextureCubePtr)

class CTexture3D(CTexture):
    __swig_setmethods__ = {}
    for _s in [CTexture]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTexture3D, name, value)
    __swig_getmethods__ = {}
    for _s in [CTexture]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CTexture3D, name)
    def __del__(self, destroy= _sylphis.delete_CTexture3D):
        try:
            if self.thisown: destroy(self)
        except: pass
    def makeAttenTexture(*args): return apply(_sylphis.CTexture3D_makeAttenTexture,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C CTexture3D instance at %s>" % (self.this,)

class CTexture3DPtr(CTexture3D):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTexture3D
_sylphis.CTexture3D_swigregister(CTexture3DPtr)

class CTextureRect(CTexture):
    __swig_setmethods__ = {}
    for _s in [CTexture]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTextureRect, name, value)
    __swig_getmethods__ = {}
    for _s in [CTexture]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CTextureRect, name)
    def __del__(self, destroy= _sylphis.delete_CTextureRect):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C CTextureRect instance at %s>" % (self.this,)

class CTextureRectPtr(CTextureRect):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTextureRect
_sylphis.CTextureRect_swigregister(CTextureRectPtr)

class CVertexProgram(CTexture):
    __swig_setmethods__ = {}
    for _s in [CTexture]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CVertexProgram, name, value)
    __swig_getmethods__ = {}
    for _s in [CTexture]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CVertexProgram, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CVertexProgram,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CVertexProgram):
        try:
            if self.thisown: destroy(self)
        except: pass
    def makeProgram(*args): return apply(_sylphis.CVertexProgram_makeProgram,args)
    def activate(*args): return apply(_sylphis.CVertexProgram_activate,args)
    def deactivate(*args): return apply(_sylphis.CVertexProgram_deactivate,args)
    def __repr__(self):
        return "<C CVertexProgram instance at %s>" % (self.this,)

class CVertexProgramPtr(CVertexProgram):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CVertexProgram
_sylphis.CVertexProgram_swigregister(CVertexProgramPtr)

class CFragmentProgram(CVertexProgram):
    __swig_setmethods__ = {}
    for _s in [CVertexProgram]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CFragmentProgram, name, value)
    __swig_getmethods__ = {}
    for _s in [CVertexProgram]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CFragmentProgram, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CFragmentProgram,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CFragmentProgram):
        try:
            if self.thisown: destroy(self)
        except: pass
    def activate(*args): return apply(_sylphis.CFragmentProgram_activate,args)
    def __repr__(self):
        return "<C CFragmentProgram instance at %s>" % (self.this,)

class CFragmentProgramPtr(CFragmentProgram):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CFragmentProgram
_sylphis.CFragmentProgram_swigregister(CFragmentProgramPtr)

class CGLShader(CTexture):
    __swig_setmethods__ = {}
    for _s in [CTexture]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CGLShader, name, value)
    __swig_getmethods__ = {}
    for _s in [CTexture]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CGLShader, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CGLShader,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CGLShader):
        try:
            if self.thisown: destroy(self)
        except: pass
    def makeShader(*args): return apply(_sylphis.CGLShader_makeShader,args)
    def activate(*args): return apply(_sylphis.CGLShader_activate,args)
    def deactivate(*args): return apply(_sylphis.CGLShader_deactivate,args)
    def getUniformLocation(*args): return apply(_sylphis.CGLShader_getUniformLocation,args)
    def setUniform(*args): return apply(_sylphis.CGLShader_setUniform,args)
    def getAttributeLocation(*args): return apply(_sylphis.CGLShader_getAttributeLocation,args)
    def setAttribute3v(*args): return apply(_sylphis.CGLShader_setAttribute3v,args)
    def setAttribute4v(*args): return apply(_sylphis.CGLShader_setAttribute4v,args)
    def setUniformMatrix(*args): return apply(_sylphis.CGLShader_setUniformMatrix,args)
    def __repr__(self):
        return "<C CGLShader instance at %s>" % (self.this,)

class CGLShaderPtr(CGLShader):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CGLShader
_sylphis.CGLShader_swigregister(CGLShaderPtr)

class CTextures(CResourceManager):
    __swig_setmethods__ = {}
    for _s in [CResourceManager]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTextures, name, value)
    __swig_getmethods__ = {}
    for _s in [CResourceManager]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CTextures, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CTextures,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CTextures):
        try:
            if self.thisown: destroy(self)
        except: pass
    def registerTexture(*args): return apply(_sylphis.CTextures_registerTexture,args)
    def registerNormalMap(*args): return apply(_sylphis.CTextures_registerNormalMap,args)
    def registerHeightMap(*args): return apply(_sylphis.CTextures_registerHeightMap,args)
    def registerCubeMap(*args): return apply(_sylphis.CTextures_registerCubeMap,args)
    def findTexture(*args): return apply(_sylphis.CTextures_findTexture,args)
    def __repr__(self):
        return "<C CTextures instance at %s>" % (self.this,)

class CTexturesPtr(CTextures):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTextures
_sylphis.CTextures_swigregister(CTexturesPtr)

UNIFORM_1F = _sylphis.UNIFORM_1F
UNIFORM_2F = _sylphis.UNIFORM_2F
UNIFORM_3F = _sylphis.UNIFORM_3F
UNIFORM_4F = _sylphis.UNIFORM_4F
UNIFORM_PYTHON_1F = _sylphis.UNIFORM_PYTHON_1F
UNIFORM_LIGHTPOS = _sylphis.UNIFORM_LIGHTPOS
UNIFORM_EXPOSURE = _sylphis.UNIFORM_EXPOSURE
UNIFORM_LIGHTCOLOR = _sylphis.UNIFORM_LIGHTCOLOR
UNIFORM_LIGHTSIZE = _sylphis.UNIFORM_LIGHTSIZE
UNIFORM_CAMERAPOS = _sylphis.UNIFORM_CAMERAPOS
UNIFORM_CURRENTRENDERNORMALIZE = _sylphis.UNIFORM_CURRENTRENDERNORMALIZE
UNIFORM_INVVIEWSIZE = _sylphis.UNIFORM_INVVIEWSIZE
UNIFORM_FRAMETIME = _sylphis.UNIFORM_FRAMETIME
UNIFORM_LASTMVPMATRIX = _sylphis.UNIFORM_LASTMVPMATRIX
class SUniformParameter(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SUniformParameter, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SUniformParameter, name)
    __swig_setmethods__["mType"] = _sylphis.SUniformParameter_mType_set
    __swig_getmethods__["mType"] = _sylphis.SUniformParameter_mType_get
    if _newclass:mType = property(_sylphis.SUniformParameter_mType_get,_sylphis.SUniformParameter_mType_set)
    __swig_setmethods__["location"] = _sylphis.SUniformParameter_location_set
    __swig_getmethods__["location"] = _sylphis.SUniformParameter_location_get
    if _newclass:location = property(_sylphis.SUniformParameter_location_get,_sylphis.SUniformParameter_location_set)
    __swig_setmethods__["data"] = _sylphis.SUniformParameter_data_set
    __swig_getmethods__["data"] = _sylphis.SUniformParameter_data_get
    if _newclass:data = property(_sylphis.SUniformParameter_data_get,_sylphis.SUniformParameter_data_set)
    __swig_setmethods__["func"] = _sylphis.SUniformParameter_func_set
    __swig_getmethods__["func"] = _sylphis.SUniformParameter_func_get
    if _newclass:func = property(_sylphis.SUniformParameter_func_get,_sylphis.SUniformParameter_func_set)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_SUniformParameter,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_SUniformParameter):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C SUniformParameter instance at %s>" % (self.this,)

class SUniformParameterPtr(SUniformParameter):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SUniformParameter
_sylphis.SUniformParameter_swigregister(SUniformParameterPtr)

SAMPLER_ATTENUATION = _sylphis.SAMPLER_ATTENUATION
SAMPLER_FILTER = _sylphis.SAMPLER_FILTER
class SSamplerParameter(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SSamplerParameter, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SSamplerParameter, name)
    __swig_setmethods__["location"] = _sylphis.SSamplerParameter_location_set
    __swig_getmethods__["location"] = _sylphis.SSamplerParameter_location_get
    if _newclass:location = property(_sylphis.SSamplerParameter_location_get,_sylphis.SSamplerParameter_location_set)
    __swig_setmethods__["mType"] = _sylphis.SSamplerParameter_mType_set
    __swig_getmethods__["mType"] = _sylphis.SSamplerParameter_mType_get
    if _newclass:mType = property(_sylphis.SSamplerParameter_mType_get,_sylphis.SSamplerParameter_mType_set)
    __swig_setmethods__["texture"] = _sylphis.SSamplerParameter_texture_set
    __swig_getmethods__["texture"] = _sylphis.SSamplerParameter_texture_get
    if _newclass:texture = property(_sylphis.SSamplerParameter_texture_get,_sylphis.SSamplerParameter_texture_set)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_SSamplerParameter,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_SSamplerParameter):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C SSamplerParameter instance at %s>" % (self.this,)

class SSamplerParameterPtr(SSamplerParameter):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SSamplerParameter
_sylphis.SSamplerParameter_swigregister(SSamplerParameterPtr)

ATTRIB_TANGENT = _sylphis.ATTRIB_TANGENT
ATTRIB_BINORMAL = _sylphis.ATTRIB_BINORMAL
BIND_NORMAL = _sylphis.BIND_NORMAL
BIND_TANGENT = _sylphis.BIND_TANGENT
BIND_TEXCOORD0 = _sylphis.BIND_TEXCOORD0
class SAttribParameter(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SAttribParameter, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SAttribParameter, name)
    __swig_setmethods__["location"] = _sylphis.SAttribParameter_location_set
    __swig_getmethods__["location"] = _sylphis.SAttribParameter_location_get
    if _newclass:location = property(_sylphis.SAttribParameter_location_get,_sylphis.SAttribParameter_location_set)
    __swig_setmethods__["mType"] = _sylphis.SAttribParameter_mType_set
    __swig_getmethods__["mType"] = _sylphis.SAttribParameter_mType_get
    if _newclass:mType = property(_sylphis.SAttribParameter_mType_get,_sylphis.SAttribParameter_mType_set)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_SAttribParameter,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_SAttribParameter):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C SAttribParameter instance at %s>" % (self.this,)

class SAttribParameterPtr(SAttribParameter):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SAttribParameter
_sylphis.SAttribParameter_swigregister(SAttribParameterPtr)

BLEND_NONE = _sylphis.BLEND_NONE
BLEND_ZERO = _sylphis.BLEND_ZERO
BLEND_ONE = _sylphis.BLEND_ONE
BLEND_DST_COLOR = _sylphis.BLEND_DST_COLOR
BLEND_ONE_MINUS_SRC_ALPHA = _sylphis.BLEND_ONE_MINUS_SRC_ALPHA
BLEND_SRC_ALPHA = _sylphis.BLEND_SRC_ALPHA
BLEND_SRC_COLOR = _sylphis.BLEND_SRC_COLOR
BLEND_ONE_MINUS_DST_COLOR = _sylphis.BLEND_ONE_MINUS_DST_COLOR
BLEND_ONE_MINUS_SRC_COLOR = _sylphis.BLEND_ONE_MINUS_SRC_COLOR
BLEND_DST_ALPHA = _sylphis.BLEND_DST_ALPHA
BLEND_ONE_MINUS_DST_ALPHA = _sylphis.BLEND_ONE_MINUS_DST_ALPHA
DEPTHWRITE_NONE = _sylphis.DEPTHWRITE_NONE
DEPTHWRITE_EQUAL = _sylphis.DEPTHWRITE_EQUAL
DEPTHWRITE_LEQUAL = _sylphis.DEPTHWRITE_LEQUAL
DEPTHWRITE_LESS = _sylphis.DEPTHWRITE_LESS
DEPTHWRITE_GREATER = _sylphis.DEPTHWRITE_GREATER
DEPTHWRITE_GEQUAL = _sylphis.DEPTHWRITE_GEQUAL
CULL_BACK = _sylphis.CULL_BACK
CULL_FRONT = _sylphis.CULL_FRONT
CULL_NONE = _sylphis.CULL_NONE
class CMaterialPass(CRefObject):
    __swig_setmethods__ = {}
    for _s in [CRefObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CMaterialPass, name, value)
    __swig_getmethods__ = {}
    for _s in [CRefObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CMaterialPass, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CMaterialPass,args)
        self.thisown = 1
    def setName(*args): return apply(_sylphis.CMaterialPass_setName,args)
    def getName(*args): return apply(_sylphis.CMaterialPass_getName,args)
    def setShader(*args): return apply(_sylphis.CMaterialPass_setShader,args)
    def getShader(*args): return apply(_sylphis.CMaterialPass_getShader,args)
    def getUniforms(*args): return apply(_sylphis.CMaterialPass_getUniforms,args)
    def getSamplers(*args): return apply(_sylphis.CMaterialPass_getSamplers,args)
    def getAttribs(*args): return apply(_sylphis.CMaterialPass_getAttribs,args)
    def setTarget(*args): return apply(_sylphis.CMaterialPass_setTarget,args)
    def getTarget(*args): return apply(_sylphis.CMaterialPass_getTarget,args)
    def setBlend(*args): return apply(_sylphis.CMaterialPass_setBlend,args)
    def getBlendSrc(*args): return apply(_sylphis.CMaterialPass_getBlendSrc,args)
    def getBlendDst(*args): return apply(_sylphis.CMaterialPass_getBlendDst,args)
    def setDepthTest(*args): return apply(_sylphis.CMaterialPass_setDepthTest,args)
    def getDepthTest(*args): return apply(_sylphis.CMaterialPass_getDepthTest,args)
    def setDepthWrite(*args): return apply(_sylphis.CMaterialPass_setDepthWrite,args)
    def getDepthWrite(*args): return apply(_sylphis.CMaterialPass_getDepthWrite,args)
    def setCullType(*args): return apply(_sylphis.CMaterialPass_setCullType,args)
    def getCullType(*args): return apply(_sylphis.CMaterialPass_getCullType,args)
    def setAmbient(*args): return apply(_sylphis.CMaterialPass_setAmbient,args)
    def isAmbient(*args): return apply(_sylphis.CMaterialPass_isAmbient,args)
    def getBindArrays(*args): return apply(_sylphis.CMaterialPass_getBindArrays,args)
    def setBindArrays(*args): return apply(_sylphis.CMaterialPass_setBindArrays,args)
    def getTexture(*args): return apply(_sylphis.CMaterialPass_getTexture,args)
    def setTexture(*args): return apply(_sylphis.CMaterialPass_setTexture,args)
    def __del__(self, destroy= _sylphis.delete_CMaterialPass):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CMaterialPass instance at %s>" % (self.this,)

class CMaterialPassPtr(CMaterialPass):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CMaterialPass
_sylphis.CMaterialPass_swigregister(CMaterialPassPtr)

class CMaterialTechnique(CRefObject):
    __swig_setmethods__ = {}
    for _s in [CRefObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CMaterialTechnique, name, value)
    __swig_getmethods__ = {}
    for _s in [CRefObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CMaterialTechnique, name)
    def setName(*args): return apply(_sylphis.CMaterialTechnique_setName,args)
    def getName(*args): return apply(_sylphis.CMaterialTechnique_getName,args)
    def getPasses(*args): return apply(_sylphis.CMaterialTechnique_getPasses,args)
    def setAmbientPass(*args): return apply(_sylphis.CMaterialTechnique_setAmbientPass,args)
    def getAmbientPass(*args): return apply(_sylphis.CMaterialTechnique_getAmbientPass,args)
    def getPass(*args): return apply(_sylphis.CMaterialTechnique_getPass,args)
    def addPass(*args): return apply(_sylphis.CMaterialTechnique_addPass,args)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CMaterialTechnique,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CMaterialTechnique):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CMaterialTechnique instance at %s>" % (self.this,)

class CMaterialTechniquePtr(CMaterialTechnique):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CMaterialTechnique
_sylphis.CMaterialTechnique_swigregister(CMaterialTechniquePtr)

class CMaterial(CResource):
    __swig_setmethods__ = {}
    for _s in [CResource]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CMaterial, name, value)
    __swig_getmethods__ = {}
    for _s in [CResource]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CMaterial, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CMaterial,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CMaterial):
        try:
            if self.thisown: destroy(self)
        except: pass
    def getTechniques(*args): return apply(_sylphis.CMaterial_getTechniques,args)
    def addTechnique(*args): return apply(_sylphis.CMaterial_addTechnique,args)
    def getActiveTechnique(*args): return apply(_sylphis.CMaterial_getActiveTechnique,args)
    def setActiveTechnique(*args): return apply(_sylphis.CMaterial_setActiveTechnique,args)
    def getTechnique(*args): return apply(_sylphis.CMaterial_getTechnique,args)
    def setMotionBlurTechnique(*args): return apply(_sylphis.CMaterial_setMotionBlurTechnique,args)
    def getMotionBlurTechnique(*args): return apply(_sylphis.CMaterial_getMotionBlurTechnique,args)
    def getDimensions(*args): return apply(_sylphis.CMaterial_getDimensions,args)
    def setDimensions(*args): return apply(_sylphis.CMaterial_setDimensions,args)
    def markUsed(*args): return apply(_sylphis.CMaterial_markUsed,args)
    def isTransparent(*args): return apply(_sylphis.CMaterial_isTransparent,args)
    def setTransparent(*args): return apply(_sylphis.CMaterial_setTransparent,args)
    def __repr__(self):
        return "<C CMaterial instance at %s>" % (self.this,)

class CMaterialPtr(CMaterial):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CMaterial
_sylphis.CMaterial_swigregister(CMaterialPtr)

class CMaterialManager(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CMaterialManager, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CMaterialManager, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CMaterialManager,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CMaterialManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    def registerMaterial(*args): return apply(_sylphis.CMaterialManager_registerMaterial,args)
    def isMaterialLoaded(*args): return apply(_sylphis.CMaterialManager_isMaterialLoaded,args)
    def addFailedMaterial(*args): return apply(_sylphis.CMaterialManager_addFailedMaterial,args)
    def isMaterialFailed(*args): return apply(_sylphis.CMaterialManager_isMaterialFailed,args)
    def __repr__(self):
        return "<C CMaterialManager instance at %s>" % (self.this,)

class CMaterialManagerPtr(CMaterialManager):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CMaterialManager
_sylphis.CMaterialManager_swigregister(CMaterialManagerPtr)

class CMaterialParser(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CMaterialParser, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CMaterialParser, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CMaterialParser,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CMaterialParser):
        try:
            if self.thisown: destroy(self)
        except: pass
    def addMaterialToRead(*args): return apply(_sylphis.CMaterialParser_addMaterialToRead,args)
    def removeMaterialToRead(*args): return apply(_sylphis.CMaterialParser_removeMaterialToRead,args)
    def setMaterialsToReadDirty(*args): return apply(_sylphis.CMaterialParser_setMaterialsToReadDirty,args)
    def isMaterialsToReadDirty(*args): return apply(_sylphis.CMaterialParser_isMaterialsToReadDirty,args)
    def getNumOfMaterialsToRead(*args): return apply(_sylphis.CMaterialParser_getNumOfMaterialsToRead,args)
    def getReadMaterials(*args): return apply(_sylphis.CMaterialParser_getReadMaterials,args)
    def parseMaterials(*args): return apply(_sylphis.CMaterialParser_parseMaterials,args)
    def setMaterialManager(*args): return apply(_sylphis.CMaterialParser_setMaterialManager,args)
    def printError(*args): return apply(_sylphis.CMaterialParser_printError,args)
    def __repr__(self):
        return "<C CMaterialParser instance at %s>" % (self.this,)

class CMaterialParserPtr(CMaterialParser):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CMaterialParser
_sylphis.CMaterialParser_swigregister(CMaterialParserPtr)

class vertex_t(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, vertex_t, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, vertex_t, name)
    __swig_setmethods__["position"] = _sylphis.vertex_t_position_set
    __swig_getmethods__["position"] = _sylphis.vertex_t_position_get
    if _newclass:position = property(_sylphis.vertex_t_position_get,_sylphis.vertex_t_position_set)
    __swig_setmethods__["normal"] = _sylphis.vertex_t_normal_set
    __swig_getmethods__["normal"] = _sylphis.vertex_t_normal_get
    if _newclass:normal = property(_sylphis.vertex_t_normal_get,_sylphis.vertex_t_normal_set)
    __swig_setmethods__["tangent"] = _sylphis.vertex_t_tangent_set
    __swig_getmethods__["tangent"] = _sylphis.vertex_t_tangent_get
    if _newclass:tangent = property(_sylphis.vertex_t_tangent_get,_sylphis.vertex_t_tangent_set)
    __swig_setmethods__["tex_coord"] = _sylphis.vertex_t_tex_coord_set
    __swig_getmethods__["tex_coord"] = _sylphis.vertex_t_tex_coord_get
    if _newclass:tex_coord = property(_sylphis.vertex_t_tex_coord_get,_sylphis.vertex_t_tex_coord_set)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_vertex_t,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_vertex_t):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C vertex_t instance at %s>" % (self.this,)

class vertex_tPtr(vertex_t):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = vertex_t
_sylphis.vertex_t_swigregister(vertex_tPtr)

class CArrays(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CArrays, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CArrays, name)
    __swig_setmethods__["mVertexBuffer"] = _sylphis.CArrays_mVertexBuffer_set
    __swig_getmethods__["mVertexBuffer"] = _sylphis.CArrays_mVertexBuffer_get
    if _newclass:mVertexBuffer = property(_sylphis.CArrays_mVertexBuffer_get,_sylphis.CArrays_mVertexBuffer_set)
    __swig_setmethods__["verts"] = _sylphis.CArrays_verts_set
    __swig_getmethods__["verts"] = _sylphis.CArrays_verts_get
    if _newclass:verts = property(_sylphis.CArrays_verts_get,_sylphis.CArrays_verts_set)
    __swig_setmethods__["normals"] = _sylphis.CArrays_normals_set
    __swig_getmethods__["normals"] = _sylphis.CArrays_normals_get
    if _newclass:normals = property(_sylphis.CArrays_normals_get,_sylphis.CArrays_normals_set)
    __swig_setmethods__["tangent"] = _sylphis.CArrays_tangent_set
    __swig_getmethods__["tangent"] = _sylphis.CArrays_tangent_get
    if _newclass:tangent = property(_sylphis.CArrays_tangent_get,_sylphis.CArrays_tangent_set)
    __swig_setmethods__["tex_st"] = _sylphis.CArrays_tex_st_set
    __swig_getmethods__["tex_st"] = _sylphis.CArrays_tex_st_get
    if _newclass:tex_st = property(_sylphis.CArrays_tex_st_get,_sylphis.CArrays_tex_st_set)
    __swig_setmethods__["colors"] = _sylphis.CArrays_colors_set
    __swig_getmethods__["colors"] = _sylphis.CArrays_colors_get
    if _newclass:colors = property(_sylphis.CArrays_colors_get,_sylphis.CArrays_colors_set)
    __swig_setmethods__["env_st"] = _sylphis.CArrays_env_st_set
    __swig_getmethods__["env_st"] = _sylphis.CArrays_env_st_get
    if _newclass:env_st = property(_sylphis.CArrays_env_st_get,_sylphis.CArrays_env_st_set)
    __swig_setmethods__["spec_st"] = _sylphis.CArrays_spec_st_set
    __swig_getmethods__["spec_st"] = _sylphis.CArrays_spec_st_get
    if _newclass:spec_st = property(_sylphis.CArrays_spec_st_get,_sylphis.CArrays_spec_st_set)
    __swig_setmethods__["numVerts"] = _sylphis.CArrays_numVerts_set
    __swig_getmethods__["numVerts"] = _sylphis.CArrays_numVerts_get
    if _newclass:numVerts = property(_sylphis.CArrays_numVerts_get,_sylphis.CArrays_numVerts_set)
    __swig_setmethods__["numElems"] = _sylphis.CArrays_numElems_set
    __swig_getmethods__["numElems"] = _sylphis.CArrays_numElems_get
    if _newclass:numElems = property(_sylphis.CArrays_numElems_get,_sylphis.CArrays_numElems_set)
    __swig_setmethods__["mElems"] = _sylphis.CArrays_mElems_set
    __swig_getmethods__["mElems"] = _sylphis.CArrays_mElems_get
    if _newclass:mElems = property(_sylphis.CArrays_mElems_get,_sylphis.CArrays_mElems_set)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CArrays,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CArrays):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CArrays instance at %s>" % (self.this,)

class CArraysPtr(CArrays):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CArrays
_sylphis.CArrays_swigregister(CArraysPtr)

class COverlayQuad(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, COverlayQuad, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, COverlayQuad, name)
    __swig_setmethods__["points"] = _sylphis.COverlayQuad_points_set
    __swig_getmethods__["points"] = _sylphis.COverlayQuad_points_get
    if _newclass:points = property(_sylphis.COverlayQuad_points_get,_sylphis.COverlayQuad_points_set)
    __swig_setmethods__["tex_st"] = _sylphis.COverlayQuad_tex_st_set
    __swig_getmethods__["tex_st"] = _sylphis.COverlayQuad_tex_st_get
    if _newclass:tex_st = property(_sylphis.COverlayQuad_tex_st_get,_sylphis.COverlayQuad_tex_st_set)
    __swig_setmethods__["colors"] = _sylphis.COverlayQuad_colors_set
    __swig_getmethods__["colors"] = _sylphis.COverlayQuad_colors_get
    if _newclass:colors = property(_sylphis.COverlayQuad_colors_get,_sylphis.COverlayQuad_colors_set)
    __swig_setmethods__["shader"] = _sylphis.COverlayQuad_shader_set
    __swig_getmethods__["shader"] = _sylphis.COverlayQuad_shader_get
    if _newclass:shader = property(_sylphis.COverlayQuad_shader_get,_sylphis.COverlayQuad_shader_set)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_COverlayQuad,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_COverlayQuad):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C COverlayQuad instance at %s>" % (self.this,)

class COverlayQuadPtr(COverlayQuad):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = COverlayQuad
_sylphis.COverlayQuad_swigregister(COverlayQuadPtr)

class CRenderer(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CRenderer, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CRenderer, name)
    def __del__(self, destroy= _sylphis.delete_CRenderer):
        try:
            if self.thisown: destroy(self)
        except: pass
    def startup(*args): return apply(_sylphis.CRenderer_startup,args)
    def shutdown(*args): return apply(_sylphis.CRenderer_shutdown,args)
    def flushShadowVolume(*args): return apply(_sylphis.CRenderer_flushShadowVolume,args)
    def isSupported(*args): return apply(_sylphis.CRenderer_isSupported,args)
    def getName(*args): return apply(_sylphis.CRenderer_getName,args)
    def setRenderingView(*args): return apply(_sylphis.CRenderer_setRenderingView,args)
    def getRenderingView(*args): return apply(_sylphis.CRenderer_getRenderingView,args)
    def addLight(*args): return apply(_sylphis.CRenderer_addLight,args)
    def ignoreNewLights(*args): return apply(_sylphis.CRenderer_ignoreNewLights,args)
    def getLights(*args): return apply(_sylphis.CRenderer_getLights,args)
    def clearLights(*args): return apply(_sylphis.CRenderer_clearLights,args)
    def useCamera(*args): return apply(_sylphis.CRenderer_useCamera,args)
    def getCamera(*args): return apply(_sylphis.CRenderer_getCamera,args)
    def setSkyShader(*args): return apply(_sylphis.CRenderer_setSkyShader,args)
    def setSkyFlags(*args): return apply(_sylphis.CRenderer_setSkyFlags,args)
    def beginFrame(*args): return apply(_sylphis.CRenderer_beginFrame,args)
    def endFrame(*args): return apply(_sylphis.CRenderer_endFrame,args)
    def setOverrideShader(*args): return apply(_sylphis.CRenderer_setOverrideShader,args)
    def getOverrideShader(*args): return apply(_sylphis.CRenderer_getOverrideShader,args)
    def setLastMVPMatrix(*args): return apply(_sylphis.CRenderer_setLastMVPMatrix,args)
    def getLastMVPMatrix(*args): return apply(_sylphis.CRenderer_getLastMVPMatrix,args)
    def getModelViewMatrix(*args): return apply(_sylphis.CRenderer_getModelViewMatrix,args)
    def getProjectionMatrix(*args): return apply(_sylphis.CRenderer_getProjectionMatrix,args)
    def getMVPMatrix(*args): return apply(_sylphis.CRenderer_getMVPMatrix,args)
    def beginObjectTransformations(*args): return apply(_sylphis.CRenderer_beginObjectTransformations,args)
    def endObjectTransformations(*args): return apply(_sylphis.CRenderer_endObjectTransformations,args)
    def clearTransformations(*args): return apply(_sylphis.CRenderer_clearTransformations,args)
    def translate(*args): return apply(_sylphis.CRenderer_translate,args)
    def rotate(*args): return apply(_sylphis.CRenderer_rotate,args)
    def scale(*args): return apply(_sylphis.CRenderer_scale,args)
    def GLBeginQuad(*args): return apply(_sylphis.CRenderer_GLBeginQuad,args)
    def GLBeginLine(*args): return apply(_sylphis.CRenderer_GLBeginLine,args)
    def GLEnableTexture2D(*args): return apply(_sylphis.CRenderer_GLEnableTexture2D,args)
    def GLDisableTexture2D(*args): return apply(_sylphis.CRenderer_GLDisableTexture2D,args)
    def GLEnd(*args): return apply(_sylphis.CRenderer_GLEnd,args)
    def GLColor4(*args): return apply(_sylphis.CRenderer_GLColor4,args)
    def GLVertex3(*args): return apply(_sylphis.CRenderer_GLVertex3,args)
    def GLVertex2(*args): return apply(_sylphis.CRenderer_GLVertex2,args)
    def GLTexCoord2(*args): return apply(_sylphis.CRenderer_GLTexCoord2,args)
    def setScissor(*args): return apply(_sylphis.CRenderer_setScissor,args)
    def setup3DMode(*args): return apply(_sylphis.CRenderer_setup3DMode,args)
    def setup2DMode(*args): return apply(_sylphis.CRenderer_setup2DMode,args)
    def setRenderQueue(*args): return apply(_sylphis.CRenderer_setRenderQueue,args)
    def getRenderQueue(*args): return apply(_sylphis.CRenderer_getRenderQueue,args)
    def flushRenderQueue(*args): return apply(_sylphis.CRenderer_flushRenderQueue,args)
    def renderShadowVolumes(*args): return apply(_sylphis.CRenderer_renderShadowVolumes,args)
    def setLight(*args): return apply(_sylphis.CRenderer_setLight,args)
    def getLight(*args): return apply(_sylphis.CRenderer_getLight,args)
    def projectPoint(*args): return apply(_sylphis.CRenderer_projectPoint,args)
    def setupScissor(*args): return apply(_sylphis.CRenderer_setupScissor,args)
    def clearStencil(*args): return apply(_sylphis.CRenderer_clearStencil,args)
    def setAmbientLight(*args): return apply(_sylphis.CRenderer_setAmbientLight,args)
    def renderChunk(*args): return apply(_sylphis.CRenderer_renderChunk,args)
    def drawPic(*args): return apply(_sylphis.CRenderer_drawPic,args)
    def pushRaw(*args): return apply(_sylphis.CRenderer_pushRaw,args)
    def renderMaterialPass(*args): return apply(_sylphis.CRenderer_renderMaterialPass,args)
    def flushWithMaterial(*args): return apply(_sylphis.CRenderer_flushWithMaterial,args)
    def getArrays(*args): return apply(_sylphis.CRenderer_getArrays,args)
    def clearArrays(*args): return apply(_sylphis.CRenderer_clearArrays,args)
    def drawArrays(*args): return apply(_sylphis.CRenderer_drawArrays,args)
    def useDefaultArrays(*args): return apply(_sylphis.CRenderer_useDefaultArrays,args)
    def pushVertex(*args): return apply(_sylphis.CRenderer_pushVertex,args)
    def pushElem(*args): return apply(_sylphis.CRenderer_pushElem,args)
    def debugDrawLine(*args): return apply(_sylphis.CRenderer_debugDrawLine,args)
    def enter2DMode(*args): return apply(_sylphis.CRenderer_enter2DMode,args)
    def exit2DMode(*args): return apply(_sylphis.CRenderer_exit2DMode,args)
    def getTextureDataSize(*args): return apply(_sylphis.CRenderer_getTextureDataSize,args)
    def grabCurrentRender(*args): return apply(_sylphis.CRenderer_grabCurrentRender,args)
    def getEPS(*args): return apply(_sylphis.CRenderer_getEPS,args)
    def setUseShadowVP(*args): return apply(_sylphis.CRenderer_setUseShadowVP,args)
    def isUseShadowVP(*args): return apply(_sylphis.CRenderer_isUseShadowVP,args)
    def setUseHDR(*args): return apply(_sylphis.CRenderer_setUseHDR,args)
    def isUseHDR(*args): return apply(_sylphis.CRenderer_isUseHDR,args)
    def setMaxLuminance(*args): return apply(_sylphis.CRenderer_setMaxLuminance,args)
    def setMinLuminance(*args): return apply(_sylphis.CRenderer_setMinLuminance,args)
    def getMaxLuminance(*args): return apply(_sylphis.CRenderer_getMaxLuminance,args)
    def getMinLuminance(*args): return apply(_sylphis.CRenderer_getMinLuminance,args)
    def setMotionBlurAmount(*args): return apply(_sylphis.CRenderer_setMotionBlurAmount,args)
    def getMotionBlurAmount(*args): return apply(_sylphis.CRenderer_getMotionBlurAmount,args)
    def setShadows(*args): return apply(_sylphis.CRenderer_setShadows,args)
    def isShadows(*args): return apply(_sylphis.CRenderer_isShadows,args)
    def needSpeedRender(*args): return apply(_sylphis.CRenderer_needSpeedRender,args)
    def renderOverlay2D(*args): return apply(_sylphis.CRenderer_renderOverlay2D,args)
    def setEnableFog(*args): return apply(_sylphis.CRenderer_setEnableFog,args)
    def setFogColor(*args): return apply(_sylphis.CRenderer_setFogColor,args)
    def setFogBounds(*args): return apply(_sylphis.CRenderer_setFogBounds,args)
    def setNearDepth(*args): return apply(_sylphis.CRenderer_setNearDepth,args)
    def getNearDepth(*args): return apply(_sylphis.CRenderer_getNearDepth,args)
    def registerAttenuation(*args): return apply(_sylphis.CRenderer_registerAttenuation,args)
    def renderPerformaceInfo(*args): return apply(_sylphis.CRenderer_renderPerformaceInfo,args)
    def calcAverageLuma(*args): return apply(_sylphis.CRenderer_calcAverageLuma,args)
    def makeBloomTexture(*args): return apply(_sylphis.CRenderer_makeBloomTexture,args)
    def downSample(*args): return apply(_sylphis.CRenderer_downSample,args)
    def scaleDown(*args): return apply(_sylphis.CRenderer_scaleDown,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C CRenderer instance at %s>" % (self.this,)

class CRendererPtr(CRenderer):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CRenderer
_sylphis.CRenderer_swigregister(CRendererPtr)

WrapToOne = _sylphis.WrapToOne

class CRenderingTarget(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CRenderingTarget, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CRenderingTarget, name)
    def setDimentions(*args): return apply(_sylphis.CRenderingTarget_setDimentions,args)
    def getDimentions(*args): return apply(_sylphis.CRenderingTarget_getDimentions,args)
    def beginCapture(*args): return apply(_sylphis.CRenderingTarget_beginCapture,args)
    def endCapture(*args): return apply(_sylphis.CRenderingTarget_endCapture,args)
    def swapBuffers(*args): return apply(_sylphis.CRenderingTarget_swapBuffers,args)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CRenderingTarget,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CRenderingTarget):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CRenderingTarget instance at %s>" % (self.this,)

class CRenderingTargetPtr(CRenderingTarget):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CRenderingTarget
_sylphis.CRenderingTarget_swigregister(CRenderingTargetPtr)

class CRenderingView(CRenderingTarget,CInputDevice):
    __swig_setmethods__ = {}
    for _s in [CRenderingTarget,CInputDevice]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CRenderingView, name, value)
    __swig_getmethods__ = {}
    for _s in [CRenderingTarget,CInputDevice]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CRenderingView, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CRenderingView,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CRenderingView):
        try:
            if self.thisown: destroy(self)
        except: pass
    def startup(*args): return apply(_sylphis.CRenderingView_startup,args)
    def shutdown(*args): return apply(_sylphis.CRenderingView_shutdown,args)
    def setDimentions(*args): return apply(_sylphis.CRenderingView_setDimentions,args)
    def getDimentions(*args): return apply(_sylphis.CRenderingView_getDimentions,args)
    def handleEvents(*args): return apply(_sylphis.CRenderingView_handleEvents,args)
    def __repr__(self):
        return "<C CRenderingView instance at %s>" % (self.this,)

class CRenderingViewPtr(CRenderingView):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CRenderingView
_sylphis.CRenderingView_swigregister(CRenderingViewPtr)

class CEdge(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CEdge, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CEdge, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CEdge,args)
        self.thisown = 1
    __swig_setmethods__["a"] = _sylphis.CEdge_a_set
    __swig_getmethods__["a"] = _sylphis.CEdge_a_get
    if _newclass:a = property(_sylphis.CEdge_a_get,_sylphis.CEdge_a_set)
    __swig_setmethods__["b"] = _sylphis.CEdge_b_set
    __swig_getmethods__["b"] = _sylphis.CEdge_b_get
    if _newclass:b = property(_sylphis.CEdge_b_get,_sylphis.CEdge_b_set)
    def __eq__(*args): return apply(_sylphis.CEdge___eq__,args)
    def getHash(*args): return apply(_sylphis.CEdge_getHash,args)
    def __del__(self, destroy= _sylphis.delete_CEdge):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CEdge instance at %s>" % (self.this,)

class CEdgePtr(CEdge):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CEdge
_sylphis.CEdge_swigregister(CEdgePtr)

class CRenderable(CResource):
    __swig_setmethods__ = {}
    for _s in [CResource]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CRenderable, name, value)
    __swig_getmethods__ = {}
    for _s in [CResource]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CRenderable, name)
    def __del__(self, destroy= _sylphis.delete_CRenderable):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setFrame(*args): return apply(_sylphis.CRenderable_setFrame,args)
    def makeFrame(*args): return apply(_sylphis.CRenderable_makeFrame,args)
    def render(*args): return apply(_sylphis.CRenderable_render,args)
    def renderShadowVolume(*args): return apply(_sylphis.CRenderable_renderShadowVolume,args)
    def getSilhoutte(*args): return apply(_sylphis.CRenderable_getSilhoutte,args)
    def renderWithShader(*args): return apply(_sylphis.CRenderable_renderWithShader,args)
    def setDefaultShader(*args): return apply(_sylphis.CRenderable_setDefaultShader,args)
    def getDefaultShader(*args): return apply(_sylphis.CRenderable_getDefaultShader,args)
    __swig_setmethods__["defShader"] = _sylphis.CRenderable_defShader_set
    __swig_getmethods__["defShader"] = _sylphis.CRenderable_defShader_get
    if _newclass:defShader = property(_sylphis.CRenderable_defShader_get,_sylphis.CRenderable_defShader_set)
    __swig_setmethods__["mNoShadows"] = _sylphis.CRenderable_mNoShadows_set
    __swig_getmethods__["mNoShadows"] = _sylphis.CRenderable_mNoShadows_get
    if _newclass:mNoShadows = property(_sylphis.CRenderable_mNoShadows_get,_sylphis.CRenderable_mNoShadows_set)
    def getVertices(*args): return apply(_sylphis.CRenderable_getVertices,args)
    def getNumOfVertices(*args): return apply(_sylphis.CRenderable_getNumOfVertices,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C CRenderable instance at %s>" % (self.this,)

class CRenderablePtr(CRenderable):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CRenderable
_sylphis.CRenderable_swigregister(CRenderablePtr)

class CRenderableElem(CRenderable):
    __swig_setmethods__ = {}
    for _s in [CRenderable]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CRenderableElem, name, value)
    __swig_getmethods__ = {}
    for _s in [CRenderable]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CRenderableElem, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CRenderableElem,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CRenderableElem):
        try:
            if self.thisown: destroy(self)
        except: pass
    def updatePlane(*args): return apply(_sylphis.CRenderableElem_updatePlane,args)
    def setFrame(*args): return apply(_sylphis.CRenderableElem_setFrame,args)
    def render(*args): return apply(_sylphis.CRenderableElem_render,args)
    def getSilhoutte(*args): return apply(_sylphis.CRenderableElem_getSilhoutte,args)
    def renderWithShader(*args): return apply(_sylphis.CRenderableElem_renderWithShader,args)
    __swig_setmethods__["a"] = _sylphis.CRenderableElem_a_set
    __swig_getmethods__["a"] = _sylphis.CRenderableElem_a_get
    if _newclass:a = property(_sylphis.CRenderableElem_a_get,_sylphis.CRenderableElem_a_set)
    __swig_setmethods__["b"] = _sylphis.CRenderableElem_b_set
    __swig_getmethods__["b"] = _sylphis.CRenderableElem_b_get
    if _newclass:b = property(_sylphis.CRenderableElem_b_get,_sylphis.CRenderableElem_b_set)
    __swig_setmethods__["c"] = _sylphis.CRenderableElem_c_set
    __swig_getmethods__["c"] = _sylphis.CRenderableElem_c_get
    if _newclass:c = property(_sylphis.CRenderableElem_c_get,_sylphis.CRenderableElem_c_set)
    __swig_setmethods__["plane"] = _sylphis.CRenderableElem_plane_set
    __swig_getmethods__["plane"] = _sylphis.CRenderableElem_plane_get
    if _newclass:plane = property(_sylphis.CRenderableElem_plane_get,_sylphis.CRenderableElem_plane_set)
    def __repr__(self):
        return "<C CRenderableElem instance at %s>" % (self.this,)

class CRenderableElemPtr(CRenderableElem):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CRenderableElem
_sylphis.CRenderableElem_swigregister(CRenderableElemPtr)

class CRenderableMesh(CRenderable):
    __swig_setmethods__ = {}
    for _s in [CRenderable]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CRenderableMesh, name, value)
    __swig_getmethods__ = {}
    for _s in [CRenderable]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CRenderableMesh, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CRenderableMesh,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CRenderableMesh):
        try:
            if self.thisown: destroy(self)
        except: pass
    def _free(*args): return apply(_sylphis.CRenderableMesh__free,args)
    def reset(*args): return apply(_sylphis.CRenderableMesh_reset,args)
    def makeNormals(*args): return apply(_sylphis.CRenderableMesh_makeNormals,args)
    def makeRElems(*args): return apply(_sylphis.CRenderableMesh_makeRElems,args)
    def setFrame(*args): return apply(_sylphis.CRenderableMesh_setFrame,args)
    def makeFrame(*args): return apply(_sylphis.CRenderableMesh_makeFrame,args)
    def render(*args): return apply(_sylphis.CRenderableMesh_render,args)
    def getSilhoutte(*args): return apply(_sylphis.CRenderableMesh_getSilhoutte,args)
    def renderShadowVolume(*args): return apply(_sylphis.CRenderableMesh_renderShadowVolume,args)
    def renderWithShader(*args): return apply(_sylphis.CRenderableMesh_renderWithShader,args)
    def getVertices(*args): return apply(_sylphis.CRenderableMesh_getVertices,args)
    def getNumOfVertices(*args): return apply(_sylphis.CRenderableMesh_getNumOfVertices,args)
    def getElements(*args): return apply(_sylphis.CRenderableMesh_getElements,args)
    def getNumOfElements(*args): return apply(_sylphis.CRenderableMesh_getNumOfElements,args)
    __swig_setmethods__["mSystemVertices"] = _sylphis.CRenderableMesh_mSystemVertices_set
    __swig_getmethods__["mSystemVertices"] = _sylphis.CRenderableMesh_mSystemVertices_get
    if _newclass:mSystemVertices = property(_sylphis.CRenderableMesh_mSystemVertices_get,_sylphis.CRenderableMesh_mSystemVertices_set)
    __swig_setmethods__["mSystemElems"] = _sylphis.CRenderableMesh_mSystemElems_set
    __swig_getmethods__["mSystemElems"] = _sylphis.CRenderableMesh_mSystemElems_get
    if _newclass:mSystemElems = property(_sylphis.CRenderableMesh_mSystemElems_get,_sylphis.CRenderableMesh_mSystemElems_set)
    def __repr__(self):
        return "<C CRenderableMesh instance at %s>" % (self.this,)

class CRenderableMeshPtr(CRenderableMesh):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CRenderableMesh
_sylphis.CRenderableMesh_swigregister(CRenderableMeshPtr)

class CRenderableManager(CResourceManager):
    __swig_setmethods__ = {}
    for _s in [CResourceManager]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CRenderableManager, name, value)
    __swig_getmethods__ = {}
    for _s in [CResourceManager]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CRenderableManager, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CRenderableManager,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CRenderableManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    def registerRenderable(*args): return apply(_sylphis.CRenderableManager_registerRenderable,args)
    def findRenderable(*args): return apply(_sylphis.CRenderableManager_findRenderable,args)
    def __repr__(self):
        return "<C CRenderableManager instance at %s>" % (self.this,)

class CRenderableManagerPtr(CRenderableManager):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CRenderableManager
_sylphis.CRenderableManager_swigregister(CRenderableManagerPtr)

class CModel(CResource):
    __swig_setmethods__ = {}
    for _s in [CResource]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CModel, name, value)
    __swig_getmethods__ = {}
    for _s in [CResource]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CModel, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CModel,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CModel):
        try:
            if self.thisown: destroy(self)
        except: pass
    def getRMeshes(*args): return apply(_sylphis.CModel_getRMeshes,args)
    def load(*args): return apply(_sylphis.CModel_load,args)
    __swig_setmethods__["radius"] = _sylphis.CModel_radius_set
    __swig_getmethods__["radius"] = _sylphis.CModel_radius_get
    if _newclass:radius = property(_sylphis.CModel_radius_get,_sylphis.CModel_radius_set)
    def __repr__(self):
        return "<C CModel instance at %s>" % (self.this,)

class CModelPtr(CModel):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CModel
_sylphis.CModel_swigregister(CModelPtr)

class CMD3Model(CModel):
    __swig_setmethods__ = {}
    for _s in [CModel]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CMD3Model, name, value)
    __swig_getmethods__ = {}
    for _s in [CModel]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CMD3Model, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CMD3Model,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CMD3Model):
        try:
            if self.thisown: destroy(self)
        except: pass
    def load(*args): return apply(_sylphis.CMD3Model_load,args)
    def __repr__(self):
        return "<C CMD3Model instance at %s>" % (self.this,)

class CMD3ModelPtr(CMD3Model):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CMD3Model
_sylphis.CMD3Model_swigregister(CMD3ModelPtr)

class CSMFModel(CModel):
    __swig_setmethods__ = {}
    for _s in [CModel]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSMFModel, name, value)
    __swig_getmethods__ = {}
    for _s in [CModel]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CSMFModel, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSMFModel,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSMFModel):
        try:
            if self.thisown: destroy(self)
        except: pass
    def load(*args): return apply(_sylphis.CSMFModel_load,args)
    def __repr__(self):
        return "<C CSMFModel instance at %s>" % (self.this,)

class CSMFModelPtr(CSMFModel):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSMFModel
_sylphis.CSMFModel_swigregister(CSMFModelPtr)

class CBrushModel(CMD3Model):
    __swig_setmethods__ = {}
    for _s in [CMD3Model]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CBrushModel, name, value)
    __swig_getmethods__ = {}
    for _s in [CMD3Model]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CBrushModel, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CBrushModel,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CBrushModel):
        try:
            if self.thisown: destroy(self)
        except: pass
    def loadBrushEntity(*args): return apply(_sylphis.CBrushModel_loadBrushEntity,args)
    def load(*args): return apply(_sylphis.CBrushModel_load,args)
    def __repr__(self):
        return "<C CBrushModel instance at %s>" % (self.this,)

class CBrushModelPtr(CBrushModel):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CBrushModel
_sylphis.CBrushModel_swigregister(CBrushModelPtr)

class CSkelModel(CModel):
    __swig_setmethods__ = {}
    for _s in [CModel]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSkelModel, name, value)
    __swig_getmethods__ = {}
    for _s in [CModel]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CSkelModel, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSkelModel,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSkelModel):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setSkeleton(*args): return apply(_sylphis.CSkelModel_setSkeleton,args)
    def getSkeleton(*args): return apply(_sylphis.CSkelModel_getSkeleton,args)
    def addMesh(*args): return apply(_sylphis.CSkelModel_addMesh,args)
    def getMeshes(*args): return apply(_sylphis.CSkelModel_getMeshes,args)
    def addSkelAnimation(*args): return apply(_sylphis.CSkelModel_addSkelAnimation,args)
    def getSkelAnimations(*args): return apply(_sylphis.CSkelModel_getSkelAnimations,args)
    def getSkelAnimation(*args): return apply(_sylphis.CSkelModel_getSkelAnimation,args)
    def addShader(*args): return apply(_sylphis.CSkelModel_addShader,args)
    def getShaderById(*args): return apply(_sylphis.CSkelModel_getShaderById,args)
    def load(*args): return apply(_sylphis.CSkelModel_load,args)
    def __repr__(self):
        return "<C CSkelModel instance at %s>" % (self.this,)

class CSkelModelPtr(CSkelModel):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSkelModel
_sylphis.CSkelModel_swigregister(CSkelModelPtr)

class CBone(CRefObject):
    __swig_setmethods__ = {}
    for _s in [CRefObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CBone, name, value)
    __swig_getmethods__ = {}
    for _s in [CRefObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CBone, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CBone,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CBone):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setName(*args): return apply(_sylphis.CBone_setName,args)
    def getName(*args): return apply(_sylphis.CBone_getName,args)
    def addChildId(*args): return apply(_sylphis.CBone_addChildId,args)
    def setSkeleton(*args): return apply(_sylphis.CBone_setSkeleton,args)
    def setParentId(*args): return apply(_sylphis.CBone_setParentId,args)
    def getParentId(*args): return apply(_sylphis.CBone_getParentId,args)
    def setTranslation(*args): return apply(_sylphis.CBone_setTranslation,args)
    def getTranslation(*args): return apply(_sylphis.CBone_getTranslation,args)
    def setDerivedTranslation(*args): return apply(_sylphis.CBone_setDerivedTranslation,args)
    def getDerivedTranslation(*args): return apply(_sylphis.CBone_getDerivedTranslation,args)
    def setTranslationBoneSpace(*args): return apply(_sylphis.CBone_setTranslationBoneSpace,args)
    def getTranslationBoneSpace(*args): return apply(_sylphis.CBone_getTranslationBoneSpace,args)
    def setRotation(*args): return apply(_sylphis.CBone_setRotation,args)
    def getRotation(*args): return apply(_sylphis.CBone_getRotation,args)
    def setDerivedRotation(*args): return apply(_sylphis.CBone_setDerivedRotation,args)
    def getDerivedRotation(*args): return apply(_sylphis.CBone_getDerivedRotation,args)
    def setRotationBoneSpace(*args): return apply(_sylphis.CBone_setRotationBoneSpace,args)
    def getRotationBoneSpace(*args): return apply(_sylphis.CBone_getRotationBoneSpace,args)
    def __repr__(self):
        return "<C CBone instance at %s>" % (self.this,)

class CBonePtr(CBone):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CBone
_sylphis.CBone_swigregister(CBonePtr)

class CSkeleton(CResource):
    __swig_setmethods__ = {}
    for _s in [CResource]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSkeleton, name, value)
    __swig_getmethods__ = {}
    for _s in [CResource]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CSkeleton, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSkeleton,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSkeleton):
        try:
            if self.thisown: destroy(self)
        except: pass
    def load(*args): return apply(_sylphis.CSkeleton_load,args)
    def getBones(*args): return apply(_sylphis.CSkeleton_getBones,args)
    def getBoneByName(*args): return apply(_sylphis.CSkeleton_getBoneByName,args)
    def __repr__(self):
        return "<C CSkeleton instance at %s>" % (self.this,)

class CSkeletonPtr(CSkeleton):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSkeleton
_sylphis.CSkeleton_swigregister(CSkeletonPtr)

class CBoneInstance(CRefObject):
    __swig_setmethods__ = {}
    for _s in [CRefObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CBoneInstance, name, value)
    __swig_getmethods__ = {}
    for _s in [CRefObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CBoneInstance, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CBoneInstance,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CBoneInstance):
        try:
            if self.thisown: destroy(self)
        except: pass
    def update(*args): return apply(_sylphis.CBoneInstance_update,args)
    def reset(*args): return apply(_sylphis.CBoneInstance_reset,args)
    def getName(*args): return apply(_sylphis.CBoneInstance_getName,args)
    def setSkeleton(*args): return apply(_sylphis.CBoneInstance_setSkeleton,args)
    def addChild(*args): return apply(_sylphis.CBoneInstance_addChild,args)
    def setParentBone(*args): return apply(_sylphis.CBoneInstance_setParentBone,args)
    def getParentBone(*args): return apply(_sylphis.CBoneInstance_getParentBone,args)
    def setTranslation(*args): return apply(_sylphis.CBoneInstance_setTranslation,args)
    def getTranslation(*args): return apply(_sylphis.CBoneInstance_getTranslation,args)
    def setDerivedTranslation(*args): return apply(_sylphis.CBoneInstance_setDerivedTranslation,args)
    def getDerivedTranslation(*args): return apply(_sylphis.CBoneInstance_getDerivedTranslation,args)
    def setTranslationBoneSpace(*args): return apply(_sylphis.CBoneInstance_setTranslationBoneSpace,args)
    def getTranslationBoneSpace(*args): return apply(_sylphis.CBoneInstance_getTranslationBoneSpace,args)
    def setRotation(*args): return apply(_sylphis.CBoneInstance_setRotation,args)
    def getRotation(*args): return apply(_sylphis.CBoneInstance_getRotation,args)
    def setDerivedRotation(*args): return apply(_sylphis.CBoneInstance_setDerivedRotation,args)
    def getDerivedRotation(*args): return apply(_sylphis.CBoneInstance_getDerivedRotation,args)
    def setRotationBoneSpace(*args): return apply(_sylphis.CBoneInstance_setRotationBoneSpace,args)
    def getRotationBoneSpace(*args): return apply(_sylphis.CBoneInstance_getRotationBoneSpace,args)
    def getBoneSpaceMatrix(*args): return apply(_sylphis.CBoneInstance_getBoneSpaceMatrix,args)
    def fixate(*args): return apply(_sylphis.CBoneInstance_fixate,args)
    def blend(*args): return apply(_sylphis.CBoneInstance_blend,args)
    def __repr__(self):
        return "<C CBoneInstance instance at %s>" % (self.this,)

class CBoneInstancePtr(CBoneInstance):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CBoneInstance
_sylphis.CBoneInstance_swigregister(CBoneInstancePtr)

class CSkeletonInstance(CRefObject):
    __swig_setmethods__ = {}
    for _s in [CRefObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSkeletonInstance, name, value)
    __swig_getmethods__ = {}
    for _s in [CRefObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CSkeletonInstance, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSkeletonInstance,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSkeletonInstance):
        try:
            if self.thisown: destroy(self)
        except: pass
    def update(*args): return apply(_sylphis.CSkeletonInstance_update,args)
    def reset(*args): return apply(_sylphis.CSkeletonInstance_reset,args)
    def getBoneByName(*args): return apply(_sylphis.CSkeletonInstance_getBoneByName,args)
    def getBone(*args): return apply(_sylphis.CSkeletonInstance_getBone,args)
    def getBones(*args): return apply(_sylphis.CSkeletonInstance_getBones,args)
    def fixate(*args): return apply(_sylphis.CSkeletonInstance_fixate,args)
    def getBound(*args): return apply(_sylphis.CSkeletonInstance_getBound,args)
    def __repr__(self):
        return "<C CSkeletonInstance instance at %s>" % (self.this,)

class CSkeletonInstancePtr(CSkeletonInstance):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSkeletonInstance
_sylphis.CSkeletonInstance_swigregister(CSkeletonInstancePtr)

class CBoneTrack(CRefObject):
    __swig_setmethods__ = {}
    for _s in [CRefObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CBoneTrack, name, value)
    __swig_getmethods__ = {}
    for _s in [CRefObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CBoneTrack, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CBoneTrack,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CBoneTrack):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setBoneId(*args): return apply(_sylphis.CBoneTrack_setBoneId,args)
    def getBoneId(*args): return apply(_sylphis.CBoneTrack_getBoneId,args)
    def addBoneKeyframe(*args): return apply(_sylphis.CBoneTrack_addBoneKeyframe,args)
    def getAt(*args): return apply(_sylphis.CBoneTrack_getAt,args)
    def load(*args): return apply(_sylphis.CBoneTrack_load,args)
    def __repr__(self):
        return "<C CBoneTrack instance at %s>" % (self.this,)

class CBoneTrackPtr(CBoneTrack):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CBoneTrack
_sylphis.CBoneTrack_swigregister(CBoneTrackPtr)

class CSkelAnimation(CResource):
    __swig_setmethods__ = {}
    for _s in [CResource]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSkelAnimation, name, value)
    __swig_getmethods__ = {}
    for _s in [CResource]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CSkelAnimation, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSkelAnimation,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSkelAnimation):
        try:
            if self.thisown: destroy(self)
        except: pass
    def load(*args): return apply(_sylphis.CSkelAnimation_load,args)
    def addBoneTrack(*args): return apply(_sylphis.CSkelAnimation_addBoneTrack,args)
    def getBoneTracks(*args): return apply(_sylphis.CSkelAnimation_getBoneTracks,args)
    def getDuration(*args): return apply(_sylphis.CSkelAnimation_getDuration,args)
    def setDuration(*args): return apply(_sylphis.CSkelAnimation_setDuration,args)
    def __repr__(self):
        return "<C CSkelAnimation instance at %s>" % (self.this,)

class CSkelAnimationPtr(CSkelAnimation):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSkelAnimation
_sylphis.CSkelAnimation_swigregister(CSkelAnimationPtr)

class CBoneControler(CBoneTrack):
    __swig_setmethods__ = {}
    for _s in [CBoneTrack]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CBoneControler, name, value)
    __swig_getmethods__ = {}
    for _s in [CBoneTrack]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CBoneControler, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CBoneControler,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CBoneControler):
        try:
            if self.thisown: destroy(self)
        except: pass
    def getAt(*args): return apply(_sylphis.CBoneControler_getAt,args)
    def setTranslation(*args): return apply(_sylphis.CBoneControler_setTranslation,args)
    def getTranslation(*args): return apply(_sylphis.CBoneControler_getTranslation,args)
    def setRotation(*args): return apply(_sylphis.CBoneControler_setRotation,args)
    def getRotation(*args): return apply(_sylphis.CBoneControler_getRotation,args)
    def __repr__(self):
        return "<C CBoneControler instance at %s>" % (self.this,)

class CBoneControlerPtr(CBoneControler):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CBoneControler
_sylphis.CBoneControler_swigregister(CBoneControlerPtr)

class CSkelControler(CSkelAnimation):
    __swig_setmethods__ = {}
    for _s in [CSkelAnimation]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSkelControler, name, value)
    __swig_getmethods__ = {}
    for _s in [CSkelAnimation]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CSkelControler, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSkelControler,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSkelControler):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CSkelControler instance at %s>" % (self.this,)

class CSkelControlerPtr(CSkelControler):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSkelControler
_sylphis.CSkelControler_swigregister(CSkelControlerPtr)

class CAnimation(CRefObject):
    __swig_setmethods__ = {}
    for _s in [CRefObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CAnimation, name, value)
    __swig_getmethods__ = {}
    for _s in [CRefObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CAnimation, name)
    STATE_NONE = _sylphis.CAnimation_STATE_NONE
    STATE_SYNC = _sylphis.CAnimation_STATE_SYNC
    STATE_ASYNC = _sylphis.CAnimation_STATE_ASYNC
    STATE_IN = _sylphis.CAnimation_STATE_IN
    STATE_STEADY = _sylphis.CAnimation_STATE_STEADY
    STATE_OUT = _sylphis.CAnimation_STATE_OUT
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CAnimation,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CAnimation):
        try:
            if self.thisown: destroy(self)
        except: pass
    def getDuration(*args): return apply(_sylphis.CAnimation_getDuration,args)
    def setBlend(*args): return apply(_sylphis.CAnimation_setBlend,args)
    def getState(*args): return apply(_sylphis.CAnimation_getState,args)
    def setState(*args): return apply(_sylphis.CAnimation_setState,args)
    def setClearing(*args): return apply(_sylphis.CAnimation_setClearing,args)
    def getClearing(*args): return apply(_sylphis.CAnimation_getClearing,args)
    def getWeight(*args): return apply(_sylphis.CAnimation_getWeight,args)
    def getTime(*args): return apply(_sylphis.CAnimation_getTime,args)
    def setName(*args): return apply(_sylphis.CAnimation_setName,args)
    def getName(*args): return apply(_sylphis.CAnimation_getName,args)
    def getSkelAnimation(*args): return apply(_sylphis.CAnimation_getSkelAnimation,args)
    def update(*args): return apply(_sylphis.CAnimation_update,args)
    def __repr__(self):
        return "<C CAnimation instance at %s>" % (self.this,)

class CAnimationPtr(CAnimation):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CAnimation
_sylphis.CAnimation_swigregister(CAnimationPtr)

class CAnimationCycle(CAnimation):
    __swig_setmethods__ = {}
    for _s in [CAnimation]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CAnimationCycle, name, value)
    __swig_getmethods__ = {}
    for _s in [CAnimation]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CAnimationCycle, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CAnimationCycle,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CAnimationCycle):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setBlend(*args): return apply(_sylphis.CAnimationCycle_setBlend,args)
    def update(*args): return apply(_sylphis.CAnimationCycle_update,args)
    def __repr__(self):
        return "<C CAnimationCycle instance at %s>" % (self.this,)

class CAnimationCyclePtr(CAnimationCycle):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CAnimationCycle
_sylphis.CAnimationCycle_swigregister(CAnimationCyclePtr)

class CAnimationAction(CAnimation):
    __swig_setmethods__ = {}
    for _s in [CAnimation]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CAnimationAction, name, value)
    __swig_getmethods__ = {}
    for _s in [CAnimation]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CAnimationAction, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CAnimationAction,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CAnimationAction):
        try:
            if self.thisown: destroy(self)
        except: pass
    def execute(*args): return apply(_sylphis.CAnimationAction_execute,args)
    def update(*args): return apply(_sylphis.CAnimationAction_update,args)
    def __repr__(self):
        return "<C CAnimationAction instance at %s>" % (self.this,)

class CAnimationActionPtr(CAnimationAction):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CAnimationAction
_sylphis.CAnimationAction_swigregister(CAnimationActionPtr)

class CAnimationBlender(CWaitable):
    __swig_setmethods__ = {}
    for _s in [CWaitable]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CAnimationBlender, name, value)
    __swig_getmethods__ = {}
    for _s in [CWaitable]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CAnimationBlender, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CAnimationBlender,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CAnimationBlender):
        try:
            if self.thisown: destroy(self)
        except: pass
    def update(*args): return apply(_sylphis.CAnimationBlender_update,args)
    def addAnimationCycle(*args): return apply(_sylphis.CAnimationBlender_addAnimationCycle,args)
    def addAnimationAction(*args): return apply(_sylphis.CAnimationBlender_addAnimationAction,args)
    def getAnimWeight(*args): return apply(_sylphis.CAnimationBlender_getAnimWeight,args)
    def blendCycle(*args): return apply(_sylphis.CAnimationBlender_blendCycle,args)
    def weightCycle(*args): return apply(_sylphis.CAnimationBlender_weightCycle,args)
    def clearCycle(*args): return apply(_sylphis.CAnimationBlender_clearCycle,args)
    def clearAllCycle(*args): return apply(_sylphis.CAnimationBlender_clearAllCycle,args)
    def executeAction(*args): return apply(_sylphis.CAnimationBlender_executeAction,args)
    def updateSkeleton(*args): return apply(_sylphis.CAnimationBlender_updateSkeleton,args)
    def __repr__(self):
        return "<C CAnimationBlender instance at %s>" % (self.this,)

class CAnimationBlenderPtr(CAnimationBlender):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CAnimationBlender
_sylphis.CAnimationBlender_swigregister(CAnimationBlenderPtr)

class CModelManager(CResourceManager):
    __swig_setmethods__ = {}
    for _s in [CResourceManager]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CModelManager, name, value)
    __swig_getmethods__ = {}
    for _s in [CResourceManager]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CModelManager, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CModelManager,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CModelManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    def registerModel(*args): return apply(_sylphis.CModelManager_registerModel,args)
    def registerMD3Model(*args): return apply(_sylphis.CModelManager_registerMD3Model,args)
    def registerSMFModel(*args): return apply(_sylphis.CModelManager_registerSMFModel,args)
    def registerSkelModel(*args): return apply(_sylphis.CModelManager_registerSkelModel,args)
    def findModel(*args): return apply(_sylphis.CModelManager_findModel,args)
    def registerSkeleton(*args): return apply(_sylphis.CModelManager_registerSkeleton,args)
    def findSkeleton(*args): return apply(_sylphis.CModelManager_findSkeleton,args)
    def registerMesh(*args): return apply(_sylphis.CModelManager_registerMesh,args)
    def findMesh(*args): return apply(_sylphis.CModelManager_findMesh,args)
    def registerSkelAnimation(*args): return apply(_sylphis.CModelManager_registerSkelAnimation,args)
    def findSkelAnimation(*args): return apply(_sylphis.CModelManager_findSkelAnimation,args)
    def __repr__(self):
        return "<C CModelManager instance at %s>" % (self.this,)

class CModelManagerPtr(CModelManager):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CModelManager
_sylphis.CModelManager_swigregister(CModelManagerPtr)

class CFont(CResource):
    __swig_setmethods__ = {}
    for _s in [CResource]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CFont, name, value)
    __swig_getmethods__ = {}
    for _s in [CResource]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CFont, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CFont,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CFont):
        try:
            if self.thisown: destroy(self)
        except: pass
    def makeFont(*args): return apply(_sylphis.CFont_makeFont,args)
    def renderChar(*args): return apply(_sylphis.CFont_renderChar,args)
    def stringSize(*args): return apply(_sylphis.CFont_stringSize,args)
    def renderString(*args): return apply(_sylphis.CFont_renderString,args)
    def getFontSize(*args): return apply(_sylphis.CFont_getFontSize,args)
    def setFontSize(*args): return apply(_sylphis.CFont_setFontSize,args)
    __swig_setmethods__["pen"] = _sylphis.CFont_pen_set
    __swig_getmethods__["pen"] = _sylphis.CFont_pen_get
    if _newclass:pen = property(_sylphis.CFont_pen_get,_sylphis.CFont_pen_set)
    def __repr__(self):
        return "<C CFont instance at %s>" % (self.this,)

class CFontPtr(CFont):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CFont
_sylphis.CFont_swigregister(CFontPtr)

class CBitmapGlyph(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CBitmapGlyph, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CBitmapGlyph, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CBitmapGlyph,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CBitmapGlyph):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_setmethods__["mTextureId"] = _sylphis.CBitmapGlyph_mTextureId_set
    __swig_getmethods__["mTextureId"] = _sylphis.CBitmapGlyph_mTextureId_get
    if _newclass:mTextureId = property(_sylphis.CBitmapGlyph_mTextureId_get,_sylphis.CBitmapGlyph_mTextureId_set)
    __swig_setmethods__["mPos"] = _sylphis.CBitmapGlyph_mPos_set
    __swig_getmethods__["mPos"] = _sylphis.CBitmapGlyph_mPos_get
    if _newclass:mPos = property(_sylphis.CBitmapGlyph_mPos_get,_sylphis.CBitmapGlyph_mPos_set)
    __swig_setmethods__["mSize"] = _sylphis.CBitmapGlyph_mSize_set
    __swig_getmethods__["mSize"] = _sylphis.CBitmapGlyph_mSize_get
    if _newclass:mSize = property(_sylphis.CBitmapGlyph_mSize_get,_sylphis.CBitmapGlyph_mSize_set)
    __swig_setmethods__["mAdvance"] = _sylphis.CBitmapGlyph_mAdvance_set
    __swig_getmethods__["mAdvance"] = _sylphis.CBitmapGlyph_mAdvance_get
    if _newclass:mAdvance = property(_sylphis.CBitmapGlyph_mAdvance_get,_sylphis.CBitmapGlyph_mAdvance_set)
    __swig_setmethods__["mStart"] = _sylphis.CBitmapGlyph_mStart_set
    __swig_getmethods__["mStart"] = _sylphis.CBitmapGlyph_mStart_get
    if _newclass:mStart = property(_sylphis.CBitmapGlyph_mStart_get,_sylphis.CBitmapGlyph_mStart_set)
    __swig_setmethods__["mEnd"] = _sylphis.CBitmapGlyph_mEnd_set
    __swig_getmethods__["mEnd"] = _sylphis.CBitmapGlyph_mEnd_get
    if _newclass:mEnd = property(_sylphis.CBitmapGlyph_mEnd_get,_sylphis.CBitmapGlyph_mEnd_set)
    def __repr__(self):
        return "<C CBitmapGlyph instance at %s>" % (self.this,)

class CBitmapGlyphPtr(CBitmapGlyph):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CBitmapGlyph
_sylphis.CBitmapGlyph_swigregister(CBitmapGlyphPtr)

class CBitmapFont(CFont):
    __swig_setmethods__ = {}
    for _s in [CFont]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CBitmapFont, name, value)
    __swig_getmethods__ = {}
    for _s in [CFont]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CBitmapFont, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CBitmapFont,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CBitmapFont):
        try:
            if self.thisown: destroy(self)
        except: pass
    def renderChar(*args): return apply(_sylphis.CBitmapFont_renderChar,args)
    def stringSize(*args): return apply(_sylphis.CBitmapFont_stringSize,args)
    def renderString(*args): return apply(_sylphis.CBitmapFont_renderString,args)
    def makeFont(*args): return apply(_sylphis.CBitmapFont_makeFont,args)
    def __repr__(self):
        return "<C CBitmapFont instance at %s>" % (self.this,)

class CBitmapFontPtr(CBitmapFont):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CBitmapFont
_sylphis.CBitmapFont_swigregister(CBitmapFontPtr)

class CFonts(CResourceManager):
    __swig_setmethods__ = {}
    for _s in [CResourceManager]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CFonts, name, value)
    __swig_getmethods__ = {}
    for _s in [CResourceManager]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CFonts, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CFonts,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CFonts):
        try:
            if self.thisown: destroy(self)
        except: pass
    def registerFont(*args): return apply(_sylphis.CFonts_registerFont,args)
    def registerBitmapFont(*args): return apply(_sylphis.CFonts_registerBitmapFont,args)
    def registerTextureFont(*args): return apply(_sylphis.CFonts_registerTextureFont,args)
    def findFont(*args): return apply(_sylphis.CFonts_findFont,args)
    def __repr__(self):
        return "<C CFonts instance at %s>" % (self.this,)

class CFontsPtr(CFonts):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CFonts
_sylphis.CFonts_swigregister(CFontsPtr)

class CTextureGlyph(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTextureGlyph, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CTextureGlyph, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CTextureGlyph,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CTextureGlyph):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_setmethods__["mShader"] = _sylphis.CTextureGlyph_mShader_set
    __swig_getmethods__["mShader"] = _sylphis.CTextureGlyph_mShader_get
    if _newclass:mShader = property(_sylphis.CTextureGlyph_mShader_get,_sylphis.CTextureGlyph_mShader_set)
    __swig_setmethods__["mPos"] = _sylphis.CTextureGlyph_mPos_set
    __swig_getmethods__["mPos"] = _sylphis.CTextureGlyph_mPos_get
    if _newclass:mPos = property(_sylphis.CTextureGlyph_mPos_get,_sylphis.CTextureGlyph_mPos_set)
    __swig_setmethods__["mSize"] = _sylphis.CTextureGlyph_mSize_set
    __swig_getmethods__["mSize"] = _sylphis.CTextureGlyph_mSize_get
    if _newclass:mSize = property(_sylphis.CTextureGlyph_mSize_get,_sylphis.CTextureGlyph_mSize_set)
    __swig_setmethods__["mAdvance"] = _sylphis.CTextureGlyph_mAdvance_set
    __swig_getmethods__["mAdvance"] = _sylphis.CTextureGlyph_mAdvance_get
    if _newclass:mAdvance = property(_sylphis.CTextureGlyph_mAdvance_get,_sylphis.CTextureGlyph_mAdvance_set)
    __swig_setmethods__["mStart"] = _sylphis.CTextureGlyph_mStart_set
    __swig_getmethods__["mStart"] = _sylphis.CTextureGlyph_mStart_get
    if _newclass:mStart = property(_sylphis.CTextureGlyph_mStart_get,_sylphis.CTextureGlyph_mStart_set)
    __swig_setmethods__["mEnd"] = _sylphis.CTextureGlyph_mEnd_set
    __swig_getmethods__["mEnd"] = _sylphis.CTextureGlyph_mEnd_get
    if _newclass:mEnd = property(_sylphis.CTextureGlyph_mEnd_get,_sylphis.CTextureGlyph_mEnd_set)
    def __repr__(self):
        return "<C CTextureGlyph instance at %s>" % (self.this,)

class CTextureGlyphPtr(CTextureGlyph):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTextureGlyph
_sylphis.CTextureGlyph_swigregister(CTextureGlyphPtr)

class CTextureFont(CFont):
    __swig_setmethods__ = {}
    for _s in [CFont]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTextureFont, name, value)
    __swig_getmethods__ = {}
    for _s in [CFont]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CTextureFont, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CTextureFont,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CTextureFont):
        try:
            if self.thisown: destroy(self)
        except: pass
    def renderChar(*args): return apply(_sylphis.CTextureFont_renderChar,args)
    def stringSize(*args): return apply(_sylphis.CTextureFont_stringSize,args)
    def renderString(*args): return apply(_sylphis.CTextureFont_renderString,args)
    def makeFont(*args): return apply(_sylphis.CTextureFont_makeFont,args)
    def __repr__(self):
        return "<C CTextureFont instance at %s>" % (self.this,)

class CTextureFontPtr(CTextureFont):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTextureFont
_sylphis.CTextureFont_swigregister(CTextureFontPtr)

class CTraceResult(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CTraceResult, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CTraceResult, name)
    __swig_setmethods__["frac"] = _sylphis.CTraceResult_frac_set
    __swig_getmethods__["frac"] = _sylphis.CTraceResult_frac_get
    if _newclass:frac = property(_sylphis.CTraceResult_frac_get,_sylphis.CTraceResult_frac_set)
    __swig_setmethods__["mEnd"] = _sylphis.CTraceResult_mEnd_set
    __swig_getmethods__["mEnd"] = _sylphis.CTraceResult_mEnd_get
    if _newclass:mEnd = property(_sylphis.CTraceResult_mEnd_get,_sylphis.CTraceResult_mEnd_set)
    __swig_setmethods__["plane"] = _sylphis.CTraceResult_plane_set
    __swig_getmethods__["plane"] = _sylphis.CTraceResult_plane_get
    if _newclass:plane = property(_sylphis.CTraceResult_plane_get,_sylphis.CTraceResult_plane_set)
    __swig_setmethods__["body"] = _sylphis.CTraceResult_body_set
    __swig_getmethods__["body"] = _sylphis.CTraceResult_body_get
    if _newclass:body = property(_sylphis.CTraceResult_body_get,_sylphis.CTraceResult_body_set)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CTraceResult,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CTraceResult):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CTraceResult instance at %s>" % (self.this,)

class CTraceResultPtr(CTraceResult):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CTraceResult
_sylphis.CTraceResult_swigregister(CTraceResultPtr)

class CSceneManager(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSceneManager, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CSceneManager, name)
    def __del__(self, destroy= _sylphis.delete_CSceneManager):
        try:
            if self.thisown: destroy(self)
        except: pass
    def registerObject(*args): return apply(_sylphis.CSceneManager_registerObject,args)
    def loadWorldMap(*args): return apply(_sylphis.CSceneManager_loadWorldMap,args)
    def unloadWorldMap(*args): return apply(_sylphis.CSceneManager_unloadWorldMap,args)
    def setAmbientLight(*args): return apply(_sylphis.CSceneManager_setAmbientLight,args)
    def getRootSceneNode(*args): return apply(_sylphis.CSceneManager_getRootSceneNode,args)
    def update(*args): return apply(_sylphis.CSceneManager_update,args)
    def render(*args): return apply(_sylphis.CSceneManager_render,args)
    def spatialize(*args): return apply(_sylphis.CSceneManager_spatialize,args)
    def isCulled(*args): return apply(_sylphis.CSceneManager_isCulled,args)
    def collideRigidBodies(*args): return apply(_sylphis.CSceneManager_collideRigidBodies,args)
    def getWorldMapModel(*args): return apply(_sylphis.CSceneManager_getWorldMapModel,args)
    def getAreaPortal(*args): return apply(_sylphis.CSceneManager_getAreaPortal,args)
    def setAreaPortalOpen(*args): return apply(_sylphis.CSceneManager_setAreaPortalOpen,args)
    def setActiveRenderQueue(*args): return apply(_sylphis.CSceneManager_setActiveRenderQueue,args)
    def getActiveRenderQueue(*args): return apply(_sylphis.CSceneManager_getActiveRenderQueue,args)
    def getRenderQueue(*args): return apply(_sylphis.CSceneManager_getRenderQueue,args)
    def addClipPlane(*args): return apply(_sylphis.CSceneManager_addClipPlane,args)
    def clearClipPlanes(*args): return apply(_sylphis.CSceneManager_clearClipPlanes,args)
    def trace(*args): return apply(_sylphis.CSceneManager_trace,args)
    def raytrace(*args): return apply(_sylphis.CSceneManager_raytrace,args)
    def makeLightStatic(*args): return apply(_sylphis.CSceneManager_makeLightStatic,args)
    def renderPerformaceInfo(*args): return apply(_sylphis.CSceneManager_renderPerformaceInfo,args)
    def addParticleSystem(*args): return apply(_sylphis.CSceneManager_addParticleSystem,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C CSceneManager instance at %s>" % (self.this,)

class CSceneManagerPtr(CSceneManager):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSceneManager
_sylphis.CSceneManager_swigregister(CSceneManagerPtr)

class CInterpolatorF32(CRefObject):
    __swig_setmethods__ = {}
    for _s in [CRefObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CInterpolatorF32, name, value)
    __swig_getmethods__ = {}
    for _s in [CRefObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CInterpolatorF32, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CInterpolatorF32,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CInterpolatorF32):
        try:
            if self.thisown: destroy(self)
        except: pass
    def insert(*args): return apply(_sylphis.CInterpolatorF32_insert,args)
    def getAt(*args): return apply(_sylphis.CInterpolatorF32_getAt,args)
    def __repr__(self):
        return "<C CInterpolatorF32 instance at %s>" % (self.this,)

class CInterpolatorF32Ptr(CInterpolatorF32):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CInterpolatorF32
_sylphis.CInterpolatorF32_swigregister(CInterpolatorF32Ptr)

class CInterpolatorVector3(CRefObject):
    __swig_setmethods__ = {}
    for _s in [CRefObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CInterpolatorVector3, name, value)
    __swig_getmethods__ = {}
    for _s in [CRefObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CInterpolatorVector3, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CInterpolatorVector3,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CInterpolatorVector3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def insert(*args): return apply(_sylphis.CInterpolatorVector3_insert,args)
    def getAt(*args): return apply(_sylphis.CInterpolatorVector3_getAt,args)
    def __repr__(self):
        return "<C CInterpolatorVector3 instance at %s>" % (self.this,)

class CInterpolatorVector3Ptr(CInterpolatorVector3):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CInterpolatorVector3
_sylphis.CInterpolatorVector3_swigregister(CInterpolatorVector3Ptr)

class CSplineInterpolatorVector3(CInterpolatorVector3):
    __swig_setmethods__ = {}
    for _s in [CInterpolatorVector3]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSplineInterpolatorVector3, name, value)
    __swig_getmethods__ = {}
    for _s in [CInterpolatorVector3]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CSplineInterpolatorVector3, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSplineInterpolatorVector3,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSplineInterpolatorVector3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def insert(*args): return apply(_sylphis.CSplineInterpolatorVector3_insert,args)
    def getAt(*args): return apply(_sylphis.CSplineInterpolatorVector3_getAt,args)
    def intepolate(*args): return apply(_sylphis.CSplineInterpolatorVector3_intepolate,args)
    def __repr__(self):
        return "<C CSplineInterpolatorVector3 instance at %s>" % (self.this,)

class CSplineInterpolatorVector3Ptr(CSplineInterpolatorVector3):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSplineInterpolatorVector3
_sylphis.CSplineInterpolatorVector3_swigregister(CSplineInterpolatorVector3Ptr)

class CPathFollower(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CPathFollower, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CPathFollower, name)
    def followPath(*args): return apply(_sylphis.CPathFollower_followPath,args)
    def stopFollowPath(*args): return apply(_sylphis.CPathFollower_stopFollowPath,args)
    def lookAtPath(*args): return apply(_sylphis.CPathFollower_lookAtPath,args)
    def stopLookAtPath(*args): return apply(_sylphis.CPathFollower_stopLookAtPath,args)
    def updatePath(*args): return apply(_sylphis.CPathFollower_updatePath,args)
    def updateLookAtPath(*args): return apply(_sylphis.CPathFollower_updateLookAtPath,args)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CPathFollower,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CPathFollower):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CPathFollower instance at %s>" % (self.this,)

class CPathFollowerPtr(CPathFollower):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CPathFollower
_sylphis.CPathFollower_swigregister(CPathFollowerPtr)

class CSceneObject(CPathFollower,CRefObject):
    __swig_setmethods__ = {}
    for _s in [CPathFollower,CRefObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSceneObject, name, value)
    __swig_getmethods__ = {}
    for _s in [CPathFollower,CRefObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CSceneObject, name)
    __swig_getmethods__["factory"] = lambda x: _sylphis.CSceneObject_factory
    if _newclass:factory = staticmethod(_sylphis.CSceneObject_factory)
    __swig_getmethods__["cast"] = lambda x: _sylphis.CSceneObject_cast
    if _newclass:cast = staticmethod(_sylphis.CSceneObject_cast)
    def getClassName(*args): return apply(_sylphis.CSceneObject_getClassName,args)
    def getClassInfo(*args): return apply(_sylphis.CSceneObject_getClassInfo,args)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSceneObject,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSceneObject):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setSceneManager(*args): return apply(_sylphis.CSceneObject_setSceneManager,args)
    def getSceneManager(*args): return apply(_sylphis.CSceneObject_getSceneManager,args)
    def setParent(*args): return apply(_sylphis.CSceneObject_setParent,args)
    def getParent(*args): return apply(_sylphis.CSceneObject_getParent,args)
    def setOrientation(*args): return apply(_sylphis.CSceneObject_setOrientation,args)
    def getOrientation(*args): return apply(_sylphis.CSceneObject_getOrientation,args)
    def lookAt(*args): return apply(_sylphis.CSceneObject_lookAt,args)
    def setPosition(*args): return apply(_sylphis.CSceneObject_setPosition,args)
    def getPosition(*args): return apply(_sylphis.CSceneObject_getPosition,args)
    def passClosedPortals(*args): return apply(_sylphis.CSceneObject_passClosedPortals,args)
    def setPassClosedPortals(*args): return apply(_sylphis.CSceneObject_setPassClosedPortals,args)
    def getDerivedOrientation(*args): return apply(_sylphis.CSceneObject_getDerivedOrientation,args)
    def getDerivedPosition(*args): return apply(_sylphis.CSceneObject_getDerivedPosition,args)
    def getTransformMatrix(*args): return apply(_sylphis.CSceneObject_getTransformMatrix,args)
    def getBound(*args): return apply(_sylphis.CSceneObject_getBound,args)
    def getDerivedBound(*args): return apply(_sylphis.CSceneObject_getDerivedBound,args)
    def move(*args): return apply(_sylphis.CSceneObject_move,args)
    def moveLocal(*args): return apply(_sylphis.CSceneObject_moveLocal,args)
    def rotate(*args): return apply(_sylphis.CSceneObject_rotate,args)
    def roll(*args): return apply(_sylphis.CSceneObject_roll,args)
    def yaw(*args): return apply(_sylphis.CSceneObject_yaw,args)
    def pitch(*args): return apply(_sylphis.CSceneObject_pitch,args)
    def updateData(*args): return apply(_sylphis.CSceneObject_updateData,args)
    def update(*args): return apply(_sylphis.CSceneObject_update,args)
    def propagateBoundToParent(*args): return apply(_sylphis.CSceneObject_propagateBoundToParent,args)
    def updateBound(*args): return apply(_sylphis.CSceneObject_updateBound,args)
    def onRender(*args): return apply(_sylphis.CSceneObject_onRender,args)
    def render(*args): return apply(_sylphis.CSceneObject_render,args)
    def postRenderUpdate(*args): return apply(_sylphis.CSceneObject_postRenderUpdate,args)
    NOCULL = _sylphis.CSceneObject_NOCULL
    NOSHADOW = _sylphis.CSceneObject_NOSHADOW
    NORENDER = _sylphis.CSceneObject_NORENDER
    def setFlags(*args): return apply(_sylphis.CSceneObject_setFlags,args)
    def getFlags(*args): return apply(_sylphis.CSceneObject_getFlags,args)
    def getExtraData(*args): return apply(_sylphis.CSceneObject_getExtraData,args)
    def setExtraData(*args): return apply(_sylphis.CSceneObject_setExtraData,args)
    def save(*args): return apply(_sylphis.CSceneObject_save,args)
    def load(*args): return apply(_sylphis.CSceneObject_load,args)
    def __repr__(self):
        return "<C CSceneObject instance at %s>" % (self.this,)

class CSceneObjectPtr(CSceneObject):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSceneObject
_sylphis.CSceneObject_swigregister(CSceneObjectPtr)
CSceneObject_factory = _sylphis.CSceneObject_factory

CSceneObject_cast = _sylphis.CSceneObject_cast


class CSceneNode(CSceneObject):
    __swig_setmethods__ = {}
    for _s in [CSceneObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSceneNode, name, value)
    __swig_getmethods__ = {}
    for _s in [CSceneObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CSceneNode, name)
    __swig_getmethods__["factory"] = lambda x: _sylphis.CSceneNode_factory
    if _newclass:factory = staticmethod(_sylphis.CSceneNode_factory)
    __swig_getmethods__["cast"] = lambda x: _sylphis.CSceneNode_cast
    if _newclass:cast = staticmethod(_sylphis.CSceneNode_cast)
    def getClassName(*args): return apply(_sylphis.CSceneNode_getClassName,args)
    def getClassInfo(*args): return apply(_sylphis.CSceneNode_getClassInfo,args)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSceneNode,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSceneNode):
        try:
            if self.thisown: destroy(self)
        except: pass
    def addChild(*args): return apply(_sylphis.CSceneNode_addChild,args)
    def removeChild(*args): return apply(_sylphis.CSceneNode_removeChild,args)
    def updateData(*args): return apply(_sylphis.CSceneNode_updateData,args)
    def updateBound(*args): return apply(_sylphis.CSceneNode_updateBound,args)
    def render(*args): return apply(_sylphis.CSceneNode_render,args)
    def postRenderUpdate(*args): return apply(_sylphis.CSceneNode_postRenderUpdate,args)
    def save(*args): return apply(_sylphis.CSceneNode_save,args)
    def load(*args): return apply(_sylphis.CSceneNode_load,args)
    def __repr__(self):
        return "<C CSceneNode instance at %s>" % (self.this,)

class CSceneNodePtr(CSceneNode):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSceneNode
_sylphis.CSceneNode_swigregister(CSceneNodePtr)
CSceneNode_factory = _sylphis.CSceneNode_factory

CSceneNode_cast = _sylphis.CSceneNode_cast


class CLight(CSceneObject):
    __swig_setmethods__ = {}
    for _s in [CSceneObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CLight, name, value)
    __swig_getmethods__ = {}
    for _s in [CSceneObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CLight, name)
    __swig_getmethods__["factory"] = lambda x: _sylphis.CLight_factory
    if _newclass:factory = staticmethod(_sylphis.CLight_factory)
    __swig_getmethods__["cast"] = lambda x: _sylphis.CLight_cast
    if _newclass:cast = staticmethod(_sylphis.CLight_cast)
    def getClassName(*args): return apply(_sylphis.CLight_getClassName,args)
    def getClassInfo(*args): return apply(_sylphis.CLight_getClassInfo,args)
    omni = _sylphis.CLight_omni
    spot = _sylphis.CLight_spot
    flickering = _sylphis.CLight_flickering
    strobo = _sylphis.CLight_strobo
    pulsating = _sylphis.CLight_pulsating
    sun = _sylphis.CLight_sun
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CLight,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CLight):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setType(*args): return apply(_sylphis.CLight_setType,args)
    def getType(*args): return apply(_sylphis.CLight_getType,args)
    def setColor(*args): return apply(_sylphis.CLight_setColor,args)
    def getColor(*args): return apply(_sylphis.CLight_getColor,args)
    def setFilter(*args): return apply(_sylphis.CLight_setFilter,args)
    def getFilter(*args): return apply(_sylphis.CLight_getFilter,args)
    def setAttenuation(*args): return apply(_sylphis.CLight_setAttenuation,args)
    def getAttenuation(*args): return apply(_sylphis.CLight_getAttenuation,args)
    def setRadius(*args): return apply(_sylphis.CLight_setRadius,args)
    def setCenter(*args): return apply(_sylphis.CLight_setCenter,args)
    def getRadius(*args): return apply(_sylphis.CLight_getRadius,args)
    def getCenter(*args): return apply(_sylphis.CLight_getCenter,args)
    def setIsShadowCaster(*args): return apply(_sylphis.CLight_setIsShadowCaster,args)
    def isShadowCaster(*args): return apply(_sylphis.CLight_isShadowCaster,args)
    def setSpecular(*args): return apply(_sylphis.CLight_setSpecular,args)
    def givesSpecular(*args): return apply(_sylphis.CLight_givesSpecular,args)
    def isActive(*args): return apply(_sylphis.CLight_isActive,args)
    def setActive(*args): return apply(_sylphis.CLight_setActive,args)
    def setPhase(*args): return apply(_sylphis.CLight_setPhase,args)
    def setFreq(*args): return apply(_sylphis.CLight_setFreq,args)
    def updateData(*args): return apply(_sylphis.CLight_updateData,args)
    def getEmissionPosition(*args): return apply(_sylphis.CLight_getEmissionPosition,args)
    def getPosition(*args): return apply(_sylphis.CLight_getPosition,args)
    def getDerivedPosition(*args): return apply(_sylphis.CLight_getDerivedPosition,args)
    def getDirection(*args): return apply(_sylphis.CLight_getDirection,args)
    def setPosition(*args): return apply(_sylphis.CLight_setPosition,args)
    def render(*args): return apply(_sylphis.CLight_render,args)
    def save(*args): return apply(_sylphis.CLight_save,args)
    def load(*args): return apply(_sylphis.CLight_load,args)
    def __repr__(self):
        return "<C CLight instance at %s>" % (self.this,)

class CLightPtr(CLight):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CLight
_sylphis.CLight_swigregister(CLightPtr)
CLight_factory = _sylphis.CLight_factory

CLight_cast = _sylphis.CLight_cast


class CCamera(CSceneNode):
    __swig_setmethods__ = {}
    for _s in [CSceneNode]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CCamera, name, value)
    __swig_getmethods__ = {}
    for _s in [CSceneNode]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CCamera, name)
    __swig_getmethods__["factory"] = lambda x: _sylphis.CCamera_factory
    if _newclass:factory = staticmethod(_sylphis.CCamera_factory)
    __swig_getmethods__["cast"] = lambda x: _sylphis.CCamera_cast
    if _newclass:cast = staticmethod(_sylphis.CCamera_cast)
    def getClassName(*args): return apply(_sylphis.CCamera_getClassName,args)
    def getClassInfo(*args): return apply(_sylphis.CCamera_getClassInfo,args)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CCamera,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CCamera):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setFov(*args): return apply(_sylphis.CCamera_setFov,args)
    def getFov(*args): return apply(_sylphis.CCamera_getFov,args)
    def updateFrustum(*args): return apply(_sylphis.CCamera_updateFrustum,args)
    def getFrustum(*args): return apply(_sylphis.CCamera_getFrustum,args)
    def yawFixed(*args): return apply(_sylphis.CCamera_yawFixed,args)
    def pitchFixed(*args): return apply(_sylphis.CCamera_pitchFixed,args)
    def setActive(*args): return apply(_sylphis.CCamera_setActive,args)
    def isActive(*args): return apply(_sylphis.CCamera_isActive,args)
    def render(*args): return apply(_sylphis.CCamera_render,args)
    def save(*args): return apply(_sylphis.CCamera_save,args)
    def load(*args): return apply(_sylphis.CCamera_load,args)
    __swig_setmethods__["fov"] = _sylphis.CCamera_fov_set
    __swig_getmethods__["fov"] = _sylphis.CCamera_fov_get
    if _newclass:fov = property(_sylphis.CCamera_fov_get,_sylphis.CCamera_fov_set)
    def __repr__(self):
        return "<C CCamera instance at %s>" % (self.this,)

class CCameraPtr(CCamera):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CCamera
_sylphis.CCamera_swigregister(CCameraPtr)
CCamera_factory = _sylphis.CCamera_factory

CCamera_cast = _sylphis.CCamera_cast


class CRenderableMultiMesh(CSceneNode):
    __swig_setmethods__ = {}
    for _s in [CSceneNode]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CRenderableMultiMesh, name, value)
    __swig_getmethods__ = {}
    for _s in [CSceneNode]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CRenderableMultiMesh, name)
    __swig_getmethods__["factory"] = lambda x: _sylphis.CRenderableMultiMesh_factory
    if _newclass:factory = staticmethod(_sylphis.CRenderableMultiMesh_factory)
    __swig_getmethods__["cast"] = lambda x: _sylphis.CRenderableMultiMesh_cast
    if _newclass:cast = staticmethod(_sylphis.CRenderableMultiMesh_cast)
    def getClassName(*args): return apply(_sylphis.CRenderableMultiMesh_getClassName,args)
    def getClassInfo(*args): return apply(_sylphis.CRenderableMultiMesh_getClassInfo,args)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CRenderableMultiMesh,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CRenderableMultiMesh):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setFrame(*args): return apply(_sylphis.CRenderableMultiMesh_setFrame,args)
    def addMesh(*args): return apply(_sylphis.CRenderableMultiMesh_addMesh,args)
    def getMeshes(*args): return apply(_sylphis.CRenderableMultiMesh_getMeshes,args)
    def clearMeshes(*args): return apply(_sylphis.CRenderableMultiMesh_clearMeshes,args)
    def render(*args): return apply(_sylphis.CRenderableMultiMesh_render,args)
    def updateBound(*args): return apply(_sylphis.CRenderableMultiMesh_updateBound,args)
    def save(*args): return apply(_sylphis.CRenderableMultiMesh_save,args)
    def load(*args): return apply(_sylphis.CRenderableMultiMesh_load,args)
    def __repr__(self):
        return "<C CRenderableMultiMesh instance at %s>" % (self.this,)

class CRenderableMultiMeshPtr(CRenderableMultiMesh):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CRenderableMultiMesh
_sylphis.CRenderableMultiMesh_swigregister(CRenderableMultiMeshPtr)
CRenderableMultiMesh_factory = _sylphis.CRenderableMultiMesh_factory

CRenderableMultiMesh_cast = _sylphis.CRenderableMultiMesh_cast


class CSceneModel(CSceneNode):
    __swig_setmethods__ = {}
    for _s in [CSceneNode]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSceneModel, name, value)
    __swig_getmethods__ = {}
    for _s in [CSceneNode]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CSceneModel, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSceneModel,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSceneModel):
        try:
            if self.thisown: destroy(self)
        except: pass
    def makeVerticesNormals(*args): return apply(_sylphis.CSceneModel_makeVerticesNormals,args)
    def hasAnim(*args): return apply(_sylphis.CSceneModel_hasAnim,args)
    def getAnimWeight(*args): return apply(_sylphis.CSceneModel_getAnimWeight,args)
    def blendAnimCycle(*args): return apply(_sylphis.CSceneModel_blendAnimCycle,args)
    def weightAnimCycle(*args): return apply(_sylphis.CSceneModel_weightAnimCycle,args)
    def clearAnimCycle(*args): return apply(_sylphis.CSceneModel_clearAnimCycle,args)
    def clearAllAnimCycle(*args): return apply(_sylphis.CSceneModel_clearAllAnimCycle,args)
    def executeAction(*args): return apply(_sylphis.CSceneModel_executeAction,args)
    def waitAction(*args): return apply(_sylphis.CSceneModel_waitAction,args)
    def addChildAtBone(*args): return apply(_sylphis.CSceneModel_addChildAtBone,args)
    def getChildAtBone(*args): return apply(_sylphis.CSceneModel_getChildAtBone,args)
    def updateData(*args): return apply(_sylphis.CSceneModel_updateData,args)
    def updateBound(*args): return apply(_sylphis.CSceneModel_updateBound,args)
    def render(*args): return apply(_sylphis.CSceneModel_render,args)
    def __repr__(self):
        return "<C CSceneModel instance at %s>" % (self.this,)

class CSceneModelPtr(CSceneModel):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSceneModel
_sylphis.CSceneModel_swigregister(CSceneModelPtr)

class CSceneMD3(CRenderableMultiMesh):
    __swig_setmethods__ = {}
    for _s in [CRenderableMultiMesh]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSceneMD3, name, value)
    __swig_getmethods__ = {}
    for _s in [CRenderableMultiMesh]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CSceneMD3, name)
    __swig_getmethods__["factory"] = lambda x: _sylphis.CSceneMD3_factory
    if _newclass:factory = staticmethod(_sylphis.CSceneMD3_factory)
    __swig_getmethods__["cast"] = lambda x: _sylphis.CSceneMD3_cast
    if _newclass:cast = staticmethod(_sylphis.CSceneMD3_cast)
    def getClassName(*args): return apply(_sylphis.CSceneMD3_getClassName,args)
    def getClassInfo(*args): return apply(_sylphis.CSceneMD3_getClassInfo,args)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSceneMD3,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSceneMD3):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setModel(*args): return apply(_sylphis.CSceneMD3_setModel,args)
    def __repr__(self):
        return "<C CSceneMD3 instance at %s>" % (self.this,)

class CSceneMD3Ptr(CSceneMD3):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSceneMD3
_sylphis.CSceneMD3_swigregister(CSceneMD3Ptr)
CSceneMD3_factory = _sylphis.CSceneMD3_factory

CSceneMD3_cast = _sylphis.CSceneMD3_cast


class CParticle(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CParticle, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CParticle, name)
    __swig_setmethods__["mPosition"] = _sylphis.CParticle_mPosition_set
    __swig_getmethods__["mPosition"] = _sylphis.CParticle_mPosition_get
    if _newclass:mPosition = property(_sylphis.CParticle_mPosition_get,_sylphis.CParticle_mPosition_set)
    __swig_setmethods__["mVelocity"] = _sylphis.CParticle_mVelocity_set
    __swig_getmethods__["mVelocity"] = _sylphis.CParticle_mVelocity_get
    if _newclass:mVelocity = property(_sylphis.CParticle_mVelocity_get,_sylphis.CParticle_mVelocity_set)
    __swig_setmethods__["mColor"] = _sylphis.CParticle_mColor_set
    __swig_getmethods__["mColor"] = _sylphis.CParticle_mColor_get
    if _newclass:mColor = property(_sylphis.CParticle_mColor_get,_sylphis.CParticle_mColor_set)
    __swig_setmethods__["mSizeX"] = _sylphis.CParticle_mSizeX_set
    __swig_getmethods__["mSizeX"] = _sylphis.CParticle_mSizeX_get
    if _newclass:mSizeX = property(_sylphis.CParticle_mSizeX_get,_sylphis.CParticle_mSizeX_set)
    __swig_setmethods__["mSizeY"] = _sylphis.CParticle_mSizeY_set
    __swig_getmethods__["mSizeY"] = _sylphis.CParticle_mSizeY_get
    if _newclass:mSizeY = property(_sylphis.CParticle_mSizeY_get,_sylphis.CParticle_mSizeY_set)
    __swig_setmethods__["mRotation"] = _sylphis.CParticle_mRotation_set
    __swig_getmethods__["mRotation"] = _sylphis.CParticle_mRotation_get
    if _newclass:mRotation = property(_sylphis.CParticle_mRotation_get,_sylphis.CParticle_mRotation_set)
    __swig_setmethods__["mRotationSpeed"] = _sylphis.CParticle_mRotationSpeed_set
    __swig_getmethods__["mRotationSpeed"] = _sylphis.CParticle_mRotationSpeed_get
    if _newclass:mRotationSpeed = property(_sylphis.CParticle_mRotationSpeed_get,_sylphis.CParticle_mRotationSpeed_set)
    __swig_setmethods__["mLifetime"] = _sylphis.CParticle_mLifetime_set
    __swig_getmethods__["mLifetime"] = _sylphis.CParticle_mLifetime_get
    if _newclass:mLifetime = property(_sylphis.CParticle_mLifetime_get,_sylphis.CParticle_mLifetime_set)
    __swig_setmethods__["mTimeLived"] = _sylphis.CParticle_mTimeLived_set
    __swig_getmethods__["mTimeLived"] = _sylphis.CParticle_mTimeLived_get
    if _newclass:mTimeLived = property(_sylphis.CParticle_mTimeLived_get,_sylphis.CParticle_mTimeLived_set)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CParticle,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CParticle):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CParticle instance at %s>" % (self.this,)

class CParticlePtr(CParticle):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CParticle
_sylphis.CParticle_swigregister(CParticlePtr)

class TParticleVertex(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, TParticleVertex, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, TParticleVertex, name)
    __swig_setmethods__["position"] = _sylphis.TParticleVertex_position_set
    __swig_getmethods__["position"] = _sylphis.TParticleVertex_position_get
    if _newclass:position = property(_sylphis.TParticleVertex_position_get,_sylphis.TParticleVertex_position_set)
    __swig_setmethods__["color"] = _sylphis.TParticleVertex_color_set
    __swig_getmethods__["color"] = _sylphis.TParticleVertex_color_get
    if _newclass:color = property(_sylphis.TParticleVertex_color_get,_sylphis.TParticleVertex_color_set)
    __swig_setmethods__["texCoord"] = _sylphis.TParticleVertex_texCoord_set
    __swig_getmethods__["texCoord"] = _sylphis.TParticleVertex_texCoord_get
    if _newclass:texCoord = property(_sylphis.TParticleVertex_texCoord_get,_sylphis.TParticleVertex_texCoord_set)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_TParticleVertex,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_TParticleVertex):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C TParticleVertex instance at %s>" % (self.this,)

class TParticleVertexPtr(TParticleVertex):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = TParticleVertex
_sylphis.TParticleVertex_swigregister(TParticleVertexPtr)

PARTICLE_BILLBOARD = _sylphis.PARTICLE_BILLBOARD
PARTICLE_TRACER = _sylphis.PARTICLE_TRACER
PARTICLE_EMITTER_BOX = _sylphis.PARTICLE_EMITTER_BOX
PARTICLE_EMITTER_SPHERE = _sylphis.PARTICLE_EMITTER_SPHERE
PARTICLE_SPAWN_DIRECTION_NATURAL = _sylphis.PARTICLE_SPAWN_DIRECTION_NATURAL
PARTICLE_SPAWN_DIRECTION_AIMED = _sylphis.PARTICLE_SPAWN_DIRECTION_AIMED
class CParticleSystemDef(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CParticleSystemDef, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CParticleSystemDef, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CParticleSystemDef,args)
        self.thisown = 1
    def setMaterial(*args): return apply(_sylphis.CParticleSystemDef_setMaterial,args)
    def getMaterial(*args): return apply(_sylphis.CParticleSystemDef_getMaterial,args)
    def getSizeEnvelope(*args): return apply(_sylphis.CParticleSystemDef_getSizeEnvelope,args)
    def getAspectRatioEnvelope(*args): return apply(_sylphis.CParticleSystemDef_getAspectRatioEnvelope,args)
    def getColorEnvelope(*args): return apply(_sylphis.CParticleSystemDef_getColorEnvelope,args)
    __swig_setmethods__["mEmitterShape"] = _sylphis.CParticleSystemDef_mEmitterShape_set
    __swig_getmethods__["mEmitterShape"] = _sylphis.CParticleSystemDef_mEmitterShape_get
    if _newclass:mEmitterShape = property(_sylphis.CParticleSystemDef_mEmitterShape_get,_sylphis.CParticleSystemDef_mEmitterShape_set)
    __swig_setmethods__["mEmitterDimensions"] = _sylphis.CParticleSystemDef_mEmitterDimensions_set
    __swig_getmethods__["mEmitterDimensions"] = _sylphis.CParticleSystemDef_mEmitterDimensions_get
    if _newclass:mEmitterDimensions = property(_sylphis.CParticleSystemDef_mEmitterDimensions_get,_sylphis.CParticleSystemDef_mEmitterDimensions_set)
    __swig_setmethods__["mSpawnDirection"] = _sylphis.CParticleSystemDef_mSpawnDirection_set
    __swig_getmethods__["mSpawnDirection"] = _sylphis.CParticleSystemDef_mSpawnDirection_get
    if _newclass:mSpawnDirection = property(_sylphis.CParticleSystemDef_mSpawnDirection_get,_sylphis.CParticleSystemDef_mSpawnDirection_set)
    __swig_setmethods__["mSpawnDirectionVariation"] = _sylphis.CParticleSystemDef_mSpawnDirectionVariation_set
    __swig_getmethods__["mSpawnDirectionVariation"] = _sylphis.CParticleSystemDef_mSpawnDirectionVariation_get
    if _newclass:mSpawnDirectionVariation = property(_sylphis.CParticleSystemDef_mSpawnDirectionVariation_get,_sylphis.CParticleSystemDef_mSpawnDirectionVariation_set)
    __swig_setmethods__["mSpawnVelocityMin"] = _sylphis.CParticleSystemDef_mSpawnVelocityMin_set
    __swig_getmethods__["mSpawnVelocityMin"] = _sylphis.CParticleSystemDef_mSpawnVelocityMin_get
    if _newclass:mSpawnVelocityMin = property(_sylphis.CParticleSystemDef_mSpawnVelocityMin_get,_sylphis.CParticleSystemDef_mSpawnVelocityMin_set)
    __swig_setmethods__["mSpawnVelocityMax"] = _sylphis.CParticleSystemDef_mSpawnVelocityMax_set
    __swig_getmethods__["mSpawnVelocityMax"] = _sylphis.CParticleSystemDef_mSpawnVelocityMax_get
    if _newclass:mSpawnVelocityMax = property(_sylphis.CParticleSystemDef_mSpawnVelocityMax_get,_sylphis.CParticleSystemDef_mSpawnVelocityMax_set)
    __swig_setmethods__["mEmitRate"] = _sylphis.CParticleSystemDef_mEmitRate_set
    __swig_getmethods__["mEmitRate"] = _sylphis.CParticleSystemDef_mEmitRate_get
    if _newclass:mEmitRate = property(_sylphis.CParticleSystemDef_mEmitRate_get,_sylphis.CParticleSystemDef_mEmitRate_set)
    __swig_setmethods__["mEmitTime"] = _sylphis.CParticleSystemDef_mEmitTime_set
    __swig_getmethods__["mEmitTime"] = _sylphis.CParticleSystemDef_mEmitTime_get
    if _newclass:mEmitTime = property(_sylphis.CParticleSystemDef_mEmitTime_get,_sylphis.CParticleSystemDef_mEmitTime_set)
    __swig_setmethods__["mBudget"] = _sylphis.CParticleSystemDef_mBudget_set
    __swig_getmethods__["mBudget"] = _sylphis.CParticleSystemDef_mBudget_get
    if _newclass:mBudget = property(_sylphis.CParticleSystemDef_mBudget_get,_sylphis.CParticleSystemDef_mBudget_set)
    __swig_setmethods__["mAcceleration"] = _sylphis.CParticleSystemDef_mAcceleration_set
    __swig_getmethods__["mAcceleration"] = _sylphis.CParticleSystemDef_mAcceleration_get
    if _newclass:mAcceleration = property(_sylphis.CParticleSystemDef_mAcceleration_get,_sylphis.CParticleSystemDef_mAcceleration_set)
    __swig_setmethods__["mRotation"] = _sylphis.CParticleSystemDef_mRotation_set
    __swig_getmethods__["mRotation"] = _sylphis.CParticleSystemDef_mRotation_get
    if _newclass:mRotation = property(_sylphis.CParticleSystemDef_mRotation_get,_sylphis.CParticleSystemDef_mRotation_set)
    __swig_setmethods__["mRotationSpeed"] = _sylphis.CParticleSystemDef_mRotationSpeed_set
    __swig_getmethods__["mRotationSpeed"] = _sylphis.CParticleSystemDef_mRotationSpeed_get
    if _newclass:mRotationSpeed = property(_sylphis.CParticleSystemDef_mRotationSpeed_get,_sylphis.CParticleSystemDef_mRotationSpeed_set)
    __swig_setmethods__["mLifeTime"] = _sylphis.CParticleSystemDef_mLifeTime_set
    __swig_getmethods__["mLifeTime"] = _sylphis.CParticleSystemDef_mLifeTime_get
    if _newclass:mLifeTime = property(_sylphis.CParticleSystemDef_mLifeTime_get,_sylphis.CParticleSystemDef_mLifeTime_set)
    __swig_setmethods__["mCollitionsEnabled"] = _sylphis.CParticleSystemDef_mCollitionsEnabled_set
    __swig_getmethods__["mCollitionsEnabled"] = _sylphis.CParticleSystemDef_mCollitionsEnabled_get
    if _newclass:mCollitionsEnabled = property(_sylphis.CParticleSystemDef_mCollitionsEnabled_get,_sylphis.CParticleSystemDef_mCollitionsEnabled_set)
    __swig_setmethods__["mBounce"] = _sylphis.CParticleSystemDef_mBounce_set
    __swig_getmethods__["mBounce"] = _sylphis.CParticleSystemDef_mBounce_get
    if _newclass:mBounce = property(_sylphis.CParticleSystemDef_mBounce_get,_sylphis.CParticleSystemDef_mBounce_set)
    __swig_setmethods__["mColorEnvelope"] = _sylphis.CParticleSystemDef_mColorEnvelope_set
    __swig_getmethods__["mColorEnvelope"] = _sylphis.CParticleSystemDef_mColorEnvelope_get
    if _newclass:mColorEnvelope = property(_sylphis.CParticleSystemDef_mColorEnvelope_get,_sylphis.CParticleSystemDef_mColorEnvelope_set)
    __swig_setmethods__["mSizeEnvelope"] = _sylphis.CParticleSystemDef_mSizeEnvelope_set
    __swig_getmethods__["mSizeEnvelope"] = _sylphis.CParticleSystemDef_mSizeEnvelope_get
    if _newclass:mSizeEnvelope = property(_sylphis.CParticleSystemDef_mSizeEnvelope_get,_sylphis.CParticleSystemDef_mSizeEnvelope_set)
    __swig_setmethods__["mAspectRatioEnvelope"] = _sylphis.CParticleSystemDef_mAspectRatioEnvelope_set
    __swig_getmethods__["mAspectRatioEnvelope"] = _sylphis.CParticleSystemDef_mAspectRatioEnvelope_get
    if _newclass:mAspectRatioEnvelope = property(_sylphis.CParticleSystemDef_mAspectRatioEnvelope_get,_sylphis.CParticleSystemDef_mAspectRatioEnvelope_set)
    __swig_setmethods__["mParticleType"] = _sylphis.CParticleSystemDef_mParticleType_set
    __swig_getmethods__["mParticleType"] = _sylphis.CParticleSystemDef_mParticleType_get
    if _newclass:mParticleType = property(_sylphis.CParticleSystemDef_mParticleType_get,_sylphis.CParticleSystemDef_mParticleType_set)
    __swig_setmethods__["mMaterial"] = _sylphis.CParticleSystemDef_mMaterial_set
    __swig_getmethods__["mMaterial"] = _sylphis.CParticleSystemDef_mMaterial_get
    if _newclass:mMaterial = property(_sylphis.CParticleSystemDef_mMaterial_get,_sylphis.CParticleSystemDef_mMaterial_set)
    def __del__(self, destroy= _sylphis.delete_CParticleSystemDef):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CParticleSystemDef instance at %s>" % (self.this,)

class CParticleSystemDefPtr(CParticleSystemDef):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CParticleSystemDef
_sylphis.CParticleSystemDef_swigregister(CParticleSystemDefPtr)

class CParticleSystem(CRenderable):
    __swig_setmethods__ = {}
    for _s in [CRenderable]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CParticleSystem, name, value)
    __swig_getmethods__ = {}
    for _s in [CRenderable]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CParticleSystem, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CParticleSystem,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CParticleSystem):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setFrame(*args): return apply(_sylphis.CParticleSystem_setFrame,args)
    def render(*args): return apply(_sylphis.CParticleSystem_render,args)
    def getSilhoutte(*args): return apply(_sylphis.CParticleSystem_getSilhoutte,args)
    def renderWithShader(*args): return apply(_sylphis.CParticleSystem_renderWithShader,args)
    def update(*args): return apply(_sylphis.CParticleSystem_update,args)
    def setBudget(*args): return apply(_sylphis.CParticleSystem_setBudget,args)
    def getBudget(*args): return apply(_sylphis.CParticleSystem_getBudget,args)
    def setDieAfterUse(*args): return apply(_sylphis.CParticleSystem_setDieAfterUse,args)
    def isDieAfterUse(*args): return apply(_sylphis.CParticleSystem_isDieAfterUse,args)
    def setEmitter(*args): return apply(_sylphis.CParticleSystem_setEmitter,args)
    def getEmitter(*args): return apply(_sylphis.CParticleSystem_getEmitter,args)
    def fire(*args): return apply(_sylphis.CParticleSystem_fire,args)
    def setDefinition(*args): return apply(_sylphis.CParticleSystem_setDefinition,args)
    def getDefinition(*args): return apply(_sylphis.CParticleSystem_getDefinition,args)
    def createParticle(*args): return apply(_sylphis.CParticleSystem_createParticle,args)
    def __repr__(self):
        return "<C CParticleSystem instance at %s>" % (self.this,)

class CParticleSystemPtr(CParticleSystem):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CParticleSystem
_sylphis.CParticleSystem_swigregister(CParticleSystemPtr)

class CActor(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CActor, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CActor, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CActor,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CActor):
        try:
            if self.thisown: destroy(self)
        except: pass
    def handleEvent(*args): return apply(_sylphis.CActor_handleEvent,args)
    def handleCollition(*args): return apply(_sylphis.CActor_handleCollition,args)
    __swig_setmethods__["self"] = _sylphis.CActor_self_set
    __swig_getmethods__["self"] = _sylphis.CActor_self_get
    if _newclass:self = property(_sylphis.CActor_self_get,_sylphis.CActor_self_set)
    def __repr__(self):
        return "<C CActor instance at %s>" % (self.this,)

class CActorPtr(CActor):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CActor
_sylphis.CActor_swigregister(CActorPtr)

class CControler(CRefObject):
    __swig_setmethods__ = {}
    for _s in [CRefObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CControler, name, value)
    __swig_getmethods__ = {}
    for _s in [CRefObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CControler, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CControler,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CControler):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setSceneObject(*args): return apply(_sylphis.CControler_setSceneObject,args)
    def __repr__(self):
        return "<C CControler instance at %s>" % (self.this,)

class CControlerPtr(CControler):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CControler
_sylphis.CControler_swigregister(CControlerPtr)

class CSurfaceResponse(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSurfaceResponse, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CSurfaceResponse, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSurfaceResponse,args)
        self.thisown = 1
    __swig_setmethods__["bounce"] = _sylphis.CSurfaceResponse_bounce_set
    __swig_getmethods__["bounce"] = _sylphis.CSurfaceResponse_bounce_get
    if _newclass:bounce = property(_sylphis.CSurfaceResponse_bounce_get,_sylphis.CSurfaceResponse_bounce_set)
    __swig_setmethods__["bounceCutoff"] = _sylphis.CSurfaceResponse_bounceCutoff_set
    __swig_getmethods__["bounceCutoff"] = _sylphis.CSurfaceResponse_bounceCutoff_get
    if _newclass:bounceCutoff = property(_sylphis.CSurfaceResponse_bounceCutoff_get,_sylphis.CSurfaceResponse_bounceCutoff_set)
    __swig_setmethods__["softness"] = _sylphis.CSurfaceResponse_softness_set
    __swig_getmethods__["softness"] = _sylphis.CSurfaceResponse_softness_get
    if _newclass:softness = property(_sylphis.CSurfaceResponse_softness_get,_sylphis.CSurfaceResponse_softness_set)
    def __del__(self, destroy= _sylphis.delete_CSurfaceResponse):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CSurfaceResponse instance at %s>" % (self.this,)

class CSurfaceResponsePtr(CSurfaceResponse):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSurfaceResponse
_sylphis.CSurfaceResponse_swigregister(CSurfaceResponsePtr)
CONTENTS_AIR = cvar.CONTENTS_AIR
CONTENTS_SOLID = cvar.CONTENTS_SOLID
CONTENTS_LAVA = cvar.CONTENTS_LAVA
CONTENTS_SLIME = cvar.CONTENTS_SLIME
CONTENTS_WATER = cvar.CONTENTS_WATER
CONTENTS_FOG = cvar.CONTENTS_FOG
CONTENTS_BODY = cvar.CONTENTS_BODY
CONTENTS_TRIGGER = cvar.CONTENTS_TRIGGER
CONTENTS_PLAYERCLIP = cvar.CONTENTS_PLAYERCLIP
CONTENTS_MONSTERCLIP = cvar.CONTENTS_MONSTERCLIP
CONTENTS_CORPSE = cvar.CONTENTS_CORPSE
MASK_ALL = cvar.MASK_ALL
MASK_SOLID = cvar.MASK_SOLID
MASK_PLAYERSOLID = cvar.MASK_PLAYERSOLID
MASK_WATER = cvar.MASK_WATER
MASK_OPAQUE = cvar.MASK_OPAQUE
MASK_SHOT = cvar.MASK_SHOT

class CRigidBody(CPathFollower,CControler):
    __swig_setmethods__ = {}
    for _s in [CPathFollower,CControler]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CRigidBody, name, value)
    __swig_getmethods__ = {}
    for _s in [CPathFollower,CControler]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CRigidBody, name)
    RIG_SPHERE = _sylphis.CRigidBody_RIG_SPHERE
    RIG_BOX = _sylphis.CRigidBody_RIG_BOX
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CRigidBody,args)
        self.thisown = 1
        self.self = self


    def __del__(self, destroy= _sylphis.delete_CRigidBody):
        try:
            if self.thisown: destroy(self)
        except: pass
    def getClass(*args): return apply(_sylphis.CRigidBody_getClass,args)
    def getGeomID(*args): return apply(_sylphis.CRigidBody_getGeomID,args)
    def getBodyID(*args): return apply(_sylphis.CRigidBody_getBodyID,args)
    def setListener(*args): return apply(_sylphis.CRigidBody_setListener,args)
    def getListener(*args): return apply(_sylphis.CRigidBody_getListener,args)
    def getBounds(*args): return apply(_sylphis.CRigidBody_getBounds,args)
    def isDisabled(*args): return apply(_sylphis.CRigidBody_isDisabled,args)
    def setDisabled(*args): return apply(_sylphis.CRigidBody_setDisabled,args)
    def setSleep(*args): return apply(_sylphis.CRigidBody_setSleep,args)
    def addForce(*args): return apply(_sylphis.CRigidBody_addForce,args)
    def addTorque(*args): return apply(_sylphis.CRigidBody_addTorque,args)
    def addLocalForce(*args): return apply(_sylphis.CRigidBody_addLocalForce,args)
    def addLocalTorque(*args): return apply(_sylphis.CRigidBody_addLocalTorque,args)
    def addForceAt(*args): return apply(_sylphis.CRigidBody_addForceAt,args)
    def addForceAtLocal(*args): return apply(_sylphis.CRigidBody_addForceAtLocal,args)
    def addLocalForceAt(*args): return apply(_sylphis.CRigidBody_addLocalForceAt,args)
    def addLocalForceAtLocal(*args): return apply(_sylphis.CRigidBody_addLocalForceAtLocal,args)
    def makeImmovable(*args): return apply(_sylphis.CRigidBody_makeImmovable,args)
    def setSolid(*args): return apply(_sylphis.CRigidBody_setSolid,args)
    def isSolid(*args): return apply(_sylphis.CRigidBody_isSolid,args)
    def applyGravity(*args): return apply(_sylphis.CRigidBody_applyGravity,args)
    def setSuspendMultiCollitions(*args): return apply(_sylphis.CRigidBody_setSuspendMultiCollitions,args)
    def getSuspendMultiCollitions(*args): return apply(_sylphis.CRigidBody_getSuspendMultiCollitions,args)
    def setDensity(*args): return apply(_sylphis.CRigidBody_setDensity,args)
    def setMass(*args): return apply(_sylphis.CRigidBody_setMass,args)
    def getMass(*args): return apply(_sylphis.CRigidBody_getMass,args)
    def setPosition(*args): return apply(_sylphis.CRigidBody_setPosition,args)
    def getPosition(*args): return apply(_sylphis.CRigidBody_getPosition,args)
    def setOrientation(*args): return apply(_sylphis.CRigidBody_setOrientation,args)
    def getOrientation(*args): return apply(_sylphis.CRigidBody_getOrientation,args)
    def setVelocity(*args): return apply(_sylphis.CRigidBody_setVelocity,args)
    def getVelocity(*args): return apply(_sylphis.CRigidBody_getVelocity,args)
    def setAngVelocity(*args): return apply(_sylphis.CRigidBody_setAngVelocity,args)
    def getAngVelocity(*args): return apply(_sylphis.CRigidBody_getAngVelocity,args)
    def setDamping(*args): return apply(_sylphis.CRigidBody_setDamping,args)
    def getDamping(*args): return apply(_sylphis.CRigidBody_getDamping,args)
    def setAngDamping(*args): return apply(_sylphis.CRigidBody_setAngDamping,args)
    def getAngDamping(*args): return apply(_sylphis.CRigidBody_getAngDamping,args)
    def setDamageFactor(*args): return apply(_sylphis.CRigidBody_setDamageFactor,args)
    def getDamageFactor(*args): return apply(_sylphis.CRigidBody_getDamageFactor,args)
    def setMaxAngularVelocity(*args): return apply(_sylphis.CRigidBody_setMaxAngularVelocity,args)
    def getContents(*args): return apply(_sylphis.CRigidBody_getContents,args)
    def setContents(*args): return apply(_sylphis.CRigidBody_setContents,args)
    def getCollisionMask(*args): return apply(_sylphis.CRigidBody_getCollisionMask,args)
    def setCollisionMask(*args): return apply(_sylphis.CRigidBody_setCollisionMask,args)
    def fitToVertices(*args): return apply(_sylphis.CRigidBody_fitToVertices,args)
    def fitToMesh(*args): return apply(_sylphis.CRigidBody_fitToMesh,args)
    def fitToModel(*args): return apply(_sylphis.CRigidBody_fitToModel,args)
    def noCollitionWith(*args): return apply(_sylphis.CRigidBody_noCollitionWith,args)
    def isCollitionExcluded(*args): return apply(_sylphis.CRigidBody_isCollitionExcluded,args)
    def update(*args): return apply(_sylphis.CRigidBody_update,args)
    def perStepUpdate(*args): return apply(_sylphis.CRigidBody_perStepUpdate,args)
    def onCollide(*args): return apply(_sylphis.CRigidBody_onCollide,args)
    def setGroundBody(*args): return apply(_sylphis.CRigidBody_setGroundBody,args)
    def getGroundBody(*args): return apply(_sylphis.CRigidBody_getGroundBody,args)
    def draw(*args): return apply(_sylphis.CRigidBody_draw,args)
    __swig_setmethods__["mPosition"] = _sylphis.CRigidBody_mPosition_set
    __swig_getmethods__["mPosition"] = _sylphis.CRigidBody_mPosition_get
    if _newclass:mPosition = property(_sylphis.CRigidBody_mPosition_get,_sylphis.CRigidBody_mPosition_set)
    __swig_setmethods__["mOrientation"] = _sylphis.CRigidBody_mOrientation_set
    __swig_getmethods__["mOrientation"] = _sylphis.CRigidBody_mOrientation_get
    if _newclass:mOrientation = property(_sylphis.CRigidBody_mOrientation_get,_sylphis.CRigidBody_mOrientation_set)
    __swig_setmethods__["mVelocity"] = _sylphis.CRigidBody_mVelocity_set
    __swig_getmethods__["mVelocity"] = _sylphis.CRigidBody_mVelocity_get
    if _newclass:mVelocity = property(_sylphis.CRigidBody_mVelocity_get,_sylphis.CRigidBody_mVelocity_set)
    __swig_setmethods__["mDamping"] = _sylphis.CRigidBody_mDamping_set
    __swig_getmethods__["mDamping"] = _sylphis.CRigidBody_mDamping_get
    if _newclass:mDamping = property(_sylphis.CRigidBody_mDamping_get,_sylphis.CRigidBody_mDamping_set)
    __swig_setmethods__["mBBox"] = _sylphis.CRigidBody_mBBox_set
    __swig_getmethods__["mBBox"] = _sylphis.CRigidBody_mBBox_get
    if _newclass:mBBox = property(_sylphis.CRigidBody_mBBox_get,_sylphis.CRigidBody_mBBox_set)
    __swig_setmethods__["disabled"] = _sylphis.CRigidBody_disabled_set
    __swig_getmethods__["disabled"] = _sylphis.CRigidBody_disabled_get
    if _newclass:disabled = property(_sylphis.CRigidBody_disabled_get,_sylphis.CRigidBody_disabled_set)
    __swig_setmethods__["mSolid"] = _sylphis.CRigidBody_mSolid_set
    __swig_getmethods__["mSolid"] = _sylphis.CRigidBody_mSolid_get
    if _newclass:mSolid = property(_sylphis.CRigidBody_mSolid_get,_sylphis.CRigidBody_mSolid_set)
    __swig_setmethods__["mGroundBox"] = _sylphis.CRigidBody_mGroundBox_set
    __swig_getmethods__["mGroundBox"] = _sylphis.CRigidBody_mGroundBox_get
    if _newclass:mGroundBox = property(_sylphis.CRigidBody_mGroundBox_get,_sylphis.CRigidBody_mGroundBox_set)
    __swig_setmethods__["mSR"] = _sylphis.CRigidBody_mSR_set
    __swig_getmethods__["mSR"] = _sylphis.CRigidBody_mSR_get
    if _newclass:mSR = property(_sylphis.CRigidBody_mSR_get,_sylphis.CRigidBody_mSR_set)
    __swig_setmethods__["mDamageFactor"] = _sylphis.CRigidBody_mDamageFactor_set
    __swig_getmethods__["mDamageFactor"] = _sylphis.CRigidBody_mDamageFactor_get
    if _newclass:mDamageFactor = property(_sylphis.CRigidBody_mDamageFactor_get,_sylphis.CRigidBody_mDamageFactor_set)
    __swig_setmethods__["mListener"] = _sylphis.CRigidBody_mListener_set
    __swig_getmethods__["mListener"] = _sylphis.CRigidBody_mListener_get
    if _newclass:mListener = property(_sylphis.CRigidBody_mListener_get,_sylphis.CRigidBody_mListener_set)
    __swig_setmethods__["self"] = _sylphis.CRigidBody_self_set
    __swig_getmethods__["self"] = _sylphis.CRigidBody_self_get
    if _newclass:self = property(_sylphis.CRigidBody_self_get,_sylphis.CRigidBody_self_set)
    def __repr__(self):
        return "<C CRigidBody instance at %s>" % (self.this,)

class CRigidBodyPtr(CRigidBody):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CRigidBody
_sylphis.CRigidBody_swigregister(CRigidBodyPtr)

class CRigidSphere(CRigidBody):
    __swig_setmethods__ = {}
    for _s in [CRigidBody]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CRigidSphere, name, value)
    __swig_getmethods__ = {}
    for _s in [CRigidBody]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CRigidSphere, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CRigidSphere,args)
        self.thisown = 1
        self.self = self


    def __del__(self, destroy= _sylphis.delete_CRigidSphere):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setRadius(*args): return apply(_sylphis.CRigidSphere_setRadius,args)
    def getRadius(*args): return apply(_sylphis.CRigidSphere_getRadius,args)
    def setDensity(*args): return apply(_sylphis.CRigidSphere_setDensity,args)
    def setMass(*args): return apply(_sylphis.CRigidSphere_setMass,args)
    def update(*args): return apply(_sylphis.CRigidSphere_update,args)
    def perStepUpdate(*args): return apply(_sylphis.CRigidSphere_perStepUpdate,args)
    def fitToVertices(*args): return apply(_sylphis.CRigidSphere_fitToVertices,args)
    def __repr__(self):
        return "<C CRigidSphere instance at %s>" % (self.this,)

class CRigidSpherePtr(CRigidSphere):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CRigidSphere
_sylphis.CRigidSphere_swigregister(CRigidSpherePtr)

class CRigidBox(CRigidBody):
    __swig_setmethods__ = {}
    for _s in [CRigidBody]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CRigidBox, name, value)
    __swig_getmethods__ = {}
    for _s in [CRigidBody]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CRigidBox, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CRigidBox,args)
        self.thisown = 1
        self.self = self


    def __del__(self, destroy= _sylphis.delete_CRigidBox):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setDimentions(*args): return apply(_sylphis.CRigidBox_setDimentions,args)
    def getDimentions(*args): return apply(_sylphis.CRigidBox_getDimentions,args)
    def setDensity(*args): return apply(_sylphis.CRigidBox_setDensity,args)
    def setMass(*args): return apply(_sylphis.CRigidBox_setMass,args)
    def fitToVertices(*args): return apply(_sylphis.CRigidBox_fitToVertices,args)
    def draw(*args): return apply(_sylphis.CRigidBox_draw,args)
    def __repr__(self):
        return "<C CRigidBox instance at %s>" % (self.this,)

class CRigidBoxPtr(CRigidBox):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CRigidBox
_sylphis.CRigidBox_swigregister(CRigidBoxPtr)

class CRigidCylinder(CRigidBody):
    __swig_setmethods__ = {}
    for _s in [CRigidBody]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CRigidCylinder, name, value)
    __swig_getmethods__ = {}
    for _s in [CRigidBody]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CRigidCylinder, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CRigidCylinder,args)
        self.thisown = 1
        self.self = self


    def __del__(self, destroy= _sylphis.delete_CRigidCylinder):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setDimentions(*args): return apply(_sylphis.CRigidCylinder_setDimentions,args)
    def getRadius(*args): return apply(_sylphis.CRigidCylinder_getRadius,args)
    def getLength(*args): return apply(_sylphis.CRigidCylinder_getLength,args)
    def setDensity(*args): return apply(_sylphis.CRigidCylinder_setDensity,args)
    def setMass(*args): return apply(_sylphis.CRigidCylinder_setMass,args)
    def fitToVertices(*args): return apply(_sylphis.CRigidCylinder_fitToVertices,args)
    def __repr__(self):
        return "<C CRigidCylinder instance at %s>" % (self.this,)

class CRigidCylinderPtr(CRigidCylinder):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CRigidCylinder
_sylphis.CRigidCylinder_swigregister(CRigidCylinderPtr)

class CRigidCapsule(CRigidBody):
    __swig_setmethods__ = {}
    for _s in [CRigidBody]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CRigidCapsule, name, value)
    __swig_getmethods__ = {}
    for _s in [CRigidBody]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CRigidCapsule, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CRigidCapsule,args)
        self.thisown = 1
        self.self = self


    def __del__(self, destroy= _sylphis.delete_CRigidCapsule):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setDimentions(*args): return apply(_sylphis.CRigidCapsule_setDimentions,args)
    def getDimentions(*args): return apply(_sylphis.CRigidCapsule_getDimentions,args)
    def setDensity(*args): return apply(_sylphis.CRigidCapsule_setDensity,args)
    def setMass(*args): return apply(_sylphis.CRigidCapsule_setMass,args)
    def fitToVertices(*args): return apply(_sylphis.CRigidCapsule_fitToVertices,args)
    def __repr__(self):
        return "<C CRigidCapsule instance at %s>" % (self.this,)

class CRigidCapsulePtr(CRigidCapsule):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CRigidCapsule
_sylphis.CRigidCapsule_swigregister(CRigidCapsulePtr)

class CRigidComplex(CRigidBody):
    __swig_setmethods__ = {}
    for _s in [CRigidBody]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CRigidComplex, name, value)
    __swig_getmethods__ = {}
    for _s in [CRigidBody]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CRigidComplex, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CRigidComplex,args)
        self.thisown = 1
        self.self = self


    def __del__(self, destroy= _sylphis.delete_CRigidComplex):
        try:
            if self.thisown: destroy(self)
        except: pass
    def addBox(*args): return apply(_sylphis.CRigidComplex_addBox,args)
    def addSphere(*args): return apply(_sylphis.CRigidComplex_addSphere,args)
    def addCapsule(*args): return apply(_sylphis.CRigidComplex_addCapsule,args)
    def translateLast(*args): return apply(_sylphis.CRigidComplex_translateLast,args)
    def addLastMass(*args): return apply(_sylphis.CRigidComplex_addLastMass,args)
    def fixate(*args): return apply(_sylphis.CRigidComplex_fixate,args)
    def setDimentions(*args): return apply(_sylphis.CRigidComplex_setDimentions,args)
    def getDimentions(*args): return apply(_sylphis.CRigidComplex_getDimentions,args)
    def setDensity(*args): return apply(_sylphis.CRigidComplex_setDensity,args)
    def setMass(*args): return apply(_sylphis.CRigidComplex_setMass,args)
    def fitToVertices(*args): return apply(_sylphis.CRigidComplex_fitToVertices,args)
    def draw(*args): return apply(_sylphis.CRigidComplex_draw,args)
    def __repr__(self):
        return "<C CRigidComplex instance at %s>" % (self.this,)

class CRigidComplexPtr(CRigidComplex):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CRigidComplex
_sylphis.CRigidComplex_swigregister(CRigidComplexPtr)

class CRigidBodyStatic(CRigidBody):
    __swig_setmethods__ = {}
    for _s in [CRigidBody]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CRigidBodyStatic, name, value)
    __swig_getmethods__ = {}
    for _s in [CRigidBody]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CRigidBodyStatic, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CRigidBodyStatic,args)
        self.thisown = 1
        self.self = self


    def __del__(self, destroy= _sylphis.delete_CRigidBodyStatic):
        try:
            if self.thisown: destroy(self)
        except: pass
    def getClass(*args): return apply(_sylphis.CRigidBodyStatic_getClass,args)
    def setMeshData(*args): return apply(_sylphis.CRigidBodyStatic_setMeshData,args)
    def setSleep(*args): return apply(_sylphis.CRigidBodyStatic_setSleep,args)
    def addForce(*args): return apply(_sylphis.CRigidBodyStatic_addForce,args)
    def addTorque(*args): return apply(_sylphis.CRigidBodyStatic_addTorque,args)
    def addLocalForce(*args): return apply(_sylphis.CRigidBodyStatic_addLocalForce,args)
    def addLocalTorque(*args): return apply(_sylphis.CRigidBodyStatic_addLocalTorque,args)
    def addForceAt(*args): return apply(_sylphis.CRigidBodyStatic_addForceAt,args)
    def addForceAtLocal(*args): return apply(_sylphis.CRigidBodyStatic_addForceAtLocal,args)
    def addLocalForceAt(*args): return apply(_sylphis.CRigidBodyStatic_addLocalForceAt,args)
    def addLocalForceAtLocal(*args): return apply(_sylphis.CRigidBodyStatic_addLocalForceAtLocal,args)
    def makeImmovable(*args): return apply(_sylphis.CRigidBodyStatic_makeImmovable,args)
    def applyGravity(*args): return apply(_sylphis.CRigidBodyStatic_applyGravity,args)
    def setPosition(*args): return apply(_sylphis.CRigidBodyStatic_setPosition,args)
    def getPosition(*args): return apply(_sylphis.CRigidBodyStatic_getPosition,args)
    def setOrientation(*args): return apply(_sylphis.CRigidBodyStatic_setOrientation,args)
    def getOrientation(*args): return apply(_sylphis.CRigidBodyStatic_getOrientation,args)
    def setVelocity(*args): return apply(_sylphis.CRigidBodyStatic_setVelocity,args)
    def getVelocity(*args): return apply(_sylphis.CRigidBodyStatic_getVelocity,args)
    def setAngVelocity(*args): return apply(_sylphis.CRigidBodyStatic_setAngVelocity,args)
    def getAngVelocity(*args): return apply(_sylphis.CRigidBodyStatic_getAngVelocity,args)
    def update(*args): return apply(_sylphis.CRigidBodyStatic_update,args)
    def __repr__(self):
        return "<C CRigidBodyStatic instance at %s>" % (self.this,)

class CRigidBodyStaticPtr(CRigidBodyStatic):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CRigidBodyStatic
_sylphis.CRigidBodyStatic_swigregister(CRigidBodyStaticPtr)

class CJoint(CRefObject):
    __swig_setmethods__ = {}
    for _s in [CRefObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CJoint, name, value)
    __swig_getmethods__ = {}
    for _s in [CRefObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CJoint, name)
    JOINT_BALL = _sylphis.CJoint_JOINT_BALL
    JOINT_HINGE = _sylphis.CJoint_JOINT_HINGE
    JOINT_SLIDER = _sylphis.CJoint_JOINT_SLIDER
    JOINT_CONTACT = _sylphis.CJoint_JOINT_CONTACT
    JOINT_UNIVERSAL = _sylphis.CJoint_JOINT_UNIVERSAL
    JOINT_HINGE2 = _sylphis.CJoint_JOINT_HINGE2
    JOINT_FIXED = _sylphis.CJoint_JOINT_FIXED
    JOINT_AMOTOR = _sylphis.CJoint_JOINT_AMOTOR
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CJoint,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CJoint):
        try:
            if self.thisown: destroy(self)
        except: pass
    def attach(*args): return apply(_sylphis.CJoint_attach,args)
    def attachToWorld(*args): return apply(_sylphis.CJoint_attachToWorld,args)
    def getBody(*args): return apply(_sylphis.CJoint_getBody,args)
    def getClass(*args): return apply(_sylphis.CJoint_getClass,args)
    def __repr__(self):
        return "<C CJoint instance at %s>" % (self.this,)

class CJointPtr(CJoint):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CJoint
_sylphis.CJoint_swigregister(CJointPtr)

class CJointBall(CJoint):
    __swig_setmethods__ = {}
    for _s in [CJoint]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CJointBall, name, value)
    __swig_getmethods__ = {}
    for _s in [CJoint]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CJointBall, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CJointBall,args)
        self.thisown = 1
    def setAnchor(*args): return apply(_sylphis.CJointBall_setAnchor,args)
    def getAnchor(*args): return apply(_sylphis.CJointBall_getAnchor,args)
    def __del__(self, destroy= _sylphis.delete_CJointBall):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CJointBall instance at %s>" % (self.this,)

class CJointBallPtr(CJointBall):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CJointBall
_sylphis.CJointBall_swigregister(CJointBallPtr)

class CJointHinge(CJoint):
    __swig_setmethods__ = {}
    for _s in [CJoint]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CJointHinge, name, value)
    __swig_getmethods__ = {}
    for _s in [CJoint]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CJointHinge, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CJointHinge,args)
        self.thisown = 1
    def setAnchor(*args): return apply(_sylphis.CJointHinge_setAnchor,args)
    def getAnchor(*args): return apply(_sylphis.CJointHinge_getAnchor,args)
    def setAxis(*args): return apply(_sylphis.CJointHinge_setAxis,args)
    def getAxis(*args): return apply(_sylphis.CJointHinge_getAxis,args)
    def setLoStop(*args): return apply(_sylphis.CJointHinge_setLoStop,args)
    def setHiStop(*args): return apply(_sylphis.CJointHinge_setHiStop,args)
    def setStopBounce(*args): return apply(_sylphis.CJointHinge_setStopBounce,args)
    def setStopCFM(*args): return apply(_sylphis.CJointHinge_setStopCFM,args)
    def setStopERP(*args): return apply(_sylphis.CJointHinge_setStopERP,args)
    def setMotorVelocity(*args): return apply(_sylphis.CJointHinge_setMotorVelocity,args)
    def setMotorMaxForce(*args): return apply(_sylphis.CJointHinge_setMotorMaxForce,args)
    def setFudgeFactor(*args): return apply(_sylphis.CJointHinge_setFudgeFactor,args)
    def setCFM(*args): return apply(_sylphis.CJointHinge_setCFM,args)
    def setSuspensionCFM(*args): return apply(_sylphis.CJointHinge_setSuspensionCFM,args)
    def setSuspensionERP(*args): return apply(_sylphis.CJointHinge_setSuspensionERP,args)
    def getLoStop(*args): return apply(_sylphis.CJointHinge_getLoStop,args)
    def getHiStop(*args): return apply(_sylphis.CJointHinge_getHiStop,args)
    def getStopBounce(*args): return apply(_sylphis.CJointHinge_getStopBounce,args)
    def getStopCFM(*args): return apply(_sylphis.CJointHinge_getStopCFM,args)
    def getStopERP(*args): return apply(_sylphis.CJointHinge_getStopERP,args)
    def getMotorVelocity(*args): return apply(_sylphis.CJointHinge_getMotorVelocity,args)
    def getMotorMaxForce(*args): return apply(_sylphis.CJointHinge_getMotorMaxForce,args)
    def getFudgeFactor(*args): return apply(_sylphis.CJointHinge_getFudgeFactor,args)
    def getCFM(*args): return apply(_sylphis.CJointHinge_getCFM,args)
    def getSuspensionCFM(*args): return apply(_sylphis.CJointHinge_getSuspensionCFM,args)
    def getSuspensionERP(*args): return apply(_sylphis.CJointHinge_getSuspensionERP,args)
    def __del__(self, destroy= _sylphis.delete_CJointHinge):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CJointHinge instance at %s>" % (self.this,)

class CJointHingePtr(CJointHinge):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CJointHinge
_sylphis.CJointHinge_swigregister(CJointHingePtr)

class CJointDoubleHinge(CJoint):
    __swig_setmethods__ = {}
    for _s in [CJoint]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CJointDoubleHinge, name, value)
    __swig_getmethods__ = {}
    for _s in [CJoint]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CJointDoubleHinge, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CJointDoubleHinge,args)
        self.thisown = 1
    def setAnchor(*args): return apply(_sylphis.CJointDoubleHinge_setAnchor,args)
    def getAnchor(*args): return apply(_sylphis.CJointDoubleHinge_getAnchor,args)
    def getWeelAngle(*args): return apply(_sylphis.CJointDoubleHinge_getWeelAngle,args)
    def setAxis1(*args): return apply(_sylphis.CJointDoubleHinge_setAxis1,args)
    def getAxis1(*args): return apply(_sylphis.CJointDoubleHinge_getAxis1,args)
    def setAxis2(*args): return apply(_sylphis.CJointDoubleHinge_setAxis2,args)
    def getAxis2(*args): return apply(_sylphis.CJointDoubleHinge_getAxis2,args)
    def setWeelVelocity(*args): return apply(_sylphis.CJointDoubleHinge_setWeelVelocity,args)
    def setWeelMaxForce(*args): return apply(_sylphis.CJointDoubleHinge_setWeelMaxForce,args)
    def setLoStop(*args): return apply(_sylphis.CJointDoubleHinge_setLoStop,args)
    def setHiStop(*args): return apply(_sylphis.CJointDoubleHinge_setHiStop,args)
    def setStopBounce(*args): return apply(_sylphis.CJointDoubleHinge_setStopBounce,args)
    def setStopCFM(*args): return apply(_sylphis.CJointDoubleHinge_setStopCFM,args)
    def setStopERP(*args): return apply(_sylphis.CJointDoubleHinge_setStopERP,args)
    def setMotorVelocity(*args): return apply(_sylphis.CJointDoubleHinge_setMotorVelocity,args)
    def setMotorMaxForce(*args): return apply(_sylphis.CJointDoubleHinge_setMotorMaxForce,args)
    def setFudgeFactor(*args): return apply(_sylphis.CJointDoubleHinge_setFudgeFactor,args)
    def setCFM(*args): return apply(_sylphis.CJointDoubleHinge_setCFM,args)
    def setSuspensionCFM(*args): return apply(_sylphis.CJointDoubleHinge_setSuspensionCFM,args)
    def setSuspensionERP(*args): return apply(_sylphis.CJointDoubleHinge_setSuspensionERP,args)
    def getWeelVelocity(*args): return apply(_sylphis.CJointDoubleHinge_getWeelVelocity,args)
    def getWeelMaxForce(*args): return apply(_sylphis.CJointDoubleHinge_getWeelMaxForce,args)
    def getLoStop(*args): return apply(_sylphis.CJointDoubleHinge_getLoStop,args)
    def getHiStop(*args): return apply(_sylphis.CJointDoubleHinge_getHiStop,args)
    def getStopBounce(*args): return apply(_sylphis.CJointDoubleHinge_getStopBounce,args)
    def getStopCFM(*args): return apply(_sylphis.CJointDoubleHinge_getStopCFM,args)
    def getStopERP(*args): return apply(_sylphis.CJointDoubleHinge_getStopERP,args)
    def getMotorVelocity(*args): return apply(_sylphis.CJointDoubleHinge_getMotorVelocity,args)
    def getMotorMaxForce(*args): return apply(_sylphis.CJointDoubleHinge_getMotorMaxForce,args)
    def getFudgeFactor(*args): return apply(_sylphis.CJointDoubleHinge_getFudgeFactor,args)
    def getCFM(*args): return apply(_sylphis.CJointDoubleHinge_getCFM,args)
    def getSuspensionCFM(*args): return apply(_sylphis.CJointDoubleHinge_getSuspensionCFM,args)
    def getSuspensionERP(*args): return apply(_sylphis.CJointDoubleHinge_getSuspensionERP,args)
    def __del__(self, destroy= _sylphis.delete_CJointDoubleHinge):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CJointDoubleHinge instance at %s>" % (self.this,)

class CJointDoubleHingePtr(CJointDoubleHinge):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CJointDoubleHinge
_sylphis.CJointDoubleHinge_swigregister(CJointDoubleHingePtr)

class CJointSlider(CJoint):
    __swig_setmethods__ = {}
    for _s in [CJoint]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CJointSlider, name, value)
    __swig_getmethods__ = {}
    for _s in [CJoint]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CJointSlider, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CJointSlider,args)
        self.thisown = 1
    def setAnchor(*args): return apply(_sylphis.CJointSlider_setAnchor,args)
    def getAnchor(*args): return apply(_sylphis.CJointSlider_getAnchor,args)
    def setAxis(*args): return apply(_sylphis.CJointSlider_setAxis,args)
    def getAxis(*args): return apply(_sylphis.CJointSlider_getAxis,args)
    def setLoStop(*args): return apply(_sylphis.CJointSlider_setLoStop,args)
    def setHiStop(*args): return apply(_sylphis.CJointSlider_setHiStop,args)
    def setStopBounce(*args): return apply(_sylphis.CJointSlider_setStopBounce,args)
    def setStopCFM(*args): return apply(_sylphis.CJointSlider_setStopCFM,args)
    def setStopERP(*args): return apply(_sylphis.CJointSlider_setStopERP,args)
    def setMotorVelocity(*args): return apply(_sylphis.CJointSlider_setMotorVelocity,args)
    def setMotorMaxForce(*args): return apply(_sylphis.CJointSlider_setMotorMaxForce,args)
    def setFudgeFactor(*args): return apply(_sylphis.CJointSlider_setFudgeFactor,args)
    def setCFM(*args): return apply(_sylphis.CJointSlider_setCFM,args)
    def setSuspensionCFM(*args): return apply(_sylphis.CJointSlider_setSuspensionCFM,args)
    def setSuspensionERP(*args): return apply(_sylphis.CJointSlider_setSuspensionERP,args)
    def getLoStop(*args): return apply(_sylphis.CJointSlider_getLoStop,args)
    def getHiStop(*args): return apply(_sylphis.CJointSlider_getHiStop,args)
    def getStopBounce(*args): return apply(_sylphis.CJointSlider_getStopBounce,args)
    def getStopCFM(*args): return apply(_sylphis.CJointSlider_getStopCFM,args)
    def getStopERP(*args): return apply(_sylphis.CJointSlider_getStopERP,args)
    def getMotorVelocity(*args): return apply(_sylphis.CJointSlider_getMotorVelocity,args)
    def getMotorMaxForce(*args): return apply(_sylphis.CJointSlider_getMotorMaxForce,args)
    def getFudgeFactor(*args): return apply(_sylphis.CJointSlider_getFudgeFactor,args)
    def getCFM(*args): return apply(_sylphis.CJointSlider_getCFM,args)
    def getSuspensionCFM(*args): return apply(_sylphis.CJointSlider_getSuspensionCFM,args)
    def getSuspensionERP(*args): return apply(_sylphis.CJointSlider_getSuspensionERP,args)
    def __del__(self, destroy= _sylphis.delete_CJointSlider):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CJointSlider instance at %s>" % (self.this,)

class CJointSliderPtr(CJointSlider):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CJointSlider
_sylphis.CJointSlider_swigregister(CJointSliderPtr)

class CJointUniversal(CJoint):
    __swig_setmethods__ = {}
    for _s in [CJoint]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CJointUniversal, name, value)
    __swig_getmethods__ = {}
    for _s in [CJoint]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CJointUniversal, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CJointUniversal,args)
        self.thisown = 1
    def setAnchor(*args): return apply(_sylphis.CJointUniversal_setAnchor,args)
    def getAnchor(*args): return apply(_sylphis.CJointUniversal_getAnchor,args)
    def setAxis1(*args): return apply(_sylphis.CJointUniversal_setAxis1,args)
    def getAxis1(*args): return apply(_sylphis.CJointUniversal_getAxis1,args)
    def setAxis2(*args): return apply(_sylphis.CJointUniversal_setAxis2,args)
    def getAxis2(*args): return apply(_sylphis.CJointUniversal_getAxis2,args)
    def __del__(self, destroy= _sylphis.delete_CJointUniversal):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CJointUniversal instance at %s>" % (self.this,)

class CJointUniversalPtr(CJointUniversal):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CJointUniversal
_sylphis.CJointUniversal_swigregister(CJointUniversalPtr)

MR_PREFECT = _sylphis.MR_PREFECT
MR_UPSTARED = _sylphis.MR_UPSTARED
MR_SLIDED = _sylphis.MR_SLIDED
ACTION_FORWARD = _sylphis.ACTION_FORWARD
ACTION_BACKWARD = _sylphis.ACTION_BACKWARD
ACTION_MOVE_LEFT = _sylphis.ACTION_MOVE_LEFT
ACTION_MOVE_RIGHT = _sylphis.ACTION_MOVE_RIGHT
ACTION_TURN_LEFT = _sylphis.ACTION_TURN_LEFT
ACTION_TURN_RIGHT = _sylphis.ACTION_TURN_RIGHT
ACTION_JUMP = _sylphis.ACTION_JUMP
ACTION_ATTACK = _sylphis.ACTION_ATTACK
MAX_TOUCHOBJECTS = _sylphis.MAX_TOUCHOBJECTS
class moveresult_t(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, moveresult_t, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, moveresult_t, name)
    __swig_setmethods__["flags"] = _sylphis.moveresult_t_flags_set
    __swig_getmethods__["flags"] = _sylphis.moveresult_t_flags_get
    if _newclass:flags = property(_sylphis.moveresult_t_flags_get,_sylphis.moveresult_t_flags_set)
    __swig_setmethods__["numtobjects"] = _sylphis.moveresult_t_numtobjects_set
    __swig_getmethods__["numtobjects"] = _sylphis.moveresult_t_numtobjects_get
    if _newclass:numtobjects = property(_sylphis.moveresult_t_numtobjects_get,_sylphis.moveresult_t_numtobjects_set)
    __swig_setmethods__["tActors"] = _sylphis.moveresult_t_tActors_set
    __swig_getmethods__["tActors"] = _sylphis.moveresult_t_tActors_get
    if _newclass:tActors = property(_sylphis.moveresult_t_tActors_get,_sylphis.moveresult_t_tActors_set)
    __swig_setmethods__["end"] = _sylphis.moveresult_t_end_set
    __swig_getmethods__["end"] = _sylphis.moveresult_t_end_get
    if _newclass:end = property(_sylphis.moveresult_t_end_get,_sylphis.moveresult_t_end_set)
    __swig_setmethods__["groundtrace"] = _sylphis.moveresult_t_groundtrace_set
    __swig_getmethods__["groundtrace"] = _sylphis.moveresult_t_groundtrace_get
    if _newclass:groundtrace = property(_sylphis.moveresult_t_groundtrace_get,_sylphis.moveresult_t_groundtrace_set)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_moveresult_t,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_moveresult_t):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C moveresult_t instance at %s>" % (self.this,)

class moveresult_tPtr(moveresult_t):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = moveresult_t
_sylphis.moveresult_t_swigregister(moveresult_tPtr)

class CActorMover(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CActorMover, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CActorMover, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CActorMover,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CActorMover):
        try:
            if self.thisown: destroy(self)
        except: pass
    def playerMove(*args): return apply(_sylphis.CActorMover_playerMove,args)
    def move(*args): return apply(_sylphis.CActorMover_move,args)
    __swig_setmethods__["mGravity"] = _sylphis.CActorMover_mGravity_set
    __swig_getmethods__["mGravity"] = _sylphis.CActorMover_mGravity_get
    if _newclass:mGravity = property(_sylphis.CActorMover_mGravity_get,_sylphis.CActorMover_mGravity_set)
    __swig_setmethods__["mAirFric"] = _sylphis.CActorMover_mAirFric_set
    __swig_getmethods__["mAirFric"] = _sylphis.CActorMover_mAirFric_get
    if _newclass:mAirFric = property(_sylphis.CActorMover_mAirFric_get,_sylphis.CActorMover_mAirFric_set)
    __swig_setmethods__["mFric"] = _sylphis.CActorMover_mFric_set
    __swig_getmethods__["mFric"] = _sylphis.CActorMover_mFric_get
    if _newclass:mFric = property(_sylphis.CActorMover_mFric_get,_sylphis.CActorMover_mFric_set)
    __swig_setmethods__["mStopSpeed"] = _sylphis.CActorMover_mStopSpeed_set
    __swig_getmethods__["mStopSpeed"] = _sylphis.CActorMover_mStopSpeed_get
    if _newclass:mStopSpeed = property(_sylphis.CActorMover_mStopSpeed_get,_sylphis.CActorMover_mStopSpeed_set)
    __swig_setmethods__["mMaxSpeed"] = _sylphis.CActorMover_mMaxSpeed_set
    __swig_getmethods__["mMaxSpeed"] = _sylphis.CActorMover_mMaxSpeed_get
    if _newclass:mMaxSpeed = property(_sylphis.CActorMover_mMaxSpeed_get,_sylphis.CActorMover_mMaxSpeed_set)
    __swig_setmethods__["mAirAccel"] = _sylphis.CActorMover_mAirAccel_set
    __swig_getmethods__["mAirAccel"] = _sylphis.CActorMover_mAirAccel_get
    if _newclass:mAirAccel = property(_sylphis.CActorMover_mAirAccel_get,_sylphis.CActorMover_mAirAccel_set)
    __swig_setmethods__["mAccel"] = _sylphis.CActorMover_mAccel_set
    __swig_getmethods__["mAccel"] = _sylphis.CActorMover_mAccel_get
    if _newclass:mAccel = property(_sylphis.CActorMover_mAccel_get,_sylphis.CActorMover_mAccel_set)
    __swig_setmethods__["mStepSize"] = _sylphis.CActorMover_mStepSize_set
    __swig_getmethods__["mStepSize"] = _sylphis.CActorMover_mStepSize_get
    if _newclass:mStepSize = property(_sylphis.CActorMover_mStepSize_get,_sylphis.CActorMover_mStepSize_set)
    def __repr__(self):
        return "<C CActorMover instance at %s>" % (self.this,)

class CActorMoverPtr(CActorMover):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CActorMover
_sylphis.CActorMover_swigregister(CActorMoverPtr)

class CSoundEnv(CResourceManager):
    __swig_setmethods__ = {}
    for _s in [CResourceManager]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSoundEnv, name, value)
    __swig_getmethods__ = {}
    for _s in [CResourceManager]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CSoundEnv, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSoundEnv,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSoundEnv):
        try:
            if self.thisown: destroy(self)
        except: pass
    def startup(*args): return apply(_sylphis.CSoundEnv_startup,args)
    def shutdown(*args): return apply(_sylphis.CSoundEnv_shutdown,args)
    def registerSoundObject(*args): return apply(_sylphis.CSoundEnv_registerSoundObject,args)
    def unregisterSoundObject(*args): return apply(_sylphis.CSoundEnv_unregisterSoundObject,args)
    def setActiveListener(*args): return apply(_sylphis.CSoundEnv_setActiveListener,args)
    def getActiveListener(*args): return apply(_sylphis.CSoundEnv_getActiveListener,args)
    def getSceneManager(*args): return apply(_sylphis.CSoundEnv_getSceneManager,args)
    def registerSoundBuffer(*args): return apply(_sylphis.CSoundEnv_registerSoundBuffer,args)
    def findSoundBuffer(*args): return apply(_sylphis.CSoundEnv_findSoundBuffer,args)
    def __repr__(self):
        return "<C CSoundEnv instance at %s>" % (self.this,)

class CSoundEnvPtr(CSoundEnv):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSoundEnv
_sylphis.CSoundEnv_swigregister(CSoundEnvPtr)

class CSoundObject(CSceneObject):
    __swig_setmethods__ = {}
    for _s in [CSceneObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSoundObject, name, value)
    __swig_getmethods__ = {}
    for _s in [CSceneObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CSoundObject, name)
    __swig_getmethods__["factory"] = lambda x: _sylphis.CSoundObject_factory
    if _newclass:factory = staticmethod(_sylphis.CSoundObject_factory)
    __swig_getmethods__["cast"] = lambda x: _sylphis.CSoundObject_cast
    if _newclass:cast = staticmethod(_sylphis.CSoundObject_cast)
    def getClassName(*args): return apply(_sylphis.CSoundObject_getClassName,args)
    def getClassInfo(*args): return apply(_sylphis.CSoundObject_getClassInfo,args)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSoundObject,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSoundObject):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setVelocity(*args): return apply(_sylphis.CSoundObject_setVelocity,args)
    def getVelocity(*args): return apply(_sylphis.CSoundObject_getVelocity,args)
    def updateSound(*args): return apply(_sylphis.CSoundObject_updateSound,args)
    def updateData(*args): return apply(_sylphis.CSoundObject_updateData,args)
    def render(*args): return apply(_sylphis.CSoundObject_render,args)
    def setListener(*args): return apply(_sylphis.CSoundObject_setListener,args)
    def save(*args): return apply(_sylphis.CSoundObject_save,args)
    def load(*args): return apply(_sylphis.CSoundObject_load,args)
    def __repr__(self):
        return "<C CSoundObject instance at %s>" % (self.this,)

class CSoundObjectPtr(CSoundObject):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSoundObject
_sylphis.CSoundObject_swigregister(CSoundObjectPtr)
CSoundObject_factory = _sylphis.CSoundObject_factory

CSoundObject_cast = _sylphis.CSoundObject_cast


class CSoundListener(CSoundObject):
    __swig_setmethods__ = {}
    for _s in [CSoundObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSoundListener, name, value)
    __swig_getmethods__ = {}
    for _s in [CSoundObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CSoundListener, name)
    __swig_getmethods__["factory"] = lambda x: _sylphis.CSoundListener_factory
    if _newclass:factory = staticmethod(_sylphis.CSoundListener_factory)
    __swig_getmethods__["cast"] = lambda x: _sylphis.CSoundListener_cast
    if _newclass:cast = staticmethod(_sylphis.CSoundListener_cast)
    def getClassName(*args): return apply(_sylphis.CSoundListener_getClassName,args)
    def getClassInfo(*args): return apply(_sylphis.CSoundListener_getClassInfo,args)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSoundListener,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSoundListener):
        try:
            if self.thisown: destroy(self)
        except: pass
    def updateSound(*args): return apply(_sylphis.CSoundListener_updateSound,args)
    def save(*args): return apply(_sylphis.CSoundListener_save,args)
    def load(*args): return apply(_sylphis.CSoundListener_load,args)
    def __repr__(self):
        return "<C CSoundListener instance at %s>" % (self.this,)

class CSoundListenerPtr(CSoundListener):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSoundListener
_sylphis.CSoundListener_swigregister(CSoundListenerPtr)
CSoundListener_factory = _sylphis.CSoundListener_factory

CSoundListener_cast = _sylphis.CSoundListener_cast


class CSoundSource(CSoundObject,CWaitable):
    __swig_setmethods__ = {}
    for _s in [CSoundObject,CWaitable]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSoundSource, name, value)
    __swig_getmethods__ = {}
    for _s in [CSoundObject,CWaitable]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CSoundSource, name)
    __swig_getmethods__["factory"] = lambda x: _sylphis.CSoundSource_factory
    if _newclass:factory = staticmethod(_sylphis.CSoundSource_factory)
    __swig_getmethods__["cast"] = lambda x: _sylphis.CSoundSource_cast
    if _newclass:cast = staticmethod(_sylphis.CSoundSource_cast)
    def getClassName(*args): return apply(_sylphis.CSoundSource_getClassName,args)
    def getClassInfo(*args): return apply(_sylphis.CSoundSource_getClassInfo,args)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSoundSource,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSoundSource):
        try:
            if self.thisown: destroy(self)
        except: pass
    def setBuffer(*args): return apply(_sylphis.CSoundSource_setBuffer,args)
    SOURCE_PLAYING = _sylphis.CSoundSource_SOURCE_PLAYING
    SOURCE_STOPPED = _sylphis.CSoundSource_SOURCE_STOPPED
    SOURCE_PAUSED = _sylphis.CSoundSource_SOURCE_PAUSED
    def play(*args): return apply(_sylphis.CSoundSource_play,args)
    def pause(*args): return apply(_sylphis.CSoundSource_pause,args)
    def stop(*args): return apply(_sylphis.CSoundSource_stop,args)
    def rewind(*args): return apply(_sylphis.CSoundSource_rewind,args)
    def setState(*args): return apply(_sylphis.CSoundSource_setState,args)
    def getState(*args): return apply(_sylphis.CSoundSource_getState,args)
    def setSpatial(*args): return apply(_sylphis.CSoundSource_setSpatial,args)
    def setLooping(*args): return apply(_sylphis.CSoundSource_setLooping,args)
    def setGain(*args): return apply(_sylphis.CSoundSource_setGain,args)
    def setPitch(*args): return apply(_sylphis.CSoundSource_setPitch,args)
    def setFalloff(*args): return apply(_sylphis.CSoundSource_setFalloff,args)
    def updateSound(*args): return apply(_sylphis.CSoundSource_updateSound,args)
    def sendNewState(*args): return apply(_sylphis.CSoundSource_sendNewState,args)
    def save(*args): return apply(_sylphis.CSoundSource_save,args)
    def load(*args): return apply(_sylphis.CSoundSource_load,args)
    def __repr__(self):
        return "<C CSoundSource instance at %s>" % (self.this,)

class CSoundSourcePtr(CSoundSource):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSoundSource
_sylphis.CSoundSource_swigregister(CSoundSourcePtr)
CSoundSource_factory = _sylphis.CSoundSource_factory

CSoundSource_cast = _sylphis.CSoundSource_cast


class CSoundBuffer(CResource):
    __swig_setmethods__ = {}
    for _s in [CResource]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSoundBuffer, name, value)
    __swig_getmethods__ = {}
    for _s in [CResource]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CSoundBuffer, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CSoundBuffer,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CSoundBuffer):
        try:
            if self.thisown: destroy(self)
        except: pass
    def getBuffer(*args): return apply(_sylphis.CSoundBuffer_getBuffer,args)
    def fillBuffer(*args): return apply(_sylphis.CSoundBuffer_fillBuffer,args)
    def isStreaming(*args): return apply(_sylphis.CSoundBuffer_isStreaming,args)
    def __repr__(self):
        return "<C CSoundBuffer instance at %s>" % (self.this,)

class CSoundBufferPtr(CSoundBuffer):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSoundBuffer
_sylphis.CSoundBuffer_swigregister(CSoundBufferPtr)

class CSingletonPhysicsEnv(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CSingletonPhysicsEnv, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CSingletonPhysicsEnv, name)
    __swig_getmethods__["getSingleton"] = lambda x: _sylphis.CSingletonPhysicsEnv_getSingleton
    if _newclass:getSingleton = staticmethod(_sylphis.CSingletonPhysicsEnv_getSingleton)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C CSingletonPhysicsEnv instance at %s>" % (self.this,)

class CSingletonPhysicsEnvPtr(CSingletonPhysicsEnv):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CSingletonPhysicsEnv
_sylphis.CSingletonPhysicsEnv_swigregister(CSingletonPhysicsEnvPtr)
CSingletonPhysicsEnv_getSingleton = _sylphis.CSingletonPhysicsEnv_getSingleton


class CPhysicsEnv(CSingletonPhysicsEnv):
    __swig_setmethods__ = {}
    for _s in [CSingletonPhysicsEnv]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CPhysicsEnv, name, value)
    __swig_getmethods__ = {}
    for _s in [CSingletonPhysicsEnv]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CPhysicsEnv, name)
    def __init__(self,*args):
        self.this = apply(_sylphis.new_CPhysicsEnv,args)
        self.thisown = 1
    def __del__(self, destroy= _sylphis.delete_CPhysicsEnv):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["getSingleton"] = lambda x: _sylphis.CPhysicsEnv_getSingleton
    if _newclass:getSingleton = staticmethod(_sylphis.CPhysicsEnv_getSingleton)
    def setAutoSleep(*args): return apply(_sylphis.CPhysicsEnv_setAutoSleep,args)
    def setAutoSleepThresh(*args): return apply(_sylphis.CPhysicsEnv_setAutoSleepThresh,args)
    def setSimulationQuality(*args): return apply(_sylphis.CPhysicsEnv_setSimulationQuality,args)
    def setUpdatesPerSecond(*args): return apply(_sylphis.CPhysicsEnv_setUpdatesPerSecond,args)
    def getUpdatesPerSecond(*args): return apply(_sylphis.CPhysicsEnv_getUpdatesPerSecond,args)
    def getStepTime(*args): return apply(_sylphis.CPhysicsEnv_getStepTime,args)
    def setGravity(*args): return apply(_sylphis.CPhysicsEnv_setGravity,args)
    def getGravity(*args): return apply(_sylphis.CPhysicsEnv_getGravity,args)
    def wakeAll(*args): return apply(_sylphis.CPhysicsEnv_wakeAll,args)
    def sleepAll(*args): return apply(_sylphis.CPhysicsEnv_sleepAll,args)
    def getStaticWorldBody(*args): return apply(_sylphis.CPhysicsEnv_getStaticWorldBody,args)
    def registerRigidBody(*args): return apply(_sylphis.CPhysicsEnv_registerRigidBody,args)
    def unregisterRigidBody(*args): return apply(_sylphis.CPhysicsEnv_unregisterRigidBody,args)
    def getRigidBodies(*args): return apply(_sylphis.CPhysicsEnv_getRigidBodies,args)
    def __repr__(self):
        return "<C CPhysicsEnv instance at %s>" % (self.this,)

class CPhysicsEnvPtr(CPhysicsEnv):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CPhysicsEnv
_sylphis.CPhysicsEnv_swigregister(CPhysicsEnvPtr)
CPhysicsEnv_getSingleton = _sylphis.CPhysicsEnv_getSingleton



