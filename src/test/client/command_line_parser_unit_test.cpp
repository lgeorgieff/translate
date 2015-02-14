// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 02/14/2015
// Description: Unit test for the command_line_parser class
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

#include "../../client/command_line_parser.hpp"
#include "../../utils/command_line_exception.hpp"

#include <cstddef>
#include <string>

using lgeorgieff::translate::CommandLineParser;
using lgeorgieff::translate::utils::CommandLineException;

using std::string;

TEST(command_line_parser, constants) {
  EXPECT_EQ(string("-h"), CommandLineParser::HELP_NAME_SHORT);
  EXPECT_EQ(string("--help"), CommandLineParser::HELP_NAME_LONG);
  EXPECT_EQ(string("-i"), CommandLineParser::IN_NAME_SHORT);
  EXPECT_EQ(string("--in"), CommandLineParser::IN_NAME_LONG);
  EXPECT_EQ(string("-o"), CommandLineParser::OUT_NAME_SHORT);
  EXPECT_EQ(string("--out"), CommandLineParser::OUT_NAME_LONG);
  EXPECT_EQ(string("de"), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(string("en"), CommandLineParser::DEFAULT_OUT_VALUE);
}

TEST(command_line_parser, short_arguments) {
  CommandLineParser clp{2, (const char* []){"app", "-h"}};
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string(""));

  clp = CommandLineParser{3, (const char* []){"app", "-i", "in"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), "in");
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string(""));

  clp = CommandLineParser{3, (const char* []){"app", "-o", "out"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), "out");
  EXPECT_EQ(clp.phrase(), string(""));

  clp = CommandLineParser{5, (const char* []){"app", "-i", "in", "-o", "out"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), "in");
  EXPECT_EQ(clp.out(), "out");
  EXPECT_EQ(clp.phrase(), string(""));
  
  clp = CommandLineParser{4, (const char* []){"app", "-h", "-i", "in"}};
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), "in");
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string(""));
  
  clp = CommandLineParser{6, (const char* []){"app", "-h", "-i", "in", "-o", "out"}};
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), "in");
  EXPECT_EQ(clp.out(), "out");
  EXPECT_EQ(clp.phrase(), string(""));
  
  clp = CommandLineParser{5, (const char* []){"app", "-i", "in1", "-i", "in2"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), "in2");
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string(""));
  
  clp = CommandLineParser{6, (const char* []){"app", "-o", "out1", "-o", "out2", "phrase"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), "out2");
  EXPECT_EQ(clp.phrase(), string("phrase"));
  
  clp = CommandLineParser{3, (const char* []){"app", "-h", "phrase"}};
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string("phrase"));

  clp = CommandLineParser{4, (const char* []){"app", "-i", "in", "another phrase"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), "in");
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string("another phrase"));

  clp = CommandLineParser{4, (const char* []){"app", "-o", "out", "phrase"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), "out");
  EXPECT_EQ(clp.phrase(), string("phrase"));

  clp = CommandLineParser{6, (const char* []){"app", "-i", "in", "-o", "out", "another phrase"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), "in");
  EXPECT_EQ(clp.out(), "out");
  EXPECT_EQ(clp.phrase(), string("another phrase"));

  clp = CommandLineParser{5, (const char* []){"app", "-h", "-i", "in", "another phrase"}};
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), "in");
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string("another phrase"));

  clp = CommandLineParser{8, (const char* []){"app", "-i", "in", "-h", "-o", "out", "-h", "another phrase"}};
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), "in");
  EXPECT_EQ(clp.out(), "out");
  EXPECT_EQ(clp.phrase(), string("another phrase"));

  clp = CommandLineParser{2, (const char* []){"app", "-j"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string("-j"));

  clp = CommandLineParser{2, (const char* []){"app", "only a phrase"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string("only a phrase"));

  EXPECT_THROW((clp = CommandLineParser{3, (const char* []){"app", "-j", "abc"}}), CommandLineException);  
  EXPECT_THROW((clp = CommandLineParser{5, (const char* []){"app", "-i", "in", "-h", "-o"}}), CommandLineException);
  EXPECT_THROW((clp = CommandLineParser{4, (const char* []){"app", "-o", "out", "-i"}}), CommandLineException);
  EXPECT_THROW((clp = CommandLineParser{7, (const char* []){"app", "-i", "in", "-o", "out", "another phrase", "-h"}}),
               CommandLineException);
  EXPECT_THROW((clp = CommandLineParser{7, (const char* []){"app", "-i", "in1", "-i", "in2", "phrase1", "phrase2"}}),
               CommandLineException);
}

TEST(command_line_parser, long_arguments) {
  CommandLineParser clp{2, (const char* []){"app", "--help"}};
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string(""));

  clp = CommandLineParser{3, (const char* []){"app", "--in", "in"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), "in");
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string(""));

  clp = CommandLineParser{3, (const char* []){"app", "--out", "out"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), "out");
  EXPECT_EQ(clp.phrase(), string(""));

  clp = CommandLineParser{5, (const char* []){"app", "--in", "in", "--out", "out"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), "in");
  EXPECT_EQ(clp.out(), "out");
  EXPECT_EQ(clp.phrase(), string(""));
  
  clp = CommandLineParser{4, (const char* []){"app", "--help", "--in", "in"}};
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), "in");
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string(""));
  
  clp = CommandLineParser{6, (const char* []){"app", "--help", "--in", "in", "--out", "out"}};
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), "in");
  EXPECT_EQ(clp.out(), "out");
  EXPECT_EQ(clp.phrase(), string(""));
  
  clp = CommandLineParser{5, (const char* []){"app", "--in", "in1", "--in", "in2"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), "in2");
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string(""));
  
  clp = CommandLineParser{6, (const char* []){"app", "--out", "out1", "--out", "out2", "phrase"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), "out2");
  EXPECT_EQ(clp.phrase(), string("phrase"));
  
  clp = CommandLineParser{3, (const char* []){"app", "--help", "phrase"}};
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string("phrase"));

  clp = CommandLineParser{4, (const char* []){"app", "--in", "in", "another phrase"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), "in");
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string("another phrase"));

  clp = CommandLineParser{4, (const char* []){"app", "--out", "out", "phrase"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), "out");
  EXPECT_EQ(clp.phrase(), string("phrase"));

  clp = CommandLineParser{6, (const char* []){"app", "--in", "in", "--out", "out", "another phrase"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), "in");
  EXPECT_EQ(clp.out(), "out");
  EXPECT_EQ(clp.phrase(), string("another phrase"));

  clp = CommandLineParser{5, (const char* []){"app", "--help", "--in", "in", "another phrase"}};
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), "in");
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string("another phrase"));

  clp = CommandLineParser{8, (const char* []){"app", "--in", "in", "--help", "--out", "out", "--help", "another phrase"}};
  EXPECT_TRUE(clp.help());
  EXPECT_EQ(clp.in(), "in");
  EXPECT_EQ(clp.out(), "out");
  EXPECT_EQ(clp.phrase(), string("another phrase"));

  clp = CommandLineParser{2, (const char* []){"app", "--jj"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string("--jj"));

  clp = CommandLineParser{2, (const char* []){"app", "only a phrase"}};
  EXPECT_FALSE(clp.help());
  EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
  EXPECT_EQ(clp.phrase(), string("only a phrase"));

  EXPECT_THROW((clp = CommandLineParser{3, (const char* []){"app", "--jj", "abc"}}), CommandLineException);
  EXPECT_THROW((clp = CommandLineParser{5, (const char* []){"app", "--in", "in", "--help", "--out"}}),
               CommandLineException);
  EXPECT_THROW((clp = CommandLineParser{4, (const char* []){"app", "--out", "out", "--in"}}), CommandLineException);
  EXPECT_THROW(
      (clp = CommandLineParser{7, (const char* []){"app", "--in", "in", "--out", "out", "another phrase", "--help"}}),
      CommandLineException);
  EXPECT_THROW(
      (clp = CommandLineParser{7, (const char* []){"app", "--in", "in1", "--in", "in2", "phrase1", "phrase2"}}),
      CommandLineException);
}
