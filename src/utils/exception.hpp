// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 01/27/2015
// Description: Provides the declaration for a self-defined base exception class.
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

#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <exception>
#include <string>

namespace lgeorgieff {
	namespace translate {
		namespace utils {
			class Exception : public std::exception {
			public:
				Exception(const std::string&) noexcept;
				Exception(const Exception&) noexcept = default;
				Exception(Exception&&) noexcept = default;

				Exception& operator=(const Exception&) noexcept = default;
				Exception& operator=(Exception&&) noexcept = default;

				const char* what() const noexcept;

				virtual ~Exception() = default;
			protected:
				std::string what_;
			}; // exception
		} // utils
	} // translate
} // lgeorgieff
#endif // EXCEPTION_HPP_
