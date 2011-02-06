#autogen this:
include(render_src.cmake)
#autogen this:
include(fse/renderfse_src.cmake)
#autogen this:
include(../../../inc/pwn/render/render_inc.cmake)

set ( src_glew
	../../../external/glew/GL/glew.c
	../../../external/glew/GL/glew.h
	../../../external/glew/GL/wglew.h
)

set(src ${render_src} ${renderfse_src} ${render_inc} ${src_glew})


source_group("" FILES ${src})
source_group(fse FILES ${renderfse_src})
source_group(glew FILES ${src_glew})

include_directories("../../../external/glew" ${SFML_INCLUDE_DIR})
add_library(pwn-render ${src})
target_link_libraries(pwn-render pwn-math pwn-mesh ${OPENGL_LIBRARIES})
target_link_libraries(pwn-render pwn-io) #temporary, todo: fix fse loader
