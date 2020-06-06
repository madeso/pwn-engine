#!/usr/bin/env python3
# This script walks recursivly thorugh the entire structure opens files and converts them to unix newlines and makes sure there is a newline at the end
import os

def isOneOf(x, lst):
    for l in lst:
        if x == l:
            return True
    return False

def read_lines(file):
    last_error = None
    for e in ['utf-8', 'cp1252']:
        try:
            with open(file, 'r', encoding=e) as f:
                return f.readlines()
        except UnicodeDecodeError as e:
            last_error = e
            print('ERROR: unicode error for', file)
    raise last_error
    

def fixFile(file):
    ext = os.path.splitext(file)[1][1:]
    if isOneOf(ext, ["c", "cpp", "cxx", "h", "hpp"]):
        print("fixing " + file)
        lines = read_lines(file)
        with open(file, 'wb') as f:
            for line in lines:
                f.write((line.rstrip() + "\n").encode('utf-8'))

def walkDirs(path):
    for name in os.listdir(path):
        dir = os.path.join(path, name)
        if os.path.isdir(dir):
            walkDirs(dir)
        elif os.path.isfile(dir):
            fixFile(dir)

def main():
    for dir in ["inc", "src"]:
        walkDirs( os.path.join(os.getcwd(), dir) )

if __name__ == "__main__":
    main()
