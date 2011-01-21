add_subdirectory(src)
add_subdirectory(test)

add_library(pwn-mesh "${mesh_src}")
add_executable(pwn-mesh-test "${meshtest_src}")
target_link_libraries(pwn-mesh-test pwn-mesh)