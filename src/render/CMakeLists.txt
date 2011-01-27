#autogen this:
include(src/CMakeLists.txt)
#autogen this:
include(src/fse/CMakeLists.txt)
#autogen this:
include(test/CMakeLists.txt)
#autogen this:
include(../../inc/pwn/render/CMakeLists.txt)

set(src ${autogen_src} ${autogen_src_fse} ${autogen___inc_pwn_render})
set(test ${autogen_test})

include_directories("../../external/glew")
source_group(src FILES ${src})
add_library(pwn-render ${src})
target_link_libraries(pwn-render pwn-math pwn-mesh)

source_group(test FILES ${test})
add_executable(pwn-render-test ${test})
target_link_libraries(pwn-render-test pwn-render)
