// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 03/22/2015
// Description: Defines the DB_Query class that allows to query the data base for language information.
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

#ifndef DB_QUERY_HPP_
#define DB_QUERY_HPP_

#include <string>

namespace lgeorgieff {
namespace translate {
namespace server {

class DB_Query {

private:
  static const std::string EXACT_MATCH_QUERY;
  static const std::string EXACT_MATCH_WORD_CLASS_QUERY;
}; // DB_Query

}  // server
}  // translate
}  // lgeorgieff

#endif  // DB_QUERY_HPP_
