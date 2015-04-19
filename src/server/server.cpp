// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/19/2015
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
#include "../utils/helper.hpp"

#include <unistd.h>
#include <cstring>

#include <iostream> // TODO: remove

using lgeorgieff::translate::utils::cstring_starts_with;
using lgeorgieff::translate::utils::cstring_ends_with;

namespace lgeorgieff {
namespace translate {
namespace server {

const char *Server::URL_LANGUAGES{"/languages/"};
const char *Server::URL_LANGUAGE_ID_PREFIX{"/language/id/"};
const char *Server::URL_LANGUAGE_NAME_PREFIX{"/language/name/"};
const char *Server::URL_WORD_CLASSES{"/word_classes"};
const char *Server::URL_WORD_CLASS_ID_PREFIX{"/word_class/id/"};
const char *Server::URL_WORD_CLASS_NAME_PREFIX{"/word_class/name/"};
const char *Server::URL_GENDERS{"/genders"};
const char *Server::URL_GENDER_ID_PREFIX{"/gender/id/"};
const char *Server::URL_GENDER_NAME_PREFIX{"/gender/name/"};
const char *Server::URL_NUMERI{"/numeri"};
const char *Server::URL_NUMERUS_ID_PREFIX{"/numerus/id/"};
const char *Server::URL_NUMERUS_NAME_PREFIX{"/numerus/name/"};
const char *Server::URL_TRANSLATION_PREFIX{"/translation/"};

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
      if(!strcmp(connection->request_method, "GET")) {
        char *url = nullptr;
        if(cstring_ends_with(connection->uri, "/")) {
          url = new char[strlen(connection->uri)];
          strcpy(url, connection->uri);
        } else {
          url = new char[strlen(connection->uri) + 1];
          strcpy(url, connection->uri);
          strcat(url, "/");
        }
        if (!strcmp(url, URL_LANGUAGES)) {
          mg_printf_data(connection, "URL_LANGUAGES: %s", url);
        } else if (cstring_starts_with(url, URL_LANGUAGE_ID_PREFIX)) {
          mg_printf_data(connection, "URL_LANGUAGES: %s", url);
        } else if (cstring_starts_with(url, URL_LANGUAGE_NAME_PREFIX)) {
          mg_printf_data(connection, "URL_LANGUAGE_NAME_PREFIX: %s", url);
        } else if (!strcmp(url, URL_WORD_CLASSES)) {
          mg_printf_data(connection, "URL_WORD_CLASSES: %s", url);
        } else if (cstring_starts_with(url, URL_WORD_CLASS_ID_PREFIX)) {
          mg_printf_data(connection, "URL_WORD_CLASS_ID_PREFIX: %s", url);
        } else if (cstring_starts_with(url, URL_WORD_CLASS_NAME_PREFIX)) {
          mg_printf_data(connection, "URL_WORD_CLASS_NAME_PREFIX: %s", url);
        } else if (!strcmp(url, URL_GENDERS)) {
          mg_printf_data(connection, "URL_GENDERS: %s", url);
        } else if (cstring_starts_with(url, URL_GENDER_ID_PREFIX)) {
          mg_printf_data(connection, "URL_GENDER_ID_PREFIX: %s", url);
        } else if (cstring_starts_with(url, URL_GENDER_NAME_PREFIX)) {
          mg_printf_data(connection, "URL_GENDER_NAME_PREFIX: %s", url);
        } else if (!strcmp(url, URL_NUMERI)) {
          mg_printf_data(connection, "URL_NUMERI: %s", url);
        } else if (cstring_starts_with(url, URL_NUMERUS_ID_PREFIX)) {
          mg_printf_data(connection, "URL_NUMERUS_ID_PREFIX: %s", url);
        } else if (cstring_starts_with(url, URL_NUMERUS_NAME_PREFIX)) {
          mg_printf_data(connection, "URL_NUMERUS_NAME_PREFIX: %s", url);
        } else {
          mg_printf_data(connection, "ERROR: bad url (GET): %s", url);
          // TODO: implement error handling
        }
      } else if (!strcmp(connection->request_method, "POS")) {
        if (cstring_starts_with(connection->uri, URL_TRANSLATION_PREFIX)) {
          // TODO: implement
        } else {
          mg_printf_data(connection, "ERROR: bad url (POST)");
          // TODO: implement error handling
        }
      } else {
        mg_printf_data(connection, "ERROR: bad method");
        // TODO: implement error handling
      }
      return MG_TRUE;
    default:
      return MG_FALSE;
  }

  return 0;
}

} // server
} // translate
} // server
