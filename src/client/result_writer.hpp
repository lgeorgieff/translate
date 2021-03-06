// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/03/2015
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
  // Writes a json string of the form ["...", "..."] into the set ostream in the form
  // <NUMERUS>
  void write_numeri(const std::string&);
  // Writes a json string of the form "..." into the set ostream in the form
  // <LANGUAGE ID>
  void write_language_id(const std::string&);
  // Writes a json string of the form "..." into the set ostream in the form
  // <LANGUAGE NAME>
  void write_language_name(const std::string&);
  // Writes a json string of the form "..." into the set ostream in the form
  // <WORD CLASS ID>
  void write_word_class_id(const std::string&);
  // Writes a json string of the form "..." into the set ostream in the form
  // <WORD CLASS NAME>
  void write_word_class_name(const std::string&);
  // Writes a json string of the form "..." into the set ostream in the form
  // <GENDER ID>
  void write_gender_id(const std::string&);
  // Writes a json string of the form "..." into the set ostream in the form
  // <GENDER NAME>
  void write_gender_name(const std::string&);
  // Writes a json string of the form [{"phrase": "...", "word_class": "...", "gender": "...", "numerus": "...",
  // "abbreviations": [ "..." ], "comments": [ "..." ] } ] into the set ostream in the form
  // <PHRASE> {<WORD_CLASS>} {<GENDER>} {<NUMERUS>} <<ABREVIATION-1>, <...>, <ABBREVIATION-N>> [<COMMENT-1>] [<...>]
  // [<COMMENT-N>]
  // <PHRASE> {<WORD_CLASS>} {<GENDER>} {<NUMERUS>} <<ABREVIATION-1>, <...>, <ABBREVIATION-N>> [<COMMENT-1>] [<...>]
  // [<COMMENT-N>]
  void write_translation(const std::string&);

 private:
  // A generic method that writes the passed json string on screen. The json string must be an array of objects.
  // Additionally the object must contain only string members.
  // The order of the printed object members depends on the order of the member names in the passed vector<string>.
  void write_json_array_string(const std::string&, const std::vector<std::string>&, bool = false);
  // A generic method that writes the passed json string on screen. The json string must be a string value.
  void write_json_string_string(const std::string&);
  // A generic method that writes the passed json string on screen. The json string must be an object value containing
  // onyl string members.
  void write_json_object_string(const std::string&, const std::vector<std::string>&);
  std::ostream* destination_;
};  // ResultWriter

}  // client
}  // translate
}  // lgeorgieff
#endif  // RESULT_WRITER_HPP_
