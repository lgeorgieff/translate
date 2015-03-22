// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 02/22/2015
// Description: Unit test for the Gender enum class and its helper functions.
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

#include "../../utils/gender.hpp"
#include "../../utils/exception.hpp"

#include <string>

using std::string;

using lgeorgieff::translate::utils::Exception;
using lgeorgieff::translate::utils::Gender;
using lgeorgieff::translate::utils::to_string;
using lgeorgieff::translate::utils::to_db_string;
using lgeorgieff::translate::utils::from_string;
using lgeorgieff::translate::utils::from_db_string;

TEST(gender, to_string) {
  EXPECT_EQ(string{"m"}, to_string(Gender::m));
  EXPECT_EQ(string{"f"}, to_string(Gender::f));
  EXPECT_EQ(string{"n"}, to_string(Gender::n));
  EXPECT_EQ(string{""}, to_string(Gender::none));
}

TEST(gender, to_db_string) {
  EXPECT_EQ(string{"'m'"}, to_db_string(Gender::m));
  EXPECT_EQ(string{"'f'"}, to_db_string(Gender::f));
  EXPECT_EQ(string{"'n'"}, to_db_string(Gender::n));
  EXPECT_EQ(string{"null"}, to_db_string(Gender::none));
}

TEST(gender, from_string) {
  EXPECT_EQ(Gender::m, from_string<Gender>("m"));
  EXPECT_EQ(Gender::n, from_string<Gender>("n"));
  EXPECT_EQ(Gender::f, from_string<Gender>("f"));
  EXPECT_EQ(Gender::none, from_string<Gender>(""));
  EXPECT_THROW(from_string<Gender>("none"), Exception);
  EXPECT_THROW(from_string<Gender>("M"), Exception);
  EXPECT_THROW(from_string<Gender>("'m'"), Exception);
  EXPECT_THROW(from_string<Gender>("''"), Exception);
}

TEST(gender, from_db_string) {
  EXPECT_EQ(Gender::m, from_db_string<Gender>("'m'"));
  EXPECT_EQ(Gender::n, from_db_string<Gender>("'n'"));
  EXPECT_EQ(Gender::f, from_db_string<Gender>("'f'"));
  EXPECT_EQ(Gender::none, from_db_string<Gender>("null"));
  EXPECT_THROW(from_db_string<Gender>("''"), Exception);
  EXPECT_THROW(from_db_string<Gender>("m"), Exception);
  EXPECT_THROW(from_db_string<Gender>("'null'"), Exception);
  EXPECT_THROW(from_db_string<Gender>("none"), Exception);
  EXPECT_THROW(from_db_string<Gender>("'M'"), Exception);
}
