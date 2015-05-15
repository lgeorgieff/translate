// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/10/2015
// Description: Defines the data type WordClass that is used in the data base to represent a word class value in a
//              grammar and declares several helper functions for it.
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

#include <cstddef>
#include <string>

namespace lgeorgieff {
namespace translate {
namespace utils {

enum class WordClass : size_t {
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
  noun = 2048,
  art = 4096,
  num = 8192,
  interj = 16834,
  phrase = 32768,
  idiom = 65536
};

std::string to_string(const WordClass &) noexcept;

std::string to_db_string(const WordClass &) noexcept;

template <typename T>
T from_string(const std::string &);

template <>
WordClass from_string(const std::string &);

bool is_word_class(const std::string &);

template <typename T>
T from_db_string(const std::string &);

template <>
WordClass from_db_string(const std::string &);

}  // utils
}  // translae
}  // lgeorgieff

#endif  // WORDCLASS_HPP_
