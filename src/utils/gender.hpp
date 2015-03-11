// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 03/15/2015
// Description: Declares and defines the data type Gender that is used in the data base to represent a gender value
//              in a grammar.
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

#ifndef GENDER_HPP_
#define GENDER_HPP_

#include <string>
#include <stdexcept>

namespace lgeorgieff {
namespace translate {
namespace utils {

enum class Gender : char { none = 1, m = 2, f = 4, n = 8 };

std::string to_string(const Gender &gender) noexcept {
  switch (gender) {
    case Gender::m:
      return "m";
    case Gender::f:
      return "f";
    case Gender::n:
      return "n";
    default:
      return "";
  }
}

std::string to_db_string(const Gender &gender) noexcept {
  switch (gender) {
    case Gender::m:
      return "m";
    case Gender::f:
      return "f";
    case Gender::n:
      return "n";
    default:
      return "null";
  }
}
  
template <typename T>
T from_string(const std::string&);

template <>
Gender from_string(const std::string &gender) {
  if (gender == "")
    return Gender::none;
  else if (gender == "m")
    return Gender::m;
  else if (gender == "f")
    return Gender::f;
  else if (gender == "n")
    return Gender::n;
  else
    throw std::runtime_error(std::string("The value \"") + gender +
                             std::string("\" is not a valid lgeorgieff::translate::utils::Gender values"));
}

template <typename T>
T from_db_string(const std::string &);

template <>
Gender from_db_string(const std::string &gender) {
  if (gender == "")
    return Gender::none;
  else if (gender == "m")
    return Gender::m;
  else if (gender == "f")
    return Gender::f;
  else if (gender == "n")
    return Gender::n;
  else
    throw std::runtime_error(std::string("The value \"") + gender +
                             std::string("\" is not a valid lgeorgieff::translate::utils::Gender values"));
}
  
}  // utils
}  // translae
}  // lgeorgieff

#endif  // GENDER_HPP_
