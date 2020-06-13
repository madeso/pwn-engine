#autogen this:
include(meshdebug_src.cmake)

source_group("" FILES ${meshdebug_src})
add_executable(pwn-meshedebug ${meshdebug_src})
target_link_libraries(pwn-meshedebug pwn-core pwn-math pwn-io pwn-mesh ${Boost_LIBRARIES})
