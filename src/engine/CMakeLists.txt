#autogen this:
include(src/CMakeLists.txt)
#include(test/CMakeLists.txt)

add_library(pwn-engine ${autogen_src})