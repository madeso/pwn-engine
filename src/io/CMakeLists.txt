#autogen this:
include(src/CMakeLists.txt)
#include(test/CMakeLists.txt)
#autogen this:
include(../../inc/pwn/io/CMakeLists.txt)

set(src ${autogen_src} ${autogen___inc_pwn_io})

include_directories(../../external/physfspp)

add_library(pwn-io ${src})
#add_executable(pwn-io-test ${autogen_test})
#target_link_libraries(pwn-io-test pwn-io)