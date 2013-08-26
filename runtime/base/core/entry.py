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

import sys, sylphis, sylphisextra

import importer
import loader
import gc
import configfile
import copy
import traceback
import cmdline
import os.path
from game.client import *
from actor import *
import __builtin__

gc.disable()

#gc.set_debug(gc.DEBUG_LEAK)
#gc.set_debug(gc.DEBUG_STATS)

import math

def stest():
    return (math.sin(CEngine.getFrameTime() * 10.0) + 1.0) * 0.11;

def dumpGarbage(p):
    print "GARBAGE:"
    gc.set_debug(gc.DEBUG_LEAK)
    gc.collect()
    if p:
        print "Garbage objects:", len(gc.garbage)
        
    for i in gc.garbage:
        s = str(i)
        if len(s) > 80: s = s[:80]
        print type(i),"\n ", s
    gc.set_debug(gc.DEBUG_STATS)

def addActor(ent):
    try:
        client.actors.addActor(ent)
    except Exception, e:
        print 'ERROR : addActor fail (' + str(e) + ')'
        traceback.print_exc()

def addActors(buf):
    tok = sylphis.CTokenizerNoComments(buf)
    tok.addSeparator("{")
    tok.addSeparator("}")

    try:
        while tok.nextToken():
            t = tok.getToken()
            if t != "{" :            
                e = sylphis.CTokenizerException(tok, "Expected { found " + str(tok.getToken()) + " .");
                raise NameError, str(e.getExplanation())

            ent = {}
            while 1:
                tok.nextToken()
                t = tok.getToken()
                if t == "}" :
                    break
                tok.nextToken()
                v = tok.getToken()
                ent[t] = v

            addActor(ent)

                
    except NameError, x:
        print "Exception :", x
        return 0
 
    return 1

import stackless, time
import rpcserver

def te(a):
    print "RPC :", a
    #return ('harry', 0)

def shutdown():
    global terminate
    terminate = True

__builtin__.shutdown = shutdown

class testser(sylphis.CAABoundBox):
    def getMaxX(self):
        return self.getMax().x

def _entry():
    global client
    global terminate
    terminate = False
    
    client = CPyClient()
    __builtin__.CClient = client

    CEngine.setClient(client)

    sys.stdout = PrintOut()
    sys.stderr = PrintErr()

    if 0:
        print "Shit"

        #a = sylphis.CAABoundBox()
        a = testser()
        a.setMax(sylphis.CVector3(1,2,3))
        print "MaxX:",a.getMaxX()
        
        ser = sylphis.CSerializer()
        fout = sylphis.CFile("tmpQuArK/test.save", sylphis.CFile.FILE_WRITE)
        ser.setFile(fout)

        ser.writeSerializable(a)

        del ser
        del fout
        del a

        print "Saved!"

        ser = sylphis.CSerializer()
        fin = sylphis.CFile("test.save", sylphis.CFile.FILE_READ)
        fin.loadData()
        ser.setFile(fin)

        a = sylphis.CAABoundBox.cast(ser.readSerializable())
        print a
        print a.getMax()
        

        print "Loaded!"
        
        return 1

    CEngine.startup()

    mthread.time = CEngine.getWorldTime
    mthread.startSCM()

    # Execute autoexec code
    autoexec = cmdline.getParamValue('exec')
    if autoexec != None:
        name, ext = os.path.splitext(autoexec) 
        __import__(name)

    print "Starting..."
    
    while not terminate:
        stackless.schedule()

        CEngine.update()
        #rpcs.handle_request()
        
    CEngine.shutdown()
    
    return 1

import mthread
from threading import *

class SDEThread(Thread):
    def run(self):
        import devenv.app
        from Debugger import Debugger # import from GUI thread
        devenv.app.main()

def entry():
    import sylphisexceptions
    global sde

    #sde = SDEThread()

    #global debugger
    #from Debugger import Debugger
    #debugger = Debugger()
    
    #global rpcs
    #rpcs = rpcserver.CRPCServer(('127.0.0.1', 8000))
    #rpcs.register_instance(debugger)
    
    #debugger.setIdleCall(rpcs.handle_request)
    #debugger.enable_trace()
    mthread.new("entry()", _entry)
    
    if 0:
        import profile
        print "profiling"
        p = profile.Profile()
        p.runcall(stackless.run)
        p.print_stats()
    else:
        retry = True
        while retry:
            try:
                stackless.run()
                retry = False
            except Exception, e:
                ## If there are more microthreads running
                ## don't terminate. Just print out the exception
                ## and continue scheduling
                if stackless.getruncount() <= 2:
                    retry = False
                traceback.print_exc()


def exit():
    pass

class PrintOut:
    def write(self, data):
        CEngine.con.printString(data)

class PrintErr:
    def write(self, data):
        CEngine.con.printError(data)


