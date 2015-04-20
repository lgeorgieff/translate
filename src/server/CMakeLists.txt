#######################################################################################################################
# Copyright (C) 2015  Lukas Georgieff
# Last modified: 04/20/2015
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

### register all source files for the server part
set(SERVER_SOURCE_FILES ../utils/exception.cpp ../utils/numerus.cpp ../utils/gender.cpp ../utils/word_class.cpp
                        ../utils/helper.cpp db_exception.cpp ../utils/command_line_exception.cpp server_exception.cpp
                        connection_string.cpp server.cpp server_main.cpp)

### create the server executable
add_executable(server ${SERVER_SOURCE_FILES})
set_target_properties(server PROPERTIES VERSION "${translate_VERSION_MAJOR}.${translate_VERSION_MINOR}")

### set required libraries to link against
target_link_libraries(server mongoose)
target_link_libraries(server pthread)
target_link_libraries(server pqxx)
target_link_libraries(server pq)
