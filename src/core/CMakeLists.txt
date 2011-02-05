#autogen this:
include(src/core_src.cmake)
#autogen this:
include(test/core_test.cmake)
#autogen this:
include(../../inc/pwn/core/core_inc.cmake)

set(src ${autogen_src} ${autogen___inc_pwn_core})

add_library(pwn-core ${src})
target_link_libraries(pwn-core pwn-assert)

add_executable(pwn-core-test ${autogen_test} ../test-main.cpp)
target_link_libraries(pwn-core-test pwn-assert pwn-core ${GOOGLEMOCK_LIBRARIES})