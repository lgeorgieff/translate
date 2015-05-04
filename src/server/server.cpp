// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 05/04/2015
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
#include "json.hpp"
#include "../utils/helper.hpp"
#include "../utils/exception.hpp"

#include "json/json.h"

#include <unistd.h>
#include <cstring>
#include <iostream>

// TODO: page/request with API info => see HTTP standard

namespace {
// A helper function that sets the passed HTTP status code on the passed connection structure and finally writes the
// given error message to the connection structure.
void handle_http_error(mg_connection *connection, int status_code, const std::string &message) {
  std::cerr << "HTTP " << status_code << ": " << message << std::endl;
  mg_send_status(connection, status_code);
  mg_printf_data(connection, "%s", message.c_str());
}
}  // anonymous namespace

namespace lgeorgieff {
namespace translate {
namespace server {

using lgeorgieff::translate::utils::cstring_starts_with;
using lgeorgieff::translate::utils::cstring_ends_with;
using lgeorgieff::translate::utils::get_last_path_from_url;
using lgeorgieff::translate::utils::Exception;

const char *Server::URL_LANGUAGES{"/languages/"};
const char *Server::URL_LANGUAGE_ID_PREFIX{"/language/id/"};
const char *Server::URL_LANGUAGE_NAME_PREFIX{"/language/name/"};
const char *Server::URL_WORD_CLASSES{"/word_classes/"};
const char *Server::URL_WORD_CLASS_ID_PREFIX{"/word_class/id/"};
const char *Server::URL_WORD_CLASS_NAME_PREFIX{"/word_class/name/"};
const char *Server::URL_GENDERS{"/genders/"};
const char *Server::URL_GENDER_ID_PREFIX{"/gender/id/"};
const char *Server::URL_GENDER_NAME_PREFIX{"/gender/name/"};
const char *Server::URL_NUMERI{"/numeri/"};
const char *Server::URL_TRANSLATION_PREFIX{"/translation/"};

Server::Server(const ConnectionString &db_connection_string, std::string service_address, size_t service_port)
    : connection_address__{service_address + ":" + std::to_string(service_port)},
      db_query__{db_connection_string},
      server__{mg_create_server(&db_query__, Server::request_handler)} {
  if (!this->server__) throw ServerException("Server resources could not be allocated!");
  mg_set_option(this->server__, "listening_port", this->connection_address__.c_str());
}

void Server::listen() {
  while (true) {
    mg_poll_server(this->server__, 1000);  // Infinite loop, Ctrl-C to stop
  }
}

Server::~Server() {
  if (this->server__) {
    mg_destroy_server(&this->server__);
    this->server__ = nullptr;
  }
}

std::string Server::get_origin_language_id_from_url(const char *url) {
  // don't check prefix here, must be ensured outside this function
  url += strlen(Server::URL_TRANSLATION_PREFIX);
  std::string language_id{};
  for (; *url && '/' != *url; ++url) language_id += *url;
  return language_id;
}

std::string Server::get_target_language_id_from_url(const char *url) {
  // don't check prefix and origin language id here, must be ensured outside this function
  url += strlen(Server::URL_TRANSLATION_PREFIX);
  for (; *url && '/' != *url; ++url)
    ;
  ++url;
  std::string language_id{};
  for (; *url && '/' != *url; ++url) language_id += *url;
  return language_id;
}

int Server::request_handler(mg_connection *connection, enum mg_event event) {
  DbQuery *db_query;
  switch (event) {
    case MG_AUTH:
      return MG_TRUE;
    case MG_REQUEST:
      db_query = (DbQuery *)connection->server_param;
      char *url;
      if (cstring_ends_with(connection->uri, "/")) {
        url = new char[strlen(connection->uri) + 1];
        strncpy(url, connection->uri, strlen(connection->uri));
      } else {
        url = new char[strlen(connection->uri) + 2];
        strncpy(url, connection->uri, strlen(connection->uri) + 1);
        strncat(url, "/", 1);
      }
      if (!strcmp(connection->request_method, "GET")) {
        try {
          if (!strcmp(url, URL_LANGUAGES)) {
            db_query->request_all_languages();
            string json{JSON::all_languages_to_json(*db_query)};
            mg_printf_data(connection, "%s", json.c_str());
          } else if (cstring_starts_with(url, URL_LANGUAGE_ID_PREFIX)) {
            db_query->request_language_by_id(get_last_path_from_url(url));
            if (db_query->empty()) {
              std::string error_message{"Language ID \"" + get_last_path_from_url(url) + "\" not found!"};
              handle_http_error(connection, 404, error_message);
            } else {
              string json{JSON::language_name_to_json(*db_query)};
              mg_printf_data(connection, "%s", json.c_str());
            }
          } else if (cstring_starts_with(url, URL_LANGUAGE_NAME_PREFIX)) {
            db_query->request_language_by_name(get_last_path_from_url(url));
            if (db_query->empty()) {
              std::string error_message{"Language name \"" + get_last_path_from_url(url) + "\" not found!"};
              handle_http_error(connection, 404, error_message);
            } else {
              string json{JSON::language_id_to_json(*db_query)};
              mg_printf_data(connection, "%s", json.c_str());
            }
          } else if (!strcmp(url, URL_WORD_CLASSES)) {
            db_query->request_all_word_classes();
            string json{JSON::all_word_classes_to_json(*db_query)};
            mg_printf_data(connection, "%s", json.c_str());
          } else if (cstring_starts_with(url, URL_WORD_CLASS_ID_PREFIX)) {
            db_query->request_word_class_by_id(get_last_path_from_url(url));
            if (db_query->empty()) {
              std::string error_message{"Word class ID \"" + get_last_path_from_url(url) + "\" not found!"};
              handle_http_error(connection, 404, error_message);
            } else {
              string json{JSON::word_class_name_to_json(*db_query)};
              mg_printf_data(connection, "%s", json.c_str());
            }
          } else if (cstring_starts_with(url, URL_WORD_CLASS_NAME_PREFIX)) {
            db_query->request_word_class_by_name(get_last_path_from_url(url));
            if (db_query->empty()) {
              std::string error_message{"Word class name \"" + get_last_path_from_url(url) + "\" not found!"};
              handle_http_error(connection, 404, error_message);
            } else {
              string json{JSON::word_class_id_to_json(*db_query)};
              mg_printf_data(connection, "%s", json.c_str());
            }
          } else if (!strcmp(url, URL_GENDERS)) {
            db_query->request_all_genders();
            string json{JSON::all_genders_to_json(*db_query)};
            mg_printf_data(connection, "%s", json.c_str());
          } else if (cstring_starts_with(url, URL_GENDER_ID_PREFIX)) {
            db_query->request_gender_by_id(get_last_path_from_url(url));
            if (db_query->empty()) {
              std::string error_message{"Gender ID \"" + get_last_path_from_url(url) + "\" not found!"};
              handle_http_error(connection, 404, error_message);
            } else {
              string json{JSON::gender_name_to_json(*db_query)};
              mg_printf_data(connection, "%s", json.c_str());
            }
          } else if (cstring_starts_with(url, URL_GENDER_NAME_PREFIX)) {
            db_query->request_gender_by_name(get_last_path_from_url(url));
            if (db_query->empty()) {
              std::string error_message{"Gender name \"" + get_last_path_from_url(url) + "\" not found!"};
              handle_http_error(connection, 404, error_message);
            } else {
              string json{JSON::gender_id_to_json(*db_query)};
              mg_printf_data(connection, "%s", json.c_str());
            }
          } else if (!strcmp(url, URL_NUMERI)) {
            db_query->request_all_numeri();
            string json{JSON::all_numeri_to_json(*db_query)};
            mg_printf_data(connection, "%s", json.c_str());
          } else {
            // 400 => Bad Request (Bad URL)
            std::string error_message{"Bad Request: called GET on \"" + std::string{url} + "\"!"};
            handle_http_error(connection, 400, error_message);
          }
        } catch (Exception &err) {
          // Internal Server Error
          std::string error_message{std::string{"Internal server error: "} + err.what()};
          handle_http_error(connection, 500, error_message);
        }
      } else if (!strcmp(connection->request_method, "POST")) {
        try {
          if (cstring_starts_with(connection->uri, URL_TRANSLATION_PREFIX)) {
            char *post_content = new char[connection->content_len + 1];
            strncpy(post_content, connection->content, connection->content_len);
            post_content[connection->content_len] = '\0';
            Json::CharReaderBuilder json_reader_factory;
            Json::CharReader *json_reader(json_reader_factory.newCharReader());
            Json::Value user_options;
            std::string errors;
            if (!json_reader->parse(post_content, post_content + strlen(post_content), &user_options, &errors)) {
              // 400 => Bad Request (Bad URL)
              std::string error_message{"Bad Request: Malformed POST content:\n" + errors};
              handle_http_error(connection, 400, error_message);
            } else {
              std::string origin_phrase{""};
              if (user_options.isObject()) {
                Json::Value extracted_phrase{user_options.get("phrase", "")};
                if (!extracted_phrase.isString()) {
                  handle_http_error(connection, 400,
                                    "Expected a JSON object with at least the string member \"phrase\"!");
                  delete[] url;
                  return MG_TRUE;
                }
                origin_phrase = extracted_phrase.asString();
              }
              if (origin_phrase.empty()) {
                handle_http_error(connection, 400, "Phrase for translation must not be empty!");
                delete[] url;
                return MG_TRUE;
              }
              std::string word_class{""};
              Json::Value extracted_word_class{user_options.get("word_class", "")};
              if (extracted_word_class.isString()) word_class = extracted_word_class.asString();
              if (word_class.empty())
                db_query->request_phrase(origin_phrase, get_origin_language_id_from_url(url),
                                         get_target_language_id_from_url(url));
              else
                db_query->request_phrase(origin_phrase, get_origin_language_id_from_url(url),
                                         get_target_language_id_from_url(url), word_class);
              if (db_query->empty()) {
                std::string error_message{"No translation found for \"" + origin_phrase +
                                          (word_class.empty() ? "" : " (" + word_class + ")") + "\"!"};
                handle_http_error(connection, 404, error_message);
              } else {
                string json{JSON::phrase_to_json(*db_query, user_options)};
                mg_printf_data(connection, "%s", json.c_str());
              }
            }
          } else {
            // 400 => Bad Request (Bad URL)
            std::string error_message{"Bad Request: called POST on \"" + std::string{url} + "\"!"};
            handle_http_error(connection, 400, error_message);
          }
        } catch (Exception &err) {
          // Internal Server Error
          std::string error_message{std::string{"Internal server error: "} + err.what()};
          handle_http_error(connection, 500, error_message);
        }
      } else {
        // 400 => Bad Request (Bad method)
        std::string error_message{"Bad Request: called " + std::string{connection->request_method} + " on \"" +
                                  std::string{url} + "\"!"};
        handle_http_error(connection, 400, error_message);
      }
      delete[] url;
      return MG_TRUE;
    default:
      return MG_FALSE;
  }

  return 0;
}  // Server::request_handler

}  // server
}  // translate
}  // server
