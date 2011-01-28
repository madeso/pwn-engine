#autogen this:
include(src/CMakeLists.txt)
#autogen this:
include(test/CMakeLists.txt)
#autogen this:
include(../../inc/pwn/mesh/CMakeLists.txt)

set(src ${autogen_src} ${autogen___inc_pwn_mesh})

add_library(pwn-mesh ${src})
target_link_libraries(pwn-mesh pwn-math)

add_executable(pwn-mesh-test ${autogen_test})
target_link_libraries(pwn-mesh-test pwn-mesh ${GOOGLEMOCK_LIBRARIES})