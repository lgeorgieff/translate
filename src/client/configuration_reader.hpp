// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/14/2015
// Description: Declares the interface of the configuration reader for the client. It reads a json string from the
//              specified file and offers some getters for the parsed data.
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
// The format of the configuration file looks as follows:
//
// {
//    "service_address" : <STRING>,
//    "service_port" : <NUMBER>,
//    "service_url_prefix" : <STRING>,
//    "show_phrase" : <BOOLEAN>,
//    "show_word_class" : <BOOLEAN>,
//    "show_gender" : <BOOLEAN>,
//    "show_numerus" : <BOOLEAN>,
//    "show_abbreviation" : <BOOLEAN>,
//    "show_comment" : <BOOLEAN>,
//    "language_in": <STRING>,
//    "language_out": <STRING>
// }
//
// If one of the presented values is not defined, the following default value will be used for the particular
// attribute:
//  *service_address: "localhost"
//  *service_port: 8885
//  *service_url_prefix: "trlt"
//  *show_phrase: true
//  *show_word_class: false
//  *show_gender: false
//  *show_numerus: false
//  *show_abbreviation: false
//  *show_comment: false
//  *language_in: "DE"
//  *language_out: "EN"
// ====================================================================================================================

#ifndef CONFIGURATION_READER_HPP_
#define CONFIGURATION_READER_HPP_

#include <cstddef>
#include <string>

namespace lgeorgieff {
namespace translate {
namespace client {

class ConfigurationReader {
 public:
  static const std::string SERVICE_ADDRESS_DEFAULT;
  static const std::string SERVICE_URL_PREFIX_DEFAULT;
  static const std::string LANGUAGE_IN_DEFAULT;
  static const std::string LANGUAGE_OUT_DEFAULT;
  static const size_t SERVICE_PORT_DEFAULT;
  static const bool SHOW_PHRASE_DEFAULT;
  static const bool SHOW_WORD_CLASS_DEFAULT;
  static const bool SHOW_GENDER_DEFAULT;
  static const bool SHOW_NUMERUS_DEFAULT;
  static const bool SHOW_ABBREVIATON_DEFAULT;
  static const bool SHOW_COMMENT_DEFAULT;

  explicit ConfigurationReader(const std::string &);
  ConfigurationReader(const ConfigurationReader &) = default;
  ConfigurationReader(ConfigurationReader &&) = default;
  ConfigurationReader &operator=(const ConfigurationReader &) = default;
  ConfigurationReader &operator=(ConfigurationReader &&) = default;
  ~ConfigurationReader() = default;

  // Reads the confgiuration file and sets all internal values depending on the parsed data.
  // May throw lgeorgieff::translate::utils::JsonException and lgeorgieff::translate::utils::Exception.
  ConfigurationReader &operator()();

  std::string configuration_file_path() const noexcept;
  std::string config_file_path() const noexcept;
  std::string service_address() const noexcept;
  std::string service_url_prefix() const noexcept;
  std::string language_in() const noexcept;
  std::string language_out() const noexcept;
  size_t service_port() const noexcept;
  bool show_phrase() const noexcept;
  bool show_word_class() const noexcept;
  bool show_gender() const noexcept;
  bool show_numerus() const noexcept;
  bool show_abbreviation() const noexcept;
  bool show_comment() const noexcept;
  // Returns true if the configuration files was read and parsed successfully.
  bool available() const noexcept;

 private:
  std::string read_file_();

  std::string config_file_path_;
  std::string service_address_;
  std::string service_url_prefix_;
  std::string language_in_;
  std::string language_out_;
  size_t service_port_;
  bool show_phrase_;
  bool show_word_class_;
  bool show_gender_;
  bool show_numerus_;
  bool show_abbreviation_;
  bool show_comment_;
  bool available_;
};

}  // client
}  // translate
}  // lgeorgieff

#endif  // CONFIGURATION_READER_HPP_
