
// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/05/2015
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

// Encapsulates the functionality to query the translation data base for predefined queries.
class DB_Query {
 public:
  DB_Query() = delete;
  // Instantiates a DB_Query object depending on the passed connection string instance.
  // Internally a pqxx::connection object is maintained for realizing the connection to the data base. It is deleted
  // when the destructor is called.
  explicit DB_Query(const ConnectionString&);
  // Instantiates a DB_Query object depending on the passed connection instance. The connection instance has to be
  // deleted by the user after this class does not need it anymore.
  explicit DB_Query(pqxx::connection*);
  DB_Query(DB_Query&&) = default;
  DB_Query& operator=(const DB_Query&) = default;
  DB_Query& operator=(DB_Query&&) = default;
  ~DB_Query();

  // Requests all data for a phrase translation without taking care on the word classes.
  DB_Query& request_phrase(const string&, const string&, const string&);
  // Requests all data for a phrase translation with taking care on the word classes.
  DB_Query& request_phrase(const string&, const string&, const string&, const string&);
  // Request an identifier for the given language (case insensitive).
  DB_Query& request_language(const string&);
  // Request all language identifiers and names.
  DB_Query& request_all_languages();
  // Request an identifier for the given word class (case insensitive).
  DB_Query& request_word_class(const string&);
  // Request all word class identifiers and names.
  DB_Query& request_all_word_classes();
  // Request an identifier for the given gender (case insensitive).
  DB_Query& request_gender(const string&);
  // Request all gender identifiers and names.
  DB_Query& request_all_genders();
  // Request all numerus identifiers.
  DB_Query& request_all_numeri();

  // Returns a start const_iterator pointing to the result data structure of the last request.
  pqxx::result::const_iterator begin() const;
  // Returns an end const_iterator pointing to the result data structure of the last request.
  pqxx::result::const_iterator end() const;
  // Returns a start const_reverse_iterator pointing to the result data structure of the last request.
  pqxx::result::const_reverse_iterator rbegin() const;
  // Returns an end const_reverse_iterator pointing to the result data structure of the last request.
  pqxx::result::const_reverse_iterator rend() const;
  // Returns the size of the result of the last request
  size_t size() const;
  // Returns true if the result of the last request is empty
  bool empty() const;
  // Clear the result data structures of the last request.
  void clear();

 private:
  //
  pqxx::connection* db_connection_;
  pqxx::result query_result_;
  bool connection_self_created_;
};  // DB_Query

}  // server
}  // translate
}  // lgeorgieff

#endif  // DB_QUERY_HPP_
