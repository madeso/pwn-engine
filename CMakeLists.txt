cmake_minimum_required (VERSION 2.6)
project(pwn)

set(pwn_version_major 1)
set(pwn_version_minor 0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake-modules")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

find_package(GoogleMock REQUIRED)
find_package(PhysFS REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem )
find_package(OpenGL REQUIRED)

option (PWN_USE_CUSTOM_MATH "Use pwn.math instead of cml" ON)

configure_file("inc/config.in.h" "${PROJECT_BINARY_DIR}/pwn-config.h")
include_directories("${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "build shared libraries" OFF)
set(BUILD_SHARED_LIBS OFF)

include_directories(inc)
include_directories(external)

add_subdirectory(src)
