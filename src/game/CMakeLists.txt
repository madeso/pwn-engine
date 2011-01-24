#autogen this:
include(src/CMakeLists.txt)

add_executable(pwn-game ${autogen_src})
target_link_libraries(pwn-game pwn-math pwn-render pwn-engine)