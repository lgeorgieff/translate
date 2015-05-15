// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/10/2015
// Description: Defines several helper functions for the data type WordClass that is used in the data base to
//              represent a word class value in a grammar.
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

#include "word_class.hpp"
#include "exception.hpp"

namespace lgeorgieff {
namespace translate {
namespace utils {

std::string to_string(const WordClass &word_class) noexcept {
  switch (word_class) {
    case WordClass::adj:
      return "adj";
    case WordClass::adv:
      return "adv";
    case WordClass::pastp:
      return "past-p";
    case WordClass::verb:
      return "verb";
    case WordClass::presp:
      return "pres-p";
    case WordClass::prep:
      return "prep";
    case WordClass::conj:
      return "conj";
    case WordClass::pron:
      return "pron";
    case WordClass::prefix:
      return "prefix";
    case WordClass::suffix:
      return "suffix";
    case WordClass::noun:
      return "noun";
    case WordClass::art:
      return "art";
    case WordClass::num:
      return "num";
    case WordClass::interj:
      return "interj";
    case WordClass::phrase:
      return "phrase";
    case WordClass::idiom:
      return "idiom";
    default:
      return "";
  }
}

std::string to_db_string(const WordClass &word_class) noexcept {
  std::string str{to_string(word_class)};
  if(str.empty()) return "null";
  else return "'" + str + "'";
}

template <typename T>
T from_string(const std::string &);

template <>
WordClass from_string(const std::string &word_class) {
  if ("adj" == word_class)
    return WordClass::adj;
  else if ("adv" == word_class)
    return WordClass::adv;
  else if ("past-p" == word_class)
    return WordClass::pastp;
  else if ("verb" == word_class)
    return WordClass::verb;
  else if ("pres-p" == word_class)
    return WordClass::presp;
  else if ("prep" == word_class)
    return WordClass::prep;
  else if ("conj" == word_class)
    return WordClass::conj;
  else if ("pron" == word_class)
    return WordClass::pron;
  else if ("prefix" == word_class)
    return WordClass::prefix;
  else if ("suffix" == word_class)
    return WordClass::suffix;
  else if ("noun" == word_class)
    return WordClass::noun;
  else if ("art" == word_class)
    return WordClass::art;
  else if("num" == word_class)
    return WordClass::num;
  else if ("interj" == word_class)
    return WordClass::interj;
  else if ("phrase" == word_class)
    return WordClass::phrase;
  else if ("idiom" == word_class)
    return WordClass::idiom;
  else if ("" == word_class)
    return WordClass::none;
  else
    throw Exception(std::string("The value \"") + word_class +
                    std::string("\" is not a valid lgeorgieff::translate::utils::WordClass value"));
}

bool is_word_class(const std::string &word_class) {
  return "adj" == word_class || "adv" == word_class || "past-p" == word_class || "verb" == word_class ||
         "pres-p" == word_class || "prep" == word_class || "conj" == word_class || "pron" == word_class ||
         "prefix" == word_class || "suffix" == word_class || "noun" == word_class || "art" == word_class ||
         "num" == word_class || "interj" == word_class || "phrase" == word_class || "idiom" == word_class ||
         "" == word_class;
}

template <typename T>
T from_db_string(const std::string &);

template <>
WordClass from_db_string(const std::string &word_class) {
  if("null" == word_class) {
    return WordClass::none;
  } else if (word_class.size() > 2) {
    return from_string<WordClass>(word_class.substr(1, word_class.size() - 2));
  } else {
    throw Exception(std::string("The value \"") + word_class +
                    std::string("\" is not a valid lgeorgieff::translate::utils::WordClass value"));
  }
}

}  // utils
}  // translae
}  // lgeorgieff
