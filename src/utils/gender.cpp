// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/10/2015
// Description: Defines several helper functions for the data type Gender that is used in the data base to represent
//              a gender value in a grammar.
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

#include "gender.hpp"
#include "exception.hpp"

namespace lgeorgieff {
namespace translate {
namespace utils {

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
  std::string str{to_string(gender)};
  if(str.empty()) return "null";
  else return "'" + str + "'";
}

template <typename T>
T from_string(const std::string &);

template <>
Gender from_string(const std::string &gender) {
  if ("" == gender)
    return Gender::none;
  else if ("m" == gender)
    return Gender::m;
  else if ("f" == gender)
    return Gender::f;
  else if ("n" == gender)
    return Gender::n;
  else
    throw Exception(std::string("The value \"") + gender +
                    std::string("\" is not a valid lgeorgieff::translate::utils::Gender value"));
}

template <typename T>
T from_db_string(const std::string &);

template <>
Gender from_db_string(const std::string &gender) {
  if("null" == gender) {
    return Gender::none;
  } else if (gender.size() > 2) {
    return from_string<Gender>(gender.substr(1, gender.size() - 2));
  } else {
    throw Exception(std::string("The value \"") + gender +
                    std::string("\" is not a valid lgeorgieff::translate::utils::Gender value"));
  }
}

}  // utils
}  // translae
}  // lgeorgieff
