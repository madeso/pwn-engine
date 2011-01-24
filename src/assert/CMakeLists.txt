#autogen this:
include(src/CMakeLists.txt)

include_directories(../../inc)

add_library(pwn-assert ${autogen_src} ../../inc/pwn/assert.h)
