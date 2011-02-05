#autogen this:
include(src/core_src.cmake)
#autogen this:
include(test/core_test.cmake)
#autogen this:
include(../../inc/pwn/core/core_inc.cmake)

set(src ${core_src} ${core_inc})

add_library(pwn-core ${src})
target_link_libraries(pwn-core pwn-assert)

add_executable(pwn-core-test ${core_test} ../test-main.cpp)
target_link_libraries(pwn-core-test pwn-assert pwn-core ${GOOGLEMOCK_LIBRARIES})