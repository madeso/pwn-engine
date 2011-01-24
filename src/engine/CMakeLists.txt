#autogen this:
include(src/CMakeLists.txt)
#include(test/CMakeLists.txt)

add_library(pwn-engine ${autogen_src})
target_link_libraries(pwn-engine pwn-math pwn-core pwn-mesh pwn-render)