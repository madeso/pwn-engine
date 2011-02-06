#autogen this:
include(math_src.cmake)
#autogen this:
include(../../../inc/pwn/math/math_inc.cmake)

set(src ${math_src} ${math_inc})

source_group("" FILES ${src})

add_library(pwn-math ${src})
target_link_libraries(pwn-math pwn-assert)