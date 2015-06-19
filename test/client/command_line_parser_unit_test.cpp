// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 06/19/2015
// Description: Unit test for the command_line_parser class
// ====================================================================================================================

// ====================================================================================================================
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public
// License as published by the Free Software Foundation in version 2.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the
// Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301, USA.
// ====================================================================================================================

#include "gtest/gtest.h"

#include "client/command_line_parser.hpp"
#include "utils/command_line_exception.hpp"

#include <cstddef>
#include <string>

using lgeorgieff::translate::CommandLineParser;
using lgeorgieff::translate::utils::CommandLineException;

using std::string;

TEST(command_line_parser, constants) {
  EXPECT_EQ(string{"-h"}, CommandLineParser::HELP_NAME_SHORT);
  EXPECT_EQ(string{"--help"}, CommandLineParser::HELP_NAME_LONG);
  EXPECT_EQ(string{"-i"}, CommandLineParser::IN_NAME_SHORT);
  EXPECT_EQ(string{"--in"}, CommandLineParser::IN_NAME_LONG);
  EXPECT_EQ(string{"-o"}, CommandLineParser::OUT_NAME_SHORT);
  EXPECT_EQ(string{"--out"}, CommandLineParser::OUT_NAME_LONG);
  EXPECT_EQ(string{"DE"}, CommandLineParser::DEFAULT_IN_VALUE);
  EXPECT_EQ(string{"EN"}, CommandLineParser::DEFAULT_OUT_VALUE);
}

TEST(command_line_parser, short_arguments) {
  CommandLineParser clp{};
  {
    const char *args[]{"app", "-h"};
    clp(2, args);
    EXPECT_TRUE(clp.help());
    EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "-i", "in"};
    clp(3, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), string{"in"});
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "-o", "out"};
    clp(3, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
    EXPECT_EQ(clp.out(), string{"out"});
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "-i", "in", "-o", "out"};
    clp(5, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), string{"in"});
    EXPECT_EQ(clp.out(), string{"out"});
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "-h", "-i", "in"};
    clp(4, args);
    EXPECT_TRUE(clp.help());
    EXPECT_EQ(clp.in(), string{"in"});
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "-h", "-i", "in", "-o", "out"};
    clp(6, args);
    EXPECT_TRUE(clp.help());
    EXPECT_EQ(clp.in(), string{"in"});
    EXPECT_EQ(clp.out(), string{"out"});
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "-i", "in1", "-i", "in2"};
    clp(5, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), string{"in2"});
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "-o", "out1", "-o", "out2", "phrase"};
    clp(6, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
    EXPECT_EQ(clp.out(), string{"out2"});
    EXPECT_EQ(clp.phrase(), string{"phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "-h", "phrase"};
    clp(3, args);
    EXPECT_TRUE(clp.help());
    EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{"phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "-i", "in", "another phrase"};
    clp(4, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), string{"in"});
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{"another phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "-o", "out", "phrase"};
    clp(4, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
    EXPECT_EQ(clp.out(), string{"out"});
    EXPECT_EQ(clp.phrase(), string{"phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "-i", "in", "-o", "out", "another phrase"};
    clp(6, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), string{"in"});
    EXPECT_EQ(clp.out(), string{"out"});
    EXPECT_EQ(clp.phrase(), string{"another phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "-h", "-i", "in", "another phrase"};
    clp(5, args);
    EXPECT_TRUE(clp.help());
    EXPECT_EQ(clp.in(), string{"in"});
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{"another phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "-i",  "in", "-h",
                       "-o",  "out", "-h", "another phrase"};
    clp(8, args);
    EXPECT_TRUE(clp.help());
    EXPECT_EQ(clp.in(), string{"in"});
    EXPECT_EQ(clp.out(), string{"out"});
    EXPECT_EQ(clp.phrase(), string{"another phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "-j"};
    clp(2, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{"-j"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "only a phrase"};
    clp(2, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{"only a phrase"});
  }

  clp = CommandLineParser{};
  {
    const char *args[]{"app", "-j", "abc"};
    EXPECT_THROW(clp(3, args), CommandLineException);
  }
  {
    const char *args[]{"app", "-i", "in", "-h", "-o"};
    EXPECT_THROW(clp(5, args), CommandLineException);
  }
  {
    const char *args[]{"app", "-o", "out", "-i"};
    EXPECT_THROW(clp(4, args), CommandLineException);
  }
  {
    const char *args[]{"app", "-i", "in", "-o", "out", "another phrase", "-h"};
    EXPECT_THROW(clp(7, args), CommandLineException);
  }
  {
    const char *args[]{"app", "-i", "in1", "-i", "in2", "phrase1", "phrase2"};
    EXPECT_THROW(clp(7, args), CommandLineException);
  }
} // TEST(command_line_parser, short_arguments)

TEST(command_line_parser, long_arguments) {
  CommandLineParser clp{};
  {
    const char *args[]{"app", "--help"};
    clp(2, args);
    EXPECT_TRUE(clp.help());
    EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--in", "in"};
    clp(3, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), string{"in"});
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--out", "out"};
    clp(3, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
    EXPECT_EQ(clp.out(), string{"out"});
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--in", "in", "--out", "out"};
    clp(5, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), string{"in"});
    EXPECT_EQ(clp.out(), string{"out"});
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--help", "--in", "in"};
    clp(4, args);
    EXPECT_TRUE(clp.help());
    EXPECT_EQ(clp.in(), string{"in"});
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--help", "--in", "in", "--out", "out"};
    clp(6, args);
    EXPECT_TRUE(clp.help());
    EXPECT_EQ(clp.in(), string{"in"});
    EXPECT_EQ(clp.out(), string{"out"});
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--in", "in1", "--in", "in2"};
    clp(5, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), string{"in2"});
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--out", "out1", "--out", "out2", "phrase"};
    clp(6, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
    EXPECT_EQ(clp.out(), string{"out2"});
    EXPECT_EQ(clp.phrase(), string{"phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--help", "phrase"};
    clp(3, args);
    EXPECT_TRUE(clp.help());
    EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{"phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--in", "in", "another phrase"};
    clp(4, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), string{"in"});
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{"another phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--out", "out", "phrase"};
    clp(4, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
    EXPECT_EQ(clp.out(), "out");
    EXPECT_EQ(clp.phrase(), string{"phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--in", "in", "--out", "out", "another phrase"};
    clp(6, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), string{"in"});
    EXPECT_EQ(clp.out(), string{"out"});
    EXPECT_EQ(clp.phrase(), string{"another phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--help", "--in", "in", "another phrase"};
    clp(5, args);
    EXPECT_TRUE(clp.help());
    EXPECT_EQ(clp.in(), string{"in"});
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{"another phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app",   "--in", "in",     "--help",
                       "--out", "out",  "--help", "another phrase"};
    clp(8, args);
    EXPECT_TRUE(clp.help());
    EXPECT_EQ(clp.in(), string{"in"});
    EXPECT_EQ(clp.out(), string{"out"});
    EXPECT_EQ(clp.phrase(), string{"another phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--jj"};
    clp(2, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{"--jj"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "only a phrase"};
    clp(2, args);
    EXPECT_FALSE(clp.help());
    EXPECT_EQ(clp.in(), CommandLineParser::DEFAULT_IN_VALUE);
    EXPECT_EQ(clp.out(), CommandLineParser::DEFAULT_OUT_VALUE);
    EXPECT_EQ(clp.phrase(), string{"only a phrase"});
  }

  clp = CommandLineParser{};
  {
    const char *args[]{"app", "--jj", "abc"};
    EXPECT_THROW(clp(3, args), CommandLineException);
  }

  {
    const char *args[]{"app", "--in", "in", "--help", "--out"};
    EXPECT_THROW(clp(5, args), CommandLineException);
  }

  {
    const char *args[]{"app", "--out", "out", "--in"};
    EXPECT_THROW(clp(4, args), CommandLineException);
  }

  {
    const char *args[]{"app", "--in", "in", "--out", "out", "another phrase",
                       "--help"};
    EXPECT_THROW(clp(7, args), CommandLineException);
  }

  {
    const char *args[]{"app", "--in",    "in1",    "--in",
                       "in2", "phrase1", "phrase2"};
    EXPECT_THROW(clp(7, args), CommandLineException);
  }
} // TEST(command_line_parser, long_arguments)

TEST(command_line_parser, language_arguments) {
  CommandLineParser clp = CommandLineParser{};
  {
    const char *args[]{"app", "--all-languages"};
    clp(2, args);
    EXPECT_TRUE(clp.all_languages());
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--language-id", "PL"};
    clp(3, args);
    EXPECT_TRUE(clp.has_language_id());
    EXPECT_EQ(clp.language_id(), string{"PL"});
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--language-name", "polski"};
    clp(3, args);
    EXPECT_TRUE(clp.has_language_name());
    EXPECT_EQ(clp.language_name(), string{"polski"});
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--all-languages", "only a phrase"};
    clp(3, args);
    EXPECT_TRUE(clp.all_languages());
    EXPECT_EQ(clp.phrase(), string{"only a phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--language-id", "PL", "only a phrase"};
    clp(4, args);
    EXPECT_TRUE(clp.has_language_id());
    EXPECT_EQ(clp.language_id(), string{"PL"});
    EXPECT_EQ(clp.phrase(), string{"only a phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--language-name", "polski", "only a phrase"};
    clp(4, args);
    EXPECT_TRUE(clp.has_language_name());
    EXPECT_EQ(clp.language_name(), string{"polski"});
    EXPECT_EQ(clp.phrase(), string{"only a phrase"});
  }

  clp = CommandLineParser{};
  {
    const char *args[]{"app", "--language-id"};
    EXPECT_THROW(clp(2, args), CommandLineException);
  }

  {
    const char *args[]{"app", "--language-name"};
    EXPECT_THROW(clp(2, args), CommandLineException);
  }
}

TEST(command_line_parser, word_class_arguments) {
  CommandLineParser clp = CommandLineParser{};
  {
    const char *args[]{"app", "--all-word-classes"};
    clp(2, args);
    EXPECT_TRUE(clp.all_word_classes());
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--word-class-id", "noun"};
    clp(3, args);
    EXPECT_TRUE(clp.has_word_class_id());
    EXPECT_EQ(clp.word_class_id(), string{"noun"});
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--word-class-name", "noun"};
    clp(3, args);
    EXPECT_TRUE(clp.has_word_class_name());
    EXPECT_EQ(clp.word_class_name(), string{"noun"});
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--all-word-classes", "only a phrase"};
    clp(3, args);
    EXPECT_TRUE(clp.all_word_classes());
    EXPECT_EQ(clp.phrase(), string{"only a phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--word-class-id", "noun", "only a phrase"};
    clp(4, args);
    EXPECT_TRUE(clp.has_word_class_id());
    EXPECT_EQ(clp.word_class_id(), string{"noun"});
    EXPECT_EQ(clp.phrase(), string{"only a phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--word-class-name", "noun", "only a phrase"};
    clp(4, args);
    EXPECT_TRUE(clp.has_word_class_name());
    EXPECT_EQ(clp.word_class_name(), string{"noun"});
    EXPECT_EQ(clp.phrase(), string{"only a phrase"});
  }

  clp = CommandLineParser{};
  {
    const char *args[]{"app", "--word-class-id"};
    EXPECT_THROW(clp(2, args), CommandLineException);
  }

  {
    const char *args[]{"app", "--word-class-name"};
    EXPECT_THROW(clp(2, args), CommandLineException);
  }
}

TEST(command_line_parser, gender_arguments) {
  CommandLineParser clp = CommandLineParser{};
  {
    const char *args[]{"app", "--all-genders"};
    clp(2, args);
    EXPECT_TRUE(clp.all_genders());
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--gender-id", "f."};
    clp(3, args);
    EXPECT_TRUE(clp.has_gender_id());
    EXPECT_EQ(clp.gender_id(), string{"f."});
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--gender-name", "female"};
    clp(3, args);
    EXPECT_TRUE(clp.has_gender_name());
    EXPECT_EQ(clp.gender_name(), string{"female"});
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--all-genders", "only a phrase"};
    clp(3, args);
    EXPECT_TRUE(clp.all_genders());
    EXPECT_EQ(clp.phrase(), string{"only a phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--gender-id", "m.", "only a phrase"};
    clp(4, args);
    EXPECT_TRUE(clp.has_gender_id());
    EXPECT_EQ(clp.gender_id(), string{"m."});
    EXPECT_EQ(clp.phrase(), string{"only a phrase"});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--gender-name", "male", "only a phrase"};
    clp(4, args);
    EXPECT_TRUE(clp.has_gender_name());
    EXPECT_EQ(clp.gender_name(), string{"male"});
    EXPECT_EQ(clp.phrase(), string{"only a phrase"});
  }

  clp = CommandLineParser{};
  {
    const char *args[]{"app", "--gender-id"};
    EXPECT_THROW(clp(2, args), CommandLineException);
  }

  {
    const char *args[]{"app", "--gender-name"};
    EXPECT_THROW(clp(2, args), CommandLineException);
  }
}

TEST(command_line_parser, numeri_arguments) {
  CommandLineParser clp = CommandLineParser{};
  {
    const char *args[]{"app", "--all-numeri"};
    clp(2, args);
    EXPECT_TRUE(clp.all_numeri());
    EXPECT_EQ(clp.phrase(), string{""});
  }

  {
    clp = CommandLineParser{};
    const char *args[]{"app", "--all-numeri", "only a phrase"};
    clp(3, args);
    EXPECT_TRUE(clp.all_numeri());
    EXPECT_EQ(clp.phrase(), string{"only a phrase"});
  }
}
