// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/08/2015
// Description: Unit test for the ConfigurationReader class
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

#include "client/configuration_reader.hpp"
#include "utils/exception.hpp"
#include "utils/json_exception.hpp"

#include <cstddef>
#include <string>
#include <fstream>
#include <cstdio>

using lgeorgieff::translate::utils::Exception;
using lgeorgieff::translate::utils::JsonException;
using lgeorgieff::translate::client::ConfigurationReader;

TEST(configuration_reader, default_configuration_file) {
  const std::string CONFIG_PATH{"test_config.json"};
  std::ofstream test_file;
  test_file.open(CONFIG_PATH);
  test_file << "{\"service_address\":\"localhost\",\"service_port\":8885,\"show_phrase\":true,\"show_word_class\":"
               "false,\"show_gender\":false,\"show_numerus\":false,\"show_abbreviation\":false,\"show_comment\":"
               "false}";
  test_file.close();
  
  ConfigurationReader config_reader{CONFIG_PATH};
  config_reader();
  EXPECT_TRUE(config_reader.available());
  EXPECT_EQ(std::string{"localhost"}, config_reader.service_address());
  EXPECT_EQ(8885, config_reader.service_port());
  EXPECT_TRUE(config_reader.show_phrase());
  EXPECT_FALSE(config_reader.show_word_class());
  EXPECT_FALSE(config_reader.show_gender());
  EXPECT_FALSE(config_reader.show_numerus());
  EXPECT_FALSE(config_reader.show_abbreviation());
  EXPECT_FALSE(config_reader.show_comment());

  std::remove(CONFIG_PATH.c_str());
}

TEST(configuration_reader, missing_configuration_file) {
  ConfigurationReader config_reader{"test_config.json"};
  EXPECT_THROW(config_reader(), Exception);
}

TEST(configuration_reader, malformed_configuration_file) {
  const std::string CONFIG_PATH{"test_config.json"};
  std::ofstream test_file;
  test_file.open(CONFIG_PATH);
  test_file << "{\"service_address:\"localhost\",\"service_port\":8885,\"show_phrase\":true,\"show_word_class\":"
               "false,\"show_gender\":false,\"show_numerus\":false,\"show_abbreviation\":false,\"show_comment\":"
               "false}";
  test_file.close();

  ConfigurationReader config_reader{CONFIG_PATH};
  EXPECT_THROW(config_reader(), JsonException);

  std::remove(CONFIG_PATH.c_str());
}
