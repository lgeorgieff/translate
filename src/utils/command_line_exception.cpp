// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 02/01/2015
// Description: Provides the implementation for the exception CommandLineException which is thrown if any error occurs
//              on the command line during the execution of this program.
// ====================================================================================================================

// ====================================================================================================================
// This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation in version 2.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
// Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
// ====================================================================================================================


#include "command_line_exception.hpp"

#include <string>
#include <utility>

using std::string;

namespace lgeorgieff {
	namespace translate {
		namespace utils {
			CommandLineException::CommandLineException(const string &what)
			:Exception{what} { }
		} // utils
	} // translate
} // lgeorgieff
