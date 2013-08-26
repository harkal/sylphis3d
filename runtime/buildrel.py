#!/usr/bin/python -O

import os, sys, string, marshal
from shutil import copy, copytree

delpyo = 0

def timestamp(name):
	try:
		s = os.stat(name)
	except OSError:
		return None
	return long(s[8])

def comp(name):
	try:
		dirs = os.listdir(name)
		for item in dirs:
			fullname = os.path.join(name, item)
			comp(fullname)
	except OSError:
		ext = name[-3:]
		if delpyo == 1:
			if ext == 'pym':
				print "Removing ", name
				os.remove(name)
			return
				
		if(ext != ".py"):
			return
			
		outname = name[:-3] + ".pym"
		intime = timestamp(name)
		outtime = timestamp(outname)
		
		if outtime is not None and outtime >= intime:
			return
		
		base = os.path.basename(name)

		print "Compiling", name, "to", outname

		f = open(name, "r")
		source = string.replace(f.read(), "\r\n", "\n") + '\n'

		try:
			code = compile(source, base, "exec")
			of = open(outname, "wb")
			marshal.dump(code, of)
		except SyntaxError, se:
			er = "Syntax error at line no" + str(se.lineno) + ":"
			print er + se.text,
			offset = se.offset + len(er) - 1
			print " "*offset + "^"

outDir = 'release_build'
pyExt = 'pym'

stdlibs = [
'types.py',
'os.py',
'__future__.py',
'ntpath.py',
'stat.py',
'UserDict.py',
'copy_reg.py',

'bisect.py',
'weakref.py',
'random.py',
'copy.py',
're.py',
'sre.py',
'sre_compile.py',
'_sre.pyd',
'sre_constants.py',
'sre_parse.py',
'SimpleXMLRPCServer.py',
'xmlrpclib.py',
'xml/__init__.py',
'xml/parsers/__init__.py',
'xml/parsers/expat.py',
'SocketServer.py',
'socket.py',
'_socket.pyd',
'BaseHTTPServer.py',
'mimetools.py',
'rfc822.py',
'tempfile.py',
'threading.py',
'traceback.py',
'linecache.py',
'StringIO.py',
'atexit.py',
'bdb.py',
'profile.py',
'wxPython/__init__.py',
'wxPython/__version__.py',
'wxPython/wxc.pyd',
'wxPython/wx.py',
'wxPython/misc.py',
'wxPython/misc2.py',
'wxPython/windows.py',
'wxPython/gdi.py',
'wxPython/fonts.py',
'wxPython/clip_dnd.py',
'wxPython/events.py',
'wxPython/streams.py',
'wxPython/utils.py',
'wxPython/mdi.py',
'wxPython/frames.py',
'wxPython/stattool.py',
'wxPython/controls.py',
'wxPython/controls2.py',
'wxPython/windows2.py',
'wxPython/cmndlgs.py',
'wxPython/windows3.py',
'wxPython/image.py',
'wxPython/printfw.py',
'wxPython/sizers.py',
'wxPython/filesys.py',
'wxPython/stc.py',
'wxPython/stc_.py',
'wxPython/stc_c.pyd',
'wxPython/lib/__init__.py',
'wxPython/lib/anchors.py',
'wxPython/grid.py',
'wxPython/gridc.pyd',
'wxPython/wxmsw24h.dll',
'keyword.py',
'pprint.py',
'wxPython/help.py',
'wxPython/helpc.pyd',
'urllib.py',
'nturl2path.py',
'pickle.py',

'string.py',
'imputil.py',
'inspect.py',
'dis.py',
'opcode.py',
'token.py',
'tokenize.py',
'base64.py'
]

def createDir(dir):
    global outDir
    md = os.path.join(outDir, dir)
    print 'Creating directory:', md
    try:
        os.mkdir(md)
    except:
        pass

def install(file):
    global outDir
    outFile = os.path.join(outDir, file)
    print 'Copying', file, 'to', outFile
    copy(file, outFile)

def installPy(file):
    global pyExt
    install(file + '.' + pyExt)

def installPys(dir, files):
    for i in files:
        installPy(os.path.join(dir, i))

def installAllPys(dir):
    global pyExt
    gext = '.' + pyExt
    files = os.listdir(dir)
    goodFiles = []
    for file in files:
        if os.path.isfile(os.path.join(dir, file)) == 0:
            continue
        (name, ext) = os.path.splitext(file)
        if ext == gext:
            goodFiles.append(name)

    installPys(dir, goodFiles)
    

def installDir(dir, files):
    createDir(dir)
    for i in files:
        install(os.path.join(dir, i))

def doit():
    global outDir
    global pyExt
    if len(sys.argv) > 1:
        outDir = sys.argv[1]

    if len(sys.argv) > 2:
        pyExt = '.' + sys.argv[2]

    createDir('')
    install('sylphis.exe')
    install('pythoncore.dll')
    install('readme.txt')
    install('license.txt')
    install('sylphis.iss')

    createDir('tools')
    install('tools/mc.exe')

    createDir('system')
    install('system/importer.pyc')
    install('system/sylphis.pyc')
    install('system/sylphisextra.pyc')

    createDir('stdlib')
    createDir('stdlib/wxPython')
    createDir('stdlib/wxPython/lib')
    createDir('stdlib/xml')
    createDir('stdlib/xml/parsers')
    installDir('stdlib', stdlibs)
    
    createDir('base')
    install('base/system.cfg')
    install('base/benchmark.py')    

    createDir('base/core')
    installPys('base/core', [ 'cmdline', 'configfile', 'actor', 'CSimpleActor', 'entry', 'loader',
                              'mthread', 'util', 'rpcserver', 'rpcproxy',
                              'sylphisexceptions' ])

    createDir('base/core/Debugger')
    installPys('base/core/Debugger', ['__init__', 'Breakpoint', 'Debugger',
                                     'DebuggerProxy'])

    
#    createDir('base/devenv')
#    install('base/devenv/stc-styles.rc.cfg')
#    installPys('base/devenv', ['__init__', 'AboutDialog', 'app', 'MDIChild',
#                               'MDISourceFrame', 'stcprops', 'STCStyleEditor',
#                               'StyledTextCtrls', 'WatchPanel', 'wxMainFrame'])
#
#    createDir('base/devenv/Images')
#    createDir('base/devenv/Images/Debug')
#    install('base/devenv/Images/Debug/Debug.png')
#    install('base/devenv/Images/Debug/Pause.png')
#    install('base/devenv/Images/Debug/Stop.png')
#    install('base/devenv/Images/Debug/Step.png')
#    install('base/devenv/Images/Debug/Over.png')
#    install('base/devenv/Images/Debug/Out.png')
#
    createDir('base/ui')
    installPys('base/ui', ['__init__', 'console', 'menus', 'about', 'startdialog',
                           'variablecontrol', 'postproc'])
    
    createDir('base/pyui')
    installPys('base/pyui', ['__init__', 'base', 'colors', 'core', 'desktop',
                             'dialogs', 'entry', 'grid', 'layouts', 'locals',
                             'renderer3d', 'rendererBase', 'sheet', 'themeBase',
                             'tree', 'viewer', 'widgets'])

    createDir('base/pyui/renderers')
    installPys('base/pyui/renderers', ['__init__', 'sylphisRenderer'])

    createDir('base/pyui/themes')
    installPys('base/pyui/themes', ['__init__', 'comic', 'future', 'green', 'win2k'])

    createDir('base/lights')
    install('base/lights/noatten.png')
    install('base/lights/square.png')
    install('base/lights/square_2D.png')
    install('base/lights/lowatten.png')
    install('base/lights/lowatten_2D.png')

    createDir('base/fonts')
    install('base/fonts/times.ttf')
    install('base/fonts/casual.font')

    createDir('base/scripts')
    install('base/scripts/system.shader')
    
    createDir('base/materials')
    install('base/materials/makeshaders.py')
    install('base/materials/system.mat')
    install('base/materials/fonts.mat')
    install('base/materials/gui.mat')
    install('base/materials/models.mat')
    install('base/materials/test.mat')

    createDir('base/shaders')
    install('base/shaders/__init__.py')
    install('base/shaders/sky.py')
    install('base/shaders/blur.afp')
    install('base/shaders/blur_v.avp')
    install('base/shaders/blur_h.avp')
    install('base/shaders/oldfilm.vp')
    install('base/shaders/oldfilm.fp')
    install('base/shaders/oldfilmglare.vp')
    install('base/shaders/oldfilmglare.fp')
    install('base/shaders/parallax.vp')
    install('base/shaders/parallax.fp')
    install('base/shaders/refract.vp')
    install('base/shaders/refract.fp')
    install('base/shaders/speed.vp')
    install('base/shaders/speed.fp')
    install('base/shaders/motionblur.vp')
    install('base/shaders/motionblur.fp')
    install('base/shaders/downsample.vp')
    install('base/shaders/downsample.fp')
    install('base/shaders/downsamplerect.fp')
    install('base/shaders/ambient.vp')
    install('base/shaders/ambient.fp')
    install('base/shaders/convtofp.fp')
    install('base/shaders/blur.vp')
    install('base/shaders/blur_h.fp')
    install('base/shaders/blur_v.fp')
    install('base/shaders/exposurecontrol.fp')
    install('base/shaders/luma_init.fp')
    install('base/shaders/bloom.fp')
    install('base/shaders/tonemap.fp')
    install('base/shaders/terrain_ambient.vp')
    install('base/shaders/terrain_ambient.fp')
    install('base/shaders/terrain.vp')
    install('base/shaders/terrain.fp')
    install('base/shaders/sky.vp')
    install('base/shaders/sky.fp')
    
    
    createDir('base/textures')
    install('base/textures/sylphis.png')
    install('base/textures/crosshair.png')
    install('base/textures/motion.png')
    install('base/textures/dust.jpg')
    install('base/textures/128x128.png')
    install('base/textures/256x128.png')

    createDir('base/textures/common')
    install('base/textures/common/caulk.jpg')
    install('base/textures/common/portal.jpg')
    install('base/textures/common/trigger.jpg')

    createDir('base/textures/system')
    install('base/textures/system/sylphis.png')
    install('base/textures/system/crosshair.png')
    install('base/textures/system/portal.jpg')
    install('base/textures/system/trigger.jpg')
    install('base/textures/system/pointer.png')

    createDir('base/game')
    installPys('base/game', ['__init__', 'client', 'hudbase'])

    createDir('base/game/actors')
    installAllPys('base/game/actors')

def main():
    global delpyo
    
    if pyExt == 'pym':
        comp('base')

    doit()

    if pyExt == 'pym':
        delpyo = 1
        comp('base')

    raw_input("Press enter")

if __name__ == '__main__':
    main()

