// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 02/13/2015
// Description: Entry point for the client unit tests.
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

int main(const int argc, const char **argv) {
  testing::InitGoogleTest(const_cast<int *>(&argc), const_cast<char **>(argv));
  return RUN_ALL_TESTS();
}
