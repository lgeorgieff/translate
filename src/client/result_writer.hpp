// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 05/31/2015
// Description: Defines the class ResultWriter which provides functionality that writes the translation results
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

#ifndef RESULT_WRITER_HPP_
#define RESULT_WRITER_HPP_

#include <ostream>
#include <string>
#include <cstddef>
#include <vector>

namespace lgeorgieff {
namespace translate {
namespace client {

class ResultWriter {
 public:
  static const size_t TAB_SPACE;

  // === Constructors, destructor, operators
  // ==========================================================================
  explicit ResultWriter(std::ostream*);
  ResultWriter(const ResultWriter&);
  ResultWriter(ResultWriter&&);
  virtual ~ResultWriter();
  ResultWriter& operator=(const ResultWriter&);
  ResultWriter& operator=(ResultWriter&&);
  bool operator==(const ResultWriter&);
  bool operator!=(const ResultWriter&);

  // Writes a json string of the form [{"id": "...", "language": "..."}] into the set ostream in the form
  // <ID>   <LANGUAGE NAME>
  void write_languages(const std::string&);
  // Writes a json string of the form [{"id": "...", "word_class": "..."}] into the set ostream in the form
  // <ID>   <LANGUAGE NAME>
  void write_word_classes(const std::string&);
  // Writes a json string of the form [{"description": "...", "gender": "...", "gender" : "..."}] into the set ostream
  // in the form // <ID>   <GENDER>   <DESCRIPTION>
  void write_genders(const std::string&);

 private:
  // A generic method the writes the passed json string on screen. The json string must be an array of objects.
  // Additionally the object must contain only string members.
  // The order of the printed object members depends on the order of the member names in the passed vector<string>.
  void write_json_array_string(const std::string&, const std::vector<std::string>&, bool = false);
  std::ostream* destination_;
};  // ResultWriter

}  // client
}  // translate
}  // lgeorgieff
#endif  // RESULT_WRITER_HPP_
