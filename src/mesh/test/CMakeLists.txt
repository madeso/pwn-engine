#autogen this:
include(mesh_test.cmake)

set(src ${mesh_test} ../../test-main.cpp)

source_group("" FILES ${src})

add_executable(pwn-mesh-test ${src})
target_link_libraries(pwn-mesh-test pwn-mesh ${GOOGLEMOCK_LIBRARIES})
