// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 03/22/2015
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

using lgeorgieff::translate::server::Connection_String;
using lgeorgieff::translate::server::DB_Query;

using std::cout;
using std::cin;
using std::string;
using std::endl;

int main(const int argc, const char **argv) {
  /*
  try {
    Connection_String connection_string{};
    connection_string.user(argv[1]);
    connection_string.password(argv[2]);
    connection_string.dbname("translate");
    connection_string.hostaddr("127.0.0.1");
    cout << connection_string << endl;

    pqxx::connection c(connection_string.to_string());
    pqxx::work w(c);
    //    pqxx::result r = w.exec(read_sql_file());
    w.commit();
    std::cout << r.size() << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  */

  return 0;
}
