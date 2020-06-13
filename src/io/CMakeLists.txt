#autogen this:
include(io_src.cmake)
#include(test/CMakeLists.txt)
#autogen this:
include(../../inc/pwn/io/io_inc.cmake)

#autogen this:
include(../../external/physfspp/physfscpp.cmake)

set(src ${io_src} ${io_inc} ${physfscpp})

source_group("" FILES ${src})
source_group("PhysFs++" FILES ${physfscpp})

include_directories(../../external/physfspp)
include_directories(${PHYSFS_INCLUDE_DIR})

add_library(pwn-io ${src})
target_link_libraries(pwn-io ${PHYSFS_LIBRARY} ${Boost_LIBRARIES})
#add_executable(pwn-io-test ${autogen_test})
#target_link_libraries(pwn-io-test pwn-io)
