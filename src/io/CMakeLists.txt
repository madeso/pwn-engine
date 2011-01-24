#autogen this:
include(src/CMakeLists.txt)
#include(test/CMakeLists.txt)

include_directories(../../external/physfspp)

add_library(pwn-io ${autogen_src})
#add_executable(pwn-io-test ${autogen_test})
#target_link_libraries(pwn-io-test pwn-io)