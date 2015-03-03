// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 03/03/2015
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


#ifndef HELPER_HPP_
#define HELPER_HPP_

#include <string>
#include <cctype>
#include <functional>
#include <algorithm>

using std::string;

namespace lgeorgieff {
namespace translate {
namespace utils {
  
void trim_left(string &str) {
  if (str.empty() || !isspace(str[0])) return;
  string::iterator stop_position{std::find_if_not(str.begin(), str.end(), std::ptr_fun<int, int>(std::isspace))};
  str.erase(str.begin(), stop_position);
}

void trim_right(string &str) {
  if (str.empty() || !isspace(str[str.size() - 1])) return;
  string::reverse_iterator start_position{
      std::find_if_not(str.rbegin(), str.rend(), std::ptr_fun<int, int>(std::isspace))};
  if (start_position != str.rend()) str.erase(start_position.base(), str.end());
}

void trim(string &str) {
  trim_left(str);
  trim_right(str);
}

}  // utils
}  // translate
}  // lgeorgieff

#endif  // HELPER_HPP_
