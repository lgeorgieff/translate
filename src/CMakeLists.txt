#######################################################################################################################
# Copyright (C) 2015  Lukas Georgieff
# Last modified: 05/05/2015
# Description: Main CMakeLists.txt file for building this project. The actual modules are built by calling
#              CMakeLists.txt of the corresponding sub-directories.
#              Common options:
#                *-DCMAKE_BUILD_TYPE=Release
#                *-DCMAKE_BUILD_TYPE=Debug
#                *-DCMAKE_CXX_COMPILER=$(which clang++)
#                *-CMAKE_CXX_FLAGS_DISTRIBUTION="-Wall -Wextra"
#                *-DWALL=(ON|OFF)
#                *-DWEXTRA=(ON|OFF)
#                *-DCPP14=(ON|OFF)
#                *-DWITH_TESTS=(ON|OFF)
#              A more complete list of variables can be found here: http://www.cmake.org/Wiki/CMake_Useful_Variables
#######################################################################################################################


#######################################################################################################################
# This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation in version 2.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
# Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#######################################################################################################################



### project setup
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(translate)
set(translate_VERSION_MAJOR 0)
set(translate_VERSION_MINOR 1)

### set output folders
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin)

### user options
option(WALL "Use the compiler option -Wall for all targets if set to ON" ON)
option(WEXTRA "Use the compiler option -Wextra for all targets if set to ON" ON)
option(CPP14 "The compiler option std=c++14 that will be used for all targets if set to ON. Otherwise c++11 is used" ON)
option(WITH_TESTS "Compiles all tests if set to ON. Otherwise the compilation of tests is ommitted" OFF)

### set default values for jsoncpp options
OPTION(JSONCPP_WITH_TESTS "Compile and (for jsoncpp_check) run JsonCpp test executables" ${WITH_TESTS})
OPTION(JSONCPP_WITH_POST_BUILD_UNITTEST "Automatically run unit-tests as a post build step" OFF)
OPTION(JSONCPP_WITH_PKGCONFIG_SUPPORT "Generate and install .pc files" OFF)

### set all include paths
include_directories("./libs/mongoose-5.5/")
include_directories("./utils/")

### set all C++ flags used for all sub-modules, e.g. warnings and C++ standard version
if(${WALL})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

if(${WEXTRA})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
endif()

if(${CPP14})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(NOT ${CPP14})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

message(STATUS "C++ flags: ${CMAKE_CXX_FLAGS}")

### register sub-modules
add_subdirectory(server)
add_subdirectory(libs)
add_subdirectory(scripts)

if(${WITH_TESTS})
	add_subdirectory(test)
endif()
