#autogen this:
include(src/CMakeLists.txt)

add_executable(pwn-convert ${autogen_src})
target_link_libraries(pwn-convert pwn-core pwn-math pwn-io pwn-mesh)