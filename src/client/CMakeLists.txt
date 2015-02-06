#######################################################################################################################
# Copyright (C) 2015  Lukas Georgieff
# Last modified: 02/07/2015
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

### register all source files for the client part
set(CLIENT_SOURCE_FILES ../utils/exception.cpp ../utils/command_line_exception.cpp command_line_parser.cpp main.cpp)

### create the client executable
add_executable(client ${CLIENT_SOURCE_FILES})
set_target_properties(client PROPERTIES VERSION "${translate_VERSION_MAJOR}.${translate_VERSION_MINOR}")
