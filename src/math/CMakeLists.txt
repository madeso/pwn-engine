add_subdirectory(src)
add_subdirectory(test)

add_library(pwn-math "${math_src}")
add_executable(pwn-math-test "${mathtest_src}")
target_link_libraries(pwn-math-test pwn-math)