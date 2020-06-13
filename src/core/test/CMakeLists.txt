#autogen this:
include(core_test.cmake)

set(src ${core_test} ../../test-main.cpp)

source_group("" FILES ${src})
add_executable(pwn-core-test ${src})
target_link_libraries(pwn-core-test pwn-assert pwn-core ${GOOGLEMOCK_LIBRARIES})
add_custom_command(TARGET pwn-core-test POST_BUILD
	COMMAND pwn-core-test
	WORKING_DIRECTORY ${PWN_DIST_FOLDER})
