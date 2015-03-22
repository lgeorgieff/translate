#######################################################################################################################
# Copyright (C) 2015  Lukas Georgieff
# Last modified: 02/22/2015
# Description: Build unit tests for the utils part
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
project(translate_test_utils)

### register all source files
set(TEST_UTILS_SOURCE_FILES ../../utils/exception.cpp gender_unit_test.cpp word_class_unit_test.cpp
                            numerus_unit_test.cpp test_main.cpp)

### set all C++ flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lgtest -lgtest_main")

### create a static library
add_executable(utils_test ${TEST_UTILS_SOURCE_FILES})
