#######################################################################################################################
# Copyright (C) 2015  Lukas Georgieff
# Last modified: 06/08/2015
# Description: A dummy CMakeLists.txt for calling the corresponding sub-CMakeLists.txt of the src folder.
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


### register sub-modules
add_subdirectory(server)
add_subdirectory(libs)
add_subdirectory(client)
add_subdirectory(scripts)
