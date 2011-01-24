#autogen this:
include(src/CMakeLists.txt)
#autogen this:
include(test/CMakeLists.txt)

add_library(pwn-core ${autogen_src})
target_link_libraries(pwn-core pwn-assert)

add_executable(pwn-core-test ${autogen_test})
target_link_libraries(pwn-core-test pwn-assert pwn-core)