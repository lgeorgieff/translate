// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/12/2015
// Description: Declares the RESTful server for the translation service.
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

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "connection_string.hpp"

#include "../libs/mongoose-5.5/mongoose.h"

#include <cstddef>
#include <string>


namespace lgeorgieff {
namespace translate {
namespace server {
class Server {
 public:
  Server(const ConnectionString &, std::string = "0.0.0.0", size_t = 8885);
  void listen();

  ~Server();
 private:
  static int request_handler(mg_connection *, enum mg_event);
  
  std::string connection_address__;
  ConnectionString db_connection_string__;
  mg_server *server__;
}; // Server
} // server
} // translate
} // lgeorgieff

#endif // SERVER_HPP_
