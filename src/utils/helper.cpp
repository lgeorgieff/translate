// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 05/31/2015
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

#include <cstddef>
#include <cctype>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <sstream>

namespace lgeorgieff {
namespace translate {
namespace utils {

void trim_left(std::string &str) {
  if (str.empty() || !isspace(str[0])) return;
  std::string::iterator stop_position{std::find_if_not(str.begin(), str.end(), std::ptr_fun<int, int>(std::isspace))};
  str.erase(str.begin(), stop_position);
}

void trim_right(std::string &str) {
  if (str.empty() || !isspace(str[str.size() - 1])) return;
  std::string::reverse_iterator start_position{
      std::find_if_not(str.rbegin(), str.rend(), std::ptr_fun<int, int>(std::isspace))};
  str.erase(start_position.base(), str.end());
}

void trim(std::string &str) {
  trim_left(str);
  trim_right(str);
}

void normalize_whitespace(std::string &str) {
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

size_t string_to_size_t(const std::string &str) {
  size_t number;
  std::stringstream ss(str);
  ss >> number;
  if (ss.fail()) throw std::invalid_argument("The value \"" + str + "\" is not a valid number!");
  return number;
}

bool cstring_starts_with(const char *container, const char *containee) {
  const char *pos_container{container};
  const char *pos_containee{containee};
  for (; *pos_container && *pos_containee; ++pos_container, ++pos_containee)
    if (*pos_container != *pos_containee) return false;
  return !*pos_containee;
}

bool cstring_ends_with(const char *container, const char *containee) {
  const char *begin_container{container};
  const char *begin_containee{containee};
  while (*container) ++container;
  while (*containee) ++containee;

  for (; container != begin_container && containee != begin_containee; --container, --containee)
    if (*container != *containee) return false;
  if (containee == begin_containee && *containee == *container) return true;
  return false;
}

std::string get_last_path_from_url(const char *url, bool ignore_closing_separator) {
  const char *begin{url};
  const char *end{url};
  while (*url) {
    if ('/' == *url && (ignore_closing_separator && *(url + 1) && '#' != *(url + 1) && '?' != *(url + 1))) {
      begin = url + 1;
    }
    if ('#' == *url || '?' == *url) {
      end = url;
      break;
    }
    ++url;
    end = url;
  }
  if (*(end - 1) == '/' && end != begin) --end;
  return std::string{begin, end};
}

std::string to_upper_case(const std::string &str) {
  std::string result{str};
  std::transform(result.begin(), result.end(), result.begin(), toupper);
  return result;
}
}  // utils
}  // translate
}  // lgeorgieff
