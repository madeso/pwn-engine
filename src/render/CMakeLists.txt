include(src/CMakeLists.txt)
include(test/CMakeLists.txt)

add_library(pwn-render ${render_src})
add_executable(pwn-render-test ${rendertest_src})
target_link_libraries(pwn-render-test pwn-render)