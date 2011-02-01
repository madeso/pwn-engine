#autogen this:
include(src/CMakeLists.txt)

add_executable(pwn-game WIN32 ${autogen_src})
target_link_libraries(pwn-game pwn-math pwn-io pwn-render pwn-engine ${Boost_LIBRARIES} ${SFML_MAIN_LIBS})