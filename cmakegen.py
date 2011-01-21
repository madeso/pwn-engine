#!/usr/bin/python
# This script walks recursivly thorugh the entire structure and wrtes a CMAkeLists.txt file where requested (using a CMakeLists.in.txt file)
import os

def fileasstring(file):
	with open(file, "r") as f:
		return f.read()

def makeCmakeFile(path):
	cmakefile = os.path.join(path, "CMakeLists.txt")
	foldername = os.path.basename(path)
	
	runcmakefile = os.path.join(path, "CMakeLists.in.txt")
	
	if os.path.exists( runcmakefile ):
		cmakein = fileasstring(runcmakefile)
		sourcename = cmakein
		if sourcename == "":
			sourcename = foldername + "_src" # override var name if left unassigned
		print "generating " + sourcename + " for " + path
		with open(cmakefile, 'w') as f:
			f.write("# atuogenerated file from cmakegen.py through listdir\n")
			f.write("# do not edit, since this file might get overwritten\n")
			f.write("# delete the in file if you need to edit this one\n")
			f.write("\n")
			f.write("set ( " + sourcename + "\n")
			for fname in os.listdir(path):
				filepath = os.path.join(path, fname)
				if os.path.isfile(filepath):
					if os.path.splitext(fname)[1]==".cpp":
						print " - adding " + fname + " to " + sourcename
						f.write("\t" + foldername + "/" +fname+"\n")
			f.write(")\n")

def walkDirs(path):
	for name in os.listdir(path):
		dir = os.path.join(path, name)
		if os.path.isdir(dir):
			walkDirs(dir)
			makeCmakeFile(dir)

walkDirs(os.getcwd())