#autogen this:
include(src/CMakeLists.txt)
#autogen this:
include(src/fse/CMakeLists.txt)
#autogen this:
include(test/CMakeLists.txt)
#autogen this:
include(../../inc/pwn/render/CMakeLists.txt)

set ( src_glew
	../../external/glew/GL/glew.c
	../../external/glew/GL/glew.h
	../../external/glew/GL/wglew.h
)

set(src ${autogen_src} ${autogen_src_fse} ${autogen___inc_pwn_render} ${src_glew})
set(test ${autogen_test})

include_directories("../../external/glew" ${SFML_INCLUDE_DIR})
source_group(src FILES ${src})
add_library(pwn-render ${src})
target_link_libraries(pwn-render pwn-math pwn-mesh ${OPENGL_LIBRARIES})
target_link_libraries(pwn-render pwn-io) #temporary, todo: fix fse loader

source_group(test FILES ${test})
add_executable(pwn-render-test ${test} ../test-main.cpp)
target_link_libraries(pwn-render-test pwn-render ${GOOGLEMOCK_LIBRARIES})
