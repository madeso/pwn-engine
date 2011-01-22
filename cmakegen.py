#!/usr/bin/python
# This script walks recursivly thorugh the entire structure and wrtes a CMAkeLists.txt file where requested (using a CMakeLists.in.txt file)
import os

def makeCmakeFile(path, sub):
	cmakefile = os.path.join(path, sub)
	folder = os.path.dirname(cmakefile)
	varname = "autogen_"+os.path.dirname(sub).replace("/", "_")
	relpath = os.path.dirname(sub)
	#open as binary to force unix/linux(LF) type of lineendings on other systems than unix/linux such as windows(that uses CRLF)
	with open(cmakefile, 'wb') as f:
		f.write("# atuogenerated file from cmakegen.py through listdir\n")
		f.write("# do not edit, since this file might get overwritten\n")
		f.write("# delete the in file if you need to edit this one\n")
		f.write("\n")
		f.write("set ( " + varname + "\n")
		for fname in sorted(os.listdir(folder)):
			filepath = os.path.join(folder, fname)
			if os.path.isfile(filepath):
				if os.path.splitext(fname)[1]==".cpp":
					print " - adding " + fname + " to " + varname
					f.write("\t" + relpath + "/" +fname+"\n")
		f.write(")\n")
	

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