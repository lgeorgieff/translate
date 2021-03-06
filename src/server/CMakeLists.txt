#######################################################################################################################
# Copyright (C) 2015  Lukas Georgieff
# Last modified: 06/14/2015
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
set(SERVER_SOURCE_FILES ../utils/exception.cpp ../utils/json_exception.cpp db_exception.cpp server_exception.cpp
                        ../utils/command_line_exception.cpp ../utils/helper.cpp ../utils/numerus.cpp
                        ../utils/gender.cpp ../utils/word_class.cpp connection_string.cpp db_query.cpp json.cpp
                        server.cpp server_main.cpp)

### create the server executable
add_executable(trlt.service ${SERVER_SOURCE_FILES})
set_target_properties(trlt.service PROPERTIES VERSION "${translate_VERSION_MAJOR}.${translate_VERSION_MINOR}")

### set required libraries to link against
target_link_libraries(trlt.service jsoncpp)
target_link_libraries(trlt.service mongoose)
target_link_libraries(trlt.service pthread)
target_link_libraries(trlt.service pqxx)
target_link_libraries(trlt.service pq)

### copy static html content to executable directory
file(MAKE_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/www")
file(COPY "www/help.html" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/www/")
