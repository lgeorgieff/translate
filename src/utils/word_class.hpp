// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 03/16/2015
// Description: Declares and defines the data type WordClass that is used in the data base to represent a word class
//              value in a grammar.
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

#ifndef WORDCLASS_HPP_
#define WORDCLASS_HPP_

#include <string>
#include <stdexcept>

namespace lgeorgieff {
namespace translate {
namespace utils {

enum class WordClass : unsigned short {
  none = 1,
  adj = 2,
  adv = 4,
  pastp = 8,
  verb = 16,
  presp = 32,
  prep = 64,
  conj = 128,
  pron = 256,
  prefix = 512,
  suffix = 1024,
  noun = 2048
};

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
    default:
      return "";
  }
}

std::string to_db_string(const WordClass &word_class) noexcept {
  switch (word_class) {
    case WordClass::adj:
      return "'adj'";
    case WordClass::adv:
      return "'adv'";
    case WordClass::pastp:
      return "past-p";
    case WordClass::verb:
      return "verb";
    case WordClass::presp:
      return "'pres-p'";
    case WordClass::prep:
      return "'prep'";
    case WordClass::conj:
      return "'conj'";
    case WordClass::pron:
      return "'pron'";
    case WordClass::prefix:
      return "'prefix'";
    case WordClass::suffix:
      return "'suffix'";
    case WordClass::noun:
      return "'noun'";
    default:
      return "null";
  }
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
  else if ("" == word_class)
    return WordClass::none;
  else
    throw std::runtime_error(std::string("The value \"") + word_class +
                             std::string("\" is not a valid lgeorgieff::translate::utils::WordClass values"));
}

template <typename T>
T from_db_string(const std::string &);

template <>
WordClass from_db_string(const std::string &word_class) {
  if ("'adj'" == word_class)
    return WordClass::adj;
  else if ("'adv'" == word_class)
    return WordClass::adv;
  else if ("'past-p'" == word_class)
    return WordClass::pastp;
  else if ("'verb'" == word_class)
    return WordClass::verb;
  else if ("'pres-p'" == word_class)
    return WordClass::presp;
  else if ("'prep'" == word_class)
    return WordClass::prep;
  else if ("'conj'" == word_class)
    return WordClass::conj;
  else if ("'pron'" == word_class)
    return WordClass::pron;
  else if ("'prefix'" == word_class)
    return WordClass::prefix;
  else if ("'suffix'" == word_class)
    return WordClass::suffix;
  else if ("'noun'" == word_class)
    return WordClass::noun;
  else if ("null" == word_class)
    return WordClass::none;
  else
    throw std::runtime_error(std::string("The value \"") + word_class +
                             std::string("\" is not a valid lgeorgieff::translate::utils::WordClass values"));
}
}  // utils
}  // translae
}  // lgeorgieff

#endif  // WORDCLASS_HPP_
