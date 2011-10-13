#autogen this:
include(component_src.cmake)
#autogen this:
include(../../inc/pwn/component/component_inc.cmake)

set(src ${component_src} ${component_inc} ${soil_src})

source_group("" FILES ${src})

add_library(pwn-component ${src})
target_link_libraries(pwn-component pwn-math pwn-io pwn-render pwn-engine ${Boost_LIBRARIES} ${SFML_MAIN_LIBS})