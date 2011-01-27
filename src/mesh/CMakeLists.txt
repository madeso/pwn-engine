#autogen this:
include(src/CMakeLists.txt)
#autogen this:
include(test/CMakeLists.txt)

add_library(pwn-mesh ${autogen_src})
target_link_libraries(pwn-mesh pwn-math)

add_executable(pwn-mesh-test ${autogen_test})
target_link_libraries(pwn-mesh-test pwn-mesh ${GOOGLEMOCK_LIBRARIES})