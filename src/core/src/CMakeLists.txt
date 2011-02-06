#autogen this:
include(core_src.cmake)
#autogen this:
include(../../../inc/pwn/core/core_inc.cmake)

set(src ${core_src} ${core_inc})
source_group("" FILES ${src})
add_library(pwn-core ${src})
target_link_libraries(pwn-core pwn-assert)