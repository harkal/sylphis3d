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
import traceback
import string
import mthread
import weakref
import gc

def _proxyMethod(method):
    def _method(self, other = None):
        thlist = method.func_name + '_threads'
        self.sendSignal(method.func_name)
        
        thread = mthread.CThread(method)
        thread.name = "-"
        id = thread.threadID

        if hasattr(self, thlist):
            tl = getattr(self, thlist)
            tl[id] = thread
        else:
            tl = weakref.WeakValueDictionary({})
            tl[id] = thread
            setattr(self, thlist, tl)

        self.threads[id] = thread
        
        thread(self, other)

    return _method

actionPrefix = "Action_"
class ActionsMetaclass(type):
    def __new__(self, classname, bases, classdict):
        # Rebind the signal methods
        for attr, item in classdict.items():
            if callable(item) and attr[:len(actionPrefix)] == actionPrefix:
                classdict['_'+attr] = item
                classdict[attr] = _proxyMethod(item)

        # Create the class
        cls = type.__new__(self, classname, bases, classdict)
        return cls
    

class CSimpleActor(sylphis.CActor):
    """
    This is the base actor class for all actors defined in Python.
    """
    __metaclass__ = ActionsMetaclass
    __classname__  = 'misc_simple_actor'
    __desc__ = 'Base class of all actors'
    __actortype__ = None
    __specs__ = [
        {'name':'name', 'doc':'The name of the actor'},
        {'name':'origin', 'doc':'The actors origin'}
    ]
    origin = sylphis.CVector3(0,0,0)
    name = ''
    
    STATE_RUNNING = 0
    STATE_PARTING = 1
    STATE_STARTING = 2
    __state__ = STATE_STARTING
    EVENT_TRIGGER = 1
    EVENT_COLLISION = 11
    def __init__(self, *args):
        sylphis.CActor.__init__(self, *args)
        self.self = self
        self.lock = mthread.CSemaphore()
        self.threads = weakref.WeakValueDictionary({})
        self.mScene = CEngine.getClient().getSceneManager()
        self.mCont = CClient

    def parallelize(self, func, *args, **kw):
        thread = mthread.CThread(func)
        thread.name = "-"
        id = thread.threadID
        self.threads[id] = thread
        thread(*args, **kw)
        
    def addSignal(self, signame, target, targetsig = ''):
        s = '_Action_'+signame+'_targets'
        if hasattr(self, s):
            getattr(self, s).append((target, actionPrefix + targetsig))
        else:
            setattr(self, s, [(target, actionPrefix + targetsig)])
            
    def sendSignal(self, signal):
        tname = '_' + signal + '_targets'
        if not hasattr(self, tname):
            return
        targets = getattr(self, tname)
        for actor, sig in targets:
            print "Signaling", actor, "with signal", sig
            if not isinstance(actor, str):
                actor(self)
                continue
            target = self.mCont.actors.getActorByName(actor)
            if not target:
                continue
            if hasattr(target, sig):
                m = getattr(target, sig)
                m(self)

    def killSignalThreads(self, signame):
        thlist = actionPrefix + signame + '_threads'
        if not hasattr(self, thlist):
            return
        thlist = getattr(self, thlist)
        for id, thread in thlist.items():
            if mthread.isCurrent(thread):
                print "Killin self"
            try:
                mthread.kill(thread)
            except RuntimeError:
                pass

    def sendEvent(self, target, *args):
        """ Send an event to another actor.
        """
        #print "send event to ", target.__class__, " from ", self.__class__," :", args
        try:
            target.handleEvent(*args)
        except:
            traceback.print_exc()
        
    def sendEventByName(self, targetName, *args):
        """Send an event to another actor named 'targetName'."""
        target = self.mCont.actors.getActorByName(targetName)
        if target is None:
            print "WARNING : No actor named '%s' to send event to..." % targetName
            return
        self.sendEvent(target, *args)

    def handleEvent(self, *args):
        """ Handles an event. This runs on the self.eventThread
            so proper locking must be done when accessing self
            members.

            It is a good practise never to sleep on this thread.
            When you need to do things that involve waiting and
            sleeping you are expected to start a new thread for
            that task.
        """
        print "CSimpleActor::handleEvent"
        pass
        
    def join(self, ent):
        """
        This method is called just after the constructor to allow the actor
        to initialize its state, usualy based on the 'specific' values entered
        in the map editor. You should overide this, but always call the father.
        """
        #print "Joining", self.__class__
        self.defs = ent

        if self.name == '':
            self.name = str(self)

        try:
            self.parent = ent['parent']
        except KeyError, x:
            pass

        for name, value in ent.items():
            if name[:len('target_')] != 'target_':
                continue
            name = name[len('target_') - len(name):]
            print "Binding signal", name, value
            targets = string.split(value)
            for t in targets:
                actor, sig = string.split(t, '.')
                self.addSignal(name, actor, sig)

    def start(self):
        """
        This is always called after join() and right before the actor is about
        to enter the world. The basic setup that happens here is to reparent the
        main actor's node to the parent that is specified by the 'parent' specific.
        
        You should overide this, but always call the father.
        """
        parent = None
        bone = None
        if hasattr(self, 'parent'):
            name = string.split(self.parent, '.')
            parent = self.mCont.actors.getActorByName(name[0])
            if len(name) is 2:
                bone = name[1]
            print name[0]
        if parent is None:
            parentNode = self.mScene.getRootSceneNode()
        else:
            parentNode = parent.node
        if hasattr(self, 'node'):
            n = self.node
            tr = parentNode.getPosition()
            #print "Parent :", tr.x, tr.y, tr.z
            n.move(-tr)
            if bone != None:
                try:
                    parentNode.addChildAtBone(bone, n)
                except AttributeError:
                    n.move(tr) # Move back
                    self.mScene.getRootSceneNode().addChild(n)
                    return
            else:
                try:
                    parentNode.addChild(n)
                except AttributeError:
                    n.move(tr) # Move back
                    self.mScene.getRootSceneNode().addChild(n)

        name = self.name
#        if self.__class__.handleEvent != CSimpleActor.handleEvent:
#            if hasattr(self, 'eventThread'):
#                raise "Double start"
#            self.eventThread = mthread.new(name + '(event loop)', self.eventLoop)
        if hasattr(self, 'run'):
            self.thread = mthread.new(name, self.run)

        self.__state__ = self.STATE_RUNNING

        #print "Starting actor", self, "at", self.origin
    

    def part(self):
        """
        Called when the actor is about to be removed from the world.
        """
        #print "Parting ", self.__class__
        self.__state__ = self.STATE_PARTING
        sm = self.mCont.getSceneManager()
        if hasattr(self, 'thread'):
            #print "removeing thread"
            mthread.kill(self.thread())
            #del self.thread
        if hasattr(self, 'body'):
            #print "removeing body"
            self.body.setListener(None)
            CPhysicsEnv.unregisterRigidBody(self.body)
            #import gc
            #print gc.get_referrers(self.body)
            self.body.self = None
            del self.body
        if hasattr(self, 'node'):
            #print "removeing node"
            sm.getRootSceneNode().removeChild(self.node)
            del self.node

        for id, thread in self.threads.items():
            mthread.kill(thread)

        
        #print "Parting end"
        

    ##
    ## Utility methods
    ##
    def sleep(self, sec = -1.0):
        """
        Utility function. Sleeps for 'sec' seconds. If 'sec' is not provided
        it sleeps forever.
        
        The method returns the second thats it actualy slept.
        """
        time = CEngine.getWorldTime()
        mthread.sleep(sec)
        return CEngine.getWorldTime() - time

    def wakeup(self, thread = None):
        """
        Wakes up a thread that is sleeping. If 'thread' is not provided
        it tries to wake the main actors thread.
        """
        if thread is None:
            thread = self.thread
        mthread.wakeup(thread())

    def remove(self):
        """
        Removes itself from the world.
        """
        self.mCont.actors.remove(self)
        #self.eventQueue.purge()  # This is here because we might have events
                                # queued that we will never handle

    def addActor(self, specifics):
        """
        Adds an actor in the world, gith the given 'specifics'.
        The new actor is returned by the method.
        The actor is not started.
        """
        return self.mCont.actors.addActor(specifics)
    
    def spawn(self, specifics):
        """
        Spawns a new actor in the world, with the given 'specifics'.
        The new actor is returned by the method.
        The actor is started.
        """
        return self.mCont.actors.spawn(specifics)

    def getUpdatePriority(self):
        """
        Returns the update priority. Overide this method to return
        the priority the actor needs. Actors with priorities closer
        to 0(ZERO) are updated first
        """
        return 100

#    def __del__(self):
#        print "Deleting :", self.__class__

        
            
        
