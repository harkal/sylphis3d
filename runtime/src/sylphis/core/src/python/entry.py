
import sys, sylphis
#import ConfigParser
from client import *
from simpleactor import *
from player import *
from light import *
from misc_model import *
from actors import *

def addActor(ent):

    e = CSimpleActor(client)
   
    if ent['classname'] == 'light':
        print 'Adding light'
        e = CLight(client)
    elif ent['classname'] == 'misc_model':
        print 'Adding misc_model'
        e = CMiscModel(client)
    elif ent['classname'] == 'info_player_deathmatch':
        print 'Adding player'
        e = CPlayer(client)
        client.player = e
    else:
        return

    e.join(ent)
    client.actors.append(e)


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

def entry():
    global engine
    global client
    global server

#    configFile = ConfigParser.ConfigParser()
#    configFile.read('sylphis.cfg')

    client = CClient()
    server = sylphis.CServer()
    engine = sylphis.CEngine()
    engine.setClient(client)
    engine.setServer(server)

    print "Engine startup"
    engine.startup()

    print "Starting..."    
    while engine.isRunning():
        engine.update()
        
    return 1

class PrintOut:
    def write(self, data):
        engine.con.printString(data)

class PrintErr:
    def write(self, data):
        engine.con.printString(data)

sys.stdout=PrintOut();
sys.stderr=PrintErr();
