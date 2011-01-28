#autogen this:
include(src/CMakeLists.txt)
#autogen this:
include(test/CMakeLists.txt)
#autogen this:
include(../../inc/pwn/core/CMakeLists.txt)

set(src ${autogen_src} ${autogen___inc_pwn_core})

add_library(pwn-core ${src})
target_link_libraries(pwn-core pwn-assert)

add_executable(pwn-core-test ${autogen_test})
target_link_libraries(pwn-core-test pwn-assert pwn-core ${GOOGLEMOCK_LIBRARIES})