#autogen this:
include(assert_src.cmake)

set(files ${assert_src} ../../inc/pwn/assert.h)

source_group("" FILES ${files})

add_library(pwn-assert ${files})
