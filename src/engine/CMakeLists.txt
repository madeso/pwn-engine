#autogen this:
include(engine_src.cmake)
#autogen this:
include(../../inc/pwn/engine/engine_inc.cmake)
#include(test/CMakeLists.txt)
#autogen this:
include(../../external/soil/soil_src.cmake)

set(src ${engine_src} ${engine_inc} ${soil_src})
include_directories(${SFML_INCLUDE_DIR})

source_group("" FILES ${src})
source_group(soil FILES ${soil_src})

add_library(pwn-engine ${src})
target_link_libraries(pwn-engine pwn-math pwn-core pwn-mesh pwn-render ${SFML_LIBS} ${Boost_LIBRARIES})
