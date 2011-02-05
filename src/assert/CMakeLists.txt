#autogen this:
include(assert_src.cmake)

add_library(pwn-assert ${assert_src} ../../inc/pwn/assert.h)
