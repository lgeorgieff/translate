// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/27/2015
// Description: Defines the exception HttpException which is thrown if any HTTP error occurs.
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

#include "http_exception.hpp"

#include <string>
#include <utility>

namespace lgeorgieff {
namespace translate {
namespace utils {
HttpException::HttpException(const std::string &what) : Exception{what} {}
}  // utils
}  // translate
}  // lgeorgieff
