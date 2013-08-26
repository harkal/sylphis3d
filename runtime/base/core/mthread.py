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

import sys
import types
import stackless
import time
import bisect
import weakref
import random
import traceback

schedule = stackless.schedule
timeReal = time.time
time = time.time


channelLockStats    = {}
channels            = weakref.WeakValueDictionary({})
channelID           = 1

class __ChStat:
    sendCount = 0
    receiveCount = 0
    receiveTime = 0
    receiveMax = 0

class CChannel:
    """
    A wrapper class for the stackless.channel object.
    """
    def __init__(self, channelName = None):
        global channelID
        global channels
        global channelLockStats
        self.channelID   = channelID
        self.channelName = channelName
        if channelName not in channelLockStats:
            channelLockStats[channelName] = __ChStat()
        channelID = channelID + 1
        self.numwaiting = 0
        self.channel = stackless.channel()
        channels[self.channelID] = self

    def receive(self):
        global channelLockStats
        before = time()
        try:
            self.numwaiting = self.numwaiting + 1
            ret = self.channel.receive()
        finally:
            self.numwaiting = self.numwaiting - 1
        after  = time()
        diff   = after - before
        channelLockStats[self.channelName].receiveCount = channelLockStats[self.channelName].receiveCount + 1
        channelLockStats[self.channelName].receiveTime = channelLockStats[self.channelName].receiveTime + diff
        if diff > channelLockStats[self.channelName].receiveMax:
            channelLockStats[self.channelName].receiveMax = diff
        return ret

    def send(self,*args,**keywords):
        global channelLockStats
        channelLockStats[self.channelName].sendCount = channelLockStats[self.channelName].sendCount + 1
        return self.channel.send(*args,**keywords)

    def sendException(self,*args,**keywords):
        return self.channel.send_exception(*args,**keywords)

    def __getattr__(self,k):
        if k in ('queue',):
            return getattr(self.channel,k)
        else:
            return self.__dict__[k]

threads = weakref.WeakValueDictionary({})
taskID = 1
sleepers = []
sleepersRealtime = []
sleepersCh = {}
yielders = stackless.channel()

class CThread(stackless.tasklet):
    threadID = 0
    name = 'None'
    def __init__(self, *args):
        stackless.tasklet.__init__(self, *args)
        global taskID
        global threads
        self.threadID = taskID
        threads[taskID] = self
        taskID = taskID + 1
##    def __del__(self):
##        print "Deleting tasklet", self

def new(name, func, *args, **kw):
    """
    Create a new thread from a function or class method
    It returns a weak reference to the thread. In general
    it is expected not to hold a reference to a thread for
    more than temporary
    """
    thread = CThread(func)
    thread.name = name
    return weakref.ref(thread(*args, **kw))

def isCurrent(thread):
    return thread == stackless.getcurrent()

def kill(thread):
    """Kill a thread"""
    if thread is None:
        return
    
    if thread == stackless.getcurrent():
        return

    thread.kill()

    try:
        del sleepersCh[thread]
    except KeyError:
        pass


def sleep(sec = -1.0):
    """sleep for a number of seconds. If sec is negative, sleeps forever
       returns the time it slept... 
    """
    ch = stackless.channel()
    stime = time()
    
    if sec == 0.0:
        return 0.0
    elif sec > 0.0:
        due = stime + sec
        bisect.insort(sleepers, (due, ch))

    thread = stackless.getcurrent()
    sleepersCh[thread] = ch
    ch.receive()
    del sleepersCh[thread]
    return time() - stime

def sleepReal(sec = -1.0):
    """sleep for a number of seconds of real time.
       If sec is negative, sleeps forever
       returns the time it slept... 
    """
    ch = stackless.channel()
    stime = timeReal()
    
    if sec == 0.0:
        return 0.0
    elif sec > 0.0:
        due = stime + sec
        bisect.insort(sleepersRealtime, (due, ch))

    thread = stackless.getcurrent()
    sleepersCh[thread] = ch
    ch.receive()
    del sleepersCh[thread]
    return timeReal() - stime

def wakeup(thread):
    """Wakes up thread"""
    try:
        sleepersCh[thread].send(None)
    except KeyError:
        pass

def giveup():
    """Give up the execution timeslice"""
    yielders.receive()

def _up():
    i = bisect.bisect(sleepers, (time(), None))
    resume = sleepers[:i]
    del sleepers[:i]

    for s in resume:
        due, ch = s
        try:
            ch.send(None)
        except: # Catch the exception in the tasklet has been killed
            #print "Sleepy thread found"
            pass

def _upReal():
    i = bisect.bisect(sleepersRealtime, (timeReal(), None))
    resume = sleepersRealtime[:i]
    del sleepersRealtime[:i]

    for s in resume:
        due, ch = s
        try:
            ch.send(None)
        except: # Catch the exception in the tasklet has been killed
            #print "Sleepy thread found"
            pass

wakethreads = []

def update():
    stackless.getcurrent().block_trap = 1
    _up()
    _upReal()

    global wakethreads
    for thread in wakethreads:
        wakeup(thread)

    wakethreads = []

    global yielders
    ch = yielders
    yielders = stackless.channel()
    while ch.queue is not None:
        ch.send(None)

def printThreads():
    """Prints all the threads on the system"""
    print " TID Thread name "
    print "--------------------------------"
    for t in threads.values():
        if t != None:
            print "%d %s" % (t.threadID, t.name)
    print "--------------------------------"
    print "Thread count : %d" % len(threads)

idIndex = 0

def uniqueId():
    """Returns a unique number ID. This function is thread safe"""
    global idIndex
    tmp = stackless.atomic()
    z = idIndex
    idIndex = z + 1
    return z

def irandom(n):
    """Returns a thread safe random number generator in the range 0...n"""
    tmp = stackless.atomic()
    n = random.randrange(0, n)
    return n

class CWaitQueue:
    def __init__(self, wqName = None):
        self.name = wqName
        self.ch = stackless.channel()

    def run(self):
        self.ch.send(None)

    def add(self):
        tmp = stackless.atomic()
        thread = stackless.getcurrent()
        self.ch.receive()

    wait = add

def sleepOn(queue):
    queue.add()

def parallelize(func, *args, **kw):
    """Runs func on a separate thread"""
    thread = CThread(func)
    thread.name = 'par'
    return weakref.ref(thread(*args, **kw))
    

semaphoreLockStats       = {}
semaphores               = weakref.WeakValueDictionary({})
semaphoreID              = 1
semaphoreLockCheck       = 1

class _SemStats:
    acquireCount = 0
    waiteCount = 0
    sumWait = 0
    maxWait = 0
    sumLockTime = 0
    maxLockTime = 0

_SEM_GET_WAITING = 1
_SEM_PROBLEM = 2

class CSemaphore:
    """A clasic semaphore"""
    def __init__(self, semName = None, maxcount = 1):
        global semaphoreID
        global semaphores
        global semaphoreLockStats
        global semaphoreLockCheck

        self.semaphoreID = semaphoreID
        self.name = semName
        if semName not in semaphoreLockStats:
            semaphoreLockStats[semName] = _SemStats()

        semaphoreID = semaphoreID + 1
        self.count = maxcount
        self.waiting = stackless.channel()
        semaphores[self.semaphoreID] = self
        self.lockedWhen = None
        self.numwaiting = 0
        if semaphoreLockCheck:
            self.frame = None
        self.thread = None

    def acquire(self):
        """
        Tries to acquire the semaphore and if it can't, it blocks until it can.
        """
        global semaphoreID
        global semaphores
        global semaphoreLockStats
        global semaphoreLockCheck

        tmp = stackless.atomic()
        lockStats = semaphoreLockStats[self.name]
        if self.count == 0:
            if self.thread is stackless.getcurrent():
                raise RuntimeError("Deadlock", "Thread trying to reaquire this semaphore")
            before = time()
            try:
                self.numwaiting = self.numwaiting + 1
                while True:
                    timeout = self.waiting.receive()
                    if timeout is None:
                        break
                    elif timeout == _SEM_GET_WAITING:
                        self.callback.send((traceback.format_list(traceback.extract_stack(None,40)[:-1]),before,))
                    elif timeout == _SEM_PROBLEM:
                        print "Semaphore problem"
                        traceback.print_exc()
                    else:
                        print "CSemaphore : Unknown command"
            finally:
                self.numwaiting = self.numwaiting + 1
            now    = time()
            diff   = now - before

            lockStats.waitCount = lockStats.waitCount + 1
            lockStats.sumWait = lockStats.sumWait + diff
            if diff > lockStats.maxWait:
                lockStats.maxWait = diff
            lockStats.acquireCount = lockStats.acquireCount + 1
        else:
            self.count = self.count - 1
            lockStats.receiveMax = lockStats.receiveMax + 1
            now = time()
        self.lockedWhen = now
        if semaphoreLockCheck:
            self.frame  = sys._getframe()
        self.thread = stackless.getcurrent()

    claim = acquire

    def getWaitingStack(self):
        if self.waiting.queue:
            self.callback = stackless.channel()
            self.waiting.send(_SEM_GET_WAITING)
            ret = self.callback.receive()
            del self.callback
            return ret
        return ("",None,)

    def release(self):
        """Releases the previously aquired semaphore"""
        global semaphoreID
        global semaphores
        global semaphoreLockStats
        global semaphoreLockCheck

        tmp = stackless.atomic()
        lockStats = semaphoreLockStats[self.semaphoreName]
        diff = time() - self.lockedWhen
        self.lockedWhen = None
        lockStats.sumLockTime = lockStats.sumLockTime + diff
        if diff > lockStats.maxLockTime:
            lockStats.maxLockTime = diff
        if semaphoreLockCheck:
            self.frame = None
        self.thread = None
        if self.waiting.queue:
            self.waiting.send(None)
        else:
            self.count = self.count + 1

def _lockCheck():
    global semaphoreID
    global semaphores
    global semaphoreLockStats
    global semaphoreLockCheck
    while 1:
        sleepReal(60)
        now = timeReal()
        for i in semaphores.values():
            if (i.count == 0) and (i.waiting.queue) and ((now - i.lockedWhen) >= (5*60)):
                i.waiting.send(_SEM_PROBLEM)
                
def startSCM():
    """Starts the Semaphore Check Deamon (SCM)"""
    new("Semaphore Check Deamon", _lockCheck)

            
