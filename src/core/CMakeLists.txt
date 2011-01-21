include(src/CMakeLists.txt)
include(test/CMakeLists.txt)

add_library(pwn-core ${core_src})
add_executable(pwn-core-test ${coretest_src})
target_link_libraries(pwn-core-test pwn-core)