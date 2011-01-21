include(src/CMakeLists.txt)

add_executable(pwn-game ${game_src})
target_link_libraries(pwn-game pwn-engine)