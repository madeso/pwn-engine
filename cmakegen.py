#!/usr/bin/python
import os

path = os.getcwd()

cmakefile = os.path.join(path, "CMakeLists.txt")
foldername = os.path.basename(path)

dirList=os.listdir(path)
with open(cmakefile, 'w') as f:
	f.write("set ( " + foldername + "_src\n")
	for fname in dirList:
		if os.path.isfile(fname) and os.path.splitext(fname)[1]==".cpp":
			f.write("\t"+fname+"\n")
	f.write(")\n")
