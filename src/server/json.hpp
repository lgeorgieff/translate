// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/30/2015
// Description: Declares a class with several static methods for transforming DB query results into JSON string.
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

#ifndef JSON_HPP_
#define JSON_HPP_

#include "db_query.hpp"

#include "json/json.h"

#include <string>
#include <vector>
#include <map>

namespace lgeorgieff {
namespace translate {
namespace server {

// A static class that transforms DB results in form of DbQuery instances into JSON strings.
class JSON {
 public:
  // Transforms a set of all languages from te DB into a JSON array.
  static std::string all_languages_to_json(const DbQuery &);
  // Transforms a language name from the DB into a JSON string.
  static std::string language_name_to_json(const DbQuery &);
  // Transforms a language id from the DB into a JSON string.
  static std::string language_id_to_json(const DbQuery &);

  // Transforms a set of all word classes from te DB into a JSON array.
  static std::string all_word_classes_to_json(const DbQuery &);
  // Transforms a word class name from the DB into a JSON object.
  static std::string word_class_name_to_json(const DbQuery &);
  // Transforms a word class id from the DB into a JSON object.
  static std::string word_class_id_to_json(const DbQuery &);

  // Transforms a set of all genders from te DB into a JSON array.
  static std::string all_genders_to_json(const DbQuery &);
  // Transforms a gender name from the DB into a JSON string.
  static std::string gender_name_to_json(const DbQuery &);
  // Transforms a gender id from the DB into a JSON string.
  static std::string gender_id_to_json(const DbQuery &);

  // Transforms a set of all numeri from te DB into a JSON array.
  static std::string all_numeri_to_json(const DbQuery &);

  // Transforms a DB result for a phrase translation corresponding to the passed DbQuery and Json::Value.
  // The Json::Value string contains the request (POST) data from the user that specifies what the answer should contain, e.g.
  // comments, abbreviations, ...
  static std::string phrase_to_json(const DbQuery &, const Json::Value &);
 private:
  JSON() = delete;
  JSON(const JSON &) = delete;
  JSON &operator=(const JSON &) = delete;
  JSON(JSON &&) = delete;
  JSON &operator=(JSON &&) = delete;
  ~JSON() = delete;

  // A private helper method for transforming a DB result with multiple rows and multiple columns into a JSON array of
  // objects.
  static std::string generic_multiple_result_to_json(const DbQuery &, const std::vector<std::string> &,
                                                     const std::map<std::string, std::string> &);
  // A private helper method for transforming a DB result with a single row and a single column into a JSON string.
  static std::string generic_single_result_to_json(const DbQuery &, const std::string &);
  // A private helper method for transforming a DB result with multiple rowa and a single column into a JSON array of
  // strings.
  static std::string generic_single_complex_result_to_json(const DbQuery &, const std::vector<std::string> &,
                                                           const std::map<std::string, std::string> &);
};  // JSON

}  // server
}  // translate
}  // lgeorgieff

#endif  // JSON_HPP_
