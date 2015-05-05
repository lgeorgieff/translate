// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 05/05/2015
// Description: Defines a class with several static methods for transforming DB query results into JSON string.
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

#include "json.hpp"
#include "utils/json_exception.hpp"

#include <pqxx/pqxx>
#include <cstring>

namespace {
// A helper function that returns a boolean value which is extracted from the passed json object.
bool get_bool_from_json_object(const Json::Value &json_object, const std::string &member_name, bool default_value) {
  Json::Value result{json_object.get(member_name, default_value)};
  if (result.isBool())
    return result.asBool();
  else
    return default_value;
}

// A helper function that returns true if the passed json object contains a string member with the passed string value.
// Missing members are handled as empty string values, i.e. if member_value == "" and the actual member is missing on
// the passed object, true is returned.
// If a member is matched the given name but is not of type string, the return value is false.
bool json_object_contains_string_member(const Json::Value &json_object, const std::string &member_name,
                                        const std::string &member_value) {
  if (json_object.isMember(member_name)) {
    Json::Value member{json_object[member_name]};
    if (!member.isString()) return false;
    return member.asString() == member_value;
  }
  return true;
}

bool json_object_contains_value_in_array(const Json::Value &json_object, const std::string &array_name,
                                         const std::string &array_value) {
  if (json_object.isMember(array_name)) {
    Json::Value array{json_object[array_name]};
    if (!array.isArray()) return false;
    for (const Json::Value &array_iter : array)
      if (array_iter.isString() && array_iter.asString() == array_value) return true;
  }
  return false;
}

// A helper function that appends the passed array value to the array referenced by array_name inside the given json
// object. If no such member existed before this function is invoked, a new one is created.
void append_value_to_array_in_json_object(Json::Value &json_object, const std::string &array_name,
                                          const std::string &array_value) {
  if (json_object.isMember(array_name)) {
    json_object[array_name].append(array_value);
  } else {
    Json::Value array;
    array.append(array_value);
    json_object[array_name] = array;
  }
}
}  // anonymous namespace

namespace lgeorgieff {
namespace translate {
namespace server {

using lgeorgieff::translate::utils::JsonException;

const std::string JSON::JSON_INDENTATION_STRING{""};

std::string JSON::all_languages_to_json(const DbQuery &db_query) {
  return generic_multiple_result_to_json(db_query, {"id", "name"}, {{"id", "id"}, {"name", "language"}});
}

std::string JSON::language_id_to_json(const DbQuery &db_query) {
  return generic_single_result_to_json(db_query, "id");
}

std::string JSON::language_name_to_json(const DbQuery &db_query) {
  return generic_single_result_to_json(db_query, "name");
}

std::string JSON::all_word_classes_to_json(const DbQuery &db_query) {
  return generic_multiple_result_to_json(db_query, {"id", "name"}, {{"id", "id"}, {"name", "word_class"}});
}

std::string JSON::word_class_id_to_json(const DbQuery &db_query) {
  return generic_single_result_to_json(db_query, "id");
}

std::string JSON::word_class_name_to_json(const DbQuery &db_query) {
  return generic_single_result_to_json(db_query, "name");
}

std::string JSON::all_genders_to_json(const DbQuery &db_query) {
  return generic_multiple_result_to_json(db_query, {"id", "description", "name"},
                                         {{"id", "id"}, {"name", "gender"}, {"description", "description"}});
}

std::string JSON::gender_name_to_json(const DbQuery &db_query) {
  return generic_single_complex_result_to_json(db_query, {"name", "description"},
                                               {{"name", "gender"}, {"description", "description"}});
}

std::string JSON::gender_id_to_json(const DbQuery &db_query) {
  return generic_single_complex_result_to_json(db_query, {"id", "description"},
                                               {{"id", "id"}, {"description", "description"}});
}

std::string JSON::all_numeri_to_json(const DbQuery &db_query) {
  Json::Value result;
  for (const pqxx::tuple &row : db_query) {
    if (row.size() != 1) {
      throw JsonException("Cannot transform DB result to JSON, expected 1 column but found " +
                          std::to_string(row.size()) + "!");
    }
    if (std::strcmp(row[0].name(), "unnest"))
      throw JsonException("Cannot transform DB result to JSON, column names do not match!");
    std::string str_container;
    row[0].to(str_container);
    result.append(str_container);
  }
  return json_value_to_string(result);
}

// Returns the following JSON string:
//
//  [{
//    "phrase": "<phrase out>", // if show_phrase
//    "word_class": "<word_class", // if show_word_class
//    "gender": "<gender>", // if show_gender
//    "numerus": "<numerus>", // if show_numerus
//    "abbreviations": [ "<abbreviation 1>", "<abbreviation 2>" ], // if show_abbreviation
//    "comments": [ "<comment 1>", "<comment 2>" ] // if show_comment
//   },
//   {...}
//  ]
std::string JSON::phrase_to_json(const DbQuery &db_query, const Json::Value &user_options) {
  bool show_phrase{true}, show_word_class{true}, show_gender{true}, show_numerus{true}, show_abbreviation{true},
      show_comment{true};
  if (user_options.isObject()) {
    show_phrase = get_bool_from_json_object(user_options, "show_phrase", show_phrase);
    show_word_class = get_bool_from_json_object(user_options, "show_word_class", show_word_class);
    show_gender = get_bool_from_json_object(user_options, "show_gender", show_gender);
    show_numerus = get_bool_from_json_object(user_options, "show_numerus", show_numerus);
    show_abbreviation = get_bool_from_json_object(user_options, "show_abbreviation", show_abbreviation);
    show_comment = get_bool_from_json_object(user_options, "show_comment", show_comment);
  }

  static std::vector<std::string> column_names{
      "language_in",  "phrase_in",  "word_class_in",  "gender_in",  "numerus_in",  "abbreviation_in",  "comment_in",
      "language_out", "phrase_out", "word_class_out", "gender_out", "numerus_out", "abbriviation_out", "comment_out"};

  Json::Value result{Json::arrayValue};
  for (const pqxx::tuple &row : db_query) {
    if (row.size() != column_names.size()) {
      throw JsonException("Cannot transform DB result to JSON, expected " + std::to_string(column_names.size()) +
                          " column but found " + std::to_string(row.size()) + "!");
    }
    std::string current_phrase, current_word_class, current_gender, current_numerus, current_abbreviation,
        current_comment;
    // 8 => start at phrase_out
    for (size_t field_pos{8}; field_pos < column_names.size(); ++field_pos) {
      if (column_names[field_pos] != row[static_cast<int>(field_pos)].name())
        throw JsonException("Cannot transform DB result to JSON, column names do not match!");
      if (!strcmp(row[static_cast<int>(field_pos)].name(), "phrase_out") && show_phrase)
        row[static_cast<int>(field_pos)].to(current_phrase);
      else if (!strcmp(row[static_cast<int>(field_pos)].name(), "word_class_out") && show_word_class)
        row[static_cast<int>(field_pos)].to(current_word_class);
      else if (!strcmp(row[static_cast<int>(field_pos)].name(), "gender_out") && show_gender)
        row[static_cast<int>(field_pos)].to(current_gender);
      else if (!strcmp(row[static_cast<int>(field_pos)].name(), "numerus_out") && show_numerus)
        row[static_cast<int>(field_pos)].to(current_numerus);
      else if (!strcmp(row[static_cast<int>(field_pos)].name(), "abbreviation_out") && show_abbreviation)
        row[static_cast<int>(field_pos)].to(current_abbreviation);
      else if (show_comment)
        row[static_cast<int>(field_pos)].to(current_comment);
    }  // for(size_t field_pos{8}; field_pos < column_names.size(); ++field_pos)

    Json::Value *basic_item{nullptr};
    for (Json::Value &result_object : result) {
      if (json_object_contains_string_member(result_object, "phrase", current_phrase) &&
          json_object_contains_string_member(result_object, "word_class", current_word_class) &&
          json_object_contains_string_member(result_object, "gender", current_gender) &&
          json_object_contains_string_member(result_object, "numerus", current_numerus)) {
        basic_item = &result_object;
        break;
      }
    }
    if (basic_item) {
      if (!current_abbreviation.empty() &&
          !json_object_contains_value_in_array(*basic_item, "abbreviations", current_abbreviation))
        append_value_to_array_in_json_object(*basic_item, "abbreviations", current_abbreviation);
      if (!current_comment.empty() && !json_object_contains_value_in_array(*basic_item, "comments", current_comment))
        append_value_to_array_in_json_object(*basic_item, "comments", current_comment);
    } else {
      Json::Value new_item;
      if (show_phrase && !current_phrase.empty()) new_item["phrase"] = current_phrase;
      if (show_word_class && !current_word_class.empty()) new_item["word_class"] = current_word_class;
      if (show_gender && !current_gender.empty()) new_item["gender"] = current_gender;
      if (show_numerus && !current_numerus.empty()) new_item["numerus"] = current_numerus;
      if (show_abbreviation && !current_abbreviation.empty()) {
        Json::Value array;
        array.append(current_abbreviation);
        new_item["abbreviations"] = array;
      }
      if (show_comment && !current_comment.empty()) {
        Json::Value array;
        array.append(current_comment);
        new_item["comments"] = array;
      }
      if (!new_item.isNull()) result.append(new_item);
    }

  }  // for(const pqxx::tuple &row : db_query)
  return json_value_to_string(result);
}  // JSON::phrase_to_json

std::string JSON::generic_multiple_result_to_json(const DbQuery &db_query,
                                                  const std::vector<std::string> &column_names,
                                                  const std::map<std::string, std::string> &name_mapping) {
  Json::Value result;
  for (const pqxx::tuple &row : db_query) {
    if (row.size() != column_names.size())
      throw JsonException("Cannot transform DB result to JSON, column names do not match!");
    Json::Value json_row;
    for (size_t row_pos{0}; row_pos != column_names.size(); ++row_pos) {
      if (column_names[row_pos] != row[static_cast<int>(row_pos)].name())
        throw JsonException("Cannot transform DB result to JSON, column names do not match!");
      std::string str_container;
      if (name_mapping.find(column_names[row_pos]) == name_mapping.end())
        throw JsonException("Cannot transform DB result to JSON, no mapping for column to JSON member!");
      row[static_cast<int>(row_pos)].to(str_container);
      const std::string identifier{name_mapping.at(column_names[row_pos])};
      json_row[identifier] = str_container;
    }
    result.append(json_row);
  }
  return json_value_to_string(result);
}

std::string JSON::generic_single_result_to_json(const DbQuery &db_query, const std::string &column_name) {
  Json::Value result;
  if (db_query.size() == 1) {
    std::string str_container;
    if (db_query.begin().size() != 1)
      throw JsonException("Cannot transform DB result to JSON string, expected one value, but found " +
                          std::to_string(db_query.begin().size()) + "!");
    if (column_name != db_query.begin()[0].name())
      throw JsonException("Cannot transform DB result to JSON string, expected column name \"" + column_name +
                          "\" but found \"" + db_query.begin()[0].name() + "\"!");
    db_query.begin()[0].to(str_container);
    result = str_container;
  } else if (db_query.size() > 1) {
    throw JsonException("Cannot transform multiple DB results (" + std::to_string(db_query.size()) +
                        std::string{") to JSON string!"});
  }
  return json_value_to_string(result);
}

std::string JSON::generic_single_complex_result_to_json(const DbQuery &db_query,
                                                        const std::vector<std::string> &column_names,
                                                        const std::map<std::string, std::string> &name_mapping) {
  Json::Value result;
  if (db_query.size() == 1) {
    std::string str_container;
    if (db_query.begin().size() != column_names.size())
      throw JsonException("Cannot transform DB result to JSON string, expected " +
                          std::to_string(column_names.size()) + " values, but found " +
                          std::to_string(db_query.begin().size()) + "!");

    const pqxx::tuple row{db_query.begin()};
    for (size_t row_pos{0}; row_pos != column_names.size(); ++row_pos) {
      if (column_names[row_pos] != row[static_cast<int>(row_pos)].name())
        throw JsonException("Cannot transform DB result to JSON, column names do not match!");
      std::string str_container;
      if (name_mapping.find(column_names[row_pos]) == name_mapping.end())
        throw JsonException("Cannot transform DB result to JSON, no mapping for column to JSON member!");
      row[static_cast<int>(row_pos)].to(str_container);
      const std::string identifier{name_mapping.at(column_names[row_pos])};
      result[identifier] = str_container;
    }

  } else if (db_query.size() > 1) {
    throw JsonException("Cannot transform multiple DB results (" + std::to_string(db_query.size()) +
                        std::string{") to JSON string!"});
  }
  return json_value_to_string(result);
}

std::string JSON::json_value_to_string(const Json::Value &value) {
  Json::StreamWriterBuilder json_writer;
  json_writer.settings_["indentation"] = "";
  return Json::writeString(json_writer, value);
}

}  // server
}  // translate
}  // lgeorgieff
