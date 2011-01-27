cmake_minimum_required (VERSION 2.6)
project(pwn)

set(pwn_version_major 1)
set(pwn_version_minor 0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake-modules")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
SET(Boost_ADDITIONAL_VERSIONS "1.43" "1.43.0" "1.45" "1.45.0")

#set(GMOCK_CMAKE_DEBUG ON)

find_package(GoogleMock)
find_package(PhysFS)
find_package(Boost COMPONENTS filesystem )
find_package(OpenGL)



message("suffixes: ${PATH_SUFFIXES}")

if(NOT PHYSFS_FOUND)
	message(SEND_ERROR "PhysFs not found")
endif()

if(NOT Boost_FOUND)
	message(SEND_ERROR "boost not found")
endif()

if(NOT GOOGLEMOCK_FOUND)
	message(SEND_ERROR "gmock not found")
endif()

if(NOT OPENGL_FOUND)
	message(SEND_ERROR "opengl not found")
endif()

option (PWN_USE_CUSTOM_MATH "Use pwn.math instead of cml" ON)

configure_file("inc/config.in.h" "${PROJECT_BINARY_DIR}/pwn-config.h")
include_directories("${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "build shared libraries" OFF)
set(BUILD_SHARED_LIBS OFF)

include_directories(inc)
include_directories(external)

add_subdirectory(src)
