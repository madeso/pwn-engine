#autogen this:
include(mesh_src.cmake)
#autogen this:
include(../../../inc/pwn/mesh/mesh_inc.cmake)

set(src ${mesh_src} ${mesh_inc})

source_group("" FILES ${src})

add_library(pwn-mesh ${src})
target_link_libraries(pwn-mesh pwn-math pwn-core)
