#######################################################################################################################
# Copyright (C) 2015  Lukas Georgieff
# Last modified: 06/07/2015
# Description: Build configuration for somce scripts written in C++.
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
project(translate_scripts)

### set the destination folder of the compiled files to the script folder itself
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/scripts)

### register all source files for the script part
set(SCRIPT_SOURCE_FILES ../utils/exception.cpp ../utils/command_line_exception.cpp ../utils/numerus.cpp
                        ../utils/gender.cpp ../utils/word_class.cpp ../utils/helper.cpp ./dict2sql.cpp)

### create the script executable
add_executable(dict2sql ${SCRIPT_SOURCE_FILES})
