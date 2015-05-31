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

#include "json/json.h"

#include <utility>

namespace {
Json::Value from_json_string(const std::string json_string) {
  Json::Reader reader;
  Json::Value root;
  if(!reader.parse(json_string, root)) throw 1; // TODO: implement error handling
  return root;
}

} // anonymous namespace

namespace lgeorgieff {
namespace translate {
namespace client {

const size_t ResultWriter::TAB_SPACE{3};

ResultWriter::ResultWriter(std::ostream* destination) : destination_{destination} {}

ResultWriter::ResultWriter(const ResultWriter& other) : destination_{other.destination_} {}

ResultWriter::ResultWriter(ResultWriter&& other) : destination_{std::move(other.destination_)} {}

ResultWriter::~ResultWriter() {
  this->destination_ = nullptr;
}

ResultWriter& ResultWriter::operator=(const ResultWriter& other) {
  this->destination_ = other.destination_;
  return *this;
}

ResultWriter& ResultWriter::operator=(ResultWriter&& other) {
  this->destination_ = std::move(other.destination_);
  return *this;
}

bool ResultWriter::operator==(const ResultWriter& other) {
  return this->destination_ == other.destination_;
}

bool ResultWriter::operator!=(const ResultWriter& other) {
  return !(*this == other);
}

void ResultWriter::write_languages(const std::string& languages) {
  Json::Value data{from_json_string(languages)};
  if(!data.isArray()) throw 1; //TODO: implement error handling
  for (const Json::Value& item : data) {
    if (!item.isObject()) throw 1;  // TODO: implement error handling
    if (!item.isMember("language") || !item["language"].isString()) throw 1;  // TODO: implement error handling
    if (!item.isMember("id") || !item["id"].isString()) throw 1;  // TODO: implement error handling

    *(this->destination_) << item["id"].asString() << (std::string(TAB_SPACE, ' ')) << item["language"].asString()
                          << std::endl;
  }
}
}  // client
}  // translate
}  // lgeorgieff
