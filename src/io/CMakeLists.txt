#autogen this:
include(io_src.cmake)
#include(test/CMakeLists.txt)
#autogen this:
include(../../inc/pwn/io/io_inc.cmake)

set(src ${io_src} ${io_inc})

source_group("" FILES ${src})

add_library(pwn-io ${src})
target_link_libraries(pwn-io ${Boost_LIBRARIES})
#add_executable(pwn-io-test ${autogen_test})
#target_link_libraries(pwn-io-test pwn-io)
