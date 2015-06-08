#######################################################################################################################
# Copyright (C) 2015  Lukas Georgieff
# Last modified: 06/08/2015
# Description: Build unit tests for the client part.
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
project(translate_test_client)

### register all source files
set(TEST_CLIENT_SOURCE_FILES ../../src/utils/exception.cpp ../../src/utils/command_line_exception.cpp
			     ../../src/utils/json_exception.cpp ../../src/client/command_line_parser.cpp
                             ../../src/client/configuration_reader.cpp command_line_parser_unit_test.cpp
                             configuration_reader_unit_test.cpp test_main.cpp)

### create a sttic library
add_executable(client_test ${TEST_CLIENT_SOURCE_FILES})

### set required libraries to link against
target_link_libraries(client_test gtest)
target_link_libraries(client_test gtest_main)
target_link_libraries(client_test pq)
target_link_libraries(client_test pqxx)
target_link_libraries(client_test jsoncpp)
