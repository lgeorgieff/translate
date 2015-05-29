// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 05/29/2015
// Description: The entry point for the entire application.
// ====================================================================================================================

// ====================================================================================================================
// This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation in version 2.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with this program; if/ not, write to the
// Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
// ====================================================================================================================

#include "utils/command_line_exception.hpp"
#include "utils/http_exception.hpp"
#include "command_line_parser.hpp"
#include "http_get_request.hpp"
#include "http_post_request.hpp"

#include "json/json.h"

#include <iostream>

using lgeorgieff::translate::CommandLineParser;
using lgeorgieff::translate::utils::CommandLineException;
using lgeorgieff::translate::utils::HttpException;
using lgeorgieff::translate::client::HttpGetRequest;
using lgeorgieff::translate::client::HttpPostRequest;

// TODO: implement reading of a configuration file => server url + defaults
const std::string BASE_URL{"localhost:8885/"};

// Create a JSON string based on the corresponding command line arguments
// that can be used for a post/translation request.
std::string create_post_data(const CommandLineParser &cmd_parser) {
  Json::Value result;
  result["phrase"] = cmd_parser.phrase();
  result["show_phrase"] = cmd_parser.show_phrase();
  result["show_word_class"] = cmd_parser.show_word_class();
  result["show_gender"] = cmd_parser.show_gender();
  result["show_numerus"] = cmd_parser.show_numerus();
  result["show_abbreviation"] = cmd_parser.show_abbreviation();
  result["show_comment"] = cmd_parser.show_comment();
  Json::StreamWriterBuilder json_writer;
  json_writer.settings_["indentation"] = "";
  return Json::writeString(json_writer, result);
}

int main(const int argc, const char** argv) {
  try {
    CommandLineParser cmd_parser{argc, argv};
    if (cmd_parser.help()) {
      std::cout << cmd_parser.usage() << std::endl
                << std::endl;
      return 0;
    } else if (cmd_parser.all_languages()) {
      HttpGetRequest request{BASE_URL + "languages"};
      // TODO: handle JSON
      std::cout << request() << std::endl;
    } else if (cmd_parser.all_word_classes()) {
      HttpGetRequest request{BASE_URL + "word_classes"};
      // TODO: handle JSON
      std::cout << request() << std::endl;
    } else if (cmd_parser.all_genders()) {
      HttpGetRequest request{BASE_URL + "genders"};
      // TODO: handle JSON
      std::cout << request() << std::endl;
    } else if (cmd_parser.all_numeri()) {
      HttpGetRequest request{BASE_URL + "numeri"};
      // TODO: handle JSON
      std::cout << request() << std::endl;
    } else if (cmd_parser.has_language_id()) {
      HttpGetRequest request{BASE_URL + "/language/id/" + cmd_parser.language_id()};
      // TODO: handle JSON
      std::cout << request() << std::endl;
    } else if (cmd_parser.has_language_name()) {
      HttpGetRequest request{BASE_URL + "/language/name/" + cmd_parser.language_name()};
      // TODO: handle JSON
      std::cout << request() << std::endl;
    } else if (cmd_parser.has_word_class_id()) {
      HttpGetRequest request{BASE_URL + "/word_class/id/" + cmd_parser.word_class_id()};
      // TODO: handle JSON
      std::cout << request() << std::endl;
    } else if (cmd_parser.has_word_class_name()) {
      HttpGetRequest request{BASE_URL + "/word_class/name/" + cmd_parser.word_class_name()};
      // TODO: handle JSON
      std::cout << request() << std::endl;
    } else if (cmd_parser.has_gender_id()) {
      HttpGetRequest request{BASE_URL + "/gender/id/" + cmd_parser.gender_id()};
      // TODO: handle JSON
      std::cout << request() << std::endl;
    } else if (cmd_parser.has_gender_name()) {
      HttpGetRequest request{BASE_URL + "/gender/name/" + cmd_parser.gender_name()};
      // TODO: handle JSON
      std::cout << request() << std::endl;
    } else if (cmd_parser.has_phrase()) {
      HttpPostRequest request{BASE_URL + "translation/" + cmd_parser.in() + "/" + cmd_parser.out() + "/",
                              create_post_data(cmd_parser)};
      // TODO: handle JSON
      std::cout << request() << std::endl;
    } else {
      std::cerr << "Expected an argument representing a phrase to be translated!" << std::endl
                << "Try \"" << *argv << " --help\" for more information" << std::endl;
      return 1;
    }
    return 0;
  } catch (CommandLineException& err) {
    std::cerr << "Bad command line syntax: " << err.what() << std::endl
              << "Try \"" << *argv << " --help\" for more information" << std::endl;
    return 1;
  } catch (HttpException &err) {
    std::cerr << "Could not contact the server: " << err.what() << std::endl;
    return 2;
  }
}
