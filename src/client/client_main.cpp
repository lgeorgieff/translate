// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/14/2015
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
#include "utils/json_exception.hpp"
#include "command_line_parser.hpp"
#include "http_get_request.hpp"
#include "http_post_request.hpp"
#include "result_writer.hpp"
#include "configuration_reader.hpp"

#include <json/json.h>

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

using lgeorgieff::translate::CommandLineParser;
using lgeorgieff::translate::utils::CommandLineException;
using lgeorgieff::translate::utils::Exception;
using lgeorgieff::translate::utils::HttpException;
using lgeorgieff::translate::utils::JsonException;
using lgeorgieff::translate::client::HttpGetRequest;
using lgeorgieff::translate::client::HttpPostRequest;
using lgeorgieff::translate::client::ResultWriter;
using lgeorgieff::translate::client::ConfigurationReader;

// Create a JSON string based on the corresponding command line arguments
// that can be used for a post/translation request.
std::string create_post_data(const CommandLineParser &cmd_parser, const ConfigurationReader &config_reader) {
  Json::Value result;
  result["phrase"] = cmd_parser.phrase();
  result["show_phrase"] = cmd_parser.has_show_phrase() || !config_reader.available() ? cmd_parser.show_phrase()
                                                                                     : config_reader.show_phrase();
  result["show_word_class"] = cmd_parser.has_show_word_class() || !config_reader.available()
                                  ? cmd_parser.show_word_class()
                                  : config_reader.show_word_class();
  result["show_gender"] = cmd_parser.has_show_gender() || !config_reader.available() ? cmd_parser.show_gender()
                                                                                     : config_reader.show_gender();
  result["show_numerus"] = cmd_parser.has_show_numerus() || !config_reader.available() ? cmd_parser.show_numerus()
                                                                                       : config_reader.show_numerus();
  result["show_abbreviation"] = cmd_parser.has_show_abbreviation() || !config_reader.available()
                                    ? cmd_parser.show_abbreviation()
                                    : config_reader.show_abbreviation();
  result["show_comment"] = cmd_parser.has_show_comment() || !config_reader.available() ? cmd_parser.show_comment()
                                                                                       : config_reader.show_comment();
  Json::StreamWriterBuilder json_writer;
  json_writer.settings_["indentation"] = "";
  return Json::writeString(json_writer, result);
}

// A helper function that performs an HTTP get request and returns the reponse's string value.
// In error case an HttpException is thrown.
std::string process_request(const std::string &url) {
  HttpGetRequest request{url};
  request();
  if (200 != request.status_code()) {
    throw HttpException{"HTTP status code " + std::to_string(request.status_code()) + " for \"" + request.url() +
                        "\"!"};
  }
  return request.result();
}

int main(const int argc, const char **argv) {
  CommandLineParser cmd_parser;
  std::string config_path{"configuration.json"};
  try {
    struct passwd *pw = getpwuid(getuid());
    config_path = std::string{pw->pw_dir} + "/.trlt/configuration.json";
  } catch (...) {
  }  // try-catch get confguration file path

  ConfigurationReader config_reader{config_path};
  std::string base_url{"localhost:8885/trlt/"};
  try {
    cmd_parser(argc, argv);

    if (!cmd_parser.help()) {
      try {
        config_reader();
        base_url = config_reader.service_address() + ":" + std::to_string(config_reader.service_port());
        if (!config_reader.service_url_prefix().empty() && config_reader.service_url_prefix()[0] != '/')
          base_url += '/';
        base_url += config_reader.service_url_prefix();
        if (!config_reader.service_url_prefix().empty() &&
            config_reader.service_url_prefix()[config_reader.service_url_prefix().size() - 1] != '/')
          base_url += '/';
      } catch (const JsonException &err) {
        std::cerr << "Could not process configuration file: " << err.what() << std::endl;
      } catch (const Exception &err) {
        std::cerr << "Could not process configuration file: " << err.what() << std::endl;
      }
    }  // try-catch configuration reader

    ResultWriter writer{&std::cout};
    if (cmd_parser.help()) {
      std::cout << cmd_parser.usage() << std::endl
                << std::endl;
      return 0;
    } else if (cmd_parser.all_languages()) {
      writer.write_languages(process_request(base_url + "languages"));
    } else if (cmd_parser.all_word_classes()) {
      writer.write_word_classes(process_request(base_url + "word_classes"));
    } else if (cmd_parser.all_genders()) {
      writer.write_genders(process_request(base_url + "genders"));
    } else if (cmd_parser.all_numeri()) {
      writer.write_numeri(process_request(base_url + "numeri"));
    } else if (cmd_parser.has_language_id()) {
      writer.write_language_name(process_request(base_url + "language/id/" + cmd_parser.language_id()));
    } else if (cmd_parser.has_language_name()) {
      writer.write_language_id(process_request(base_url + "language/name/" + cmd_parser.language_name()));
    } else if (cmd_parser.has_word_class_id()) {
      writer.write_word_class_name(process_request(base_url + "word_class/id/" + cmd_parser.word_class_id()));
    } else if (cmd_parser.has_word_class_name()) {
      writer.write_word_class_id(process_request(base_url + "word_class/name/" + cmd_parser.word_class_name()));
    } else if (cmd_parser.has_gender_id()) {
      writer.write_gender_name(process_request(base_url + "gender/id/" + cmd_parser.gender_id()));
    } else if (cmd_parser.has_gender_name()) {
      writer.write_gender_id(process_request(base_url + "gender/name/" + cmd_parser.gender_name()));
    } else if (cmd_parser.has_phrase()) {
      std::string language_in{cmd_parser.has_in() ? cmd_parser.in() : config_reader.language_in()};
      std::string language_out{cmd_parser.has_out() ? cmd_parser.out() : config_reader.language_out()};
      HttpPostRequest request{base_url + "translation/" + language_in + "/" + language_out + "/",
                              create_post_data(cmd_parser, config_reader)};
      request();
      if (200 != request.status_code() && 404 != request.status_code()) {
        throw HttpException{"Could not process \"" + request.url() + "\", HTTP status code: " +
                            std::to_string(request.status_code())};
      } else if (404 != request.status_code()) {
        writer.write_translation(request.result());
      }
    } else {
      std::cerr << "Expected an argument representing a phrase to be translated!" << std::endl
                << "Try \"" << *argv << " --help\" for more information" << std::endl;
      return 1;
    }
    return 0;
  } catch (CommandLineException &err) {
    std::cerr << "Bad command line syntax: " << err.what() << std::endl
              << "Try \"" << *argv << " --help\" for more information" << std::endl;
    return 1;
  } catch (HttpException &err) {
    std::cerr << "Could not process server request: " << err.what() << std::endl;
    return 2;
  } catch (JsonException &err) {
    std::cerr << "Could not process the server's response: " << err.what() << std::endl;
    return 2;
  }  // try-catch command line parser
}
