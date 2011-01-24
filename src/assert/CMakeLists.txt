#autogen this:
include(src/CMakeLists.txt)

add_library(pwn-assert ${autogen_src} ../../inc/pwn/assert.h)
