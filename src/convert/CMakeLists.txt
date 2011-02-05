#autogen this:
include(convert_src.cmake)

source_group("" FILES ${convert_src})
add_executable(pwn-convert ${convert_src})
target_link_libraries(pwn-convert pwn-core pwn-math pwn-io pwn-mesh ${Boost_LIBRARIES})