#!/usr/bin/env python3
# This script walks recursivly thorugh the entire structure and wrtes a CMAkeLists.txt file where requested (using a CMakeLists.in.txt file)
import os

def isOneOf(x, lst):
    for l in lst:
        if x == l:
            return True
    return False

def makeCmakeFile(path, sub):
    cmakefile = os.path.join(path, sub)
    folder = os.path.dirname(cmakefile)
    filenameWithoutExt = os.path.basename(sub)
    varname = os.path.splitext(filenameWithoutExt)[0].replace(".", "_").replace("-", "_")
    relpath = os.path.dirname(sub)
    #open as binary to force unix/linux(LF) type of lineendings on other systems than unix/linux such as windows(that uses CRLF)
    with open(cmakefile, 'w') as f:
        print("# atuogenerated file from cmakegen.py through listdir", file=f)
        print("# do not edit, since this file might get overwritten", file=f)
        print("# delete the comment where this file is included to remove the autogeneration", file=f)
        print(file=f)
        print("set ( " + varname, file=f)
        for fname in sorted(os.listdir(folder)):
            filepath = os.path.join(folder, fname)
            if os.path.isfile(filepath):
                ext = os.path.splitext(fname)[1][1:]
                if isOneOf(ext, ["c", "cpp", "cxx", "h", "hpp"]):
                    print(" - adding " + fname + " to " + varname)
                    if relpath == "":
                        print("\t" + fname+"", file=f)
                    else:
                        print("\t" + relpath + "/" +fname, file=f)
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

def main():
    walkDirs(os.getcwd())

if __name__ == "__main__":
    main()
