#autogen this:
include(src/mesh_src.cmake)
#autogen this:
include(test/mesh_test.cmake)
#autogen this:
include(../../inc/pwn/mesh/mesh_inc.cmake)

set(src ${autogen_src} ${autogen___inc_pwn_mesh})

add_library(pwn-mesh ${src})
target_link_libraries(pwn-mesh pwn-math pwn-core)

add_executable(pwn-mesh-test ${autogen_test} ../test-main.cpp)
target_link_libraries(pwn-mesh-test pwn-mesh ${GOOGLEMOCK_LIBRARIES})