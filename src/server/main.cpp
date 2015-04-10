// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 03/25/2015
// Description: This file serves just as a stub for running the server part until the final server entry point will be
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

#include "connection_string.hpp"
#include "db_query.hpp"

#include <pqxx/pqxx>
#include <iostream>
#include <fstream>
#include <string>

#include <cctype>
#include <algorithm>

using lgeorgieff::translate::server::ConnectionString;
using lgeorgieff::translate::server::DB_Query;

using std::cout;
using std::cin;
using std::string;
using std::endl;

int main(const int argc, const char **argv) {
  ConnectionString connection_string{};
  connection_string.user(argv[1]);
  connection_string.dbname("translate");
  connection_string.hostaddr("127.0.0.1");
  DB_Query db_query(connection_string);
  db_query.request_phrase("stehen", "DE", "EN", "verb");
  // db_query.request_phrase("stehen", "DE", "EN");
  // db_query.request_all_languages();
  // db_query.request_language("deutsch");
  // db_query.request_word_class("Adjective");
  // db_query.request_all_word_classes();
  // db_query.request_gender("das");
  // db_query.request_all_genders();
  // db_query.request_all_numeri();
  for(const pqxx::result::tuple tuple : db_query) {
    for(const auto item : tuple) {
      if (!to_string(item).empty()) cout << "_" << item << "_" << " ";
    }
    cout << endl;
  }
  return 0;
}
