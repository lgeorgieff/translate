// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/12/2015
// Description: Defines the RESTful server for the translation service.
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


#include "server.hpp"
#include "server_exception.hpp"

#include <unistd.h>

namespace lgeorgieff {
namespace translate {
namespace server {

// TODO: implement multi-threading for server => 15 threads (make variable) =>
//       https://github.com/cesanta/mongoose/blob/master/examples/multi_threaded_server/multi_threaded_server.c

Server::Server(const ConnectionString &db_connection_string, std::string service_address, size_t service_port)
    : connection_address__{service_address + ":" + std::to_string(service_port)},
      db_connection_string__{db_connection_string},
      server__{mg_create_server(this, Server::request_handler)} {
  if (!this->server__) throw ServerException("Server resources could not be allocated!");
  mg_set_option(this->server__, "listening_port", this->connection_address__.c_str());
}

void Server::listen() {
  while (true) {
    mg_poll_server(this->server__, 1000);  // Infinite loop, Ctrl-C to stop
  }
}

Server::~Server() {
  if(this->server__) {
    mg_destroy_server(&this->server__);
    this->server__ = nullptr;
  }
}

int Server::request_handler(mg_connection *connection, enum mg_event event) {
  switch (event) {
    case MG_AUTH:
      return MG_TRUE;
    case MG_REQUEST:
      mg_printf_data(connection, "Hello World!!!");
      return MG_TRUE;
    default:
      return MG_FALSE;
  }

  return 0;
}

} // server
} // translate
} // server
