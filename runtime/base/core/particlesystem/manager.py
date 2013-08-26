"""
This file is part of the "Sylphis" 3D engine

Copyright (c) 2002 - 2006 by Harry Kalogirou
All rights reserved. Email: harkal@sylphis3d.com

<<--LICENSE-->>

"""

import sylphis
import xml.dom.minidom
import loader

def registerParticleSystem(name):
    l = loader.CPSParser()
        
    f = sylphis.CFileText(name)
    f.loadData()
    doc = f.read()
        
    return l.parseString(doc)
    
    