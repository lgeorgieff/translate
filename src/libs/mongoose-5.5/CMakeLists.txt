#######################################################################################################################
# Copyright (C) 2015  Lukas Georgieff
# Last modified: 02/01/2015
# Description: Build the mongoose web-server as static library, so it can be finally used by the server code of
#              translate.
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
project(translate_mongoose)
set (translate_mongoose_VERSION_MAJOR 5)
set (translate_mongoose_VERSION_MINOR 5)

### register all source files
set(MONGOOSE_SOURCE_FILES mongoose.c)

### create a sttic library
add_library(mongoose STATIC ${MONGOOSE_SOURCE_FILES})
set_target_properties(mongoose PROPERTIES VERSION
			       "${translate_mongoose_VERSION_MAJOR}.${translate_mongoose_VERSION_MINOR}")
