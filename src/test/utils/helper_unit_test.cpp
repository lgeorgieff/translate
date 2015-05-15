// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 05/05/2015
// Description: Unit test for the helper mudule.
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

#include "gtest/gtest.h"

#include "utils/helper.hpp"
#include "utils/exception.hpp"

#include <string>

using std::string;

using lgeorgieff::translate::utils::trim_left;
using lgeorgieff::translate::utils::trim_right;
using lgeorgieff::translate::utils::trim;
using lgeorgieff::translate::utils::normalize_whitespace;

TEST(helper, trim_left) {
  string str{};
  trim_left(str);
  EXPECT_EQ(string{}, str);
  str= "  \n\t   \n";
  trim_left(str);
  EXPECT_EQ(string{}, str);
  str= "  \n\td   \n";
  trim_left(str);
  EXPECT_EQ(string{"d   \n"}, str);
  str= ".  \n\td   \n";
  trim_left(str);
  EXPECT_EQ(string{".  \n\td   \n"}, str);
  str = "8";
  trim_left(str);
  EXPECT_EQ(string{"8"}, str);
}

#include <iostream>
TEST(helper, trim_right) {
  string str{};
  trim_right(str);
  EXPECT_EQ(string{}, str);
  str= "\n\t   \n";
  trim_right(str);
  EXPECT_EQ(string{}, str);
  str= "  \n\td   \n";
  trim_right(str);
  EXPECT_EQ(string{"  \n\td"}, str);
  str= "  \n\td   \n.";
  trim_right(str);
  EXPECT_EQ(string{"  \n\td   \n."}, str);
  str = "8";
  trim_right(str);
  EXPECT_EQ(string{"8"}, str);
}

TEST(helper, trim) {
  string str{};
  trim(str);
  EXPECT_EQ(string{}, str);
  str= "  \n\t   \n";
  trim(str);
  EXPECT_EQ(string{}, str);
  str= "  \n\td   \n";
  trim(str);
  EXPECT_EQ(string{"d"}, str);
  str= ".  \n\td   \n";
  trim(str);
  EXPECT_EQ(string{".  \n\td"}, str);
  str = "8";
  trim(str);
  EXPECT_EQ(string{"8"}, str);
}

TEST(helper, normalize_whitespace) {
  string str{};
  normalize_whitespace(str);
  EXPECT_EQ(string{}, str);
  str= "  \n\t   \n";
  normalize_whitespace(str);
  EXPECT_EQ(string{}, str);
  str= "  \n\td   \n";
  normalize_whitespace(str);
  EXPECT_EQ(string{"d"}, str);
  str= ".  \n\td   \n";
  normalize_whitespace(str);
  EXPECT_EQ(string{". d"}, str);
  str = ".  \n\td   \nabc  \n\n\n\tx ";
  normalize_whitespace(str);
  EXPECT_EQ(string{". d abc x"}, str);
  str = "8";
  normalize_whitespace(str);
  EXPECT_EQ(string{"8"}, str);
}
