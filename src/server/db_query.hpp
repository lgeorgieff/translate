// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 03/24/2015
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

#include "connection_string.hpp"

#include <pqxx/pqxx>
#include <string>

namespace lgeorgieff {
namespace translate {
namespace server {

// TODO: add comments
class DB_Query {
 public:
  DB_Query() = delete;
  explicit DB_Query(const Connection_String&);
  explicit DB_Query(pqxx::connection*);
  DB_Query(DB_Query&&) = default;
  DB_Query& operator=(const DB_Query&) = default;
  DB_Query& operator=(DB_Query&&) = default;
  ~DB_Query();

  DB_Query& operator()(const string&, const string&, const string&);
  DB_Query& operator()(const string&, const string&, const string&, const string&);

 private:
  std::string generate_exact_match_query(const std::string&, const std::string&, const std::string&);
  std::string generate_exact_match_word_class_query(const std::string&, const std::string&, const std::string&,
                                                           const std::string&);
  pqxx::connection* db_connection_;
  pqxx::result query_result_;
  bool connection_self_created_;
};  // DB_Query

}  // server
}  // translate
}  // lgeorgieff

#endif  // DB_QUERY_HPP_
