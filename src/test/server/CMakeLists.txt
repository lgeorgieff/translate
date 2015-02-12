#######################################################################################################################
# Copyright (C) 2015  Lukas Georgieff
# Last modified: 02/12/2015
# Description: Build unit tests for the server part.
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
project(translate_test_server)

### register all source files
set(TEST_SERVER_SOURCE_FILES ../../utils/exception.cpp ../../server/db_exception.cpp ../../server/connection_string.cpp
			     connection_string_unit_test.cpp test_main.cpp)

### set all C++ flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lgtest -lgtest_main")

### create a sttic library
add_executable(server_test ${TEST_SERVER_SOURCE_FILES})
