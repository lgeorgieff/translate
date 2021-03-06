// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/11/2015
// Description: Provides the implementation for the exception DbException which is thrown if any error occurs during
//              a data base operation.
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

#include "db_exception.hpp"

#include <string>
#include <utility>

using std::string;

namespace lgeorgieff {
namespace translate {
namespace server {
DbException::DbException(const string &what) : Exception{what} {}
}  // server
}  // translate
}  // lgeorgieff
