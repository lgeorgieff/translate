// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/03/2015
// Description: Declares the exception ServerException which is thrown if any error occurs during a server operation.
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

#ifndef SERVER_EXCEPTION_HPP_
#define SERVER_EXCEPTION_HPP_

#include "utils/exception.hpp"

#include <string>

namespace lgeorgieff {
namespace translate {
namespace server {
class ServerException : public lgeorgieff::translate::utils::Exception {
 public:
  explicit ServerException(const std::string&);
  ServerException(const ServerException&) = default;
  ServerException(ServerException&&) = default;

  ServerException& operator=(const ServerException&) = default;
  ServerException& operator=(ServerException&&) = default;

  virtual ~ServerException() = default;
};  // ServerException
}  // server
}  // translate
}  // lgeorgieff

#endif  // SERVER_EXCEPTION_HPP_
