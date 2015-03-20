// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 03/20/2015
// Description: Declares and defines the data type Numerus that is used in the data base to represent a numerus
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

#ifndef NUMERUS_HPP_
#define NUMERUS_HPP_

#include "exception.hpp"
#include <string>

namespace lgeorgieff {
namespace translate {
namespace utils {

enum class Numerus : char { none = 1, sg = 2, pl = 4 };

std::string to_string(const Numerus &numerus) noexcept {
  switch (numerus) {
    case Numerus::sg:
      return "sg.";
    case Numerus::pl:
      return "pl.";
    default:
      return "";
  }
}

std::string to_db_string(const Numerus &numerus) noexcept {
  switch (numerus) {
    case Numerus::sg:
      return "'sg.'";
    case Numerus::pl:
      return "'pl.'";
    default:
      return "null";
  }
}

template <typename T>
T from_string(const std::string &);

template <>
Numerus from_string(const std::string &numerus) {
  if ("" == numerus)
    return Numerus::none;
  else if ("sg." == numerus)
    return Numerus::sg;
  else if ("pl." == numerus)
    return Numerus::pl;
  else
    throw Exception(std::string("The value \"") + numerus +
                    std::string("\" is not a valid lgeorgieff::translate::utils::Numerus value"));
}

template <typename T>
T from_db_string(const std::string &);

template <>
Numerus from_db_string(const std::string &numerus) {
  if ("null" == numerus)
    return Numerus::none;
  else if ("'sg.'" == numerus)
    return Numerus::sg;
  else if ("'pl.'" == numerus)
    return Numerus::pl;
  else
    throw Exception(std::string("The value \"") + numerus +
                    std::string("\" is not a valid lgeorgieff::translate::utils::Numerus value"));
}

}  // utils
}  // translae
}  // lgeorgieff

#endif  // NUMERUS_HPP_
