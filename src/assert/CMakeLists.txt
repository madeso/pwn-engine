#autogen this:
include(src/assert.cmake)

add_library(pwn-assert ${autogen_src} ../../inc/pwn/assert.h)
