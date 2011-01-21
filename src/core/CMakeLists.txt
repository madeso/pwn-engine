add_subdirectory(src)
add_subdirectory(test)

add_library(pwn-core "${core_src}")
add_executable(pwn-core-test "${coretest_src}")
target_link_libraries(pwn-core-test pwn-core)