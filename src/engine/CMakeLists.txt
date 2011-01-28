#autogen this:
include(src/CMakeLists.txt)
#autogen this:
include(../../inc/pwn/engine/CMakeLists.txt)
#include(test/CMakeLists.txt)
#autogen this:
include(../../external/soil/CMakeLists.txt)

set(src ${autogen_src} ${autogen___inc_pwn_engine} ${autogen___external_soil})

add_library(pwn-engine ${src})
target_link_libraries(pwn-engine pwn-math pwn-core pwn-mesh pwn-render ${Boost_LIBRARIES})