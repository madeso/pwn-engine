add_subdirectory(src)
add_subdirectory(test)

add_library(pwn-render "${render_src}")
add_executable(pwn-render-test "${rendertest_src}")
target_link_libraries(pwn-render-test pwn-render)