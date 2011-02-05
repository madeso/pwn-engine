#autogen this:
include(src/assert_src.cmake)

add_library(pwn-assert ${autogen_src} ../../inc/pwn/assert.h)
