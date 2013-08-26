#!/usr/bin/python -O

import os, string, marshal

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
				

import sys

if len(sys.argv) > 1:
	if sys.argv[1] == '-clean': delpyo = 1
comp("./")

raw_input("Press enter")


