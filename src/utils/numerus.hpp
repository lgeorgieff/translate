// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/10/2015
// Description: Defines the data type Numerus that is used in the data base to represent a numerus value in a grammar
//              and declares several helper functions for it.
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

#include <string>

namespace lgeorgieff {
namespace translate {
namespace utils {

enum class Numerus : char { none = 1, sg = 2, pl = 4 };

std::string to_string(const Numerus &) noexcept;

std::string to_db_string(const Numerus &) noexcept;

template <typename T>
T from_string(const std::string &);

template <>
Numerus from_string(const std::string &);

template <typename T>
T from_db_string(const std::string &);

template <>
Numerus from_db_string(const std::string &);

}  // utils
}  // translae
}  // lgeorgieff

#endif  // NUMERUS_HPP_
