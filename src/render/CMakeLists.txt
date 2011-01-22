#autogen this:
include(src/CMakeLists.txt)
#autogen this:
include(test/CMakeLists.txt)

add_library(pwn-render ${autogen_src})
add_executable(pwn-render-test ${autogen_test})
target_link_libraries(pwn-render-test pwn-render)