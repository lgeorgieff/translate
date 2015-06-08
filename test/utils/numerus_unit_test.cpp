// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 05/05/2015
// Description: Unit test for the Numerus enum class and its helper functions.
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

#include "utils/numerus.hpp"
#include "utils/exception.hpp"

#include <string>

using std::string;

using lgeorgieff::translate::utils::Exception;
using lgeorgieff::translate::utils::Numerus;
using lgeorgieff::translate::utils::to_string;
using lgeorgieff::translate::utils::to_db_string;
using lgeorgieff::translate::utils::from_string;
using lgeorgieff::translate::utils::from_db_string;

TEST(numerus, to_string) {
  EXPECT_EQ(string{"sg."}, to_string(Numerus::sg));
  EXPECT_EQ(string{"pl."}, to_string(Numerus::pl));
  EXPECT_EQ(string{""}, to_string(Numerus::none));
}

TEST(numerus, to_db_string) {
  EXPECT_EQ(string{"'sg.'"}, to_db_string(Numerus::sg));
  EXPECT_EQ(string{"'pl.'"}, to_db_string(Numerus::pl));
  EXPECT_EQ(string{"null"}, to_db_string(Numerus::none));
}

TEST(numerus, from_string) {
  EXPECT_EQ(Numerus::sg, from_string<Numerus>("sg."));
  EXPECT_EQ(Numerus::pl, from_string<Numerus>("pl."));
  EXPECT_EQ(Numerus::none, from_string<Numerus>(""));
  EXPECT_THROW(from_string<Numerus>("none"), Exception);
  EXPECT_THROW(from_string<Numerus>("Sg."), Exception);
  EXPECT_THROW(from_string<Numerus>("'sg.'"), Exception);
  EXPECT_THROW(from_string<Numerus>("."), Exception);
  EXPECT_THROW(from_string<Numerus>("sg"), Exception);
}

TEST(numerus, from_db_string) {
  EXPECT_EQ(Numerus::sg, from_db_string<Numerus>("'sg.'"));
  EXPECT_EQ(Numerus::pl, from_db_string<Numerus>("'pl.'"));
  EXPECT_EQ(Numerus::none, from_db_string<Numerus>("null"));
  EXPECT_THROW(from_db_string<Numerus>("''"), Exception);
  EXPECT_THROW(from_db_string<Numerus>("sg."), Exception);
  EXPECT_THROW(from_db_string<Numerus>("'sg'"), Exception);
  EXPECT_THROW(from_db_string<Numerus>("'.'"), Exception);
  EXPECT_THROW(from_db_string<Numerus>("'null'"), Exception);
  EXPECT_THROW(from_db_string<Numerus>("none"), Exception);
  EXPECT_THROW(from_db_string<Numerus>("'Sg.'"), Exception);
}
