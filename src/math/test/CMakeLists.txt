#autogen this:
include(math_test.cmake)

set(src ${math_test} ../../test-main.cpp)
source_group("" FILES ${src})

add_executable(pwn-math-test ${src})
target_link_libraries(pwn-math-test catchy pwn-math ${GOOGLEMOCK_LIBRARIES})
