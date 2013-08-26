#!BPY

""" Registration info for Blender menus:
Name: 'SMF...'
Blender: 234
Group: 'Export'
Tooltip: 'Export to SMF format (*.smf)'
"""

import Blender
#import mod_meshtools
import math

def image_filename(image):
	if image == None:
		return "";
	return image.getFilename()

def getTextureName(material):
	for texture in material.getTextures():
		if not hasattr(texture, 'tex'):
			return material.name
		if texture.tex.image:
			return texture.tex.image.filename
		
def fixTextureName(name):
	return name[name.find("base\\") + 5:]


def file_callback(filename):
	if filename.find('.smf', -4) < 0: filename += '.smf'
	print "Exporting to", filename
	file = open(filename, "wb")
	objects = Blender.Object.GetSelected()
	for object in objects:
		objname = object.name
		meshname = object.data.name
		mesh = Blender.NMesh.GetRaw(meshname)
		mesh.transform(object.matrixWorld)
		obj = Blender.Object.Get(objname)
	
		mat_index = 0
		for material in mesh.materials:
			#print "Material:", fixTextureName(getTextureName(material))
	
			print >> file, "begin", fixTextureName(getTextureName(material))
			have_uv = mesh.hasFaceUV()
			verts = [] # list of (position, normal, [uv]) triplets
			vdict = {} # (index, normal, uv) -> new index
			for f in mesh.faces:
				if f.materialIndex != mat_index:
					continue
				
				for i in range(len(f.v)):
					v = f.v[i]
					index = v.index
					if mesh.hasFaceUV():
						uv = f.uv[i]
					elif mesh.hasVertexUV():
						uv = (f.v[i].uvco[0], face.v[i].uvco[1])
					else:
						uv = (0.0, 0.0)
						
					if not vdict.has_key(index):                   
						verts.append((v, f.v[i].no, uv))
						vdict[index] = len(verts)
						
			print "Vertices :", len(verts)
				
			for vn in verts:
				v = vn[0]
				n = vn[1]
				t = vn[2]
				print >> file,  "v %f %f %f" % (v.co[0], v.co[1], v.co[2])
				print >> file,  "n %f %f %f" % (n[0], n[1], n[2])
				print >> file,  "t %f %f" % (t[0], t[1])
					
			for f in mesh.faces:
				if f.materialIndex != mat_index:
					continue
				print >> file, "\nf",
				for i in range(len(f.v)):
					v = f.v[i]
					index = v.index
					print >> file,  "%d" % (vdict[index]),

			print >> file, "\nend\n"
			mat_index += 1

	Blender.Window.DrawProgressBar(1.0, '')  # clear progressbar
	file.close()
	message = "Successfully exported " + Blender.sys.basename(filename)
	#mod_meshtools.print_boxed(message)

Blender.Window.FileSelector(file_callback, "SMF Export")