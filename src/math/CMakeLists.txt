include(src/CMakeLists.txt)
include(test/CMakeLists.txt)

add_library(pwn-math ${math_src})
add_executable(pwn-math-test ${mathtest_src})
target_link_libraries(pwn-math-test pwn-math)