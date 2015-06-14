// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/14/2015
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

#include <string>
#include <vector>

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

// Returns the last path segment of the passed URL string, e.g. return "file" for the following URL
// "http://localhost:1234/my/path/to/file#def?attr=12"
std::string get_last_path_from_url(const char *, bool = true);

// Returns a copy of the passed string that contains each character in upper case format.
std::string to_upper_case(const std::string &);

// Returns a vector of strings that are generated from the original string value which is split by the passed delimiter
// character. If the bool value is set to true the split values are trimmed for white space characters in the beginning
// and in the end.
std::vector<std::string> split_string(const std::string &, char, bool = false);

// Parses one item from the accept header field and sets the type and subtype string references to the corresponding
// values.
void parse_accept_header_item(const std::string &, std::string &, std::string &);

// Returns true if the passed accept header string contains the specified accept type.
// This function takes also placeholders like "*" into account.
// see: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.1
bool check_accept_header(const std::string &, const std::string & = "application/json");

// Returns the folder which contains the executable file.
// May throw an instance of Exception if cannot read from /proc/.
std::string get_exe_path();
}  // utils
}  // translate
}  // lgeorgieff

#endif  // HELPER_HPP_
