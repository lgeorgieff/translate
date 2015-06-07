// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/07/2015
// Description: Implements the class ConfigurationReader which reads the configuration for the client. It reads a json
//              string from the specified file and offers some getters for the parsed data.
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

#include "configuration_reader.hpp"
#include "utils/exception.hpp"
#include "utils/json_exception.hpp"

#include "json/json.h"

#include <fstream>
#include <streambuf>
#include <cerrno>

namespace {
// Tries to read the boolean member with member_name from json_object and finally the parsed boolean value is written
// into the destination.
void process_json_value(const Json::Value &json_object, const std::string &member_name, bool &destination) {
  if (json_object.isMember(member_name)) {
    if (!json_object[member_name].isBool()) {
      throw lgeorgieff::translate::utils::JsonException{
          "Cannot process the json data " + json_object[member_name].toStyledString() + ". Expected a boolean value!"};
    }
    destination = json_object["service_port"].asBool();
  }
}
}

namespace lgeorgieff {
namespace translate {
namespace client {

using lgeorgieff::translate::utils::JsonException;
using lgeorgieff::translate::utils::Exception;

const std::string ConfigurationReader::SERVICE_ADDRESS_DEFAULT{"localhost"};
const size_t ConfigurationReader::SERVICE_PORT_DEFAULT{8885};
const bool ConfigurationReader::SHOW_PHRASE_DEFAULT{true};
const bool ConfigurationReader::SHOW_WORD_CLASS_DEFAULT{};
const bool ConfigurationReader::SHOW_GENDER_DEFAULT{};
const bool ConfigurationReader::SHOW_NUMERUS_DEFAULT{};
const bool ConfigurationReader::SHOW_ABBREVIATON_DEFAULT{};
const bool ConfigurationReader::SHOW_COMMENT_DEFAULT{};

ConfigurationReader::ConfigurationReader(const std::string &configuration_file_path)
    : config_file_path_{configuration_file_path},
      service_address_{SERVICE_ADDRESS_DEFAULT},
      service_port_{SERVICE_PORT_DEFAULT},
      show_phrase_{SHOW_PHRASE_DEFAULT},
      show_word_class_{SHOW_WORD_CLASS_DEFAULT},
      show_gender_{SHOW_GENDER_DEFAULT},
      show_numerus_{SHOW_NUMERUS_DEFAULT},
      show_abbreviation_{SHOW_ABBREVIATON_DEFAULT},
      show_comment_{SHOW_COMMENT_DEFAULT},
      available_{} {}

ConfigurationReader &ConfigurationReader::operator()() {
  std::string json_string{this->read_file_()};
  Json::Reader reader;
  Json::Value json;
  if (!reader.parse(json_string, json)) throw JsonException{"The json data " + json_string + " cannot be parsed!"};

  if (json.isNull()) return *this;
  if (!json.isObject())
    throw JsonException{"Cannot process the json data " + json_string + ". Expected a json object!"};

  if (json.isMember("service_address")) {
    if (!json["service_address"].isString()) {
      throw JsonException{"Cannot process the json data " + json["service_address"].toStyledString() +
                          ". Expected a json string!"};
    }
    this->service_address_ = json["service_address"].asString();
  }
  if (json.isMember("service_port")) {
    if (!json["service_port"].isUInt()) {
      throw JsonException{"Cannot process the json data " + json["service_port"].toStyledString() +
                          ". Expected a positive json number!"};
    }
    this->service_port_ = json["service_port"].asUInt();
  }
  process_json_value(json, "show_phrase", this->show_phrase_);
  process_json_value(json, "show_word_class", this->show_word_class_);
  process_json_value(json, "show_gender", this->show_gender_);
  process_json_value(json, "show_numerus", this->show_numerus_);
  process_json_value(json, "show_abbreviation", this->show_abbreviation_);
  process_json_value(json, "show_comment", this->show_comment_);

  this->available_ = true;
  return *this;
}

std::string ConfigurationReader::configuration_file_path() const noexcept { return this->config_file_path_; }

std::string ConfigurationReader::service_address() const noexcept { return this->service_address_; }

size_t ConfigurationReader::service_port() const noexcept { return this->service_port_; }

bool ConfigurationReader::show_phrase() const noexcept { return this->show_phrase_; }

bool ConfigurationReader::show_word_class() const noexcept { return this->show_word_class_; }

bool ConfigurationReader::show_gender() const noexcept { return this->show_gender_; }

bool ConfigurationReader::show_numerus() const noexcept { return this->show_numerus_; }

bool ConfigurationReader::show_abbreviation() const noexcept { return this->show_abbreviation_; }

bool ConfigurationReader::show_comment() const noexcept { return this->show_comment_; }

bool ConfigurationReader::available() const noexcept { return this->available_; }

std::string ConfigurationReader::read_file_() {
  std::ifstream in{this->config_file_path_, std::ifstream::in};
  if (in) {
    return std::string{std::istreambuf_iterator<char>{in}, std::istreambuf_iterator<char>{}};
  } else {
    throw Exception{"Cannot read the file \"" + this->config_file_path_ + "\"! Errno: " + std::to_string(errno)};
  }
}

}  // client
}  // translate
}  // lgeorgieff
