/* File : sylphis.i */
%module sylphis
%include "stl.i"
%include "std_string.i"
%include "exception.i"

%{
#include <platform/types.h>
#include <serialize.h>
#include <serializer.h>
#include <refobject.h>
#include <pointer.h>
#include <exception.h>
#include <singleton.h>

#include <timer.h>
#include <vector.h>
#include <plane.h>
#include <quaternion.h>
#include <matrix.h>
#include <matrix3.h>
#include <bound.h>
#include <boundbox.h>

#include <console.h>
#include <python/pyconsole.h>
#include <tokenizer.h>
#include <inputdevice.h>
#include <engine.h>
#include <client.h>
#include <vfs.h>

#include <resource.h>
#include <resourcemanager.h>
#include <texture.h>
#include <material.h>
#include <materialparser.h>
#include <renderer.h>
#include <renderingview.h>
#include <renderable.h>
#include <model.h>
#include <md3model.h>
#include <smfmodel.h>
#include <cmapscene/brushmodel.h>
#include <skelmodel.h>
#include <skeleton.h>
#include <skeletoninstance.h>
#include <bonetrack.h>
#include <skelanimation.h>
#include <animationblender.h>
#include <modelmanager.h>
#include <font.h>
#include <texturefont.h>

#include <trace.h>
#include <interpolator.h>
#include <splineinterpolator.h>
#include <scenemanager.h>
#include <sceneobject.h>
#include <scenenode.h>
#include <light.h>
#include <camera.h>
#include <renderablemultimesh.h>
#include <scenemodel.h>
#include <scenemd3.h>
#include <particlesystem.h>


#include <eventlistener.h>
#include <actor.h>
#include <controler.h>
#include <contents.h>
#include <rigidbody.h>
#include <rigidsphere.h>
#include <rigidbox.h>
#include <rigidcyl.h>
#include <rigidcapsule.h>
#include <rigidcomplex.h>
#include <rigidbodystatic.h>
#include <joint.h>
#include <actormover.h>

#include <sound/soundobject.h>
#include <sound/soundlistener.h>
#include <sound/soundsource.h>
#include <sound/soundbuffer.h>
#include <sound/soundenv.h>

#include <physicsenvironment.h>
%}

/* Let's just grab the original header file here */
%include "hklib/include/platform/types.h"
%include "hklib/include/serialize.h"
%include "hklib/include/serializer.h"
%include "hklib/include/refobject.h"
%include "hklib/include/pointer.h"
%include "hklib/include/exception.h"
%include "hklib/include/console.h"
%include "hklib/include/singleton.h"

%include "core/include/timer.h"
%include "core/include/vector.h"
%include "core/include/plane.h"
%include "core/include/quaternion.h"
%include "core/include/matrix.h"
%include "core/include/matrix3.h"
%include "core/include/bound.h"
%include "core/include/boundbox.h"

%include "core/include/tokenizer.h"
%include "core/include/inputdevice.h"
%template(CSingletonEngine) CSingleton<CEngine>;
%include "core/include/engine.h"

%include "core/include/client.h"

%include "core/src/python/waitable.h"
%include "core/src/python/pyconsole.h"

%exception CFile::loadData {
    try {
        $action
    } catch (CException& e) {
        SWIG_exception(SWIG_IOError, (char * const)e.what());
    }
}

%typemap(argout) (char **d, int *len) {
    PyObject *o, *o2, *o3;
    o = PyString_FromStringAndSize(*$1, *$2);
    $result = o;
}

%typemap(ignore) char **d(char *temp1) {
    $1 = &temp1;
}

%typemap(ignore) int *len(int temp1) {
    $1 = &temp1;
}

%include "hklib/include/vfs.h"

%include "core/include/resource.h"
%include "core/include/resourcemanager.h"
%include "core/include/texture.h"
%include "core/include/material.h"
%include "core/include/materialparser.h"
%include "core/include/renderer.h"
%include "core/include/renderingview.h"
%include "core/include/renderable.h"
%include "core/include/model.h"
%include "core/include/md3model.h"
%include "core/include/smfmodel.h"
%include "core/src/cmapscene/brushmodel.h"
%include "core/include/skelmodel.h"
%include "core/include/skeleton.h"
%include "core/include/skeletoninstance.h"
%include "core/include/bonetrack.h"
%include "core/include/skelanimation.h"
%include "core/include/animationblender.h"
%include "core/include/modelmanager.h"

/* This makes the
void CFont::stringSize(const std::string &str, float *w, float *h)
callable from python like this : (w,h) = f.stringSize("test");
*/

%typemap(argout) float * {
    PyObject *o, *o2, *o3;
    o = PyFloat_FromDouble(*$1);
    if ((!$result) || ($result == Py_None)) {
        $result = o;
    } else {
        if (!PyTuple_Check($result)) {
            PyObject *o2 = $result;
            $result = PyTuple_New(1);
            PyTuple_SetItem($result,0,o2);
        }
        o3 = PyTuple_New(1);
        PyTuple_SetItem(o3,0,o);
        o2 = $result;
        $result = PySequence_Concat(o2,o3);
        Py_DECREF(o2);
        Py_DECREF(o3);
    }
}

%typemap(ignore) float *(float temp) {
    $1 = &temp;
}

%include "core/include/font.h"
%include "core/include/texturefont.h"

%include "core/include/trace.h"
%include "core/include/scenemanager.h"

%exception CCMapScene::loadWorldMap {
    try {
        $action
    } catch (CException& e) {
        SWIG_exception(SWIG_IOError, (char * const)e.what());
    }
}

%include "core/include/interpolator.h"
%template(CInterpolatorF32) CInterpolator<F32>;
%template(CInterpolatorVector3) CInterpolator<CVector3>;
%include "core/include/splineinterpolator.h"
%include "core/include/sceneobject.h"
%include "core/include/scenenode.h"
%include "core/include/light.h"
%include "core/include/camera.h"
%include "core/include/renderablemultimesh.h"
%include "core/include/scenemodel.h"
%include "core/include/scenemd3.h"
%include "core/include/particlesystem.h"

%include "core/include/eventlistener.h"
%include "core/include/actor.h"
%include "core/include/controler.h"

//
// This classes have a "self" member that points back to the python
// proxy object. This 'shadows' set that member at proxy initialization
//
%feature("shadow") CRigidBody::CRigidBody {
def __init__(self,*args):
    self.this = apply(_sylphis.new_CRigidBody,args)
    self.thisown = 1
    self.self = self
}
%feature("shadow") CRigidBody::CRigidSphere {
def __init__(self,*args):
    self.this = apply(_sylphis.new_CRigidSphere,args)
    self.thisown = 1
    self.self = self
}
%feature("shadow") CRigidBody::CRigidBox {
def __init__(self,*args):
    self.this = apply(_sylphis.new_CRigidBox,args)
    self.thisown = 1
    self.self = self
}
%feature("shadow") CRigidBody::CRigidCylinder {
def __init__(self,*args):
    self.this = apply(_sylphis.new_CRigidCylinder,args)
    self.thisown = 1
    self.self = self
}
%feature("shadow") CRigidBody::CRigidCapsule {
def __init__(self,*args):
    self.this = apply(_sylphis.new_CRigidCapsule,args)
    self.thisown = 1
    self.self = self
}
%feature("shadow") CRigidBody::CRigidComplex {
def __init__(self,*args):
    self.this = apply(_sylphis.new_CRigidComplex,args)
    self.thisown = 1
    self.self = self
}
%feature("shadow") CRigidBody::CRigidBodyStatic {
def __init__(self,*args):
    self.this = apply(_sylphis.new_CRigidBodyStatic,args)
    self.thisown = 1
    self.self = self
}

%include "core/include/contents.h"
%include "core/include/rigidbody.h"
%include "core/include/rigidsphere.h"
%include "core/include/rigidbox.h"
%include "core/include/rigidcyl.h"
%include "core/include/rigidcapsule.h"
%include "core/include/rigidcomplex.h"
%include "core/include/rigidbodystatic.h"
%include "core/include/joint.h"
%include "core/include/actormover.h"

%include "core/src/sound/soundenv.h"
%include "core/src/sound/soundobject.h"
%include "core/src/sound/soundlistener.h"
%include "core/src/sound/soundsource.h"
%include "core/src/sound/soundbuffer.h"

%template(CSingletonPhysicsEnv) CSingleton<CPhysicsEnv>;
%include "core/include/physicsenvironment.h"
