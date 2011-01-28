#autogen this:
include(src/CMakeLists.txt)
#autogen this:
include(test/CMakeLists.txt)
#autogen this:
include(../../inc/pwn/math/CMakeLists.txt)

set(src ${autogen_src} ${autogen___inc_pwn_math})

add_library(pwn-math ${src})
add_executable(pwn-math-test ${autogen_test})
target_link_libraries(pwn-math-test pwn-math ${GOOGLEMOCK_LIBRARIES})