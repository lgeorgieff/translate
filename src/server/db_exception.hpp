// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/03/2015
// Description: Declares the exception DbException which is thrown if any error occurs during a data base operation.
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

#ifndef DB_EXCEPTION_HPP_
#define DB_EXCEPTION_HPP_

#include "utils/exception.hpp"

#include <string>

namespace lgeorgieff {
namespace translate {
namespace server {
class DbException : public lgeorgieff::translate::utils::Exception {
 public:
  explicit DbException(const std::string&);
  DbException(const DbException&) = default;
  DbException(DbException&&) = default;

  DbException& operator=(const DbException&) = default;
  DbException& operator=(DbException&&) = default;

  virtual ~DbException() = default;
};  // DbException
}  // server
}  // translate
}  // lgeorgieff

#endif  // DB_EXCEPTION_HPP_
