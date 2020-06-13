#!/bin/sh
clang-format -i inc/**/*.h
# clang-format -i inc/**/*.hpp
# only one file, is ignored
clang-format -i src/**/*.h
clang-format -i src/**/*.hpp
clang-format -i src/**/*.cpp

