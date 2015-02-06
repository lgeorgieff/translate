#######################################################################################################################
# Copyright (C) 2015  Lukas Georgieff
# Last modified: 02/01/2015
# Description: Build configuration for the server part of translate.
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
project(translate_server)
set (translate_VERSION_MAJOR 0)
set (translate_VERSION_MINOR 1)

### register all source files for the server part
set(SERVER_SOURCE_FILES ../utils/exception.cpp db_exception.cpp connection_string.cpp main.cpp)

### set all C++ flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

### create the server executable
add_executable(server ${SERVER_SOURCE_FILES})
set_target_properties(server PROPERTIES VERSION "${translate_VERSION_MAJOR}.${translate_VERSION_MINOR}")
