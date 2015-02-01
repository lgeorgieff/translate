// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 02/01/2015
// Description: Declares the exception CommandLineException which is thrown if any error occurs on the command line
//              during the execution of this program.
// ====================================================================================================================

// ====================================================================================================================
// This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation in version 2.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with this program; if not, write to the
// Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
// ====================================================================================================================


#ifndef COMMAND_LINE_EXCEPTION_HPP_
#define COMMAND_LINE_EXCEPTION_HPP_

#include "exception.hpp"

#include <string>


namespace lgeorgieff {
	namespace translate {
		namespace utils {
			class CommandLineException : public Exception {
			public:
				CommandLineException(const std::string&);
				CommandLineException(const CommandLineException&) = default;
				CommandLineException(CommandLineException&&) = default;
				
				CommandLineException& operator=(const CommandLineException&) = default;
				CommandLineException& operator=(CommandLineException&&) = default;
				
				virtual ~CommandLineException() = default;
			}; // CommandLineException
		} // utils
	} // translate
} // lgeorgieff

#endif // COMMAND_LINE_EXCEPTION_HPP_
