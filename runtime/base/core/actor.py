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

import sylphis, mthread, weakref
import string
from copy import copy

from CSimpleActor import *
from util import updateKeepType
import gc

actorClasses = {}       

  
def registerActorClasses(dir):
    f = sylphis.CFileText(dir)
    try:
        f.loadData()
    except IOError, e:
        pass

    if not f.isDirectory():
        return

    import string, os
    fnames = string.split(f.read())
    for name in fnames:
        name = os.path.basename(name)
        name, ext = os.path.splitext(name)
        if ext == '.' or name == '__init__': continue
        if ext != '.py' and ext != '.pyc' and ext !=  '.pyo' and ext !=  '.pym' and ext !=  '.pyd': continue
        try:
            m = __import__(name, globals(), locals())
        except:
            import traceback
            traceback.print_exc()
            continue
            
        for i in m.__dict__.values():
            if not hasattr(i, '__dict__'):continue
            classname = i.__dict__.get('__classname__', None)
            if classname is None: continue
            if actorClasses.has_key(classname):continue
            actorClasses[classname] = i

    #print actorClasses
    defs, forms = getActorSpecs()
    f = open('Sylphis_autogen.qrk', 'w')
    f.write("""QQRKSRC1
{
  Description = "Actor definitions for Sylphis"
  SylphisEntities.qtx =
  {
    ToolBox = "New map items..."
    Root = "Sylphis Entities.qtxfolder"
    Sylphis Entities.qtxfolder =
    {
""")
    f.write(defs)
    f.write("""
    }
  }
""")
    f.write(forms)
    f.write('}')
    f.close()

#
# FIXME : make it work with multiple inheritance
#
def getSpecs(cl):
    array = []
    while cl != sylphis.CActor:
        if cl.__dict__.has_key('__specs__'):
            specs = cl.__dict__.get('__specs__')
            for i in specs:
                name = i['name']
                new = 1
                for j in array:
                    if j['name'] is name:
                        new = 0
                        break
                if not new:
                    continue
                array.append(i)
        cl = cl.__bases__[0]
    return array
        
def addActionSpecs(actor, specs):
    dict = []
    for i in dir(actor):
        if i[:len('Action_')] == 'Action_':
             if callable(getattr(actor, i)):
                 dict.append(i)
    for i in dict:
        name = 'target_' + i[len('Action_') - len(i):]
        doc = str(getattr(actor, '_'+i).func_doc)
        doc = string.replace(doc, '\n', '"\n$0D"')
        doc = string.replace(doc, '\t', ' ')
        n = {'name':name, 'doc':doc}
        specs.append(n)
    

def getActorSpecs():
    out = ''
    forms = 'Entity forms.qctx =\n{\n'
    names = actorClasses.keys()
    names.sort()
    for name in names:
        ac = actorClasses[name]
        if not hasattr(ac, '__specs__'):
            continue

        specs = getSpecs(ac)
        addActionSpecs(ac, specs)

        type = getattr(ac, '__actortype__', 'e')
        if type is None:
            continue
        out += ac.__classname__ + ':' + type + ' = \n'
        out += '{\n'
        if type is 'b':
            out += '  ;incl = "brush64"\n'
        if hasattr(ac, '__desc__'):
            out += '  ;desc = "' + ac.__desc__ + '"\n'

        forms += ac.__classname__ + ':form = \n'
        forms += '{\n'
        if hasattr(ac, '__desc__'):
            forms += '  Help = "' + ac.__desc__ + '"\n'
        if ac.__classname__ == 'misc_model':
            forms += '  mdl = "[model]"\n'
        elif ac.__classname__ == 'info_player_start':
            forms += "  bbox = '-20 -20 -90 20 20 5'\n"
        for spec in specs:
            name = spec['name']
            out += '  ' + name + ' = "' + str(getattr(ac, name, '')) + '"\n'
            ## ----
            forms += '  ' + name + ': =\n'
            forms += '  {\n'
            forms += '    Txt = "&"\n'
            forms += '    Hint = "' + spec.get('doc', '') + '"\n'
            if spec.has_key('type'):
                type = spec['type']
                if type == 'bool':
                    forms += '    Typ = "X1"\n'
                elif type == 'color':
                    forms += '    Typ = "L"\n'
                elif type == 'file':
                    pass
            elif spec.has_key('select'):
                select = spec['select']
                forms += '    Typ = "C"\n'
                forms += '    items = '
                for i in select.values():
                    forms += '"' + i + '"$0D'
                forms = forms[:-3]
                forms += '\n'
                forms += '    values = '
                for i in select.keys():
                    forms += '"' + str(i) + '"$0D'
                forms = forms[:-3]
                forms += '\n'
            forms += '  }\n'

        out += '}\n\n'
        forms += '}\n\n'

    forms += '}\n\n'
    return out, forms
    
def sendEvent(target, *args):
    t = target()
    if t != None: t.eventQueue.put(*args)

class CActors:
    def __init__(self, container):
        self.mCont = container
        self.actors = []
        self.updaters = []
        self.namedActors = {}
        self.queuedEvents = []
        self.watchers = []
        registerActorClasses('game/actors')

    def registerWatcher(self, func):
        self.watchers.append(func)
        
    def signalWatchers(self):
        for func in self.watchers:
            func()

    def printActors(self):
        i = 0
        print " Actors (Id / Name / Class) "
        print "-----------------------------------------"
        for a in self.actors:
            print "%d / %s  / %s" % (i, a.__dict__.get('name','none'), a.__class__)
            i += 1

    def getActors(self):
        return self.actors

    def addActor(self, ent):
        """
        The actor is not started, so a call to
        the start() method might be required.
        """
        cl = actorClasses.get(ent['classname'], CSimpleActor)               

        try:
            script = ent['script']
            a = {}
            a['CBaseActor'] = cl
            import loader
            loader.execute(script, a, a)
            #for k in a.keys(): sylphis._object.__setattr__(newcl, k, a[k])
            cl = a['CScript']
        except KeyError:
            pass

        e = cl()
        # load the specifics
        try:
            updateKeepType(e, ent)
        except Exception, ex:
            raise Exception, ent.get('name', 'unnamed') + ' : ' + str(ex)

        e.origin.swapHand()
        
        e.angles = sylphis.CQuaternion()
        try:
            o = string.split(ent['angles'])
            pi = 3.14159
            angles = sylphis.CVector3(-pi * (float(o[0])) / 180, -pi * (float(o[1])+0.0) / 180, pi * float(o[2]) / 180)

            
            m = sylphis.CMatrix4()
            m.rotate(angles)
            
            o = sylphis.CQuaternion()
            o.fromRotationMatrix(m)
            print "Q",o
            e.angles = o
            
            #axis = sylphis.CVector3.UNIT_Y
            #e.angles.fromAngleAxis(axis, angles.y)
            #axis = sylphis.CVector3.UNIT_Z
            #q = sylphis.CQuaternion(e.angles)
            #q.invert()
            #q.fromAngleAxis(q * axis, angles.x)
            #e.angles = e.angles * q
        except KeyError, x:
            pass
            
        try:
            o = string.split(ent['orientation'])
            e.orientation = sylphis.CQuaternion(float(o[0]),float(o[1]),float(o[2]),float(o[3]))
        except KeyError, x:
            pass
                        
        e.join(ent)
        self.append(e)

        if ent['classname'] == 'info_player_start':
            self.mCont.player = e

        self.signalWatchers()

        return e

    def spawn(self, ent):
        """
        Spawns a new actor.
        """
        cl = actorClasses.get(ent['classname'], CSimpleActor)               
        a = cl()
        # load the specifics
        a.__dict__.update(ent)
        a.join(ent)
        self.append(a)
        a.start()
        return a
            
    def append(self, a):
        self.actors.append(a)
        if hasattr(a, 'update'):
            if a.getUpdatePriority() == 0:
                self.updaters.insert(0, a)
            else:
                self.updaters.append(a)
            
        try:
            self.namedActors[a.name] = a
        except AttributeError:
            pass
        
        self.signalWatchers()

    def getActorByName(self, name):
        return self.namedActors.get(name, None)

    def remove(self, a):    
        mthread.parallelize(self.remove_p, a)

    def remove_p(self, a):
        #print "Removing :", a, a.name, a.__class__
        if not(a in self.actors):
            return
        
        a.part()
        self.actors.remove(a)
        if hasattr(a, 'name'):
            del self.namedActors[a.name]
        if hasattr(a, 'update'):
            self.updaters.remove(a)
        self.signalWatchers()

    def removeAll(self):
        actors = copy(self.actors)
        for a in actors:
            self.remove(a)

    def start(self):
        for a in self.actors:
            #print "Starting", a.__class__
            a.start()

    def update(self, timeDelta):
        for a in self.updaters:
            a.update(timeDelta)
        
