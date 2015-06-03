// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/04/2015
// Description: Implements the class ResultWriter which provides functionality that writes the translation results
//              (json strings) into a stream in a pretty format.
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

#include "result_writer.hpp"
#include "utils/helper.hpp"
#include "utils/json_exception.hpp"

#include "json/json.h"

#include <utility>
#include <map>
#include <set>

namespace {
// Returns a json value the is the pared result of the give json string.
Json::Value from_json_string(const std::string json_string) {
  Json::Reader reader;
  Json::Value root;
  if (!reader.parse(json_string, root))
    throw lgeorgieff::translate::utils::JsonException{"The json data " + json_string + " cannot be parsed!"};
  return root;
}

// A helper function that writes the passed item into the ostream target.
// The written string is checked for length and adapted to the defined max_length.
void write_item_to_stream(const std::string& data, size_t max_length, size_t tab_space, std::ostream* destination,
                          bool is_last_item) {
  size_t local_tab_space;
  size_t local_max_length;
  if (is_last_item) {
    local_tab_space = local_max_length = 0;
  } else {
    local_tab_space = tab_space;
    local_max_length = max_length;
  }
  ssize_t padding;
  if (local_max_length + local_tab_space < data.size())
    padding = 0;
  else
    padding = local_max_length + local_tab_space - data.size();
  *destination << data << std::string(padding < 0 ? 0 : padding, ' ');
}
}  // anonymous namespace

namespace lgeorgieff {
namespace translate {
namespace client {

using lgeorgieff::translate::utils::JsonException;

const size_t ResultWriter::TAB_SPACE{4};

ResultWriter::ResultWriter(std::ostream* destination) : destination_{destination} {}

ResultWriter::ResultWriter(const ResultWriter& other) : destination_{other.destination_} {}

ResultWriter::ResultWriter(ResultWriter&& other) : destination_{std::move(other.destination_)} {}

ResultWriter::~ResultWriter() { this->destination_ = nullptr; }

ResultWriter& ResultWriter::operator=(const ResultWriter& other) {
  this->destination_ = other.destination_;
  return *this;
}

ResultWriter& ResultWriter::operator=(ResultWriter&& other) {
  this->destination_ = std::move(other.destination_);
  return *this;
}

bool ResultWriter::operator==(const ResultWriter& other) { return this->destination_ == other.destination_; }

bool ResultWriter::operator!=(const ResultWriter& other) { return !(*this == other); }

void ResultWriter::write_json_array_string(const std::string& json_string,
                                           const std::vector<std::string>& object_members, bool write_headers) {
  Json::Value data{from_json_string(json_string)};
  if (data.isNull()) return;
  if (!data.isArray()) throw JsonException{"Cannot process the json data " + json_string + ". Expected a json array!"};
  // We just go through the data and check for completeness, validity and chek the max length of each member type.
  // We don't write into the set ostream here until it is clear that all data is valid.
  std::map<std::string, size_t> max_lengths;
  for (const Json::Value& item : data) {
    if (!item.isObject()) throw JsonException{"Cannot process the json data " + item.toStyledString() + ". Expected a json object!"};
    for (const std::string member_name : object_members) {
      if (!item.isMember(member_name) || !item[member_name].isString())
        throw JsonException{"Cannot process the json data " + item[member_name].toStyledString() + ". Expected a json string!"};
      if (max_lengths.end() == max_lengths.find(member_name) ||
          max_lengths[member_name] < item[member_name].asString().size()) {
        max_lengths[member_name] = item[member_name].asString().size();  // TODO: handle unicode string length
      }
    }
  }

  // Write the header line
  if (write_headers) {
    for (const std::string& member_name : object_members)
      if (max_lengths[member_name] < member_name.size()) max_lengths[member_name] = member_name.size();

    for (size_t member_pos{0}; member_pos != object_members.size(); ++member_pos) {
      write_item_to_stream(lgeorgieff::translate::utils::to_upper_case(object_members[member_pos]),
                           max_lengths[object_members[member_pos]], TAB_SPACE, this->destination_,
                           member_pos == object_members.size() - 1);
    }
    *(this->destination_) << std::endl;
  }

  // Write the data to its destination.
  for (size_t array_pos{0}; array_pos != data.size(); ++array_pos) {
    for (size_t member_pos{0}; member_pos != object_members.size(); ++member_pos) {
      write_item_to_stream(data[static_cast<int>(array_pos)][object_members[member_pos]].asString(),
                           max_lengths[object_members[member_pos]], TAB_SPACE, this->destination_,
                           member_pos == object_members.size() - 1);
    }
    *(this->destination_) << std::endl;
  }
}

void ResultWriter::write_languages(const std::string& languages) {
  this->write_json_array_string(languages, std::vector<std::string>{"id", "language"}, true);
}

void ResultWriter::write_word_classes(const std::string& word_classes) {
  this->write_json_array_string(word_classes, std::vector<std::string>{"id", "word_class"}, true);
}

void ResultWriter::write_genders(const std::string& genders) {
  this->write_json_array_string(genders, std::vector<std::string>{"id", "gender", "description"}, true);
}

void ResultWriter::write_numeri(const std::string& numeri) {
  Json::Value data{from_json_string(numeri)};
  if (data.isNull()) return;
  if (!data.isArray()) throw JsonException{"Cannot process the json data " + numeri + ". Expected a json array!"};

  *(this->destination_) << "NUMERUS" << std::endl;
  for (const Json::Value numerus : data) *(this->destination_) << numerus.asString() << std::endl;
}

void ResultWriter::write_json_string_string(const std::string& json_string) {
  Json::Value data{from_json_string(json_string)};
  if (data.isNull()) return;
  if (!data.isString()) throw JsonException{"Cannot process the json data " + json_string + ". Expected a json string!"};
  *(this->destination_) << data.asString() << std::endl;
}

void ResultWriter::write_language_id(const std::string& language_id) { this->write_json_string_string(language_id); }

void ResultWriter::write_language_name(const std::string& language_name) {
  this->write_json_string_string(language_name);
}

void ResultWriter::write_word_class_id(const std::string& word_class_id) {
  this->write_json_string_string(word_class_id);
}

void ResultWriter::write_word_class_name(const std::string& word_class_name) {
  this->write_json_string_string(word_class_name);
}

void ResultWriter::write_json_object_string(const std::string& json_string,
                                            const std::vector<std::string>& member_names) {
  Json::Value data{from_json_string(json_string)};
  if (data.isNull()) return;
  if (!data.isObject()) throw JsonException{"Cannot process the json data " + json_string + ". Expected a json object!"};

  for(size_t i{0}; member_names.size() != i; ++i) {
    if (!data.isMember(member_names[i])) {
      throw JsonException{"Cannot process the json data " + json_string + ". Expected a the member \"" +
                          member_names[i] + "\"!"};
    }
    if (!data[member_names[i]].isString()) {
      throw JsonException{"Cannot process the json data " + json_string +
                          ". Expected a json object containing only string members!"};
    }
    *(this->destination_) << member_names[i] << ": " << data[member_names[i]].asString();
    if (member_names.size() - 1 > i) *(this->destination_) << ", ";
    else *(this->destination_) << std::endl;
  }
}
  
void ResultWriter::write_gender_id(const std::string& gender_id) {
  this->write_json_object_string(gender_id, {"id", "description"});
}

void ResultWriter::write_gender_name(const std::string& gender_name) {
  this->write_json_object_string(gender_name, {"gender", "description"});
}

void ResultWriter::write_translation(const std::string& translation) {
  // Writes a json string of the form [{"phrase": "...", "word_class": "...", "gender": "...", "numerus": "...",
  // "abbreviations": [ "..." ], "comments": [ "..." ] } ] into the set ostream in the form
  // <PHRASE> {<WORD_CLASS>} {<GENDER>} {<NUMERUS>} <<ABREVIATION-1>, <...>, <ABBREVIATION-N>> [<COMMENT-1>] [<...>]
  // [<COMMENT-N>]
  // <PHRASE> {<WORD_CLASS>} {<GENDER>} {<NUMERUS>} <<ABREVIATION-1>, <...>, <ABBREVIATION-N>> [<COMMENT-1>] [<...>]
  // [<COMMENT-N>]
  Json::Value data{from_json_string(translation)};
  if (data.isNull()) return;
  if (!data.isArray()) throw JsonException{"Cannot process the json data " + translation + ". Expected a json array!"};

  for (const Json::Value& item : data) {
    if (item.isNull()) continue;
    if (!item.isObject())
      throw JsonException{"Cannot process the json data " + item.toStyledString() + ". Expected a json object!"};
    std::string phrase;
    if (item.isMember("phrase") && !item["phrase"].isNull()) {
      if (!item["phrase"].isString()) {
        throw JsonException{"Cannot process the json data " + item["phrase"].toStyledString() +
                            ". Expected a json string!"};
      }
      phrase = item["phrase"].asString();
    }
    std::string word_class;
    if (item.isMember("word_class") && !item["word_class"].isNull()) {
      if (!item["word_class"].isString()) throw JsonException{"Cannot process the json data " + item["word_class"].toStyledString() + ". Expected a json string!"};
      word_class = item["word_class"].asString();
    }
    std::string gender;
    if (item.isMember("gender") && !item["gender"].isNull()) {
      if (!item["gender"].isString()) throw JsonException{"Cannot process the json data " + item["gender"].toStyledString() + ". Expected a json string!"};
      gender = item["gender"].asString();
    }
    std::string numerus;
    if (item.isMember("numerus") && !item["numerus"].isNull()) {
      if (!item["numerus"].isString()) throw JsonException{"Cannot process the json data " + item["numerus"].toStyledString() + ". Expected a json string!"};
      numerus = item["numerus"].asString();
    }
    std::set<std::string> abbreviations;
    if (item.isMember("abbreviations") && !item["abbreviations"].isNull()) {
      if (!item["abbreviations"].isArray()) throw JsonException{"Cannot process the json data " + item["abbreviations"].toStyledString() + ". Expected a json array!"};
      for (const Json::Value& abbreviation : item["abbreviations"]) {
        if (abbreviation.isNull()) continue;
        if (!abbreviation.isString()) throw JsonException{"Cannot process the json data " + abbreviation.toStyledString() + ". Expected a json string!"};
        abbreviations.insert(abbreviation.asString());
      }
    }
    std::set<std::string> comments;
    if (item.isMember("comments") && !item["comments"].isNull()) {
      if (!item["comments"].isArray()) {
        throw JsonException{"Cannot process the json data " + item["comments"].toStyledString() +
                            ". Expected a json array!"};
      }
      for (const Json::Value& comment : item["comments"]) {
        if (comment.isNull()) continue;
        if (!comment.isString()) throw JsonException{"Cannot process the json data " + comment.toStyledString() + ". Expected a json string!"};
        comments.insert(comment.asString());
      }
    }

    if (!phrase.empty()) {
      *(this->destination_) << phrase;
      if (!word_class.empty() || !gender.empty() || !numerus.empty() || !abbreviations.empty() || !comments.empty())
        *(this->destination_) << " ";
    }
    if (!word_class.empty()) {
      *(this->destination_) << "{" << word_class << "}";
      if (!gender.empty() || !numerus.empty() || !abbreviations.empty() || !comments.empty())
        *(this->destination_) << " ";
    }
    if (!gender.empty()) {
      *(this->destination_) << "{" << gender << "}";
      if (!numerus.empty() || !abbreviations.empty() || !comments.empty()) *(this->destination_) << " ";
    }
    if (!numerus.empty()) {
      *(this->destination_) << "{" << numerus << "}";
      if (!abbreviations.empty() || !comments.empty()) *(this->destination_) << " ";
    }
    if (!abbreviations.empty()) {
      std::set<std::string>::const_iterator end_iter{abbreviations.cend()};
      std::set<std::string>::const_iterator before_end_iter{--abbreviations.cend()};
      for (std::set<std::string>::const_iterator iter{abbreviations.cbegin()}; iter != end_iter; ++iter) {
        *(this->destination_) << "<" << *iter << ">";
        if (iter != before_end_iter) *(this->destination_) << " ";
      }
      if (!comments.empty()) *(this->destination_) << " ";
    }
    if (!comments.empty()) {
      std::set<std::string>::const_iterator end_iter{comments.cend()};
      std::set<std::string>::const_iterator before_end_iter{--comments.cend()};
      for (std::set<std::string>::const_iterator iter{comments.cbegin()}; iter != end_iter; ++iter) {
        *(this->destination_) << "[" << *iter << "]";
        if (iter != before_end_iter) *(this->destination_) << " ";
      }
    }
    *(this->destination_) << std::endl;
  }  // for (const Json::Value& item : data)
}
}  // client
}  // translate
}  // lgeorgieff
