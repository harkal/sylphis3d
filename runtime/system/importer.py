"""
This file is part of the "Sylphis" 3D engine

Copyright (c) 2002 - 2006 by Harry Kalogirou
All rights reserved. Email: harkal@sylphis3d.com

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

import sys, marshal, imputil, string, os.path

class VFSImporter(imputil.Importer):
    def __init__(self, path = ''):
        self.path = path

    def get_code(self, parent, modname, fqname):
        if parent:
            dir = parent.__pkgdir__ + '/'
            path = ''
        else:
            dir = ''
            path = self.path
            
        fileName = os.path.join(path, dir, modname)
        if fileName[:2] == './' or fileName[:2] == '.\\':
            fileName = '/' + fileName[2:]
        
        f = sylphis.CFileText(fileName)
        try:
            f.loadData()
        except IOError, e:
            pass

        if f.isDirectory():
            values = { '__pkgdir__' : fileName }
            ispkg = 1
            fileName = fileName + '/__init__'
        else:
            values = { }
            ispkg = 0
            
        fileName = fileName + ".py"
        f = sylphis.CFileText(fileName)
        try:
            f.loadData()
            issource = 1
        except IOError:
            fileName = fileName + "m"
            f = sylphis.CFile(fileName)
            try:
                f.loadData()
                issource = 0
            except IOError:
                return None
        
        if ispkg == 0:
            values['__file__'] = fileName
        
        if issource == 1:
            source = string.replace(f.read(), "\r\n", "\n") + '\n'
        
            try:        
                code = compile(source, fileName, "exec")
            except:
                import traceback
                traceback.print_exc()
                return None
        else:
            try:
                s = f.read()
                code = marshal.loads(s)
            except:
                return None
       
        return ispkg, code, values

    def __repr__(self):
        return 'VFSImporter(' + self.path + ')'
        
def getObject(name):
    components = name.split('.')
    name = string.join(components[:-1], '.')
    mod = __import__(name)
    for comp in components[1:]:
        mod = getattr(mod, comp)
    return mod

import __builtin__
__builtin__.getObject = getObject

if 0:
    import sylphis
    imputil.ImportManager().install()

    sys.path.insert(0, imputil.BuiltinImporter())
    sys.path.insert(1, VFSImporter())
    sys.path.insert(2, VFSImporter('core/'))
    sys.path.insert(2, VFSImporter('game/actors'))
