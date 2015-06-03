// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/03/2015
// Description: Declares the exception NotImplementedException which is thrown if any invoked functionality is not
//              implemented.
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

#ifndef NOT_IMPLEMENTED_EXCEPTION_HPP_
#define NOT_IMPLEMENTED_EXCEPTION_HPP_

#include "exception.hpp"

#include <string>

namespace lgeorgieff {
namespace translate {
namespace utils {
class NotImplementedException : public Exception {
 public:
  explicit NotImplementedException(const std::string&);
  NotImplementedException(const NotImplementedException&) = default;
  NotImplementedException(NotImplementedException&&) = default;

  NotImplementedException& operator=(const NotImplementedException&) = default;
  NotImplementedException& operator=(NotImplementedException&&) = default;

  virtual ~NotImplementedException() = default;
};  // NotImplementedException
}  // utils
}  // translate
}  // lgeorgieff

#endif  // Not_Implemented_EXCEPTION_HPP_
