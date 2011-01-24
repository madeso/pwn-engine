cmake_minimum_required (VERSION 2.6)
project(pwn)

set(pwn_version_major 1)
set(pwn_version_minor 0)

option (PWN_USE_CUSTOM_MATH "Use pwn.math instead of cml" ON)

configure_file("inc/config.in.h" "${PROJECT_BINARY_DIR}/pwn-config.h")
include_directories("${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "build shared libraries" OFF)
set(BUILD_SHARED_LIBS OFF)

include_directories(inc)
include_directories(external)

add_subdirectory(src)