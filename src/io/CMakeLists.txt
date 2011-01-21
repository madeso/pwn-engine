include(src/CMakeLists.txt)
#include(test/CMakeLists.txt)

add_library(pwn-io ${io_src})
#add_executable(pwn-io-test ${iotest_src})
#target_link_libraries(pwn-io-test pwn-io)