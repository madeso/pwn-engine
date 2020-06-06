cmake_minimum_required (VERSION 3.1.0)
project(pwn)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(pwn_version_major 1)
set(pwn_version_minor 0)

set(OpenGL_GL_PREFERENCE "GLVND")

option (PWN_USE_FOLDERS "Use solution folders" OFF)

if(PWN_USE_FOLDERS)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	message("Using project folders")
else()
	set_property(GLOBAL PROPERTY USE_FOLDERS OFF)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake-modules")

# set(Boost_USE_STATIC_LIBS ON)
# set(Boost_USE_MULTITHREADED ON)
# set(Boost_ADDITIONAL_VERSIONS "1.43" "1.43.0" "1.45" "1.45.0" "1.72")

if(WIN32)
	#on windows, link to sfml static, to avoid crash on exit with ati cards
	set(SFML_STATIC_LIBRARIES ON)
else(WIN32)
	#on linux, link with dynamic since static library dependencies are horrible on linux
	set(SFML_STATIC_LIBRARIES OFF)
endif(WIN32)

set(SFMLDIR $ENV{SFMLDIR})

#set(GMOCK_CMAKE_DEBUG ON)
#set(PHYSFS_DEBUG ON)

# force visual studio and the like to only have debug and release builds
if(CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES Debug Release)
	set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations to what we need" FORCE)
 endif()

find_package(PhysFS REQUIRED)
find_package(Boost REQUIRED)
find_package(OpenGL REQUIRED)

find_package(SFML COMPONENTS system window REQUIRED)

set(SFML_MAIN_LIBS "")
set(SFML_LIBS sfml-system sfml-window)

message("libs: ${SFML_LIBS}" )

option (PWN_USE_CUSTOM_MATH "Use pwn.math instead of cml" ON)

configure_file("inc/config.in.h" "${PROJECT_BINARY_DIR}/pwn-config.h")
include_directories("${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "build shared libraries" OFF)
set(BUILD_SHARED_LIBS OFF)

set(PWN_DIST_FOLDER "${PROJECT_SOURCE_DIR}/dist")
if(NOT MSVC)
	# set it to a subfolder deeper, pwn expects it to run from say dist/Release
	set(PWN_DIST_FOLDER "${PWN_DIST_FOLDER}/build")
endif(NOT MSVC)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PWN_DIST_FOLDER})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PWN_DIST_FOLDER})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PWN_DIST_FOLDER})
message("will place builds at ${PWN_DIST_FOLDER}")

include_directories(inc)
include_directories(external)
include_directories(external/cml-1_0_2)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(src)
