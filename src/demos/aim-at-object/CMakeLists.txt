#autogen this:
include(demo_aim_at_object_src.cmake)

source_group("" FILES ${demo_aim_at_object_src})

add_executable(pwn-demo-aim-at-object WIN32 ${demo_aim_at_object_src})
target_link_libraries(pwn-demo-aim-at-object pwn-math pwn-io pwn-render pwn-engine ${Boost_LIBRARIES} ${SFML_MAIN_LIBS})
set_target_properties(pwn-demo-aim-at-object PROPERTIES FOLDER "demo")