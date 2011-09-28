#autogen this:
include(demo_basic_camera_src.cmake)

source_group("" FILES ${demo_basic_camera_src})

add_executable(pwn-demo-basic-camera WIN32 ${demo_basic_camera_src})
target_link_libraries(pwn-demo-basic-camera pwn-math pwn-io pwn-render pwn-engine ${Boost_LIBRARIES} ${SFML_MAIN_LIBS})