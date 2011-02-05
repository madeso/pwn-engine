#autogen this:
include(src/engine_src.cmake)
#autogen this:
include(../../inc/pwn/engine/engine_inc.cmake)
#include(test/CMakeLists.txt)
#autogen this:
include(../../external/soil/soil_src.cmake)

set(src ${autogen_src} ${autogen___inc_pwn_engine} ${autogen___external_soil})
include_directories(${PHYSFS_INCLUDE_DIR} ${SFML_INCLUDE_DIR})

add_library(pwn-engine ${src})
target_link_libraries(pwn-engine pwn-math pwn-core pwn-mesh pwn-render ${SFML_LIBS} ${Boost_LIBRARIES})