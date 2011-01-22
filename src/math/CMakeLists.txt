#autogen this:
include(src/CMakeLists.txt)
#autogen this:
include(test/CMakeLists.txt)

add_library(pwn-math ${autogen_src})
add_executable(pwn-math-test ${autogen_test})
target_link_libraries(pwn-math-test pwn-math)