#autogen this:
include(src/render_src.cmake)
#autogen this:
include(src/fse/renderfse_src.cmake)
#autogen this:
include(test/render_test.cmake)
#autogen this:
include(../../inc/pwn/render/render_inc.cmake)

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
