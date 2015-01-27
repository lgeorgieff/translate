// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 01/17/2015
// Description: The entry point for the entire application.
// ====================================================================================================================

// ====================================================================================================================
// This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation in version 2.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with this program; if/ not, write to the
// Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
// ====================================================================================================================


#include "command_line_parser.hpp"

#include <iostream>


using lgeorgieff::translate::CommandLineParser;

using std::cout;
using std::endl;


int main(const int argc, const char** argv) {
	CommandLineParser cmdParser{argc, argv};
	
	return 0;
}
