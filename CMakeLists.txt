cmake_minimum_required (VERSION 2.6)
project(pwn)

set(pwn_version_major 1)
set(pwn_version_minor 0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake-modules")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
SET(Boost_ADDITIONAL_VERSIONS "1.43" "1.43.0" "1.45" "1.45.0")

set(SFML_STATIC_LIBRARIES ON)

set(SFMLDIR $ENV{SFMLDIR})

#set(GMOCK_CMAKE_DEBUG ON)
#set(PHYSFS_DEBUG ON)

find_package(GoogleMock)
find_package(PhysFS)
find_package(Boost COMPONENTS filesystem system)
find_package(OpenGL)
find_package(SFML COMPONENTS system window)

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

if( NOT SFML_FOUND)
	message(SEND_ERROR "sfml not found")
endif()

set(SFML_LIBS ${SFML_LIBRARIES})

option (PWN_USE_CUSTOM_MATH "Use pwn.math instead of cml" ON)

configure_file("inc/config.in.h" "${PROJECT_BINARY_DIR}/pwn-config.h")
include_directories("${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "build shared libraries" OFF)
set(BUILD_SHARED_LIBS OFF)

set(PWN_DIST_FOLDER "${PROJECT_SOURCE_DIR}/dist")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PWN_DIST_FOLDER})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PWN_DIST_FOLDER})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PWN_DIST_FOLDER})
message("will place builds at ${PWN_DIST_FOLDER}")

include_directories(inc)
include_directories(external)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GOOGLEMOCK_INCLUDE_DIRS})

add_subdirectory(src)
