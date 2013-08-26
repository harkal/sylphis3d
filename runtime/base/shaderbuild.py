#!/usr/bin/python
"""

Run from base/ and give tha relative path
to the images directory.

Example:
c:\sylphis\base>python shaderbuild.py textures/new > scripts/new.shader

"""
import os, string, sys

baseDir = sys.argv[1]

files = os.listdir(baseDir)

chopedFiles = []
for file in files:
    fullname = os.path.join(baseDir, file)
    ext = file[-3:]
    if ext != 'jpg' and ext == 'png' and ext != 'tga': continue
    chopedFiles.append(string.replace(fullname[:-4], '\\', '/'))

for file in chopedFiles:
    bump = None
    gloss = None
    if file + '_bump' in chopedFiles: bump = file + '_bump'
    if file + '_gloss' in chopedFiles: gloss = file + '_gloss'

    if bump is None and gloss is None:
        if file[-4:] == 'bump': continue
        if file[-5:] == 'gloss': continue

    print file
    print '{'
    print '    {'
    print '        map', file + '.jpg'
    if bump != None:
        print '        heightMap', bump + '.jpg 5.0'
    if gloss != None:
        print '        glossMap', gloss + '.jpg'
    print '    }'
    print '}'
    print ' '
    
    
