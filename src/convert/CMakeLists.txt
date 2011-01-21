add_subdirectory(src)

add_executable(pwn-convert "${convert_src}")
target_link_libraries(pwn-convert pwn-mesh)