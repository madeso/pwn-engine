#autogen this:
include(render_test.cmake)

set(src ${render_test} ../../test-main.cpp)
source_group("" FILES ${src})

add_executable(pwn-render-test ${src})
target_link_libraries(pwn-render-test pwn-render ${GOOGLEMOCK_LIBRARIES})
