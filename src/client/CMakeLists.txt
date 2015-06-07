#######################################################################################################################
# Copyright (C) 2015  Lukas Georgieff
# Last modified: 06/07/2015
# Description: Build configuration for the client part of translate.
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
project(translate_client)

### set all include paths
include_directories("../")

### register all source files for the client part
set(CLIENT_SOURCE_FILES ../utils/command_line_exception.cpp ../utils/exception.cpp ../utils/http_exception.cpp
                        ../utils/json_exception.cpp ../utils/helper.cpp http_request.cpp http_get_request.cpp
                        http_post_request.cpp command_line_parser.cpp result_writer.cpp configuration_reader.cpp
                        client_main.cpp)

### create the client executable
add_executable(trlt ${CLIENT_SOURCE_FILES})
set_target_properties(trlt PROPERTIES VERSION "${translate_VERSION_MAJOR}.${translate_VERSION_MINOR}")

### set required libraries to link against
target_link_libraries(trlt curl)
target_link_libraries(trlt jsoncpp)
