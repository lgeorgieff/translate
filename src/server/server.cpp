// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/14/2015
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
#include "utils/helper.hpp"
#include "utils/exception.hpp"

#include "json/json.h"

#include <cstring>
#include <iostream>
#include <cstddef>

namespace {
// A helper function that sets the passed HTTP status code on the passed connection structure and finally writes the
// given error message to the connection structure.
void handle_http_error(mg_connection *connection, int status_code, const std::string &message) {
  std::cerr << "HTTP " << status_code << ": " << message << std::endl;
  mg_send_status(connection, status_code);
  mg_send_header(connection, "content-type", "application/json");
  std::string json_string{lgeorgieff::translate::server::JSON::json_value_to_string(message)};
  mg_printf_data(connection, "%s", json_string.c_str());
}

// A helper function that sets the HTTP content-type "application/json" and writes the passed json string to the HTTP
// connection.
void send_json_data(mg_connection *connection, const std::string &json_string) {
  mg_send_header(connection, "content-type", "application/json");
  mg_printf_data(connection, "%s", json_string.c_str());
}

// A helper function that checks the given connection for the accept header value.
bool check_accept_header(mg_connection *connection, const std::string &expected = "application/json") {
  return mg_get_header(connection, "accept") != nullptr &&
         lgeorgieff::translate::utils::check_accept_header(mg_get_header(connection, "accept"), expected);
}
}  // anonymous namespace

namespace lgeorgieff {
namespace translate {
namespace server {

using lgeorgieff::translate::utils::cstring_starts_with;
using lgeorgieff::translate::utils::cstring_ends_with;
using lgeorgieff::translate::utils::get_last_path_from_url;
using lgeorgieff::translate::utils::get_exe_path;
using lgeorgieff::translate::utils::Exception;

std::string Server::service_prefix_{"/trlt/"};
std::string Server::url_help_{service_prefix_ + "help/"};
std::string Server::url_languages_{service_prefix_ + "languages/"};
std::string Server::url_language_id_prefix_{service_prefix_ + "language/id/"};
std::string Server::url_language_name_prefix_{service_prefix_ + "language/name/"};
std::string Server::url_word_classes_{service_prefix_ + "word_classes/"};
std::string Server::url_word_class_id_prefix_{service_prefix_ + "word_class/id/"};
std::string Server::url_word_class_name_prefix_{service_prefix_ + "word_class/name/"};
std::string Server::url_genders_{service_prefix_ + "genders/"};
std::string Server::url_gender_id_prefix_{service_prefix_ + "gender/id/"};
std::string Server::url_gender_name_prefix_{service_prefix_ + "gender/name/"};
std::string Server::url_numeri_{service_prefix_ + "numeri/"};
std::string Server::url_translation_prefix_{service_prefix_ + "translation/"};

Server::Server(const ConnectionString &db_connection_string, const std::string &service_address, size_t service_port)
    : connection_address_{service_address + ":" + std::to_string(service_port)},
      db_query_{db_connection_string},
      server_{mg_create_server(&db_query_, Server::request_handler)} {
  if (!this->server_) throw ServerException("Server resources could not be allocated!");
  mg_set_option(this->server_, "listening_port", this->connection_address_.c_str());
}

void Server::listen() {
  while (true) {
    mg_poll_server(this->server_, 1000);  // Infinite loop, Ctrl-C to stop
  }
}

Server::~Server() {
  if (this->server_) {
    mg_destroy_server(&this->server_);
    this->server_ = nullptr;
  }
}

std::string Server::get_origin_language_id_from_url(const char *url) {
  // don't check prefix here, must be ensured outside this function
  url += strlen(url_translation_prefix_.c_str());
  std::string language_id{};
  for (; *url && '/' != *url; ++url) language_id += *url;
  return language_id;
}

std::string Server::get_target_language_id_from_url(const char *url) {
  // don't check prefix and origin language id here, must be ensured outside this function
  url += strlen(url_translation_prefix_.c_str());
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
        if (!strcmp(url, url_help_.c_str())) {
          if (!check_accept_header(connection, "text/html")) {
            std::string error_message{"Only the content-type \"text/html\" is supported!"};
            handle_http_error(connection, 406, error_message);
          } else {
            try {
              std::string html_path{get_exe_path() + "www/help.html"};
              mg_send_file(connection, html_path.c_str(), nullptr);
              return MG_MORE;
            } catch (const Exception &) {
              std::string error_message{"Internal server error!"};
              handle_http_error(connection, 500, error_message);
            }
          }
        } else if (!check_accept_header(connection)) {
          std::string error_message{"Only the content-type \"application/json\" is supported!"};
          handle_http_error(connection, 406, error_message);
        } else {
          try {
            if (!strcmp(url, url_languages_.c_str())) {
              db_query->request_all_languages();
              string json{JSON::all_languages_to_json(*db_query)};
              send_json_data(connection, json);
            } else if (cstring_starts_with(url, url_language_id_prefix_.c_str())) {
              db_query->request_language_by_id(get_last_path_from_url(url));
              if (db_query->empty()) {
                std::string error_message{"Language ID \"" + get_last_path_from_url(url) + "\" not found!"};
                handle_http_error(connection, 404, error_message);
              } else {
                string json{JSON::language_name_to_json(*db_query)};
                send_json_data(connection, json);
              }
            } else if (cstring_starts_with(url, url_language_name_prefix_.c_str())) {
              db_query->request_language_by_name(get_last_path_from_url(url));
              if (db_query->empty()) {
                std::string error_message{"Language name \"" + get_last_path_from_url(url) + "\" not found!"};
                handle_http_error(connection, 404, error_message);
              } else {
                string json{JSON::language_id_to_json(*db_query)};
                send_json_data(connection, json);
              }
            } else if (!strcmp(url, url_word_classes_.c_str())) {
              db_query->request_all_word_classes();
              string json{JSON::all_word_classes_to_json(*db_query)};
              send_json_data(connection, json);
            } else if (cstring_starts_with(url, url_word_class_id_prefix_.c_str())) {
              db_query->request_word_class_by_id(get_last_path_from_url(url));
              if (db_query->empty()) {
                std::string error_message{"Word class ID \"" + get_last_path_from_url(url) + "\" not found!"};
                handle_http_error(connection, 404, error_message);
              } else {
                string json{JSON::word_class_name_to_json(*db_query)};
                send_json_data(connection, json);
              }
            } else if (cstring_starts_with(url, url_word_class_name_prefix_.c_str())) {
              db_query->request_word_class_by_name(get_last_path_from_url(url));
              if (db_query->empty()) {
                std::string error_message{"Word class name \"" + get_last_path_from_url(url) + "\" not found!"};
                handle_http_error(connection, 404, error_message);
              } else {
                string json{JSON::word_class_id_to_json(*db_query)};
                send_json_data(connection, json);
              }
            } else if (!strcmp(url, url_genders_.c_str())) {
              db_query->request_all_genders();
              string json{JSON::all_genders_to_json(*db_query)};
              send_json_data(connection, json);
            } else if (cstring_starts_with(url, url_gender_id_prefix_.c_str())) {
              db_query->request_gender_by_id(get_last_path_from_url(url));
              if (db_query->empty()) {
                std::string error_message{"Gender ID \"" + get_last_path_from_url(url) + "\" not found!"};
                handle_http_error(connection, 404, error_message);
              } else {
                string json{JSON::gender_name_to_json(*db_query)};
                send_json_data(connection, json);
              }
            } else if (cstring_starts_with(url, url_gender_name_prefix_.c_str())) {
              db_query->request_gender_by_name(get_last_path_from_url(url));
              if (db_query->empty()) {
                std::string error_message{"Gender name \"" + get_last_path_from_url(url) + "\" not found!"};
                handle_http_error(connection, 404, error_message);
              } else {
                string json{JSON::gender_id_to_json(*db_query)};
                send_json_data(connection, json);
              }
            } else if (!strcmp(url, url_numeri_.c_str())) {
              db_query->request_all_numeri();
              string json{JSON::all_numeri_to_json(*db_query)};
              send_json_data(connection, json);
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
        }
      } else if (!strcmp(connection->request_method, "POST")) {
        if (!check_accept_header(connection)) {
          std::string error_message{"Only the content-type \"application/json\" is supported!"};
          handle_http_error(connection, 406, error_message);
        } else if (mg_get_header(connection, "content-type") != nullptr &&
                   strcmp(mg_get_header(connection, "content-type"), "application/json")) {
          std::string error_message{"Only the content-type \"application/json\" of POST data is supported!"};
          handle_http_error(connection, 406, error_message);
        } else {
          try {
            if (cstring_starts_with(connection->uri, url_translation_prefix_.c_str())) {
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
                std::string origin_language_id{get_origin_language_id_from_url(url)};
                std::string target_language_id{get_target_language_id_from_url(url)};
                if (word_class.empty()) {
                  db_query->request_phrase(origin_phrase, origin_language_id, target_language_id);
                } else {
                  db_query->request_phrase(origin_phrase, origin_language_id, target_language_id, word_class);
                }
                if (db_query->empty()) {
                  std::string error_message{"No translation found for \"" + origin_phrase +
                                            (word_class.empty() ? "" : " (" + word_class + ")") + "\" (" +
                                            origin_language_id + " => " + target_language_id + ")!"};
                  handle_http_error(connection, 404, error_message);
                } else {
                  string json{JSON::phrase_to_json(*db_query, user_options)};
                  send_json_data(connection, json);
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
