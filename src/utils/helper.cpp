// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/10/2015
// Description: Defines several helper functions for the entire project.
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

#include "helper.hpp"

using std::string;

namespace lgeorgieff {
namespace translate {
namespace utils {

// Removes all whitespace at the beginning of the passed string
void trim_left(string &str) {
  if (str.empty() || !isspace(str[0])) return;
  string::iterator stop_position{std::find_if_not(str.begin(), str.end(), std::ptr_fun<int, int>(std::isspace))};
  str.erase(str.begin(), stop_position);
}

// Removes all whitespce at the end of the passed string
void trim_right(string &str) {
  if (str.empty() || !isspace(str[str.size() - 1])) return;
  string::reverse_iterator start_position{
      std::find_if_not(str.rbegin(), str.rend(), std::ptr_fun<int, int>(std::isspace))};
  str.erase(start_position.base(), str.end());
}

// Removes all whitespace at the beginning and at the end of the passed string
void trim(string &str) {
  trim_left(str);
  trim_right(str);
}

// Removes all whitespace at the beginning and at the end of the passed string. In addition all whitespace in the
// string which are not at the beginning or at the end are transformed to blanks. Finally, all remaining whitespace
// characters are replaced by blanks.
void normalize_whitespace(string &str) {
  trim(str);
  size_t pos{0};
  while (pos < str.size()) {
    if (std::isspace(str[pos]) && str[pos - 1] == ' ') {
      str.erase(pos, 1);
    } else if (std::isspace(str[pos])) {
      str[pos] = ' ';
      ++pos;
    } else {
      ++pos;
    }
  }
}

//
size_t string_to_size_t(const std::string &str) {
  size_t number;
  std::stringstream ss(str);
  ss >> number;
  if (ss.fail()) throw std::invalid_argument("The value \"" + str + "\" is not a valid number!");
  return number;
}

}  // utils
}  // translate
}  // lgeorgieff
