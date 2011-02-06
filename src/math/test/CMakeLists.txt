#autogen this:
include(test/math_test.cmake)

set(src ${math_test} ../test-main.cpp)
source_group("" FILES ${src})

add_executable(pwn-math-test ${src})
target_link_libraries(pwn-math-test pwn-math ${GOOGLEMOCK_LIBRARIES})