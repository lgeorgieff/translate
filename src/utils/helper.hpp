// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/10/2015
// Description: Declares several helper functions for the entire project.
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

#ifndef HELPER_HPP_
#define HELPER_HPP_

#include <cstddef>
#include <string>
#include <cctype>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <sstream>

namespace lgeorgieff {
namespace translate {
namespace utils {

// Removes all whitespace at the beginning of the passed string
void trim_left(std::string &);

// Removes all whitespce at the end of the passed string
void trim_right(std::string &);

// Removes all whitespace at the beginning and at the end of the passed string
void trim(std::string &);

// Removes all whitespace at the beginning and at the end of the passed string. In addition all whitespace in the
// string which are not at the beginning or at the end are transformed to blanks. Finally, all remaining whitespace
// characters are replaced by blanks.
void normalize_whitespace(std::string &);

// Returns a size_t value corresponding to the passed string value. If the passed string value is not a valid size_t,
// an std::invalid_argument exception is thrown.
size_t string_to_size_t(const std::string &);

  // Returns true if the first c-string starts with the second c-string. Returns false otherwise.
bool cstring_starts_with(const char *, const char *);

// Returns true if the first c-string ends with the second c-string. Returns false otherwise.
bool cstring_ends_with(const char *, const char *);
}  // utils
}  // translate
}  // lgeorgieff

#endif  // HELPER_HPP_
