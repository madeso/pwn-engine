#!/usr/bin/python
# This script walks recursivly thorugh the entire structure and wrtes a CMAkeLists.txt file where requested (using a CMakeLists.in.txt file)
import os

def makeCmakeFile(path, sub):
	cmakefile = os.path.join(path, sub)
	folder = os.path.dirname(cmakefile)
	varname = "autogen_"+os.path.dirname(sub).replace("/", "_")
	relpath = os.path.dirname(sub)
	with open(cmakefile, 'w') as f:
		os.write(f.fileno(), "# atuogenerated file from cmakegen.py through listdir\n")
		os.write(f.fileno(), "# do not edit, since this file might get overwritten\n")
		os.write(f.fileno(), "# delete the in file if you need to edit this one\n")
		os.write(f.fileno(), "\n")
		os.write(f.fileno(), "set ( " + varname + "\n")
		for fname in os.listdir(folder):
			filepath = os.path.join(folder, fname)
			if os.path.isfile(filepath):
				if os.path.splitext(fname)[1]==".cpp":
					print " - in " + os.path.basename(path) + " adding " + fname + " to " + varname
					os.write(f.fileno(), "\t" + relpath + "/" +fname+"\n")
		os.write(f.fileno(), ")\n")
	

def parseCmakeFile(path):
	cmakefile = os.path.join(path, "CMakeLists.txt")
	if os.path.exists(cmakefile):
		lastline = ""
		with open(cmakefile) as f:
			for line in f:
				line = line.strip()
				if line.startswith("include") and lastline.strip(" #")=="autogen this:":
					sub = line[7:].strip(" ()\"")
					makeCmakeFile(path, sub)
				lastline = line

def walkDirs(path):
	for name in os.listdir(path):
		dir = os.path.join(path, name)
		if os.path.isdir(dir):
			walkDirs(dir)
			parseCmakeFile(dir)

walkDirs(os.getcwd())