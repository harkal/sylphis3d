
import os
import re
import string

r = re.compile(r"{")
r2 = re.compile(r"}")
rcom = re.compile(r"//.*")

def parse(fname):
    f = open(fname, 'r')

    shaders = []

    data = f.read()
    f.close()
    
    data = rcom.sub('', data) # Remove comments
    data = r.sub(' { ', data)
    data = r2.sub(' } ', data)

    tokens = string.split(data)

    i = 0
    while i < len(tokens):
        name = tokens[i]
        shaders.append(name)
        while tokens[i] != '{':
            i += 1
        count = 0
        while True:
            if tokens[i] == '{':
                count += 1
            elif tokens[i] == '}':
                count -= 1
            i += 1
            if count == 0:
                break
            
    
    return shaders

for root, dirnames, filenames in os.walk('.'):
    for fname in filenames:
        name, ext = os.path.splitext(fname)
        if ext != '.mat':
            continue
        fullpath = os.path.join(root, fname)
        print fullpath
        shaders = parse(fullpath)
        f = open('../scripts/' + name + '.shader', 'w')
        for s in shaders:
            f.write(s + '\n {\n')
            f.write('   qer_editorimage ' + s + ' \n')
            f.write('}\n\n')
        f.close()
        
        
