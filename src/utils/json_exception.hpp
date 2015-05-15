// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/27/2015
// Description: Declares the exception JsonException which is thrown if any JSON transformation occurs.
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

#ifndef JSON_EXCEPTION_HPP_
#define JSON_EXCEPTION_HPP_

#include "exception.hpp"

#include <string>

namespace lgeorgieff {
namespace translate {
namespace utils {
class JsonException : public Exception {
 public:
  JsonException(const std::string&);
  JsonException(const JsonException&) = default;
  JsonException(JsonException&&) = default;

  JsonException& operator=(const JsonException&) = default;
  JsonException& operator=(JsonException&&) = default;

  virtual ~JsonException() = default;
};  // JsonException
}  // utils
}  // translate
}  // lgeorgieff

#endif  // JSON_EXCEPTION_HPP_
