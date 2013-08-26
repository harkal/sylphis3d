#!BPY

""" Registration info for Blender menus:
Name: 'SMF...'
Blender: 234
Group: 'Import'
Tip: 'Import a Stanford PLY file.'
"""

# Copyright (C) 2004: Bruce Merry, bmerry@smuts.uct.ac.za
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.


# Portions of this code are taken from mod_meshtools.py in Blender
# 2.32.

import Blender
import re, struct, StringIO

def read(filename):       
        try:
                file = open(filename, "rb")
                signature = file.readline()
                if (signature != "ply\n"):
                        print "Signature line was invalid"
                        return None
                while 1:
                        line = file.readline()
                        tokens = re.split(r'[ \n]+', line)
                        if (len(tokens) == 0):
                                continue
                        if (tokens[0] == 'end_header'):
                                break
                        elif (tokens[0] == 'comment' or tokens[0] == 'obj_info'):
                                continue
                        elif (tokens[0] == 'format'):
                                if (len(tokens) < 3):
                                        print "Invalid format line"
                                        return None
                                if (tokens[1] not in format_specs.keys()):
                                        print "Unknown format " + tokens[1]
                                        return None
                                if (tokens[2] != version):
                                        print "Unknown version " + tokens[2]
                                        return None
                                format = tokens[1]
                        elif (tokens[0] == "element"):
                                if (len(tokens) < 3):
                                        print "Invalid element line"
                                        return None
                                obj_spec.specs.append(element_spec(tokens[1], int(tokens[2])))
                        elif (tokens[0] == "property"):
                                if (not len(obj_spec.specs)):
                                        print "Property without element"
                                        return None
                                if (tokens[1] == "list"):
                                        obj_spec.specs[-1].properties.append(property_spec(tokens[4], type_specs[tokens[2]], type_specs[tokens[3]]))
                                else:
                                        obj_spec.specs[-1].properties.append(property_spec(tokens[2], None, type_specs[tokens[1]]))
                obj = obj_spec.load(format_specs[format], file)

        except IOError, (errno, strerror):
                file.close()
                return None

        file.close()
        return (obj_spec, obj);

def add_face(mesh, vertices, indices, uvindices, colindices, texture_map):
        face = Blender.NMesh.Face()
        for index in indices:
                vertex = vertices[index];
                face.v.append(mesh.verts[index])
                face.mode = 0  
                face.smooth = 1
                if uvindices:
                        face.uv.append((vertex[uvindices[0]], 1.0 - vertex[uvindices[1]]))
                if colindices:
                        face.col.append(Blender.NMesh.Col(vertex[colindices[0]], vertex[colindices[1]], vertex[colindices[2]]))
        if texture_map != None:
                face.image = texture_map
                face.mode = Blender.NMesh.FaceModes.TEX
        mesh.faces.append(face)

def filesel_callback(filename):
	f = open(filename,'r')
	
	scene = Blender.Scene.getCurrent()
	mesh = Blender.NMesh.GetRaw()
	
	#line = f.readline()
	#vertNum = int(line.split()[3])
	#line = f.readline()
	#faceNum = int(line.split()[3])
	
	while 1:
		line = f.readline()
		toks = line.split()
		if len(toks) < 1:
			print "Empty line:", line
			continue
		elif toks[0] == 'v':
			x = float(toks[1])
			y = float(toks[2])
			z = float(toks[3])
		
			mesh.verts.append(Blender.NMesh.Vert(x,y,z))
		elif toks[0] == 'f':
			a = int(toks[1]) - 1
			b = int(toks[2]) - 1
			c = int(toks[3]) - 1
		
			face = Blender.NMesh.Face()
			face.v.append(mesh.verts[a])
			face.v.append(mesh.verts[b])
			face.v.append(mesh.verts[c])
		
			mesh.faces.append(face)
		elif toks[0] == 'end':
			break
	
	objname = Blender.sys.splitext(Blender.sys.basename(filename))[0]
 	Blender.NMesh.PutRaw(mesh, objname, 1)
	Blender.Redraw()
	Blender.Window.DrawProgressBar(1.0, '')
	message = "Successfully imported " + Blender.sys.basename(filename)

Blender.Window.FileSelector(filesel_callback, "Import SMF")