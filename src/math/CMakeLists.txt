#autogen this:
include(src/math_src.cmake)
#autogen this:
include(test/math_test.cmake)
#autogen this:
include(../../inc/pwn/math/math_inc.cmake)

set(src ${autogen_src} ${autogen___inc_pwn_math})

add_library(pwn-math ${src})
target_link_libraries(pwn-math pwn-assert)

add_executable(pwn-math-test ${autogen_test} ../test-main.cpp)
target_link_libraries(pwn-math-test pwn-math ${GOOGLEMOCK_LIBRARIES})