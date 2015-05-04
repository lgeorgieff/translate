// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 05/04/2015
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

// ====================================================================================================================
//  RESTful API
//  ===========
//
//  GET /languages => [{"language": "<language name>", "id": "<language id>"}]
//  GET /language/id/<id> => "<language name>"
//  GET /language/name/<name> => "<language id>"
//
//  GET /word_classes => [{"word_class": "<word class name>", "id" : "<word class id>"}]
//  GET /word_class/id/<id> => "<word class name>"
//  GET /word_class/name/<name> => "<word class id>"
//
//  GET /genders => [{"gender": "<gender name>", "id": "<gender id>", "description": "<gender description>"}]
//  GET /gender/id/<id> => {"gender" : "<gender name>", "description": "<gender description>"}
//  GET /gender/name/<name> => {"id" : "<gender id>", "description": "<gender description>"}
//
//  GET /numeri => ["numerus-id", "numerus-id"]
//
//  POST /translation/<language id source>/<language id target>/:
//    {"phrase": "<phrase origin>", "word_class": "<word class id>", "show_phrase": <bool>, "show_word_class": <bool>,
//    "show_gender": <bool>, "show_numerus": <bool>, "show_abbreviation": bool, "show_comment": <bool>}
//    => [{"origin_phrase": "<phrase origin language>", "origin_word_class": "<word class origin language>",
//         "origin_gender": "<gender origin language>", "origin_numerus": "<numerus>": "<numerus origin language>",
//         "origin_comment": "<comment origin language>", "origin_abbreviation": "<abbreviation origin language>",
//         "translations":
//           ["target_phrase": "<phrase target language>", "target_word_class": "<word class target language>",
//            "target_gender": "<gender target language>", "target_numerus": "<numerus>": "<numerus target language>",
//            "target_comment": "<comment target language>", "target_abbreviation": "<abbreviation target language>"]}]
// ====================================================================================================================

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "connection_string.hpp"
#include "db_query.hpp"

#include "mongoose.h"

#include <cstddef>
#include <string>

namespace lgeorgieff {
namespace translate {
namespace server {
// Defines the RESTful server API for the translation service.
class Server {
 public:
  // Instantiates an instance of this class with a connection string to the translation data base, an address and a
  // port the running server will be bound to
  Server(const ConnectionString &, std::string = "0.0.0.0", size_t = 8885);
  // Starts the server.
  void listen();

  ~Server();

  // Some constants that are used to describe the different URL of the RESTful API
  static const char *URL_LANGUAGES;
  static const char *URL_LANGUAGE_ID_PREFIX;
  static const char *URL_LANGUAGE_NAME_PREFIX;

  static const char *URL_WORD_CLASSES;
  static const char *URL_WORD_CLASS_ID_PREFIX;
  static const char *URL_WORD_CLASS_NAME_PREFIX;

  static const char *URL_GENDERS;
  static const char *URL_GENDER_ID_PREFIX;
  static const char *URL_GENDER_NAME_PREFIX;

  static const char *URL_NUMERI;

  static const char *URL_TRANSLATION_PREFIX;

  // Some methods that extract information from the passed URL
  static std::string get_origin_language_id_from_url(const char *);
  static std::string get_target_language_id_from_url(const char *);

 private:
  // The handler that is invoked by the server when a new request is received
  static int request_handler(mg_connection *, enum mg_event);

  // The connection address of the running server, i.e. address and port
  std::string connection_address__;
  // The query object that connects this server instance to the data base
  DbQuery db_query__;
  // The mongoose server instance
  mg_server *server__;
};  // Server
}  // server
}  // translate
}  // lgeorgieff

#endif  // SERVER_HPP_
