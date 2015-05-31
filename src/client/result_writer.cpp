// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 05/31/2015
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

#include "json/json.h"

#include <utility>
#include <map>

namespace {
Json::Value from_json_string(const std::string json_string) {
  try {
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(json_string, root)) throw 1;  // TODO: implement error handling
    return root;
  } catch(...) { // TODO: check exception type
    throw 1; // TODO: implement error handling
  }
}

void write_item_to(const std::string& data, size_t max_length, size_t tab_space, std::ostream* destination,
                   bool is_last_item) {
  size_t local_tab_space;
  size_t local_max_length;
  if (is_last_item) {
    local_tab_space = local_max_length = 0;
  } else {
    local_tab_space = tab_space;
    local_max_length = max_length;
    ;
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
  if (!data.isArray()) throw 1;  // TODO: implement error handling
  // We just go through the data and check for completeness, validity and chek the max length of each member type.
  // We don't write into the set ostream here until it is clear that all data is valid.
  std::map<std::string, size_t> max_lengths;
  for (const Json::Value& item : data) {
    if (!item.isObject()) throw 1;  // TODO: implement error handling
    for (const std::string member_name : object_members) {
      if (!item.isMember(member_name) || !item[member_name].isString()) throw 1;  // TODO: implement error handling
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
      write_item_to(lgeorgieff::translate::utils::to_upper_case(object_members[member_pos]),
                    max_lengths[object_members[member_pos]], TAB_SPACE, this->destination_,
                    member_pos == object_members.size() - 1);
    }
    *(this->destination_) << std::endl;
  }

  // Write the data to its destination.
  for (size_t array_pos{0}; array_pos != data.size(); ++array_pos) {
    for (size_t member_pos{0}; member_pos != object_members.size(); ++member_pos) {
      write_item_to(data[static_cast<int>(array_pos)][object_members[member_pos]].asString(),
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
  if (!data.isArray()) throw 1;  // TODO: implement error handling

  *(this->destination_) << "NUMERUS" << std::endl;
  for (const Json::Value numerus : data) *(this->destination_) << numerus.asString() << std::endl;
}

void ResultWriter::write_json_string_string(const std::string& json_string) {
  Json::Value data{from_json_string(json_string)};
  if (!data.isString()) throw 1;  // TODO: implement error handling
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

void ResultWriter::write_gender_id(const std::string& gender_id) { this->write_json_string_string(gender_id); }

void ResultWriter::write_gender_name(const std::string& gender_name) { this->write_json_string_string(gender_name); }


}  // client
}  // translate
}  // lgeorgieff
